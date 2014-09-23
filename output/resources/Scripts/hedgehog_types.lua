
class("Ground");

function Lua.Ground.new()
	local self = {};
	self.m_transform = Core.Transform();
	self.m_transform.position:set(0,-4.5);
	
	self.col = Core.Color(0.7, 0, 0.7);
	
	self.halfSize = Core.Vec2(20, 2);
	
	self.m_collisionRect = Core.Rect();
	self.m_collisionRect.halfWidth = self.halfSize.x;
	self.m_collisionRect.halfHeight = self.halfSize.y;
	
	return self;
end;