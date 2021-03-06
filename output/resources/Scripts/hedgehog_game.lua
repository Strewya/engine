depend("Scripts/lib.lua");
depend("Scripts/console.lua");
depend("Scripts/asm.lua");
depend("Scripts/hedgehog_asm.lua");
depend("Scripts/hedgehog_globals.lua");
depend("Scripts/hedgehog_input.lua");
depend("Scripts/hedgehog_types.lua");
	
reloaded = true;
function game_init(game)
	gActions = {};

	local tbl = dofile("../resources/Defs/hedgehog.pkg");
	if( tbl ) then
		for pkgName, pkgFiles in pairs(tbl) do
			local pkg = game.m_packageCache:getPackage(pkgName);
			for i, file in ipairs(pkgFiles) do
				pkg:addFile(Core.ResourceFile(file));
			end;
		end;
	end;

	game.m_window:showCursor(true);
	
	game.m_packageLoader:loadPackage("base");
	game.m_packageLoader:loadPackage("gameplay");
	
	gState.propList = {};
	gState.asm = StateMachine();
	gState.rand = Core.Random(os.time());
	gState.timer = Core.Time();
	
	game.m_player.m_transform.position:set(0,0);
	game.m_player.m_transform.scale:set(1,1);
	
	return onReload(game);
end;

function onReload(game)
	reloaded = false;
	gState.ground = Lua.Ground();
	
	setupInput();
	
	game.m_graphicsSystem:setBackgroundColor(0.92, 0.94, 0.87);
	game.m_graphicsSystem:setCulling(false);
	
	game.m_camera:setSpeed(0.01);
	
	makeStates(gState.asm);
	gState.asm:setState("idle");
	
	gState.timer:reset();
	
	return true;
end;

function doMovement(dt, position)
	gState.velocity = gState.velocity + gState.acceleration;
	if(gState.velocity > gState.maxVelocity) then
		gState.velocity = gState.maxVelocity;
	end;
	local dpos = gState.direction*gState.velocity*dt;
	position.x = position.x + dpos;
	return dpos;
end;

function intersection_old(a, b)
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

function intersection(o, p)
	local a, b = Core.Rect(), Core.Rect();
	local oPos = o.m_transform.position;
	local oScale = o.m_transform.scale;
	local oRect = o.m_collisionRect;
	local pPos = p.m_transform.position;
	local pScale = p.m_transform.scale;
	local pRect = p.m_collisionRect;
	
	a.center = oPos + Core.Vec2(oRect.center.x*oScale.x, oRect.center.y*oScale.y);
	a.halfWidth = oRect.halfWidth*math.abs(oScale.x);
	a.halfHeight = oRect.halfHeight*math.abs(oScale.y);
	
	b.center = pPos + Core.Vec2(pRect.center.x*pScale.x, pRect.center.y*pScale.y);
	b.halfWidth = pRect.halfWidth*math.abs(pScale.x);
	b.halfHeight = pRect.halfHeight*math.abs(pScale.y);
	
	local hs = Core.Vec2(a.halfWidth, a.halfHeight) + Core.Vec2(b.halfWidth, b.halfHeight);
	local dist = a.center - b.center;
	if(hs.x <= math.abs(dist.x)) then
		return false;
	end;
	if(hs.y <= math.abs(dist.y)) then
		return false;
	end;
	return true;
end;

function doCollision(game, player)
	for i = #gState.propList, 1, -1 do
		local prop = gState.propList[i];
		local collides = intersection(player, prop);
		if(collides) then
			table.remove(gState.propList, i);
			gState.eatenApples = gState.eatenApples + 1;
		end;
	end;
end;

