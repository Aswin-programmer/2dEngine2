run_script("ASSETS/SCRIPTS/TESTPROJECT1/script_list.lua")

load_script_table(ScriptList)

LoadAssets(AssetDefs)

gCollisionEvent = nil 
gTriggerSystem = nil 
gSensorEvent = nil
gSensorTriggerEvent = nil
gPlayer = nil 
rainGen = nil

gStateStack = StateStack()

local soundTest = Entity("soundTest", "")
local sound = soundTest:add_component(OpenALSoundComponent())
sound:Play("sample")

local title = TitleState:Create(gStateStack)
gStateStack:change_state(title)

local i = 0

main = {
	[1] = {
		update = function()
			--[[
            rainGen:Update(0.016) 
            UpdatePlayer(gPlayer)
            --gFollowCam:update()

            --Checking some collision and trigger stuff.
			
            local uda, udb = ContactListener.get_user_data()
 			if uda and udb then 
 				gCollisionEvent:EmitEvent(uda, udb)
 			end 
			]]

			-- local sensor1, sensor2 = SensorListener:get_user_data()
			-- if sensor1 and sensor2 then
			-- 	--gSensorEvent:EmitEvent(sensor1, sensor2)
			-- 	sensor1:to_string()
			-- 	sensor2:to_string()
			-- end
			i = i + 1
			--LUA_INFO({}, i)
			if i==500 then 
				soundTest:kill()
			end

            Debug()

			gStateStack:update(0.016)

		end
	},
	[2] = {
		render = function()
			--gStateStack:render()
		end
	},
}