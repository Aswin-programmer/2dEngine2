

run_script("ASSETS/SCRIPTS/TOP_DOWN_GAME/game_data.lua")
run_script("ASSETS/SCRIPTS/TOP_DOWN_GAME/assetDefs.lua")
run_script("ASSETS/SCRIPTS/TOP_DOWN_GAME/entityDefs.lua")
run_script("ASSETS/SCRIPTS/TOP_DOWN_GAME/utilities.lua")
run_script("ASSETS/SCRIPTS/TOP_DOWN_GAME/player.lua")
run_script("ASSETS/SCRIPTS/TOP_DOWN_GAME/projectile.lua")



LoadAssets()

local entity = LoadEntity(PlayerDef["playerIDLE"])
local player = Player:Create({id = entity})


main = {
	[1] = {
		update = function()
			player:UpdatePlayer()
			UpdateProjectiles()
		end
	},
	[2] = {
		render = function()
			
		end
	},
}