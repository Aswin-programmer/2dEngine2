

TriggerSystem = {}
TriggerSystem.__index = TriggerSystem
setmetatable(TriggerSystem, Subscriber)

function TriggerSystem.new()
	local instance = Subscriber.new()
	local instance = setmetatable({}, TriggerSystem)
	return instance
end

function TriggerSystem:OnCollision(object_a, object_b)
	if object_a.bTrigger and object_b.tag == "player" then 
		self:OnPlayerTriggered(object_a, object_b)
	end 

	if object_b.bTrigger and object_a.tag == "player" then 
		self:OnPlayerTriggered(object_b, object_a)
	end 
end

function TriggerSystem:OnPlayerTriggered(trigger, player)
	--print(trigger.group .. "has been activated by " ..player.tag)
	local playerEntity = Entity(player.entityID)
	local playerTransform = playerEntity:get_component(PhysicsComp)
	--print(playerTransform.position.x)
	--print(playerTransform.position.y)
	playerTransform:set_transform(vec2(528,224))
	playerTransform:set_linear_velocity(vec2(0,0))
end
