class WR_RandomSpawnPointClass: SCR_SpawnPointClass
{
}

class WR_RandomSpawnPoint: SCR_SpawnPoint
{
	// Self assign as the random spawn point in the player spawn manager, if multiple random spawns exist, only the last one initialised will be valid
	override void EOnInit(IEntity owner)
	{
		WR_PlayerSpawnManagerComponent.Cast( GetGame().GetGameMode().FindComponent(WR_PlayerSpawnManagerComponent) ).SetRandomSpawn(this);
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