class WR_RandomSpawnPointClass: SCR_SpawnPointClass
{
}

class WR_RandomSpawnPoint: SCR_SpawnPoint
{
	override void EOnInit(IEntity owner)
	{
		super.EOnInit(owner);
		GetInfo()._WR_SetName("Random Spawn Location...");
	}
	
	override void EOnPlayerSpawn(IEntity entity)
	{
		SCR_SpawnPoint randomSpawn;
		for (int i = 0; i < CountSpawnPoints(); i++)
		{
			randomSpawn = GetSpawnPoints().GetRandomElement();
			if (randomSpawn.IsInherited(WR_SpawnPoint))
			{
				break;
			}
		}
		
		WR_SpawnPointDetectionTrigger trigger = WR_SpawnPointDetectionTrigger.Cast( randomSpawn.GetParent() );
		if (!trigger)
		{
			Print("WR_SpawnPoint @ " + GetOrigin() + " has no trigger!", LogLevel.WARNING); return;
		}
		
		float triggerRadius = trigger.GetSphereRadius();
		entity.SetOrigin( WR_SpawnPoint.Cast(randomSpawn).GetRandomSpawnLocation(triggerRadius, WR_SpawnPoint.RECURSION_LIMIT) );
	}
}