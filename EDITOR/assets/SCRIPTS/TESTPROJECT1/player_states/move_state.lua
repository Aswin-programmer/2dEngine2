
MoveState = {}
MoveState.__index = MoveState

function MoveState:Create(character)
    assert(character)
    local this = 
    {
        m_Character = character,
        m_EntityID = character.m_EntityID,
        m_Controller = character.m_Controller,
        m_bPlayerState = ANIMATION_STATE.IDLE,  -- Initialize with IDLE state
        m_JumpSteps = 10,
        m_NumJumps = 0,
        m_bAttacking = false
    }

    -- Initialize character's animation state if not already set
    if character.m_bPlayerState == nil then
        character.m_bPlayerState = ANIMATION_STATE.IDLE
    end

    local state = State("move")
    state:set_variable_table(this)
    state:set_on_enter( function(...) this:OnEnter(...) end )
    state:set_on_exit( function() this:OnExit() end)
    state:set_on_update( function(dt) this:OnUpdate(dt) end)
    state:set_on_render( function() end)

    setmetatable(this, self)
    return state
end

function MoveState:OnEnter(params)
    -- Initialize animation to idle state when entering move state
    local player = Entity(self.m_EntityID)
    local animation = player:get_component(Animation)
    
    -- Set to idle animation
    self.m_Character.m_bPlayerState = ANIMATION_STATE.IDLE
    self.m_bPlayerState = ANIMATION_STATE.IDLE
    animation.num_frames = 2
    animation.frame_offset = 0
    animation.bLooped = true
end

function MoveState:OnUpdate(dt) 
    local player = Entity(self.m_EntityID)
    local physics = player:get_component(PhysicsComp)
    local transform = player:get_component(Transform)
    local sprite = player:get_component(Sprite)
    local animation = player:get_component(Animation)

    local velocity = physics:get_linear_velocity()
    
    -- Better grounded check with a small threshold
    local isGrounded = math.abs(velocity.y) < 0.1
    
    -- Improved movement control
    local moveSpeed = 400
    local horizontalInput = 0
    
    -- Handle movement and animations
    if Keyboard.pressed(KEY_A) then
        horizontalInput = -1
        if self.m_Character.m_bPlayerState ~= ANIMATION_STATE.RUN_LEFT then
            self.m_Character.m_bPlayerState = ANIMATION_STATE.RUN_LEFT
            animation.num_frames = 4
            animation.frame_offset = 2
            animation.bLooped = true
        end
    elseif Keyboard.pressed(KEY_D) then
        horizontalInput = 1
        if self.m_Character.m_bPlayerState ~= ANIMATION_STATE.RUN_RIGHT then
            self.m_Character.m_bPlayerState = ANIMATION_STATE.RUN_RIGHT
            animation.num_frames = 4
            animation.frame_offset = 6
            animation.bLooped = true
        end
    else
        -- No movement keys pressed - idle state
        if self.m_Character.m_bPlayerState ~= ANIMATION_STATE.IDLE then
            self.m_Character.m_bPlayerState = ANIMATION_STATE.IDLE
            animation.num_frames = 2
            animation.frame_offset = 0
            animation.bLooped = true
        end
    end
    
    -- Apply horizontal movement
    if horizontalInput == 0 then
        -- Apply friction/damping when not pressing movement keys
        local dampingFactor = 8.0 * dt  -- Adjust for smoother deceleration
        local newVelocityX = velocity.x * (1.0 - dampingFactor)
        
        -- If velocity is very small, just stop completely
        if math.abs(newVelocityX) < 1.0 then
            newVelocityX = 0
        end
        
        physics:set_linear_velocity(vec2(newVelocityX, velocity.y))
    else
        -- Apply movement force - use impulse for more responsive control
        local targetSpeed = horizontalInput * moveSpeed
        local speedDiff = targetSpeed - velocity.x
        local impulse = speedDiff * 0.5  -- Adjust this factor for responsiveness
        
        physics:apply_force_center(vec2(impulse, 0))
    end
    
    -- Improved jump handling
    if Keyboard.pressed(KEY_W) and isGrounded then
        -- Using impulse for more consistent jump
        physics:linear_impulse(vec2(0, -200))
        
    end
    
    -- Better speed cap implementation
    local maxSpeed = 400
    velocity = physics:get_linear_velocity()  -- Get updated velocity after forces
    
    if math.abs(velocity.x) > maxSpeed then
        physics:set_linear_velocity(vec2(maxSpeed * (velocity.x > 0 and 1 or -1), velocity.y))
    end
end

function MoveState:OnExit() end