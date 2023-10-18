enum WR_LootTableID
{
	Default,
	CivillianResidential, 	// basic items, canned food if implemented, civillian clothes, etc. May not get used at all
	CivillianGarage,		// civi vehicle parts, tools, fuel containers, scrap metal and other basic tier 1 components. 
	CivillianIndustrial,	// tier 2 components, extra resources compared to garages but very similar
	CivillianHarbour,		// boat parts, otherwise similar again to industrial
	MilitaryCheckpoint,		// small amounts of ammo, basic weapons, binoculars, and other assorted low tier military equipment and small amounts of tier 3 components
	MilitaryBase,			// higher tier weapons and ammo. machine guns, rocket launchers, body armour, and tier 3 components
	MilitaryHarbour,		// a combination of checkpoint and civi harbour loot with more tier 2 components compared to civi harbours
	Medical,
	RadZoneSmall,			// basic weapons with rare chance for higher grade weapons. Tier 2 component spawns and occasional tier 3 components. Occasional keycode spawns
	RadZoneLarge,			// similar loot to military base with backup rad gear and more tier 3 components. Also spawns codes for ammo depots and rarely weapon blueprints
	RadIsland,				// same as large rad and military base, but spawns keys/key fragments for uplinks
	AmmoDepot,				// spawns rare weapons and large quantities of ammo, as well as weapon blueprints
	VEHICLE_SPAWN			// loot table used for vehicle spawners, not loot items
}

[BaseContainerProps()]
class WR_LootTableEntry
{
	[Attribute()]
	ResourceName LootResource;
	[Attribute(params: "0 1 0.001")]
	float SpawnProbabilityWeight;
}

[BaseContainerProps()]
class WR_LootTable
{
	[Attribute(uiwidget: UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(WR_LootTableID))]
	WR_LootTableID LootTableID;
	
	[Attribute()]
	ref array<ref WR_LootTableEntry> Entries;
	
	ResourceName 	GetResource(int i) 		{ return Entries.Get(i).LootResource; }
	float 			GetProbability(int i) 	{ return Entries.Get(i).SpawnProbabilityWeight; }
	
	int Count() {return Entries.Count();}
}

[BaseContainerProps()]
class WR_LootTable_Old: map<ResourceName, float>
{
	// Map that contains name of resource to spawn, and chance of being selected.
	// chance goes from 0 to 1, with 0 meaning it will never be selected, and 1 meaning it has the highest chance of being selected
	// having a probability of 1 *does not* garnuntee it will spawn every time, as it's still a random selection, just a weighted one
}