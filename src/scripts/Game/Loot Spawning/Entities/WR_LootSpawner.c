class WR_LootSpawnerClass: GenericEntityClass
{
}

class WR_LootSpawner: GenericEntity
{
	[Attribute(defvalue: "1800", desc: "Time after loot has been taken before a new loot item can respawn in it's place")]
	int LootRespawnDelaySeconds;
	
	[Attribute(defvalue: "1.0", desc: "Probability that any loot will be spawned when the spawner is ready to spawn a new entity", params: "0 1 0.001")]
	float LootSpawnProbability;
	
	[Attribute(defvalue: "0", uiwidget: UIWidgets.ComboBox, desc: "Loot table used for this spawner", enums: ParamEnumArray.FromEnum(WR_LootTableID))]
	WR_LootTableID LootTableID;
	
	[Attribute(defvalue: "1", desc: "Modifier that increases chances of rare loot above 1, and decreases it below 1", params: "0 inf 0.001")]
	float LootRarityModifier;
	
	int lastLootedTimeMilliseconds;
	
	//------------------------------------------------------------------------------------------------
	void TryRespawnLoot(int currentTimeMilliseconds)
	{
		// if time since last looting is less than the respawn delay just return, nothing else to do here (most likely scenario so checked first)
		if (currentTimeMilliseconds - lastLootedTimeMilliseconds < LootRespawnDelaySeconds * 1000)
			return;
		
		// get and check if child entity exists, if so see if it's far enough away to discard it from the heirarchy and spawn a new one
		IEntity child = GetChildren();
		Print(child);
		if (child)
		{
			Print("1-");
			if ( vector.Distance(GetOrigin(), child.GetOrigin()) > 5 )
			{
				Print("1a");
				// RemoveChild() might be acting weird with keeping the same world transform, so do it manually for now
				vector transform[4];
				child.GetWorldTransform(transform);
				RemoveChild(child);
				child.SetWorldTransform(transform);
			}
			// if it's reached it's respawn delay but the loot entity still hasn't moved, just reset it's last looted time to current and try again later
			else
			{
				Print("1b");
				lastLootedTimeMilliseconds = currentTimeMilliseconds;
				return;
			}
		}
		
		// if we get to here, enough time has passed and the old loot entity either no longer exists or has been removed as our child, so we can do our spawn logic
		
		// get random value from 0 - 1 and see if it's above the spawn probability cutoff, if so reset last looted time as nothing will be spawned this cycle
		if ( Math.RandomFloat01() > LootSpawnProbability )
		{
			lastLootedTimeMilliseconds = currentTimeMilliseconds;
			Print("failed: " + lastLootedTimeMilliseconds);
			return;
		}
		
		// #TODO - This is awful and temporary until I find a better way to get a weighted random selection
		WR_LootTable lootTable = WR_LootSpawningManagerComponent.Cast( GetGame().GetGameMode().FindComponent(WR_LootSpawningManagerComponent) ).allLootTables.Get(LootTableID);
		float rand = Math.RandomFloat01() / LootRarityModifier;
		array<ResourceName> validItems = {};
		
		for (int i = 0; i < lootTable.Count(); i++)
		{
			// if our random value is below the item's spawn probability, insert into valid items
			if (rand <= lootTable.GetProbability(i))
			{
				validItems.Insert( lootTable.GetResource(i) );
			}
		}
		
		if (validItems.IsEmpty())
		{
			return;
		}
		
		
		
		// get random element from valid items and spawn it
		vector mat[4];
		GetTransform(mat);
		mat[3][1] = mat[3][1] + 0.1;
		
		DoLootSpawn(mat, validItems.GetRandomElement());
		lastLootedTimeMilliseconds = currentTimeMilliseconds;
	}
	
	protected void DoLootSpawn(vector transform[4], ResourceName res, bool MagazineMultiSpawn=false)
	{
		IEntity newLoot = WR_Utils.SpawnPrefab( transform, res );
		// if we picked a magazine, spawn between 1 and 3 more magazines with it
		if (newLoot.FindComponent(MagazineComponent) && !MagazineMultiSpawn)
		{
			for (int i = 0; i < Math.RandomIntInclusive(1, 3); i++)
			{
				DoLootSpawn(transform, res, true);
			}
		}
		Print(newLoot.GetPrefabData().GetPrefab().GetResourceName());
		
		// if using physics is disabled by config, just add as child and return from here
		if (!WR_LOOT_SPAWNER_USE_PHYSICS)
		{
			vector mat[4];
			newLoot.GetWorldTransform(mat);
			AddChild(newLoot, -1);
			newLoot.SetWorldTransform(mat);
			return;
		}
		// enable physics and apply a small force to knock the item over to make it look more natural
		Physics phys = newLoot.GetPhysics();
		if (!phys)
		{
			phys = Physics.CreateDynamic(newLoot, 1);
		}
		if (!phys.IsDynamic())
		{
			phys.Destroy();
			phys = Physics.CreateDynamic(newLoot, 1);
		}
		phys.SetDamping(0.1, 0.1);
		phys.ChangeSimulationState(SimulationState.SIMULATION);
		phys.SetInteractionLayer(EPhysicsLayerPresets.Building);
		phys.SetActive(ActiveState.ACTIVE);
		
		float force = 0.0005 * phys.GetMass();
		phys.ApplyImpulseAt(phys.GetCenterOfMass() + "0 0.1 0", Vector(force, force, force));
		
		// after 3 seconds, set the entity back to inactive to save performance
		GetGame().GetCallqueue().CallLater(SetLootInactive, 3000, false, newLoot);
	}
	
	void SetLootInactive(IEntity ent)
	{
		Print("physics settled");
		ent.GetPhysics().SetActive(ActiveState.INACTIVE);
		vector mat[4];
		ent.GetWorldTransform(mat);
		AddChild(ent, -1);
		ent.SetWorldTransform(mat);
	}
	
	//------------------------------------------------------------------------------------------------
	override void EOnInit(IEntity owner)
	{
		Print("hello?");
		WR_LootSpawningManagerComponent manager = WR_LootSpawningManagerComponent.Cast( GetGame().GetGameMode().FindComponent(WR_LootSpawningManagerComponent) );
		Print("=================================");
		Print(LootRespawnDelaySeconds * 1000);
		LootRespawnDelaySeconds *= manager.globalLootRespawnDelayMultiplier;
		Print(LootRespawnDelaySeconds * 1000);
		Print("=================================");
		LootSpawnProbability *= manager.globalLootSpawnProbabilityMultiplier;
		Print(LootSpawnProbability);
		manager.RegisterLootSpawn(this);
	}
	
	void WR_LootSpawner(IEntitySource src, IEntity parent)
	{
		BaseWorld world = GetGame().GetWorld();
		if (world)
		{
			SetEventMask(EntityEvent.INIT);
		}
	}
}