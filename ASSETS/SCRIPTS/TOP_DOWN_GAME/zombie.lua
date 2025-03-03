Zombie = {}
Zombie.__index = Zombie

function Zombie:Create(def)
	local zombie = ZombieDefs[def]
	assert(zombie, "The zombie doesn't exist!")
	local this = 
	{
		m_Def = def,
		m_EntityID = -1,
		m_MinSpeed = zombie.zombie_speed_min,
		m_MaxSpeed = zombie.zombie_speed_max,
		m_Speed = math.random(zombie.zombie_speed_min, zombie.zombie_speed_max),
		m_RotationSpeed = math.random(1,4)
	}
	this.m_EntityID = LoadEntity(zombie)
	local entity = Entity(this.m_EntityID)
	local transform = entity:get_component(Transform)
	transform.position = getRandomZombiePosition()

	setmetatable(this,self)
	return this
end

function Zombie:Update()
    local player = players[1]
    assert(player, "The player doesn't exist!")

    local enemyEntity = Entity(self.m_EntityID)
    local enemyTransform = enemyEntity:get_component(Transform)

    local playerEntity = Entity(player.m_EntityID)
    local playerTransform = playerEntity:get_component(Transform)

    -- Step 1: Get vector from enemy to player
    local vectorBWPlayerEnemy = vec2(
        playerTransform.position.x - enemyTransform.position.x,
        playerTransform.position.y - enemyTransform.position.y
    )

    local distance = vectorBWPlayerEnemy:length()

    -- Step 2: If too close, maintain distance but don't match player's rotation
    if (distance < 10) then
        return
    end
    
    print(distance)

    -- Step 3: Normalize direction but introduce a slight delay in turning
    local direction = vectorBWPlayerEnemy:normalize()

    -- Smoothly adjust rotation instead of instantly matching the player
    local targetRotation = math.deg(math.atan2(direction.y, direction.x))

    if distance > 100 then
        -- Interpolation to smooth rotation (prevents instant alignment)
        local rotationSpeed = 2 -- Adjust this for slower or faster reaction
        enemyTransform.rotation = enemyTransform.rotation + (targetRotation - enemyTransform.rotation) * (rotationSpeed * 0.1)
    end
    
    -- Step 4: Move towards the player
    enemyTransform.position.x = enemyTransform.position.x + (direction.x * self.m_Speed)
    enemyTransform.position.y = enemyTransform.position.y + (direction.y * self.m_Speed)
end
