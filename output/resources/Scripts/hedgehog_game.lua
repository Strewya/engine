
reloaded = true;
function game_init(game)
	local st = game.m_scriptCache:loadFromFile(Core.ResourcePath("Scripts/lib.lua"), false);
	st = st and game.m_scriptCache:loadFromFile(Core.ResourcePath("Scripts/asm.lua"), false);
	st = st and game.m_scriptCache:loadFromFile(Core.ResourcePath("Scripts/console.lua"), false);
	
	gActions = {};
	gInputMap = {};
	gState = {};
	
	game.m_player.m_transform.position:set(0, 0);
	game.m_player.m_transform.scale:set(1,1);
	game.m_graphics:setCulling(false);
	
	st = st and onReload(game);
	return st;
end;

function onReload(game)
	reloaded = false;
	
	gInputMap[Core.Keyboard.m_ArrowLeft] = function(event)
		if(event.m_keyboard.m_isDown) then
			gActions.moveLeft = true;
		else
			gActions.moveLeft = nil;
		end;
	end;
	
	gInputMap[Core.Keyboard.m_ArrowRight] = function(event)
		if(event.m_keyboard.m_isDown) then
			gActions.moveRight = true;
		else
			gActions.moveRight = nil;
		end;
	end;
	
	gInputMap[Core.Keyboard.m_ArrowUp] = function(event)
		if(event.m_keyboard.m_isDown and event.m_keyboard.m_previouslyDown == false) then
			gActions.jump = true;
		end;
	end;
	
	gInputMap[Core.Keyboard.m_F1] = function(event)
		if(event.m_keyboard.m_isDown and event.m_keyboard.m_previouslyDown == false) then
			Console.isOpen = not Console.isOpen;
		end;
	end;
	
	gInputMap[Core.Keyboard.m_F2] = function(event)
		if(event.m_keyboard.m_isDown and event.m_keyboard.m_previouslyDown == false) then
			gState.changeProj = true;
		end;
	end;
	
	gState.impulse = 0;
	gState.maxJumpsAvailable = 3;
	gState.jumpsAvailable = gState.maxJumpsAvailable;
	gState.gravity = -5;
	gState.minY = -270;
	gState.proj = "ortho";
	gState.changeProj = false;
	gState.camera = Core.Vec2(0,0);
	gState.animStates = {};
	table.insert(gState.animStates, State("walk"));
	
	return true;
end;

function game_tick(game)
	if(reloaded) then
		onReload(game);
	end;
	
	game.m_graphics:setBackgroundColor(1,1,1);
	parseInput(game.m_input);
	
	local player = game.m_player;
	local gameTimer = game.m_logicTimer;
	
	player.m_mainTimer:updateBy(gameTimer:getDeltaMicros());
	local milis = Core.Time:microsToSeconds(player.m_mainTimer:getCurMicros());
	
	local walk = game.m_animationCache:getAnimationID("walk");
	local sit = game.m_animationCache:getAnimationID("sit");
	local unsit = game.m_animationCache:getAnimationID("unsit");
	
	if(gActions.moveLeft) then
		if(player.m_transform.scale.x > 0) then
			player.m_transform.scale.x = -200;
			Console:add("Moving to the left i see...");
		end;
		player.m_transform.position.x = player.m_transform.position.x - 20;
	end;
	if(gActions.moveRight) then
		if(player.m_transform.scale.x < 0) then
			player.m_transform.scale.x = 200;
			Console:add("Moving to the right i see...");
		end;
		player.m_transform.position.x = player.m_transform.position.x + 20;
	end;
	
	if(gActions.jump == true) then
		gActions.jump = nil;
		if(gState.jumpsAvailable > 0) then
			gState.jumpsAvailable = gState.jumpsAvailable - 1;
			gState.impulse = 34;
			Console:add("Jumpy hedgehog!");
		else
			Console:add("No jumpy more than " .. gState.maxJumpsAvailable .. " times!");
		end;
	end;
	
	player.m_transform.position.y = player.m_transform.position.y + gState.impulse;
	gState.impulse = gState.impulse + gState.gravity;
		
	if(player.m_transform.position.y < gState.minY) then
		gState.impulse = 0;
		player.m_transform.position.y = gState.minY;
		gState.jumpsAvailable = gState.maxJumpsAvailable;
	end;
	
	if(gState.changeProj) then
		gState.changeProj = false;
		if(gState.proj == "ortho") then
			gState.proj = "persp";
			game.m_graphics:setPerspectiveProjection();
		else
			gState.proj = "ortho";
			game.m_graphics:setOrthographicProjection();
		end;
	end;
	
	game.m_graphics:moveCamera(gState.camera, true);
	
	gState.camera.x = gState.camera.x+1;
	local bound = 5;
	if(gState.camera.x > bound) then
		gState.camera.x = -bound;
	end;
	
	--[[
	//the c++ side parses the raw input events, and maps/binds them to game specific states/actions/ranges
	//i should make a special function for this so i can invoke it from Lua for prototyping purposes
	//also, current state of hhog is needed, with transitional data (was sitting, should move, and v.versa)
	
	//input system should receive key, type and bind function which binds the state/action/range of the key to a variable in memory!
	//separate Images into their own cache
	//animation system should be able to start a new animation, and have a trigger for it's end, either repeat or transition or do nothing.
	]]
end;

function game_render(game)
	local tx = Core.Transform();
	tx.position:set(0,200);
	local col = Core.Color();
	col:set(0,0,0.5);
	local text = "Hello from the beautiful land of Lua!!!";
	game.m_graphics:drawText(text, tx, col, 1, true);
	tx.position.y = tx.position.y + 40;
	game.m_graphics:drawText(text, tx, col, 1, false);
	
	col:set(1,1,1);
	local img = game.m_imageCache:getImage(game.m_player.m_animationData.m_imageID);
	game.m_graphics:drawTexturedQuad(game.m_player.m_transform, col, img, img.m_textureID);
	
	Console:draw(game.m_graphics);
end;

--[[

the loop:
1. update the wall clock, and invoke the logic tick 'n' number of times - C++
for every update of logic tick:
2. get input events for this frame - C++
3. map raw input events to game events: buttons to states or actions, axis to ranges - C++
4. update for game objects based on game events
5. update physics
6. update value interpolation system
7. update animation
8. update rendering


]]