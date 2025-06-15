
Subscriber = {}
Subscriber.__index = Subscriber

function Subscriber.new()
	local instance = setmetatable({}, Subscriber)
	return instance
end

function Subscriber:Update(param)
end

Event = {}
Event.__index = Event

function Event.new()
	this = {
		m_Subscribers = {}
	}
	
	local instance = setmetatable(this, Event)
	return instance
end

function Event:SubscribeToEvent(subscriber)
	table.insert(self.m_Subscribers, subscriber)
end

function Event:Unsubscribe(subscriber)
	for k, v in pairs(self.m_Subscribers) do 
		if v == subscriber then 
			table.remove(self.m_Subscribers, k)
		end 
	end 
end

function Event:Execute()
end 

function Event:ResetSubscribers()
	for k, v in ipairs(self.m_Subscribers) do 
		self.m_Subscribers[k] = nil 
	end
end
