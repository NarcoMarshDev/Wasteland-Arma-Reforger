[ComponentEditorProps(category: "GameScripted/Misc", description: "")]
class WR_BuildingObjectComponentClass : ScriptComponentClass
{
};
class WR_BuildingObjectComponent : ScriptComponent
{
	override void EOnInit(IEntity owner)
	{
		// TODO - Self insert with the garbage manager
	}
	override void OnPostInit(IEntity owner)
	{
		SetEventMask(owner, EntityEvent.INIT);
	}
	override void OnDelete(IEntity owner)
	{
	}
}
