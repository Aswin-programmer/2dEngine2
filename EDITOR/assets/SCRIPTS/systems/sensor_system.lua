
SensorTriggerSystem = {}
SensorTriggerSystem.__index = SensorTriggerSystem
setmetatable(SensorTriggerSystem, Subscriber)

function SensorTriggerSystem.new()
	local instance = Subscriber.new()
	local instance = setmetatable({}, SensorTriggerSystem)
	return instance
end

function SensorTriggerSystem:onSensorDetection(object_a, object_b)
	if object_a.group == "sensors" and object_b.tag == "player" then 
		self:OnPlayerTriggered(object_a, object_b)
	end 

	if object_b.group == "sensors" and object_a.tag == "player" then 
		self:OnPlayerTriggered(object_b, object_a)
	end 
end

function SensorTriggerSystem:OnPlayerTriggered(trigger, player)
	LUA_INFO("PLAYER GROUNDED!")
end