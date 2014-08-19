
class("Ground");

function Lua.Ground.new()
	local self = {};
	self.m_transform = Core.Transform();
	self.m_transform.position:set(0,-4.5);
	
	self.col = Core.Color(1, 1, 0);
	
	self.halfSize = Core.Vec2(20, 2);
	
	self.m_collisionRect = Core.Rect();
	self.m_collisionRect.halfWidth = 20;
	self.m_collisionRect.halfHeight = 2;
	
	return self;
end;