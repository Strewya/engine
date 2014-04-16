
Console = {};
Console.content = {};
Console.isOpen = false;
Console.showCount = 10;
Console.tf = Core.Transform();
Console.tf.position:set(-200,200);
Console.size = Core.Vec2(300,160);
Console.fill = Core.Color(0.2, 0.2, 0.2, 0.3);

function Console:add(text)
	table.insert(self.content, 1, text);
	if(#self.content > self.showCount) then
		table.remove(self.content);
	end;
end;

function Console:draw(gfx)
	if(self.isOpen) then
		gfx:drawQuad(self.tf, self.size, self.fill);
		for i,text in ipairs(self.content) do
			local tf = Core.Transform();
			tf.position:set(self.tf.position.x - self.size.x, self.tf.position.y - self.size.y + i*30);
			local tint = Core.Color(1,1,1);
			gfx:drawText(text, tf, tint, 0, false);
		end;
	end;
end;