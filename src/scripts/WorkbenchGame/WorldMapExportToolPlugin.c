#ifdef NON_WASTELAND
[WorkbenchPluginAttribute("Export Map data (Custom)", "Export data of various types from the current world", "", category: "Wasteland Additions...", wbModules: {"WorldEditor"}, awesomeFontCode: 0xf0ac)]
class WorldMapExportToolCustom: WorkbenchPlugin //WorldEditorTool
{
	[Attribute(defvalue: "", desc: "Folder where the file will be saved", params: "unregFolders", enums: NULL, category: "Export file")]
	ResourceName destinationPath;
	string exportPath;
	
	[Attribute("0", UIWidgets.ComboBox, "Export type", "", ParamEnumArray.FromEnum(EMapDataExportType), category: "Export file")]
	int type;

	[Attribute("1", UIWidgets.CheckBox, "2D Geometry: Generation of Roads' data", category: "Geometry 2D")]
	bool generateRoads;
	[Attribute("1", UIWidgets.CheckBox, "2D Geometry: Save Roads' data to exported file", category: "Geometry 2D")]
	bool saveRoads;
	
	[Attribute("1", UIWidgets.CheckBox, "2D Geometry: Generation of Powerlines' data", category: "Geometry 2D")]
	bool generatePowerlines;
	[Attribute("1", UIWidgets.CheckBox, "2D Geometry: Save Powerlines' data to exported file", category: "Geometry 2D")]
	bool savePowerlines;
	
	[Attribute("1", UIWidgets.CheckBox, "2D Geometry: Generation of Buildings' data", category: "Geometry 2D")]
	bool generateBuildings;
	[Attribute("1", UIWidgets.CheckBox, "2D Geometry: Save Buildings' data to exported file", category: "Geometry 2D")]
	bool saveBuildings;
	
	[Attribute("1", UIWidgets.CheckBox, "2D Geometry: Generation of Areas' data", category: "Geometry 2D")]
	bool generateAreas;
	[Attribute("1", UIWidgets.CheckBox, "2D Geometry: Ignore Generator entities during generation", category: "Geometry 2D")]
	bool ignoreGeneratorAreas;
	[Attribute("1", UIWidgets.CheckBox, "2D Geometry: Save Areas' data to exported file", category: "Geometry 2D")]
	bool saveAreas;
	
	[Attribute("1", UIWidgets.CheckBox, "2D Geometry: Generation of Water Bodies' data", category: "Geometry 2D")]
	bool generateWaterBodies;
	[Attribute("1", UIWidgets.CheckBox, "2D Geometry: Save Water Bodies' data to exported file", category: "Geometry 2D")]
	bool saveWaterBodies;
	
	[Attribute("1", UIWidgets.CheckBox, "2D Geometry: Generation of Hills' data", category: "Geometry 2D")]
	bool generateHills;
	[Attribute("50.0", UIWidgets.Slider, "Minimum hill height", "30 155 5", category: "Geometry 2D")]
	float hillMinimumHeight;
	[Attribute("1", UIWidgets.CheckBox, "2D Geometry: Save Hills' data to exported file", category: "Geometry 2D")]
	bool saveHills;

