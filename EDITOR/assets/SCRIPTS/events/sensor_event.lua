
SensorEvent = {}
SensorEvent.__index = SensorEvent
setmetatable(SensorEvent, Event)

function SensorEvent.new()
	-- Fixed: Call parent constructor first
    local this = Event.new()
    
    -- Add CollisionEvent specific properties
    this.sensorObjectA = nil  -- Fixed: corrected "nill" typo
    this.sensorObjectB = nil  -- Fixed: corrected "nill" typo

	local instance = setmetatable(this, SensorEvent)
	return instance
end

function SensorEvent:Execute()
	for k,v in pairs(self.m_Subscribers) do
		v:onSensorDetection(self.sensorObjectA, self.sensorObjectB)
	end
end

function SensorEvent:EmitEvent(objA, objB)
	self.sensorObjectA = objA
	self.sensorObjectB = objB
	self:Execute()
end