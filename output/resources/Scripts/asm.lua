

State = {};
State.__index = State;
setmetatable(State, { __call = function(cls, ...) return cls.new(...) end });

function State.new(animation)
	local self = setmetatable({}, State);
	self.animation = animation;
	self.transitions = {};
	return self;
end;

function State:getAnimation()
	return self.animation;
end;

function State:setAnimation(animation)
	self.animation = animation;
end;

function State:addTransition(transitionState)
	table.insert(self.transitions, transitionState);
end;


StateMachine = {};
StateMachine.__index = StateMachine;
setmetatable(StateMachine, { __call = function(cls, ...) return cls.new(...) end });

function StateMachine.new()
	local self = {};
	self.states = {};
	
	return self;
end;