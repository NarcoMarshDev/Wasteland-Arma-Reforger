FactionKey WR_FACTIONKEY_BLUFOR 	 		= "US";
FactionKey WR_FACTIONKEY_OPFOR 		 		= "USSR";
FactionKey WR_FACTIONKEY_INDEPENDENT 		= "FIA";

FactionKey WR_FACTIONKEY_BLUFOR_LOCKED 		= "US_LOCKED";
FactionKey WR_FACTIONKEY_OPFOR_LOCKED 		= "USSR_LOCKED";
FactionKey WR_FACTIONKEY_INDEPENDENT_LOCKED = "FIA_LOCKED";

class WR_Utils
{
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
}