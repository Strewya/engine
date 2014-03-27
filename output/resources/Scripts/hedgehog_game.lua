
function game_tick(game)
	game.m_window:resize(800,600);
	
	local player = game.m_player;
	local gameTimer = game.m_logicTimer;
	
	player.m_mainTimer:updateBy(gameTimer:getDeltaMicros(), Core.Time.NORMAL_TIME);
	local milis = Core.Time:microsToMilis(player.m_mainTimer:getCurMicros());
	
	if(milis > 0 and milis < 100) then
		player.m_animationData.m_animationID = game.m_animationCache:getAnimationID("walk");
		player.m_animationData.m_time = 0;
	end;
	if(milis >= 3000 and milis < 3100) then
		player.m_animationData.m_animationID = game.m_animationCache:getAnimationID("sit");
		player.m_animationData.m_time = 0;
	end;
	if(milis >= 5000 and milis < 5100) then
		player.m_animationData.m_animationID = game.m_animationCache:getAnimationID("unsit");
		player.m_animationData.m_time = 0;
	end;
	if(milis > 5500) then
		player.m_mainTimer:reset();
	end;
end;

function game_render(game)
	local tx = Core.Transform();
	tx.position:set(0,0);
	local col = Core.Color();
	col:set(1,1,0.5);
	game.m_graphics:drawText("ivana je luda malo.", tx, col, 1);
	
	tx.position:set(-200, -50);
	col:set(1,1,1);
	tx.scale:set(200,200);
	local animID = game.m_player.m_animationData.m_animationID;
	local animation = game.m_animationCache:getAnimation(animID);
	local spritesheet = game.m_spritesheetCache:getSpritesheet(animation.m_spritesheetID);
	local img = spritesheet:getImage(game.m_player.m_animationData.m_imageID);
	local texID = spritesheet.m_textureID;
	game.m_graphics:drawTexturedQuad(tx, col, img, texID);
end;

