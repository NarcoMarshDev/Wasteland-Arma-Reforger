class WR_GarbageManagerComponentClass: SCR_BaseGameModeComponentClass
{
}

class WR_GarbageManagerComponent: SCR_BaseGameModeComponent
{
	// map of all entities being managed with their last interacted time
	ref map<IEntity, int> managedEntities = new map<IEntity, int>();
	
	void RegisterEntity(IEntity ent)
	{
		
	}
}