[WorkbenchPluginAttribute(name: "World Export Plugin", shortcut: "", wbModules: { "WorldEditor" }, category: "Wasteland Plugins", awesomeFontCode: 0xf56e)]
class WorldExportPlugin : WorkbenchPlugin
{
	[Attribute(defvalue: "$WastelandArmaReforger:worlds/")]
	protected ResourceName path;
	
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
		
		if (Workbench.ScriptDialog("World Export Plugin", "", this))
		{
			api.BeginEntityAction();
			
			
			
			api.EndEntityAction();
		}
	}
	
	[ButtonAttribute("Close")]
	bool ButtonClose() {return false;}
	[ButtonAttribute("OK")]
	bool ButtonOk() {return true;}
};