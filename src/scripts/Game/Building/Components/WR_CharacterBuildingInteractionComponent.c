class WR_CharacterBuildingInteractionComponentClass: ScriptComponentClass
{
}

class WR_CharacterBuildingInteractionComponent: ScriptComponent
{
	override void EOnInit(IEntity owner)
	{
		RplComponent rpl = RplComponent.Cast(owner.FindComponent(RplComponent));
		Print("is proxy: " + rpl.IsProxy() );
		Print("is owner: " + rpl.IsOwner() );
		
		Print("is client: " + Replication.IsClient());
		Print("is server: " + Replication.IsServer());
	}
	override void OnPostInit(IEntity owner)
	{
		SetEventMask(owner, EntityEvent.INIT);
	}
}