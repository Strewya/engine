
reloaded = true;
function game_init(game)
	local st = game.m_scriptCache:loadFromFile(Core.ResourcePath("Scripts/lib.lua"), false);
	st = st and game.m_scriptCache:loadFromFile(Core.ResourcePath("Scripts/hedgehog_input.lua"), false)
	st = st and game.m_scriptCache:loadFromFile(Core.ResourcePath("Scripts/asm.lua"), false);
	st = st and game.m_scriptCache:loadFromFile(Core.ResourcePath("Scripts/console.lua"), false);
	
	gActions = {};
	gState = {};
	game.m_player.m_transform.position:set(0,0);
	
	st = st and onReload(game);
	return st;
end;

function onReload(game)
	reloaded = false;
	
	setupInput();
	
	game.m_graphics:setBackgroundColor(0.92, 0.94, 0.87);
	game.m_player.m_transform.scale:set(0.5,0.5);
	game.m_graphics:setCulling(false);
	
	game.m_prop1.m_imageID = game.m_imageCache:getImageID("apple_0");
	game.m_player.m_imageID = game.m_imageCache:getImageID("idle_00");
	
	game.m_prop1.m_transform.scale:set(0.2,0.2);
	
	game.m_camera:setSpeed(0.01);
	
	gState.moveCamRight = false;
	gState.moveCamLeft = false;
	gState.moveCamUp = false;
	gState.moveCamDown = false;
	gState.moveCamFwd = false;
	gState.moveCamBack = false;
	gState.resetCam = false;
	gState.rotateCam = false;
	gState.mx = 0;
	gState.my = 0;
	gState.rmx = 0;
	gState.rmy = 0;
	gState.impulseStrength = 0.34;
	gState.impulse = 0;
	gState.maxJumpsAvailable = 3;
	gState.jumpsAvailable = gState.maxJumpsAvailable;
	gState.gravity = -0.05;
	gState.minY = -3.4;
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
	
	parseInput(game.m_input);
	
	local player = game.m_player;
	local gameTimer = game.m_logicTimer;
	
	if(gActions.moveLeft) then
		if(player.m_transform.scale.x > 0) then
			player.m_transform.scale.x = -player.m_transform.scale.x;
			Console:add("DON'T GO BACK!");
		end;
		player.m_transform.position.x = player.m_transform.position.x - 0.2;
	end;
	if(gActions.moveRight) then
		if(player.m_transform.scale.x < 0) then
			player.m_transform.scale.x = -player.m_transform.scale.x;
			Console:add("You're a right mover!");
		end;
		player.m_transform.position.x = player.m_transform.position.x + 0.2;
	end;
	
	if(gActions.jump == true) then
		gActions.jump = nil;
		if(gState.jumpsAvailable > 0) then
			gState.jumpsAvailable = gState.jumpsAvailable - 1;
			gState.impulse = gState.impulseStrength;
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
	
	if(gState.moveCamLeft) then	
		game.m_camera:move(Core.Vec3(-0.1, 0, 0));
		Console:add("moving the camera left");
	end;
	if(gState.moveCamRight) then
		game.m_camera:move(Core.Vec3(0.1, 0, 0));
		Console:add("moving the camera right");
	end;
	if(gState.moveCamUp) then
		game.m_camera:move(Core.Vec3(0, 0.1, 0));
		Console:add("moving the camera up");
	end;
	if(gState.moveCamDown) then
		game.m_camera:move(Core.Vec3(0, -0.1, 0));
		Console:add("moving the camera down");
	end;
	if(gState.moveCamFwd) then
		game.m_camera:move(Core.Vec3(0, 0, 0.5));
		Console:add("zooming in");
		gState.moveCamFwd = false;
	end;
	if(gState.moveCamBack) then
		game.m_camera:move(Core.Vec3(0, 0, -0.5));
		Console:add("zooming out");
		gState.moveCamBack = false;
	end;
	if(gState.resetCam) then
		Console:add("resetting camera");
		game.m_camera:setPosition(Core.Vec3(0,0,-10));
		game.m_camera:setRotation(Core.Vec3(0,0,0));
		gState.resetCam = false;
	end;
	if(gState.rotateCam) then
		game.m_camera:rotate(Core.Vec3(gState.rmy, gState.rmx, 0));
		gState.rmx = 0;
		gState.rmy = 0;
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
	game.m_graphics:setPerspectiveProjection();
	local col = Core.Color();
	col:set(1,1,1);
	game.m_graphics:applyCamera(game.m_camera);
	
	local img = game.m_imageCache:getImage(game.m_prop1.m_imageID);
	game.m_graphics:drawTexturedQuad(game.m_prop1.m_transform, col, img);
	
	img = game.m_imageCache:getImage(game.m_player.m_imageID);
	game.m_graphics:drawTexturedQuad(game.m_player.m_transform, col, img);
	
	
	
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