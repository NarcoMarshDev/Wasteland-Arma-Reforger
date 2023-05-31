class WR_SpawnPointDetectionTriggerClass: SCR_BaseTriggerEntityClass
{
}

// IMPORTANT - Do not use periodic updates on the trigger entity in the world editor, it handles it's own queries but make sure to have trigger activation set to "Every query"

class WR_SpawnPointDetectionTrigger: SCR_BaseTriggerEntity
{
	[Attribute(defvalue: "Unnamed Town", category: "Town Settings")]
	string TownName;
	
	WR_SpawnPoint spawnBlufor;
	WR_SpawnPoint spawnOpfor;
	WR_SpawnPoint spawnIndependent;
	
	ref array<IEntity> playersInside;
	
	protected void UpdateSpawnLocks()
	{
		QueryEntitiesInside();
		GetEntitiesInside(playersInside);
		//Print(playersInside);
		
		// lock all spawns to start with, we will unlock them if a player of that faction is present
		spawnBlufor.SetFactionKey(WR_FACTIONKEY_BLUFOR_LOCKED);
		spawnOpfor.SetFactionKey(WR_FACTIONKEY_OPFOR_LOCKED);
		spawnIndependent.SetFactionKey(WR_FACTIONKEY_INDEPENDENT_LOCKED);
		
		foreach (IEntity player: playersInside)
		{
			SCR_ChimeraCharacter character = SCR_ChimeraCharacter.Cast(player);
			// if character is dead ignore them, they dont count for a spawn being enabled
			if (character.GetCharacterController().IsDead())
			{
				continue;
			}
			
			// if player in trigger has matching faction key, unlock the spawn point again
			switch (character.GetFactionKey())
			{
				case WR_FACTIONKEY_BLUFOR:
					spawnBlufor.SetFactionKey(WR_FACTIONKEY_BLUFOR);
					break;
				
				case WR_FACTIONKEY_OPFOR:
					spawnOpfor.SetFactionKey(WR_FACTIONKEY_OPFOR);
					break;
				
				case WR_FACTIONKEY_INDEPENDENT:
					spawnIndependent.SetFactionKey(WR_FACTIONKEY_INDEPENDENT);
					break;
			}
		}
	}
	
	override event protected void EOnInit(IEntity owner)
	{
		// get references to child spawn points
		foreach (auto child: WR_Utils.GetAllChildren(owner))
		{
			WR_SpawnPoint spawn = WR_SpawnPoint.Cast(child);
			if (!spawn)
				continue;
			
			// match spawn factions and set their FactionKey to a locked faction to deactivate them
			switch (spawn.GetFactionKey())
			{
				case WR_FACTIONKEY_BLUFOR:
					spawnBlufor = spawn;
					spawnBlufor.SetFactionKey(WR_FACTIONKEY_BLUFOR_LOCKED);
					spawnBlufor.GetInfo()._WR_SetName(TownName);
					break;
				
				case WR_FACTIONKEY_OPFOR:
					spawnOpfor = spawn;
					spawnOpfor.SetFactionKey(WR_FACTIONKEY_OPFOR_LOCKED);
					spawnOpfor.GetInfo()._WR_SetName(TownName);
					break;
				
				case WR_FACTIONKEY_INDEPENDENT:
					spawnIndependent = spawn;
					spawnIndependent.SetFactionKey(WR_FACTIONKEY_INDEPENDENT_LOCKED);
					spawnIndependent.GetInfo()._WR_SetName(TownName);
					break;
				
				default:
					Print("WR_SpawnPoint @ " + spawn.GetOrigin() + " has invalid FactionKey!", LogLevel.WARNING);
					break;
			}
		}
		
		// check that all factions have a valid spawn point, if not dont insert the update function into the call queue
		bool spawnsValid = true;
		
		if (!spawnBlufor) 		{ Print(""+ TownName + " is missing spawn point for blufor!", 		LogLevel.ERROR); spawnsValid = false; }
		if (!spawnOpfor) 		{ Print(""+ TownName + " is missing spawn point for opfor!", 		LogLevel.ERROR); spawnsValid = false; }
		if (!spawnIndependent) 	{ Print(""+ TownName + " is missing spawn point for independent!", 	LogLevel.ERROR); spawnsValid = false; }
		
		if (!spawnsValid) { return;	}
		
		playersInside = new array<IEntity>();
		
		// get spawn settings from config file
		auto config = SCR_ConfigHelperT<WR_PlayerSpawnSettingsConfig>.GetConfigObject("{F47DA5C510D4C5B7}Configs/WR_PlayerSpawnSettings.conf");
		int updateFrequency = config.SpawnDelaySeconds * 1000;
		
		// if using random update timing, multiply the update frequency so it's + or - 10% of original frequency
		if (config.UseRandomUpdateTiming)
		{
			updateFrequency *= Math.RandomFloat(0.9, 1.1);
		}
		
		// insert update function into call queue for automatic querying (not using periodic queries on the trigger entity itself)
		GetGame().GetCallqueue().CallLater(UpdateSpawnLocks, updateFrequency, true);
	}
}