Player = {}
Player.__index = Player
function Player:Create(def)
	local this = 
	{
		m_EntityID = def.id,
		m_ForwardSpeed = def.fwd_speed or 5,
		m_AngularSpeed = def.ang_speed or 4,
		m_State = playerState.IDLE,
		needIDLE_Enable = true,
		needMOVE_Enable = false
	}

	setmetatable(this,self)
	return this
end

function Player:UpdatePlayerIdleAnimation()
	local player = Entity(self.m_EntityID)
	local animation = player:get_component(Animation)
	animation.num_frames = 20
	animation.frame_rate = 15
	animation.bLooped = true
	animation:reset()

	local sprite = player:get_component(Sprite)
	sprite.texture_name = "playerIdle"
	sprite.width = 200
	sprite.height = 132
	sprite:generate_uvs()
end

function Player:UpdatePlayerMoveAnimation()
	local player = Entity(self.m_EntityID)
	local animation = player:get_component(Animation)
	animation.num_frames = 20
	animation.frame_rate = 15
	animation.bLooped = true

	local sprite = player:get_component(Sprite)
	sprite.texture_name = "playerMove"
	sprite.width = 200
	sprite.height = 132
	sprite:generate_uvs()

end

function Player:UpdatePlayer()
	if self.m_State == playerState.IDLE and self.needIDLE_Enable then
		self:UpdatePlayerIdleAnimation();
	end

	if self.m_State == playerState.IDLE and self.needMOVE_Enable then
		self:UpdatePlayerMoveAnimation();
	end
	
	local player = Entity(self.m_EntityID)
	local transform = player:get_component(Transform)
	local sprite = player:get_component(Sprite)
	local forward = vec2(
		math.cos(math.rad(transform.rotation)),
		math.sin(math.rad(transform.rotation))
	)

	if Keyboard.pressed(KEY_A) then
		transform.rotation = transform.rotation - self.m_AngularSpeed
	elseif Keyboard.pressed(KEY_D) then
		transform.rotation = transform.rotation + self.m_AngularSpeed
	end

	if Keyboard.pressed(KEY_W) then
		transform.position = transform.position + forward * self.m_ForwardSpeed
		self.needMOVE_Enable = true
		self.needIDLE_Enable = false
	elseif Keyboard.pressed(KEY_S) then
		transform.position = transform.position - forward * self.m_ForwardSpeed
		self.needMOVE_Enable = true
		self.needIDLE_Enable = false
	else
		self.needMOVE_Enable = false
		self.needIDLE_Enable = true
	end

	if Keyboard.pressed(KEY_SPACE) then
		local projectile = Projectile:Create(
			{
				def = "proj1",
				dir = forward,
				start_pos = vec2(
					transform.position.x + sprite.width/2,
					transform.position.y + sprite.height/2
				),
				rotation = transform.rotation
			}
		)
		AddProjectile(projectile)
	end
end