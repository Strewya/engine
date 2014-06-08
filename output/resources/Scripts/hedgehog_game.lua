
reloaded = true;
function game_init(game)
	gState = {};
	gActions = {};
	
	local st = game.m_scriptCache:loadFromFile(Core.ResourcePath("Scripts/lib.lua"), false);
	st = st and game.m_scriptCache:loadFromFile(Core.ResourcePath("Scripts/console.lua"), false);
	st = st and game.m_scriptCache:loadFromFile(Core.ResourcePath("Scripts/asm.lua"), false);
	
	st = st and game.m_scriptCache:loadFromFile(Core.ResourcePath("Scripts/hedgehog_globals.lua"), false);
	st = st and game.m_scriptCache:loadFromFile(Core.ResourcePath("Scripts/hedgehog_input.lua"), false);
	st = st and game.m_scriptCache:loadFromFile(Core.ResourcePath("Scripts/hedgehog_asm.lua"), false);
	
	
	
	game.m_player.m_transform.position:set(0,0);
	
	st = st and onReload(game);
	return st;
end;

function onReload(game)
	reloaded = false;
	
	setupInput();
	
	game.m_graphics:setBackgroundColor(0.92, 0.94, 0.87);
	game.m_graphics:setCulling(false);
	
	game.m_camera:setSpeed(0.01);
	
	gState.camera = Core.Vec2(0,0);
	gState.rand = Core.Random(os.time());
	gState.asm = StateMachine();
	
	makeStates(gState.asm);
	gState.asm:setState("idle");
	
	gState.timer = Core.Time();
	gState.timer:reset();
	
	game.m_player.m_imageID = game.m_imageCache:getImageID("idle_00");
	game.m_player.m_transform.scale:set(1,1);
	
	return true;
end;

function doMovement(dt, position)
	gState.velocity = gState.velocity + gState.acceleration;
	if(gState.velocity > gState.maxVelocity) then
		gState.velocity = gState.maxVelocity;
	end;
	position.x = position.x + gState.direction*gState.velocity*dt;
end;

function intersection(a, b)
	local r1, r2 = Core.Rect(), Core.Rect();
	r1.center = a.m_collisionRect.center;
	r1.center.x = r1.center.x*a.m_transform.scale.x;
	r1.center = r1.center + a.m_transform.position;
	r1.halfWidth = a.m_collisionRect.halfWidth*math.abs(a.m_transform.scale.x);
	r1.halfHeight = a.m_collisionRect.halfHeight*math.abs(a.m_transform.scale.y);
	
	r2.center = b.m_collisionRect.center;
	r2.center.x = r2.center.x*b.m_transform.scale.x;
	r2.center = r2.center + b.m_transform.position;
	r2.halfWidth = b.m_collisionRect.halfWidth*math.abs(b.m_transform.scale.x);
	r2.halfHeight = b.m_collisionRect.halfHeight*math.abs(b.m_transform.scale.y);
	
	if(r1:right() < r2:left()) then
		return false;
	end;
	if(r1:left() > r2:right()) then
		return false;
	end;
	if(r1:top() < r2:bottom()) then
		return false;
	end;
	if(r1:bottom() > r2:top()) then
		return false;
	end;
	return true;
end;

function doCollision(game, player)
	for i = #gState.propList, 1, -1 do
		local prop = game:getProp(gState.propList[i]);
		local collides = intersection(player, prop);
		if(collides) then
			game:removeProp(gState.propList[i]);
			table.remove(gState.propList, i);
			gState.spawnCount = gState.spawnCount + 1;
			gState.eatenApples = gState.eatenApples + 1;
		end;
	end;	
end;

