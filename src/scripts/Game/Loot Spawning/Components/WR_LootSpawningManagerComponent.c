bool WR_LOOT_SPAWNER_USE_PHYSICS;

class WR_LootSpawningManagerComponentClass: SCR_BaseGameModeComponentClass
{
}

class WR_LootSpawningManagerComponent: SCR_BaseGameModeComponent
{
	ref array<WR_LootSpawner> allLootSpawners = {};
	int currentSpawnerIndex;
	
	float globalLootRespawnDelayMultiplier;
	float globalLootSpawnProbabilityMultiplier;
	
	ref map<WR_LootTableID, ref WR_LootTable> allLootTables = new map<WR_LootTableID, ref WR_LootTable>();
	
	//------------------------------------------------------------------------------------------------
	void RegisterLootSpawn(WR_LootSpawner spawn)
	{
		allLootSpawners.Insert(spawn);
	}
	
	//------------------------------------------------------------------------------------------------
	// Every call, one spawner is updated in the world sequentially.
	// This gives us constant performance independent of number of spawners, but time between updates per spawner is longer with more in the world.
	// *DO NOT* call in main thread in a high frequency function e.g EOnFrame(), it will cause crashes if a spawn takes over one frame. Use ScriptCallQueue.CallLater() instead.
	void UpdateNextSpawner()
	{
		int tick = System.GetTickCount();
		int count = allLootSpawners.Count();
		if (count == 0)
		{
			return;
		}
		allLootSpawners.Get( currentSpawnerIndex % count ).TryRespawnLoot(tick + (4*60*60*1000)); // could optimise by having a variable for the array size instead of .Count()?
		currentSpawnerIndex++;
	}
	
	//------------------------------------------------------------------------------------------------
	override event void EOnInit(IEntity owner)
	{		
		// load settings config
		WR_LootSpawningSettingsConfig config = SCR_ConfigHelperT<WR_LootSpawningSettingsConfig>.GetConfigObject("{E080A47BC9EA7184}Configs/WR_LootSpawningSettings.conf");
		
		globalLootRespawnDelayMultiplier 	 = config.GlobalLootRespawnDelayMultiplier;
		globalLootSpawnProbabilityMultiplier = config.GlobalLootSpawnProbabilityMultiplier;
		WR_LOOT_SPAWNER_USE_PHYSICS			 = config.PlaceLootWithPhysics;
		int lootSpawnerUpdateInterval		 = config.LootSpawnerUpdateInterval;
		
		allLootTables.Set(WR_LootTableID.Default, 				config.Default				);
		allLootTables.Set(WR_LootTableID.CivillianResidential, 	config.CivillianResidential	);
		allLootTables.Set(WR_LootTableID.CivillianGarage, 		config.CivillianGarage		);
		allLootTables.Set(WR_LootTableID.CivillianIndustrial, 	config.CivillianIndustrial	);
		allLootTables.Set(WR_LootTableID.CivillianHarbour, 		config.CivillianHarbour		);
		allLootTables.Set(WR_LootTableID.MilitaryCheckpoint, 	config.MilitaryCheckpoint	);
		allLootTables.Set(WR_LootTableID.MilitaryBase, 			config.MilitaryBase			);
		allLootTables.Set(WR_LootTableID.MilitaryHarbour, 		config.MilitaryHarbour		);
		allLootTables.Set(WR_LootTableID.Medical, 				config.Medical				);
		allLootTables.Set(WR_LootTableID.RadZoneSmall, 			config.RadZoneSmall			);
		allLootTables.Set(WR_LootTableID.RadZoneLarge, 			config.RadZoneLarge			);
		allLootTables.Set(WR_LootTableID.RadIsland, 			config.RadIsland			);
		allLootTables.Set(WR_LootTableID.AmmoDepot, 			config.AmmoDepot			);
		
		GetGame().GetCallqueue().CallLater(UpdateNextSpawner, lootSpawnerUpdateInterval, true);
		//GetGame().GetCallqueue().CallLater(SetEventMask, 5000, false, owner, EntityEvent.FIXEDFRAME);
	}
	
	//------------------------------------------------------------------------------------------------
	override void OnPostInit(IEntity owner)
	{
		SetEventMask(owner, EntityEvent.INIT);
	}
}

//------------------------------------------------------------------------------------------------

[BaseContainerProps(configRoot: true)]
class WR_LootSpawningSettingsConfig
{
	[Attribute(defvalue: "1.0", desc: "Multiplier that affects all loot respawn delays by a fixed amount")]
	float GlobalLootRespawnDelayMultiplier;
	
	[Attribute(defvalue: "1.0", desc: "Multiplier that affects all loot spawn probabilities by a fixed amount")]
	float GlobalLootSpawnProbabilityMultiplier;
	
	[Attribute(defvalue: "250", desc: "Time between loot spawner update events in milliseconds")]
	int LootSpawnerUpdateInterval;
	
	[Attribute(defvalue: "true", desc: "Do loot entities have their physics temporarily simulated to place them more natually")]
	bool PlaceLootWithPhysics;
	
	// add loot tables here
	[Attribute(category: "Loot Tables")]
	ref WR_LootTable Default;
	[Attribute(category: "Loot Tables")]
	ref WR_LootTable CivillianResidential;
	[Attribute(category: "Loot Tables")]
	ref WR_LootTable CivillianGarage;
	[Attribute(category: "Loot Tables")]
	ref WR_LootTable CivillianIndustrial;
	[Attribute(category: "Loot Tables")]
	ref WR_LootTable CivillianHarbour;
	[Attribute(category: "Loot Tables")]
	ref WR_LootTable MilitaryCheckpoint;
	[Attribute(category: "Loot Tables")]
	ref WR_LootTable MilitaryBase;
	[Attribute(category: "Loot Tables")]
	ref WR_LootTable MilitaryHarbour;
	[Attribute(category: "Loot Tables")]
	ref WR_LootTable Medical;
	[Attribute(category: "Loot Tables")]
	ref WR_LootTable RadZoneSmall;
	[Attribute(category: "Loot Tables")]
	ref WR_LootTable RadZoneLarge;
	[Attribute(category: "Loot Tables")]
	ref WR_LootTable RadIsland;
	[Attribute(category: "Loot Tables")]
	ref WR_LootTable AmmoDepot;
}