	[Attribute("1.0 1.0 1.0 1.0", UIWidgets.ColorPicker, "Rasterization: Land color", category: "Rasterization")]
	ref Color landColor;
	[Attribute("0.863 0.980 1.0 1.0", UIWidgets.ColorPicker, "Rasterization: Ocean color", category: "Rasterization")]
	ref Color oceanColor;
	[Attribute("3.0", UIWidgets.Slider, "Rasterization: Land scale factor", "0 10 0.1", category: "Rasterization")]
	float scaleLand;
	[Attribute("1.0", UIWidgets.Slider, "Rasterization: Ocean scale factor", "0 10 0.1", category: "Rasterization")]
	float scaleOcean;
	[Attribute("400.0", UIWidgets.Slider, "Rasterization: Height scale factor", "-1000 1000 1.0", category: "Rasterization")]
	float heightScale;
	[Attribute("60.0", UIWidgets.Slider, "Rasterization: Depth scale factor", "-1000 1000 1.0", category: "Rasterization")]
	float depthScale;
	[Attribute("-50.0", UIWidgets.Slider, "Rasterization: Depth Lerp meters", "-1000 1000 1.0", category: "Rasterization")]
	float depthLerpMeters;
	[Attribute("0.5", UIWidgets.Slider, "Rasterization: Shade intensity factor", "0.5 15 0.01", category: "Rasterization")]
	float shadeIntensity;
	[Attribute("2.0", UIWidgets.Slider, "Rasterization: Height intensity factor", "0.5 15 0.01", category: "Rasterization")]
	float heightIntensity;
	[Attribute("1", UIWidgets.CheckBox, "Rasterization: Include Areas defined by Generators into Rasterization", category: "Rasterization")]
	bool includeGeneratorAreas;
	[Attribute("0.608 0.784 0.529 1.0", UIWidgets.ColorPicker, "Rasterization: Forest area color", category: "Rasterization")]
	ref Color forestAreaColor;
	[Attribute("1.25", UIWidgets.Slider, "Rasterization: Forest area intensity factor", "0.5 15 0.01", category: "Rasterization")]
	float forestAreaIntensity;
	[Attribute("0.745 0.745 0.745 1.0", UIWidgets.ColorPicker, "Rasterization: Other area color", category: "Rasterization")]
	ref Color otherAreaColor;
	[Attribute("1.0", UIWidgets.Slider, "Rasterization: Other area intensity factor", "0.5 15 0.01", category: "Rasterization")]
	float otherAreaIntensity;
	
	override void Run()
	{
		WorldEditor worldEditor = Workbench.GetModule(WorldEditor);
		if (!worldEditor)
			return;

		WorldEditorAPI api = worldEditor.GetApi();
		if (!api)
			return;

		BaseWorld world = api.GetWorld();
		if (!world)
			return;
		
		if (Workbench.ScriptDialog("Export Map Entities Plugin", "", this))
		{
			
		}
	}
	
	[ButtonAttribute("Export")]
	void Execute()
	{
		Export();
	}
	
