AssetDefs = 
{
	textures =  
	{
		{ name = "ship", path = "ASSETS/TEXTURES/ship.png"},
		{ name = "ast_1", path = "ASSETS/TEXTURES/asteroid_1.png"},
		{ name = "ast_2", path = "ASSETS/TEXTURES/asteroid_2.png"},
		{ name = "bg", path = "ASSETS/TEXTURES/purple.png"},
		{ name = "proj_1", path = "ASSETS/TEXTURES/laserBlue04.png"},
		{ name = "lives", path = "ASSETS/TEXTURES/playerLife1_blue.png"},
		{ name = "game_over", path = "ASSETS/TEXTURES/game_over.png"},
		{ name = "score", path = "ASSETS/TEXTURES/score.png"},
		{ name = "numbers", path = "ASSETS/TEXTURES/numeral-sheet.png"},
		{ name = "ship_explosion", path = "ASSETS/TEXTURES/explosions_sheet.png"}

	}
}
function LoadAssets()
	for k, v in pairs(AssetDefs.textures) do
		if not AssetManager.add_texture(v.name, v.path) then
			print("Failed to load texture [" ..v.name .."] at path [" ..v.path .."]")
		else
			print("Loaded Texture [" ..v.name .."]")
		end
	end
end