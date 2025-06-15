
CollisionEvent = {}
CollisionEvent.__index = CollisionEvent
setmetatable(CollisionEvent, Event)

function CollisionEvent.new()

	-- Fixed: Call parent constructor first
    local this = Event.new()
    
    -- Add CollisionEvent specific properties
    this.object_a = nil  -- Fixed: corrected "nill" typo
    this.object_b = nil  -- Fixed: corrected "nill" typo

	local instance = setmetatable(this, CollisionEvent)
	return instance
end

function CollisionEvent:Execute()
	for k, v in pairs(self.m_Subscribers) do 
		v:OnCollision(self.object_a, self.object_b)
	end
end

function CollisionEvent:EmitEvent(obj_a, obj_b)
	self.object_a = obj_a 
	self.object_b = obj_b 
	self:Execute()
end
