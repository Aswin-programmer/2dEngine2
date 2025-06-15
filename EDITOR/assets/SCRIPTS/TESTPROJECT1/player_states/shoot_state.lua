ShootState = {}
ShootState.__index = ShootState

function ShootState:Create(character)
	assert(character)
	local this = 
	{
		m_Character = character,
		m_EntityID = character.m_EntityID,
		m_Controller = character.m_Controller,
        m_PlayerState = character.m_bPlayerState,
		m_ProjectilesTables = {}
	}

	local state = State("shoot")
	state:set_variable_table(this)
	state:set_on_enter( function(...) this:OnEnter(...) end )
	state:set_on_exit( function() this:OnExit() end)
	state:set_on_update( function(dt) this:OnUpdate(dt) end)
	state:set_on_render( function() end)

	setmetatable(this, self)
	return state
end

function ShootState:OnEnter(params) end
function ShootState:OnExit() end
function ShootState:OnUpdate(dt) 
	local player = Entity(self.m_EntityID)
    local transform = player:get_component(Transform)

    if Keyboard.just_pressed(KEY_Q) then
		local proj = Projectile:Create(
			{
				def = "regular_shot",
				dir = -1,
				start_pos = transform.position,
				life_time = 1000
			}
		)
		table.insert(self.m_ProjectilesTables, proj)
	elseif Keyboard.just_pressed(KEY_E) then
		local proj = Projectile:Create(
			{
				def = "regular_shot",
				dir = 1,
				start_pos = transform.position,
				life_time = 1000
			}
		)
		table.insert(self.m_ProjectilesTables, proj)
    end

	for i = #self.m_ProjectilesTables, 1, -1 do
		local proj = self.m_ProjectilesTables[i]
		proj:Update(dt)
		if proj:TimesUp() then
			proj:Destroy()
			table.remove(self.m_ProjectilesTables, i)
		end
	end
end

