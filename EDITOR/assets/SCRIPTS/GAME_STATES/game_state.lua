ANIMATION_STATE = 
{
    IDLE = 0,
    RUN_LEFT = 1,
    RUN_RIGHT = 2,
    JUMP = 4
}

GameState = {}
GameState.__index = GameState

function GameState:Create(stack)
	local this = 
	{
		m_Stack = stack,
        playerAnimationState = ANIMATION_STATE.IDLE
	}

	local state = State("game state")
 	state:set_variable_table(this)
 	state:set_on_enter( function() this:OnEnter() end )
 	state:set_on_exit( function() this:OnExit() end )
 	state:set_on_update( function(dt) this:OnUpdate(dt) end )
 	state:set_on_render( function() this:OnRender() end )
 	state:set_handle_inputs( function() this:HandleInputs() end )
 
 	setmetatable(this, self)
 
 	this:Initialize()
 
 	return state
end

function GameState:Initialize()
    -- Create events and systems for triggers
 	if not gCollisionEvent then 
 		gCollisionEvent = CollisionEvent:new()
 	end 
 
 	if not gTriggerSystem then 
 		gTriggerSystem = TriggerSystem:new()
 	end 
 
 	gCollisionEvent:SubscribeToEvent(gTriggerSystem)

    if not gSensorEvent then 
 		gSensorEvent = SensorEvent:new()
 	end 
 
 	if not gSensorTriggerSystem then 
 		gSensorTriggerSystem = SensorTriggerSystem:new()
 	end 
 
 	gSensorEvent:SubscribeToEvent(gSensorTriggerSystem)

     -- Create the player
    if not gPlayer then
    	local character = Character:Create({ name = "main_player" })
 		gPlayer = Entity(character.m_EntityID)
 		AddActiveCharacter(gPlayer:id(), character)
    end

    if not rainGen then
       rainGen = RainGenerator:Create()
    end
end

function GameState:OnEnter()
    LoadMap(createFBWGWorld())
end

function GameState:OnExit()
end

function GameState:OnUpdate(dt)
    UpdateActiveCharacters(dt)
    self:UpdateContacts()
    rainGen:Update(dt) 
    --LUA_INFO("H")
end

function GameState:OnRender()
end

function GameState:HandleInputs()
	if Keyboard.just_pressed(KEY_G) then 
        LUA_INFO("Hello")
		self.m_Stack:pop()
		return 
	end
end

function GameState:UpdateContacts()
	local uda, udb = ContactListener.get_user_data()
	if uda and udb then 
		gCollisionEvent:EmitEvent(uda, udb)
	end 

	local allSensorContact = SensorListener:get_user_data()
	for i, contactPair in ipairs(allSensorContact) do
		local dataA = contactPair[1]
		local dataB = contactPair[2]
		gSensorEvent:EmitEvent(dataA, dataB)
	end

    -- local sensor1, sensor2 = SensorListener:get_user_data()
    -- if sensor1 and sensor2 then
        
    --     --LUA_INFO(sensor1:to_string())
    --     --LUA_INFO(sensor2:to_string())
    -- end

end
