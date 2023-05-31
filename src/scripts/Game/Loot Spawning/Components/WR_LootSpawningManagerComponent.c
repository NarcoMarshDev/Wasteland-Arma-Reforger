class WR_LootSpawningManagerComponentClass: SCR_BaseGameModeComponentClass
{
}

class WR_LootSpawningManagerComponent: SCR_BaseGameModeComponent
{
	int updateFrequencySeconds;
	
	
	ref array<WR_LootSpawner> allLootSpawners;
	
	void RegisterLootSpawn(WR_LootSpawner spawn)
	{
		allLootSpawners.Insert(spawn);
	}
	
	void Update
}