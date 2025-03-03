---------------------------------------------------------------------------
--Helper Functions
----------------------------------------------------------------------------
players = {}
function AddPlayer(player)
	table.insert(players,player)
end


---------------------------------------------------------------------------
--Helper Functions
----------------------------------------------------------------------------

function LoadEntity(def)
	assert(def,"Def doesn't exist")

	local tag = ""
	if def.tag then
		tag = def.tag
	end

	local group = ""
	if def.group then
		group = def.group
	end

	local newEntity = Entity(tag,group)

	if def.components.transform then
		newEntity:add_component(
			Transform(
				vec2(
					def.components.transform.position.x,
					def.components.transform.position.y
				),
				vec2(
					def.components.transform.scale.x,
					def.components.transform.scale.y
				),
				def.components.transform.rotation
			)
		)
	end

	if def.components.sprite then
		local sprite = newEntity:add_component(
			Sprite(
				def.components.sprite.asset_name,
				def.components.sprite.width,
				def.components.sprite.height,
				def.components.sprite.start_x,
				def.components.sprite.start_y,
				def.components.sprite.layer
			)
		)
		sprite.bHidden = def.components.sprite.bHidden or false
		sprite:generate_uvs()
	end

	if def.components.animation then
		newEntity:add_component(
			Animation(
				def.components.animation.num_frames,
				def.components.animation.frame_rate,
				def.components.animation.frame_offset,
				def.components.animation.bVertical,
				def.components.animation.bLooped
			)
		)
	end

	return newEntity:id()

end

------------------------------------------------------------------------
-------------PROJECTILES-----------------------------------------------
--------------------------------------------------------------------------
Projectiles = {}

function AddProjectile(projectile)
	table.insert(Projectiles,projectile)
end

function UpdateProjectiles()
	for k,v in pairs(Projectiles) do
		if v:TimesUp() then
			v:Destroy()
			Projectiles[k] = nil
		else
			v:Update()
		end
	end
end

------------------------------------------------------------------------
-------------ZOMBIE-----------------------------------------------
--------------------------------------------------------------------------
zombies = {}
function AddZombies(zombie)
	table.insert(zombies,zombie)
end

function getRandomZombiePosition()
    local side = math.random(4) -- Pick a random side (1=Left, 2=Right, 3=Top, 4=Bottom)
    local offset = 100 -- Distance away from screen

    if side == 1 then  -- Left
        return vec2(-offset, math.random(WINDOW_HEIGHT))
    elseif side == 2 then  -- Right
        return vec2(WINDOW_WIDTH + offset, math.random(WINDOW_HEIGHT))
    elseif side == 3 then  -- Top
        return vec2(math.random(WINDOW_WIDTH), -offset)
    else  -- Bottom
        return vec2(math.random(WINDOW_WIDTH), WINDOW_HEIGHT + offset)
    end
end


zombieTimer = Timer()
zombieTimer:start()
function SpamZombies()
	if zombieTimer:elapsed_sec() > 2 then
		local zombie = Zombie:Create("Zombie")
		AddZombies(zombie)
		zombieTimer:restart()
	end
end

function updateZombies()
	for k,v in pairs(zombies) do
		v:Update()
	end
end