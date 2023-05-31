class WR_LootSpawnerClass: GenericEntityClass
{
}

class WR_LootSpawner: GenericEntity
{
	int lastSpawnedTime;
	
	override void EOnInit(IEntity owner)
	{
		WR_LootSpawningManagerComponent.Cast( GetGame().GetGameMode().FindComponent(WR_LootSpawningManagerComponent) ).RegisterLootSpawn(this);
	}
}