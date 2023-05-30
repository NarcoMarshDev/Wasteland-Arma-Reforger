[BaseContainerProps(configRoot: true)]
class WR_PlayerClassConfigRoot
{
	[Attribute()];
	protected ref array<ref WR_PlayerClass> PlayerClasses;
}

[BaseContainerProps(configRoot: true)]
class WR_PlayerClass
{
	[Attribute()];
	protected string Name;
	[Attribute()];
	protected string DisplayName;
	
	[Attribute()];
	protected int StartingMoney;
	
	[Attribute(params: "et")];
	protected ResourceName PrimaryWeapon;
	[Attribute(params: "et")];
	protected ResourceName SecondaryWeapon;
	[Attribute(params: "et")];
	protected ResourceName Handgun;
	
	[Attribute(params: "et")];
	protected ResourceName helmet;
	[Attribute(params: "et")];
	protected ResourceName Jacket;
	[Attribute(params: "et")];
	protected ResourceName Pants;
	[Attribute(params: "et")];
	protected ResourceName Boots;
	[Attribute(params: "et")];
	protected ResourceName Backpack;
	[Attribute(params: "et")];
	protected ResourceName Vest;
	
	[Attribute(params: "et")];
	protected ref array<ResourceName> InventoryItems;
}