function doAppleSpawn(game)
	while(gState.spawnCount > 0) do
		local x, y, a = gState.rand:randFloat()*10-5, gState.rand:randFloat()*3-2, gState.rand:randInt(0,2);
		table.insert(gState.propList, game:createProp());
		local prop = game:getProp(gState.propList[#gState.propList]);
		prop.m_transform.scale:set(0.2,0.2);
		prop.m_transform.position:set(x,y);
		prop.m_imageID = game.m_imageCache:getImageID("apple_"..tostring(a));
		prop.m_collisionRect.halfWidth = 1;
		prop.m_collisionRect.halfHeight = 1;
		gState.spawnCount = gState.spawnCount - 1;
	end;
end;

function game_tick(game)
	if(reloaded) then
		onReload(game);
	end;
	
	parseInput(game.m_input);
	
	local player = game.m_player;
	local gameTimer = game.m_logicTimer;
	local dt = gameTimer:getDeltaTime();
	
	if(gActions.moveLeft) then
		gState.asm:transition("walk");
		if(player.m_transform.scale.x > 0) then
			player.m_transform.scale.x = -player.m_transform.scale.x;
			Console:add("DON'T GO BACK!");
		end;
		gState.direction = -1;
	end;		
	if(gActions.moveRight) then
		gState.asm:transition("walk");
		if(player.m_transform.scale.x < 0) then
			player.m_transform.scale.x = -player.m_transform.scale.x;
			Console:add("You're a right mover!");
		end;
		gState.direction = 1;
	end;
	if(not gActions.moveLeft and not gActions.moveRight) then
		gState.asm:transition("idle");
		gState.direction = 0;
		gState.velocity = 0;
	end;
	doMovement(dt, player.m_transform.position);
	
	if(gActions.jump == true) then
		gActions.jump = nil;
		if(gState.jumpsAvailable > 0) then
			gState.jumpsAvailable = gState.jumpsAvailable - 1;
			gState.yVel = gState.impulseStrength;
			gState.yAcc = 0;
			Console:add("Jumpy hedgehog!");
		else
			Console:add("No jumpy more than " .. gState.maxJumpsAvailable .. " times!");
		end;
	end;
	
	gState.yAcc = gState.yAcc + gState.gravity;
	gState.yVel = gState.yVel + gState.yAcc*dt;
	player.m_transform.position.y = player.m_transform.position.y + gState.yVel*dt;
	
		
	if(player.m_transform.position.y < gState.minY) then
		gState.yAcc = 0;
		gState.yVel = 0;
		player.m_transform.position.y = gState.minY;
		gState.jumpsAvailable = gState.maxJumpsAvailable;
	end;
	
	if(gState.gameOver) then
		if(gActions.resetGame) then
			gActions.resetGame = nil;
			gState.gameOver = false;
			gState.timer:reset();
			gState.eatenApples = 0;
		end;
		if(gActions.resetScore) then
			gActions.resetScore = nil;
			gState.bestTime = 0;
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

	gState.asm:update(game);
	doAppleSpawn(game);
	doCollision(game, player);
	
	if(gState.eatenApples < gState.targetApples) then
		gState.timer:updateBy(gameTimer:getDeltaMicros());
	end;
	
	if(gState.eatenApples >= gState.targetApples and gState.gameOver == false) then
		gState.gameOver = true;
		local timeMicros = gState.timer:getCurMicros();
		if(gState.bestTime == 0 or timeMicros < gState.bestTime) then
			gState.bestTime = timeMicros;
		end;
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
	local textTf = Core.Transform();
	textTf.scale:set(0.7,0.7);
	game.m_graphics:applyCamera(game.m_camera);
	
	for k,v in ipairs(gState.propList) do
		local prop = game:getProp(v);
		local img = game.m_imageCache:getImage(prop.m_imageID);
		col:set(0,0,0);
		game.m_graphics:drawPolygon(prop.m_transform, prop.m_collisionRect, col);
		col:set(1,1,1);
		game.m_graphics:drawTexturedQuad(prop.m_transform, col, img);
		col:set(0,0,0);
		--[[game.m_graphics:setOrthographicProjection();
		textTf.position = prop.m_transform.position;
		textTf.position.x = textTf.position.x*100;
		textTf.position.y = textTf.position.y*100;
		game.m_graphics:drawText(tostring(prop.m_transform.position.x .. "/" .. prop.m_transform.position.y),
								 textTf, col, 0, false);
		game.m_graphics:setPerspectiveProjection();]]
	end;
	
	img = game.m_imageCache:getImage(game.m_player.m_imageID);
	col:set(0,0,0);
	game.m_graphics:drawPolygon(game.m_player.m_transform, game.m_player.m_collisionRect, col);
	col:set(1,1,1);
	game.m_graphics:drawTexturedQuad(game.m_player.m_transform, col, img);
	--[[game.m_graphics:setOrthographicProjection();
	textTf.position = game.m_player.m_transform.position;
	textTf.position.x = textTf.position.x*100;
	textTf.position.y = textTf.position.y*100;
	col:set(0,0,0);
	game.m_graphics:drawText(tostring(game.m_player.m_transform.position.x .. "/" .. game.m_player.m_transform.position.y),
							 textTf, col, 0, false);
	game.m_graphics:setPerspectiveProjection();]]
	
	game.m_graphics:setOrthographicProjection();
	local text = "Eat "..gState.targetApples.." apples as fast as you can! You ate " .. gState.eatenApples .. " apples";
	textTf.position:set(-game.m_window:getSizeX()/2+5,game.m_window:getSizeY()/2-20);
	col:set(0,0,0);
	game.m_graphics:drawText(text, textTf, col, 0, false);
	textTf.position.y = textTf.position.y - 20;
	text = "Best time:  " .. makeTimeStringFromMicros(gState.bestTime);
	game.m_graphics:drawText(text, textTf, col, 0, false);
	textTf.position.y = textTf.position.y - 20;
	local currentTime = gState.timer:getCurMicros();
	text = "Your time:  " .. makeTimeStringFromMicros(currentTime);
	if(gState.gameOver and currentTime == gState.bestTime) then
		text = text .. "   NEW BEST!";
	end;
	game.m_graphics:drawText(text, textTf, col, 0, false);
	if(gState.gameOver) then
		textTf.position.y = textTf.position.y - 20;
		text = "Press R to try again!";
		game.m_graphics:drawText(text, textTf, col, 0, false);
		
		textTf.position.y = textTf.position.y - 20;
		text = "Press Q to reset the score!";
		game.m_graphics:drawText(text, textTf, col, 0, false);
	end;
	
	Console:draw(game.m_graphics);
end;

function makeTimeStringFromMicros(micros)
	local minutes = Core.Time:countMinutesInMicros(micros);
	local seconds = Core.Time:countSecondsInMicros(micros)-60*minutes;
	local milis = Core.Time:countMilisInMicros(micros)-1000*seconds - 60000*minutes;
	return string.format("%02d:%02d:%03d", minutes, seconds, milis);
end;