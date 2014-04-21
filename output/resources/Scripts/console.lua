
if(Console == nil) then
	Console = {};
	Console.content = {};
	Console.isOpen = false;
	Console.tf = Core.Transform();	
end;
Console.showCount = 20;
Console.tf.position:set(-210,222);
Console.size = Core.Vec2(300,160);
Console.fill = Core.Color(0.2, 0.2, 0.2, 0.5);

function Console:add(text)
	table.insert(self.content, 1, text);
	if(#self.content > self.showCount) then
		table.remove(self.content);
	end;
end;

function Console:draw(gfx)
	if(self.isOpen) then
		gfx:clearCamera();
		gfx:setOrthographicProjection();
		gfx:setTransparencyMode(true);
		gfx:drawQuad(self.tf, self.size, self.fill);
		gfx:setTransparencyMode(false);
		for i,text in ipairs(self.content) do
			local tf = Core.Transform();
			tf.position:set(self.tf.position.x - self.size.x + 5, self.tf.position.y - self.size.y + i*15);
			tf.scale:set(0.5, 0.5);
			local tint = Core.Color(1,1,0);
			gfx:drawText(text, tf, tint, 0, false);
		end;
	end;
end;