
if(Console == nil) then
	Console = {};
	Console.content = {};
	Console.commandHistory = {};
	
	Console.isOpen = false;
	Console.tf = Core.Transform();	
end;
Console.showCount = 35;
Console.tf.position:set(-210,222);
Console.size = Core.Vec2(300,400);
Console.fill = Core.Color(0.2, 0.2, 0.2, 0.5);
Console.command = "";
Console.carret = 1;
Console.commandHistorySize = 30;
Console.commandHistoryIndex = 0;
Console.commandHistory[0] = "";
Console.isCmdMode = false;

function Console:add(text)
	table.insert(self.content, 1, text);
	while(#self.content > self.showCount) do
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
		self.command = self.command:sub(0,self.carret-1) .. string.char(char) .. self.command:sub(self.carret);
		self.carret = math.min(self.carret + 1, #self.command+1);
	elseif(char == Core.Keyboard.m_Backspace) then
		self.command = self.command:sub(0, math.max(self.carret-2, 0)) .. self.command:sub(self.carret);
		self.carret = math.max(self.carret - 1, 1);
	end;
end;

function Console:moveCarret(key)
	if(key == Core.Keyboard.m_ArrowLeft) then
		self.carret = math.max(self.carret - 1, 1);
	elseif(key == Core.Keyboard.m_ArrowRight) then
		self.carret = math.min(self.carret+1, #self.command+1);
	elseif(key == Core.Keyboard.m_Delete) then
		self.command = self.command:sub(0,self.carret-1) .. self.command:sub(self.carret+1);
	elseif(key == Core.Keyboard.m_Home) then
		self.carret = 1;
	elseif(key == Core.Keyboard.m_End) then
		self.carret = #self.command+1;
	elseif(key == Core.Keyboard.m_ArrowUp) then
		self.commandHistoryIndex = math.min(self.commandHistoryIndex + 1, #self.commandHistory);
		self.command = self.commandHistory[self.commandHistoryIndex];
		self.carret = #self.command+1;
	elseif(key == Core.Keyboard.m_ArrowDown) then
		self.commandHistoryIndex = math.max(self.commandHistoryIndex - 1, 0);
		self.command = self.commandHistory[self.commandHistoryIndex];
		self.carret = #self.command+1;
	end;
end;

function Console:applyCommand()
	self.isCmdMode = false;
	if(self.command ~= "") then
		table.insert(self.commandHistory, 1, self.command);
		while(#Console.commandHistory > Console.commandHistorySize) do
			table.remove(Console.commandHistory);
		end;
		self.commandHistoryIndex = 0;
		local chunk, err = loadstring(self.command);
		if(chunk) then
			local result, err = pcall(chunk);
			if(result) then
				Console:add("exec: " .. self.command);
			else
				Console:add("Failed to exec: " .. self.command);
			end;
		else
			self:add(err);
		end;
		self.command = "";
		self.carret = 1;
	end;
end;

function Console:clearCommand()
	self.isCmdMode = false;
	self.command = "";
end;

function Console:draw(gfx, fontID)
	if(self.isOpen or self.isCmdMode) then
		gfx:clearCamera();
		gfx:setOrthographicProjection();
		gfx:setTransparencyMode(true);
		gfx:drawQuad(self.tf, self.size, self.fill);
		local tf = Core.Transform();
		local tint = Core.Color(1,1,0);
		tf.scale:set(0.5, 0.5);
		tf.position.x = self.tf.position.x - self.size.x + 5;
		for i,text in ipairs(self.content) do
			tf.position.y = self.tf.position.y - self.size.y + 25 + i*15
			gfx:drawText(fontID, text, tf, tint, 0, false);
		end;
		if(self.isCmdMode) then
			tf.position.y = self.tf.position.y - self.size.y + 10;
			local s = self.command:sub(0, self.carret-1) .. "|" .. self.command:sub(self.carret);
			gfx:drawText(fontID, s, tf, tint, 0, false);
		end;
	end;
end;