[BaseContainerProps(configRoot: true)]
class WR_PlayerSpawnSettingsConfig
{
	[Attribute(defvalue: "true", desc: "Use slight randomised update times for spawn zones to prevent lag spikes from them all updating at the same time")]
	bool UseRandomUpdateTiming;
	
	[Attribute(defvalue: "5", desc: "Average number of seconds between spawn zone updates")]
	int SpawnDelaySeconds;
}