
if(Console == nil) then
	Console = {};
	Console.content = {};
	Console.isOpen = false;
	Console.tf = Core.Transform();	
end;
Console.showCount = 43;
Console.tf.position:set(-210,222);
Console.size = Core.Vec2(300,400);
Console.fill = Core.Color(0.2, 0.2, 0.2, 0.5);
Console.command = "";
Console.carret = 1;
Console.isCmdMode = false;

function Console:add(text)
	table.insert(self.content, 1, text);
	if(#self.content > self.showCount) then
		table.remove(self.content);
	end;
end;

function Console:enterCommandMode()
	self.isCmdMode = true;
	Console.carret = 1;
	Console.command = "";
end;

function Console:addCmdChar(char)
	if(char >= 32 and char <= 126) then
		self.command = self.command .. string.char(char);
		if(self.carret == #self.command) then
			self.carret = self.carret + 1;
		end;
	elseif(char == Core.Keyboard.m_Backspace) then
		if(self.carret-1 == #self.command) then
			self.carret = self.carret - 1;
		end;
		self.command = self.command:sub(1, #self.command-1);
	end;
end;

function Console:moveCarret(key)
	if(char == Core.Keyboard.m_ArrowLeft) then
		self.carret = math.max(self.carret - 1, 1);
	elseif(char == Core.Keyboard.m_ArrowRight) then
		self.carret = math.min(self.carret+1, #self.command+1);
	end;
end;

function Console:applyCommand()
	self.isCmdMode = false;
	if(self.command ~= "") then
		local chunk, err = loadstring(self.command);
		if(chunk) then
			Console:add("exec: " .. self.command);
			chunk();
		else
			self:add(err);
		end;
		self.command = "";
	end;
end;

function Console:clearCommand()
	self.isCmdMode = false;
	self.command = "";
end;

function Console:draw(gfx)
	if(self.isOpen or self.isCmdMode) then
		gfx:clearCamera();
		gfx:setOrthographicProjection();
		gfx:setTransparencyMode(true);
		gfx:drawQuad(self.tf, self.size, self.fill);
		local tf = Core.Transform();
		local tint = Core.Color(1,1,0);
		tf.scale:set(0.5, 0.5);
		for i,text in ipairs(self.content) do
			tf.position:set(self.tf.position.x - self.size.x + 5, self.tf.position.y - self.size.y + i*15);
			gfx:drawText(text, tf, tint, 0, false);
		end;
		if(self.isCmdMode) then
			tf.position:set(self.tf.position.x - self.size.x + 5, self.tf.position.y - self.size.y);
			local s = self.command:sub(1, self.carret) .. "|" .. self.command:sub(self.carret);
			gfx:drawText(s, tf, tint, 0, false);
		end;
	end;
end;