function doAppleSpawn(game)
	while(#gState.propList < gState.spawnCount) do
		local x, y = gState.rand:randFloat()*gState.treeTopHS.x*2+gState.treeTopPos.x-gState.treeTopHS.x, gState.rand:randFloat()*gState.treeTopHS.y+gState.treeTopPos.y-gState.treeTopHS.y;
		local a, d = gState.rand:randInt(0,2), gState.rand:randInt(0,1);
		if(d == 0) then
			x = -x;
		end;
		table.insert(gState.propList, Core.Prop());
		local prop = gState.propList[#gState.propList];
		prop.m_transform.scale:set(0.2,0.2);
		prop.m_transform.position:set(x,y);
		prop.m_imageID = game.m_imageCache:getResourceID("apple_"..tostring(a));
		prop.m_collisionRect.halfWidth = 1;
		prop.m_collisionRect.halfHeight = 1;
	end;
end;

function game_tick(game)
	if(reloaded) then
		onReload(game);
	end;
	
	parseInput(game.m_inputSystem);
	
	if(gState.close) then
		game.m_window:close();
		game.m_isRunning = false;
		return;
	end;
	
	local player = game.m_player;
	local gameTimer = game.m_logicTimer;
	local dt = gameTimer:getDeltaTime()*gameTimer:getTimeScale();
	
	if(gState.pause) then
		local ts = gameTimer:getTimeScale();
		if(ts > Core.Time.STOP_TIME) then
			gameTimer:setTimeScale(Core.Time.STOP_TIME);
		end;
	else
		local ts = gameTimer:getTimeScale();
		if(ts < Core.Time.NORMAL_TIME) then
			gameTimer:setTimeScale(Core.Time.NORMAL_TIME);
		end;
	end;
	
	if(gState.pause and gState.step) then
		gState.step = false;
		gameTimer:setTimeScale(Core.Time.NORMAL_TIME);
	end;
	
	if(gState.resetHog) then
		gState.resetHog = false;
		player.m_transform.position:set(0,0);
	end;
	
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
	local px, py = player.m_transform.position.x, player.m_transform.position.y;
	
	if(gActions.jump == true) then
		gActions.jump = nil;
		if(gState.jumpsAvailable > 0) then
			gState.asm:transition("jump");
			gState.jumpsAvailable = gState.jumpsAvailable - 1;
			gState.yVel = gState.impulseStrength;
			Console:add("Jumpy hedgehog!");
		else
			Console:add("No jumpy more than " .. gState.maxJumpsAvailable .. " times!");
		end;
	end;
	
	gState.yVel = gState.yVel + gState.gravity*dt;
	player.m_transform.position.y = player.m_transform.position.y + gState.yVel*dt;
	
	if(intersection(player, gState.ground)) then
		gState.yAcc = 0;
		gState.yVel = 0;
		player.m_transform.position.y = gState.ground.m_transform.position.y + gState.ground.m_collisionRect.halfHeight + gState.ground.m_collisionRect.center.y + player.m_collisionRect.halfHeight - player.m_collisionRect.center.y;
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
		game.m_camera:setPosition(Core.Vec3(0,0,-15));
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
		gState.timer:updateBy(gameTimer:getDeltaMicros()*gameTimer:getTimeScale());
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
	
	local col = Core.Color();
	local textTf = Core.Transform();
	local tf = Core.Transform();
	textTf.scale:set(0.01,0.01);
	
	game.m_graphicsSystem:setPerspectiveProjection();
	game.m_graphicsSystem:applyCamera(game.m_camera);
	game.m_graphicsSystem:setTransparencyMode(true);
	
	game.m_graphicsSystem:drawQuad(gState.ground.m_transform, gState.ground.halfSize, gState.ground.col);
	if(gState.drawCollisionRect) then
		game.m_graphicsSystem:drawPolygon(gState.ground.m_transform, gState.ground.m_collisionRect, gState.bboxColor);
	end;
	if(gState.drawPositions) then
		col:set(0,0,0);
		textTf.position = gState.ground.m_transform.position;
		game.m_graphicsSystem:drawText(game.m_defaultFont, tostring(gState.ground.m_transform.position.x .. "/" .. gState.ground.m_transform.position.y), textTf, col, 0, false);
		col:set(1,1,1);
	end;
	
	tf.position = gState.treePos;
	game.m_graphicsSystem:drawQuad(tf, gState.treeHS, gState.treeCol);
	tf.position.x = -tf.position.x;
	game.m_graphicsSystem:drawQuad(tf, gState.treeHS, gState.treeCol);
	
	tf.position = gState.treeTopPos;
	game.m_graphicsSystem:drawQuad(tf, gState.treeTopHS, gState.treeTopCol);
	tf.position.x = -tf.position.x;
	game.m_graphicsSystem:drawQuad(tf, gState.treeTopHS, gState.treeTopCol);
	
	
	for k,v in ipairs(gState.propList) do
		local prop = v;
		local img = game.m_imageCache:getResource(prop.m_imageID);
		if(gState.drawCollisionRect) then
			game.m_graphicsSystem:drawPolygon(prop.m_transform, prop.m_collisionRect, gState.bboxColor);
		end;
		col:set(1,1,1);
		game.m_graphicsSystem:drawTexturedQuad(prop.m_transform, col, img);
		col:set(0,0,0);
		if(gState.drawPositions) then
			textTf.position = prop.m_transform.position;
			game.m_graphicsSystem:drawText(game.m_defaultFont, tostring(prop.m_transform.position.x .. "/" .. prop.m_transform.position.y), textTf, col, 0, false);
		end;
	end;
	
	img = game.m_imageCache:getResource(game.m_player.m_imageID);
	if(gState.drawCollisionRect) then
		game.m_graphicsSystem:drawPolygon(game.m_player.m_transform, game.m_player.m_collisionRect, gState.bboxColor);
	end;
	col:set(1,1,1);
	game.m_graphicsSystem:drawTexturedQuad(game.m_player.m_transform, col, img);
	
	if(gState.drawPositions) then
		textTf.position = game.m_player.m_transform.position;
		col:set(0,0,0);
		game.m_graphicsSystem:drawText(game.m_defaultFont, tostring(game.m_player.m_transform.position.x .. "/" .. game.m_player.m_transform.position.y), textTf, col, 0, false);
	end;
	
	textTf.scale:set(0.7,0.7);
	game.m_graphicsSystem:clearCamera();
	game.m_graphicsSystem:setOrthographicProjection();
	local text = "Eat "..gState.targetApples.." apples as fast as you can! You ate " .. gState.eatenApples .. " apples";
	textTf.position:set(-game.m_window:getSizeX()/2+5,game.m_window:getSizeY()/2-20);
	col:set(0,0,0);
	game.m_graphicsSystem:drawText(game.m_defaultFont, text, textTf, col, 0, false);
	textTf.position.y = textTf.position.y - 20;
	text = "Best time: " .. makeTimeStringFromMicros(gState.bestTime);
	game.m_graphicsSystem:drawText(game.m_defaultFont, text, textTf, col, 0, false);
	textTf.position.y = textTf.position.y - 20;
	local currentTime = gState.timer:getCurMicros();
	text = "Your time: " .. makeTimeStringFromMicros(currentTime);
	if(gState.gameOver and currentTime == gState.bestTime) then
		text = text .. "   NEW BEST!";
	end;
	game.m_graphicsSystem:drawText(game.m_defaultFont, text, textTf, col, 0, false);
	if(gState.gameOver) then
		textTf.position.y = textTf.position.y - 20;
		text = "Press R to try again!";
		game.m_graphicsSystem:drawText(game.m_defaultFont, text, textTf, col, 0, false);
		
		textTf.position.y = textTf.position.y - 20;
		text = "Press Q to reset the score!";
		game.m_graphicsSystem:drawText(game.m_defaultFont, text, textTf, col, 0, false);
	end;
	
	Console:draw(game.m_graphicsSystem, game.m_defaultFont);
end;

function makeTimeStringFromMicros(micros)
	local oneMicroSecond = 1000000;
	local minutes = Core.Time:countMinutesInMicros(micros);
	micros = micros - minutes*60*oneMicroSecond;
	local seconds = Core.Time:countSecondsInMicros(micros);
	micros = micros - seconds*oneMicroSecond;
	local milis = Core.Time:countMilisInMicros(micros);
	return string.format("%02d:%02d:%03d", minutes, seconds, milis);
end;