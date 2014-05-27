

State = {};
State.__index = State;
setmetatable(State, { __call = function(cls, ...) return cls.new(...) end });

function State.new(name)
	local self = setmetatable({}, State);
	self.fenter = function() end;
	self.fexit = function() end;
	self.fupdate = function() end;
	self.fevent = function() end;
	self.name = name or "UNKNOWN EVENT";
	
	return self;
end;

StateMachine = {};
StateMachine.__index = StateMachine;
setmetatable(StateMachine, { __call = function(cls, ...) return cls.new(...) end });

function StateMachine.new()
	local self = setmetatable({}, StateMachine);
	self.m_current = nil;
	self.m_states = {};
	self.m_queue = {};
	
	return self;
end;

function StateMachine:addState(state)
	self.m_states[state.name] = state;
end;


function StateMachine:setState(name)
	if(self.m_states[name] ~= nil) then
		if(self.m_current ~= nil) then
			self.m_current:onExit(self);
		end;
		self.m_current = self.m_states[name];
		self.m_current:onEnter(self);
	end;
end;

function StateMachine:transition(state)
	self.m_current:onEvent(state, self);
end;

function StateMachine:update(game)
	while(#self.m_queue > 0) do
		self.m_queue[1](game);
		table.remove(self.m_queue, 1);
	end;
	self.m_current:onUpdate(game, self);
end;


function State:onEnter(asm)
	table.insert(asm.m_queue, self.fenter);
end;

function State:onUpdate(game, asm)
	self.fupdate(game, asm);
end;

function State:onExit(asm)
	table.insert(asm.m_queue, self.fexit);
end;

function State:onEvent(state, asm)
	self.fevent(state, asm);
end;