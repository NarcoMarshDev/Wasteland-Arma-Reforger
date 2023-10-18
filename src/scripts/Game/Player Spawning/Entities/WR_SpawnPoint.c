class WR_SpawnPointClass: SCR_SpawnPointClass
{
}

class WR_SpawnPoint: SCR_SpawnPoint
{
	static int RECURSION_LIMIT = 10;
	
	override bool PrepareSpawnedEntity_S(SCR_SpawnRequestComponent requestComponent, SCR_SpawnData data, IEntity entity)
	{
		super.PrepareSpawnedEntity_S(requestComponent, data, entity);
		WR_SpawnPointDetectionTrigger trigger = WR_SpawnPointDetectionTrigger.Cast( GetParent() );
		
		// if we're not part of a town spawn heirarchy, we must be broken so just spawn directly on the point
		if (!trigger)
		{
			Print("WR_SpawnPoint @ " + GetOrigin() + " has no trigger!", LogLevel.WARNING); return false;
		}
		
		float triggerRadius = trigger.GetSphereRadius();
		entity.SetOrigin( GetRandomSpawnLocation(triggerRadius, RECURSION_LIMIT) );
		return true;
	}
	
	// Recursive function to attempt to find a valid spawn location
	vector GetRandomSpawnLocation(float triggerRadius, int recursionDepthLimit, int currentRecursionDepth=0)
	{
		vector coords = GetOrigin();
		
		// if recursion depth limit is hit, just return a position directly on the terrain at the spawn point x and z coords. Either really bad luck or something is wrong
		if (currentRecursionDepth == recursionDepthLimit)
		{
			Print("RECURSION LIMIT HIT", LogLevel.WARNING);
			return Vector(coords[0], SCR_TerrainHelper.GetTerrainY(coords, GetGame().GetWorld(), true), coords[2]);
		}
		
		// get random x and z points within trigger radius and find terrain height for the y coordinate
		coords[0] = coords[0] + Math.RandomFloatInclusive(-triggerRadius, triggerRadius); // x
		coords[2] = coords[2] + Math.RandomFloatInclusive(-triggerRadius, triggerRadius); // z
		coords[1] = SCR_TerrainHelper.GetTerrainY(coords, GetGame().GetWorld(), true);    // y
		
		// if our y coord is close to the ocean height, we must be in the water so try again
		if ( Math.AbsFloat( GetGame().GetWorld().GetOceanHeight(coords[0], coords[2]) - coords[1] ) < 0.5 )
		{
			return GetRandomSpawnLocation(triggerRadius, recursionDepthLimit, currentRecursionDepth + 1);
		}
		
		// check from 1 meter above if the terrain matches actual ground height
		TraceParam downTrace = MakeTraceParam(coords + vector.Up, coords, TraceFlags.ENTS | TraceFlags.WORLD);
		float downDistance = GetGame().GetWorld().TraceMove(downTrace, null);
		
		// if it doesn't, snap to wherever the ground actually is
		if (downDistance < 1)
		{
			coords = (coords + vector.Up) - Vector(0, downDistance, 0); // simple subtraction since all movement is along the y axis
		}
		
		// trace upwards 5 meters to make sure we're not inside a building (big hangars etc. are okay)
		TraceParam upTrace = MakeTraceParam(coords, coords + (vector.Up * 5), TraceFlags.ENTS);
		float upDistance = GetGame().GetWorld().TraceMove(upTrace, null);
		
		// if the trace maxes out, we must have a valid spawn location not inside so return;
		if (upDistance == 1)
		{
			return coords;
		}
		// if the trace didn't max out, location is invalid, so try again recursively until we get a valid location or hit the recursion depth limit
		return GetRandomSpawnLocation(triggerRadius, recursionDepthLimit, currentRecursionDepth + 1);
	}
}