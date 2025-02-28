AssetDefs = 
{
	textures = 
	{
		{ name = "playerIdle", path = "ASSETS/TOP_DOWN/player/survivor-idle-sheet_enhanced.png"},
		{ name = "playerMove", path = "ASSETS/TOP_DOWN/player/survivor-move_shotgun_0-sheet_enhanced.png"},
		{ name = "proj_1", path = "ASSETS/TOP_DOWN/bullet/bullet.png"}
	}
}

function LoadAssets()
	for k,v in pairs(AssetDefs.textures) do
		if not AssetManager.add_texture(v.name, v.path) then
			print("Failed to load texture ["..v.name.."] at path ["..v.path.."]")
		else
			print("Loaded Texture ["..v.name.."]")
		end
	end
end