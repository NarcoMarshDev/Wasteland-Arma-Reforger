class WR_GameLogicComponentClass : SCR_BaseGameModeComponentClass
{
}

class WR_GameLogicComponent : SCR_BaseGameModeComponent
{
	
	
	
	
	
	
	
	
	
	// ---------------------------------------------------------------------------------------------------------------- //
	override void EOnInit(IEntity owner)
	{

	}
	// ---------------------------------------------------------------------------------------------------------------- //
	override void OnPlayerConnected(int playerId)
	{
		
	}
	// ---------------------------------------------------------------------------------------------------------------- //
	override void OnPlayerKilled(int playerId, IEntity player, IEntity killer)
	{

	}
	// ---------------------------------------------------------------------------------------------------------------- //
	override void OnPlayerSpawned(int playerId, IEntity controlledEntity)
	{
		
	}
	// ---------------------------------------------------------------------------------------------------------------- //
	override void OnPostInit(IEntity owner)
	{
		SetEventMask(owner, EntityEvent.INIT);
	}
}