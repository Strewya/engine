
function game_init(game)

end;

function game_tick(game)
	game.m_window:resize(800,600);
	
	local player = game.m_player;
	local gameTimer = game.m_logicTimer;
	
	player.m_mainTimer:updateBy(gameTimer:getDeltaMicros());
	local milis = Core.Time:microsToSeconds(player.m_mainTimer:getCurMicros());
	
	if(milis > 0 and milis < 0.1) then
		player.m_animationData.m_animationID = game.m_animationCache:getAnimationID("walk");
		player.m_animationData.m_time = 0;
	end;
	if(milis >= 3 and milis < 3.1) then
		player.m_animationData.m_animationID = game.m_animationCache:getAnimationID("sit");
		player.m_animationData.m_time = 0;
	end;
	if(milis >= 5 and milis < 5.1) then
		player.m_animationData.m_animationID = game.m_animationCache:getAnimationID("unsit");
		player.m_animationData.m_time = 0;
	end;
	if(milis > 5.2) then
		player.m_mainTimer:reset();
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
	col:set(1,1,0.5);
	game.m_graphics:drawText("Hello from the beautiful land of Lua!", tx, col, 1, true);
	tx.position.y = tx.position.y + 40;
	game.m_graphics:drawText("Hello from the beautiful land of Lua!", tx, col, 1, false);
	
	tx.position:set(-200, -50);
	col:set(1,1,1);
	tx.scale:set(200,200);
	local img = game.m_imageCache:getImage(game.m_player.m_animationData.m_imageID);
	local texID = spritesheet.m_textureID;
	game.m_graphics:drawTexturedQuad(tx, col, img, texID);
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