	//----------------------------------------------------------------------------------------------
	bool Export()
	{
		WorldEditor we = Workbench.GetModule(WorldEditor);
		WorldEditorAPI api = we.GetApi();
		string worldPath;
		api.GetWorldPath(worldPath);

		WBProgressDialog progress = new WBProgressDialog("Processing", we);
		MapDataExporter exporter = new MapDataExporter();


		exportPath = destinationPath.GetPath();

		if (type == EMapDataExportType.Geometry2D)
		{
			Print("Exporting map 2D map data...");
			ExcludeGenerateFlags generateFlags = ExcludeGenerateFlags.ExcludeGenerateNone;
			if(!generateRoads) generateFlags = generateFlags | ExcludeGenerateFlags.ExcludeGenerateRoads;
			if(!generatePowerlines) generateFlags = generateFlags | ExcludeGenerateFlags.ExcludeGeneratePowerlines;
			if(!generateBuildings) generateFlags = generateFlags | ExcludeGenerateFlags.ExcludeGenerateBuildings;
			if(!generateAreas) generateFlags = generateFlags | ExcludeGenerateFlags.ExcludeGenerateAreas;
			if(!generateWaterBodies) generateFlags = generateFlags | ExcludeGenerateFlags.ExcludeGenerateWaterBodies;
			if(!generateHills) generateFlags = generateFlags | ExcludeGenerateFlags.ExcludeGenerateHills;
			ExcludeSaveFlags saveFlags = ExcludeSaveFlags.ExcludeSaveNone;
			if(!saveRoads) saveFlags = saveFlags | ExcludeSaveFlags.ExcludeSaveRoads;
			if(!savePowerlines) saveFlags = saveFlags | ExcludeSaveFlags.ExcludeSavePowerlines;
			if(!saveBuildings) saveFlags = saveFlags | ExcludeSaveFlags.ExcludeSaveBuildings;
			if(!saveAreas) saveFlags = saveFlags | ExcludeSaveFlags.ExcludeSaveAreas;
			if(!saveWaterBodies) saveFlags = saveFlags | ExcludeSaveFlags.ExcludeSaveWaterBodies;
			if(!saveHills) saveFlags = saveFlags | ExcludeSaveFlags.ExcludeSaveHills;

			DataExportErrorType result = exporter.ExportData(EMapDataType.Geometry2D, exportPath, worldPath, hillMinimumHeight, ignoreGeneratorAreas, generateFlags, saveFlags);
			if(result != DataExportErrorType.DataExportErrorNone)
			{
				string reason = "";
				switch(result)
				{
					case DataExportErrorType.DataExportErrorNoGame: reason = "Game not found"; break;
					case DataExportErrorType.DataExportErrorNoDataType: reason = "Data type not found"; break;
					case DataExportErrorType.DataExportErrorNoMapEntity: reason = "Map Entity not found"; break;
					case DataExportErrorType.DataExportErrorNoWorld: reason = "World not found"; break;
					case DataExportErrorType.DataExportErrorNoWorldPath: reason = "World path empty"; break;
					case DataExportErrorType.DataExportErrorNoOutputFile: reason = "Could not open output file"; break;
					default: reason = "Unkown, code=" + result; break;
				}
				Print(string.Format("Could not export 2D map data. (%1)", reason), LogLevel.ERROR);
				return false;
			}
		}
		else if (type == EMapDataExportType.Soundmap)
		{
			Print("Exporting Soundmap...");
			DataExportErrorType result = exporter.ExportData(EMapDataType.Soundmap, exportPath, worldPath);
			if(result != DataExportErrorType.DataExportErrorNone)
			{
				string reason = "";
				switch(result)
				{
					case DataExportErrorType.DataExportErrorNoGame: reason = "Game not found"; break;
					case DataExportErrorType.DataExportErrorNoDataType: reason = "Data type not found"; break;
					case DataExportErrorType.DataExportErrorNoWorld: reason = "World not found"; break;
					case DataExportErrorType.DataExportErrorNoCompatibleWorld: reason = "World is not compatible"; break;
					case DataExportErrorType.DataExportErrorNoSoundWorld: reason = "Sound World not found"; break;
					case DataExportErrorType.DataExportErrorNoWorldPath: reason = "World path empty"; break;
					case DataExportErrorType.DataExportErrorNotSavingToFile: reason = "Not saving to file, Default Map created"; break;
					case DataExportErrorType.DataExportErrorInvalidSoundmap: reason = "Invalid Soundmap"; break;
					default: reason = "Unkown, code=" + result; break;
				}
				Print(string.Format("Could not export Soundmap data. (%1)", reason), LogLevel.ERROR);
				return false;
			}
		}
		else if (type == EMapDataExportType.Rasterization)
		{
			Print("Exporting map rasterization...");
			DataExportErrorType result = exporter.ExportRasterization(exportPath, worldPath, landColor, oceanColor, scaleLand, scaleOcean, heightScale, depthScale, depthLerpMeters, shadeIntensity, heightIntensity, includeGeneratorAreas, forestAreaColor, forestAreaIntensity, otherAreaColor, otherAreaIntensity);
			if(result != DataExportErrorType.DataExportErrorNone)
			{
				string reason = "";
				switch(result)
				{
					case DataExportErrorType.DataExportErrorNoGame: reason = "Game not found"; break;
					case DataExportErrorType.DataExportErrorNoLandOrOcean: reason = "No Land or Ocean color found"; break;
					case DataExportErrorType.DataExportErrorNoMapEntity: reason = "Map Entity not found"; break;
					case DataExportErrorType.DataExportErrorNoWorld: reason = "World not found"; break;
					case DataExportErrorType.DataExportErrorNoWorldPath: reason = "World path empty"; break;
					case DataExportErrorType.DataExportErrorNoTerrain: reason = "Terrain not found"; break;
					case DataExportErrorType.DataExportErrorNoOutputFile: reason = "Could not open output file"; break;
					default: reason = "Unkown, code=" + result; break;
				}
				Print(string.Format("Could not export map rasterization. (%1)", reason), LogLevel.ERROR);
				return false;
			}
		}
		return true;
	}
};
#endif