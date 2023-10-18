FactionKey WR_FACTIONKEY_BLUFOR 	 		= "US";
FactionKey WR_FACTIONKEY_OPFOR 		 		= "USSR";
FactionKey WR_FACTIONKEY_INDEPENDENT 		= "FIA";

FactionKey WR_FACTIONKEY_BLUFOR_LOCKED 		= "US_LOCKED";
FactionKey WR_FACTIONKEY_OPFOR_LOCKED 		= "USSR_LOCKED";
FactionKey WR_FACTIONKEY_INDEPENDENT_LOCKED = "FIA_LOCKED";

class WR_Utils
{
	//------------------------------------------------------------------------------------------------
	static array<IEntity> GetAllChildren(IEntity ent)
	{
		IEntity child = ent.GetChildren();
		if (!child)
			return null;
		
		array<IEntity> allChildren = {};
		while (child)
		{
			allChildren.Insert(child);
			child = child.GetSibling();
		}
		return allChildren;
	}
	
	//------------------------------------------------------------------------------------------------
	static IEntity SpawnPrefab(vector transform[4], ResourceName prefabName)
	{
		if (!prefabName)
		{
			Print("Missing Prefab: " + prefabName, LogLevel.ERROR);
			return null;
		}
		if (!transform)
		{
			Print("No valid transform! Using default transform instead.", LogLevel.WARNING);
			Math3D.MatrixIdentity4(transform)
		}
		Resource prefab = Resource.Load(prefabName);
		EntitySpawnParams spawnParams = new EntitySpawnParams();
		spawnParams.TransformMode = ETransformMode.WORLD;
		spawnParams.Transform = transform;
		
		return GetGame().SpawnEntityPrefab(prefab, GetGame().GetWorld(), spawnParams);
	}
	//------------------------------------------------------------------------------------------------
	static WR_BuildingObjectComponent GetBuildingObjectComponent(IEntity ent)
	{
		return WR_BuildingObjectComponent.Cast( ent.FindComponent(WR_BuildingObjectComponent) );
	}
}