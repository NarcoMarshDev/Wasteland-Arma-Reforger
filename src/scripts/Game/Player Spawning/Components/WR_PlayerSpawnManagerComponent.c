// #UNUSED
class WR_PlayerSpawnManagerComponentClass: SCR_BaseGameModeComponentClass
{
}

class WR_PlayerSpawnManagerComponent: SCR_BaseGameModeComponent
{
	ref array<WR_SpawnPointDetectionTrigger> allTriggers = {};
	protected WR_RandomSpawnPoint randomSpawnPoint;
	
	void SetRandomSpawn(WR_RandomSpawnPoint sp) {randomSpawnPoint = sp;}
	
	override void OnPlayerSpawned(int playerId, IEntity controlledEntity)
	{
		
	}
}