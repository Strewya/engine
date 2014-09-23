
function setupInput()
	gInput = {};
	gInput.layerQueue = {};
	defineLayer("GAMEPLAY",  1, true);
	defineLayer("CMD_PROMPT",  2, true);
	-- default layer
	Console:add("adding gameplay to layer queue");
	pushLayer(gInput.GAMEPLAY);
	
	addInput(gInput.GAMEPLAY, Core.WE_KEYBOARDKEY, function(event)
		if(Core.Keyboard.m_Enter ~= event.m_keyboard.m_keyCode) then return false; end;
		
		if(event.m_keyboard.m_isDown and not event.m_keyboard.m_previouslyDown) then
			Console:enterCommandMode();
			pushLayer(gInput.CMD_PROMPT);
		end;
	end);
	
	addInput(gInput.CMD_PROMPT, Core.WE_KEYBOARDKEY, function(event)
		if(Core.Keyboard.m_Enter ~= event.m_keyboard.m_keyCode) then return false; end;
		
		if(event.m_keyboard.m_isDown and not event.m_keyboard.m_previouslyDown) then
			Console:applyCommand();
			popLayer(gInput.CMD_PROMPT);
		end;
	end);
	
	addInput(gInput.CMD_PROMPT, Core.WE_KEYBOARDKEY, function(event)
		if(Core.Keyboard.m_Escape ~= event.m_keyboard.m_keyCode) then return false; end;
		
		if(event.m_keyboard.m_isDown and not event.m_keyboard.m_previouslyDown) then
			Console:clearCommand();
			popLayer(gInput.CMD_PROMPT);
		end;
	end);
	
	addInput(gInput.CMD_PROMPT, Core.WE_KEYBOARDTEXT, function(event)
		Console:addCmdChar(event.m_keyboard.m_keyCode);
		return true;
	end);
	
	addInput(gInput.CMD_PROMPT, Core.WE_KEYBOARDKEY, function(event)
		if(event.m_keyboard.m_isDown) then
			Console:moveCarret(event.m_keyboard.m_keyCode);
		end;
	end);
	
	addInput(gInput.GAMEPLAY, Core.WE_KEYBOARDKEY, function(event)
		if(Core.Keyboard.m_R ~= event.m_keyboard.m_keyCode) then return false; end;
		if(event.m_keyboard.m_isDown and event.m_keyboard.m_previouslyDown == false) then
			gActions.resetGame = true;
		end;
	end);
	
	addInput(gInput.GAMEPLAY, Core.WE_KEYBOARDKEY, function(event)
		if(Core.Keyboard.m_Q ~= event.m_keyboard.m_keyCode) then return false; end;
		if(event.m_keyboard.m_isDown and event.m_keyboard.m_previouslyDown == false) then
			gActions.resetScore = true;
		end;
	end);
	
	addInput(gInput.GAMEPLAY, Core.WE_KEYBOARDKEY, function(event)
		if(Core.Keyboard.m_ArrowLeft ~= event.m_keyboard.m_keyCode) then return false; end;
		if(event.m_keyboard.m_isDown) then
			gActions.moveLeft = true;
		else
			gActions.moveLeft = nil;
		end;
	end);
	
	addInput(gInput.GAMEPLAY, Core.WE_GAMEPADBUTTON, function(event)
		if(Core.Gamepad.m_DPadLeft ~= event.m_gamepadButton.m_button) then return false; end;
		if(event.m_gamepadButton.m_isDown) then
			gActions.moveLeft = true;
		else
			gActions.moveLeft = nil;
		end;
	end);
	
	addInput(gInput.GAMEPLAY, Core.WE_KEYBOARDKEY, function(event)
		if(Core.Keyboard.m_ArrowRight ~= event.m_keyboard.m_keyCode) then return false; end;
		if(event.m_keyboard.m_isDown) then
			gActions.moveRight = true;
		else
			gActions.moveRight = nil;
		end;
	end);
	
	addInput(gInput.GAMEPLAY, Core.WE_GAMEPADBUTTON, function(event)
		if(Core.Gamepad.m_DPadRight ~= event.m_gamepadButton.m_button) then return false; end;
		if(event.m_gamepadButton.m_isDown) then
			gActions.moveRight = true;
		else
			gActions.moveRight = nil;
		end;
	end);
	
	addInput(gInput.GAMEPLAY, Core.WE_GAMEPADAXIS, function(event)
		if(Core.Gamepad.m_LeftStick ~= event.m_gamepadAxis.m_axis) then return false; end;
		print(event.m_gamepadAxis.m_x);
		if(event.m_gamepadAxis.m_x > 15000) then
			gActions.moveRight = true;
		else
			gActions.moveRight = nil;
		end;
		if(event.m_gamepadAxis.m_x < -15000) then
			gActions.moveLeft = true;
		else
			gActions.moveLeft = nil;
		end;
	end);
	
	addInput(gInput.GAMEPLAY, Core.WE_KEYBOARDKEY, function(event)
		if(Core.Keyboard.m_ArrowUp ~= event.m_keyboard.m_keyCode) then return false; end;
		if(event.m_keyboard.m_isDown and event.m_keyboard.m_previouslyDown == false) then
			gActions.jump = true;
		end;
	end);
	
	addInput(gInput.GAMEPLAY, Core.WE_GAMEPADBUTTON, function(event)
		if(Core.Gamepad.m_BottomButton ~= event.m_gamepadButton.m_button) then return false; end;
		if(event.m_gamepadButton.m_isDown) then
			gActions.jump = true;
		end;
	end);
	
	addInput(gInput.GAMEPLAY, Core.WE_KEYBOARDKEY, function(event)
		if(Core.Keyboard.m_F1 ~= event.m_keyboard.m_keyCode) then return false; end;
		if(event.m_keyboard.m_isDown and event.m_keyboard.m_previouslyDown == false) then
			Console.isOpen = not Console.isOpen;
		end;
	end);
	
	addInput(gInput.GAMEPLAY, Core.WE_KEYBOARDKEY, function(event)
		if(Core.Keyboard.m_A ~= event.m_keyboard.m_keyCode) then return false; end;
		if(event.m_keyboard.m_isDown) then
			gState.moveCamLeft = true;
		else
			gState.moveCamLeft = false;
		end;
	end);
	addInput(gInput.GAMEPLAY, Core.WE_KEYBOARDKEY, function(event)
		if(Core.Keyboard.m_D ~= event.m_keyboard.m_keyCode) then return false; end;
		if(event.m_keyboard.m_isDown) then
			gState.moveCamRight = true;
		else
			gState.moveCamRight = false;
		end;
	end);
	addInput(gInput.GAMEPLAY, Core.WE_KEYBOARDKEY, function(event)
		if(Core.Keyboard.m_W ~= event.m_keyboard.m_keyCode) then return false; end;
		if(event.m_keyboard.m_isDown) then
			gState.moveCamUp = true;
		else
			gState.moveCamUp = false;
		end;
	end);
	addInput(gInput.GAMEPLAY, Core.WE_KEYBOARDKEY, function(event)
		if(Core.Keyboard.m_S ~= event.m_keyboard.m_keyCode) then return false; end;
		if(event.m_keyboard.m_isDown) then
			gState.moveCamDown = true;
		else
			gState.moveCamDown = false;
		end;
	end);
	
	addInput(gInput.GAMEPLAY, Core.WE_MOUSEWHEEL, function(event)
		if(event.m_mouseWheel.m_scroll > 0) then
			gState.moveCamFwd = true;
		elseif(event.m_mouseWheel.m_scroll < 0) then
			gState.moveCamBack = true;
		end;
	end);
	
	addInput(gInput.GAMEPLAY, Core.WE_MOUSEBUTTON, function(event)
		if(Core.Mouse.m_RightButton ~= event.m_mouseButton.m_button) then return false; end;
		if(event.m_mouseButton.m_isDown) then
			gState.resetCam = true;
		end;
	end);
	
	addInput(gInput.GAMEPLAY, Core.WE_KEYBOARDKEY, function(event)
		if(Core.Keyboard.m_Escape ~= event.m_keyboard.m_keyCode) then return false; end;
		gState.close = true;
	end);
	
	addInput(gInput.GAMEPLAY, Core.WE_KEYBOARDKEY, function(event)
		if(Core.Keyboard.m_Space ~= event.m_keyboard.m_keyCode) then return false; end;
		if(event.m_keyboard.m_isDown) then
			gState.pause = not gState.pause;
		end;
	end);
	
	addInput(gInput.GAMEPLAY, Core.WE_KEYBOARDKEY, function(event)
		if(Core.Keyboard.m_F11 ~= event.m_keyboard.m_keyCode) then return false; end;
		if(event.m_keyboard.m_isDown and not event.m_keyboard.m_previouslyDown) then
			gState.step = true;
		end;
	end);
end;