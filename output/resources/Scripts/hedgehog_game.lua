
function game_init(game)
	
end;

function game_tick(game)
	game.m_window:resize(800,600);
	
	local player = game.m_player;
	local gameTimer = game.m_logicTimer;
	
	--player.m_mainTimer:updateBy(gameTimer:getDeltaMicros(), Time.NORMAL_TIME);
	--local milis = Time.microsToMilis(player.m_mainTimer:getCurMicros());
	--[[
	if(milis < 5000) then
		player.m_animationData.m_animationID = game.m_graphics:getAnimation("walk");
	elseif(milis < 7000) then
		player.m_animationData.m_animationID = game.m_graphics:getAnimation("sit");
	elseif(milis < 9000) then
		player.m_animationData.m_animationID = game.m_graphics:getAnimation("unsit");
	elseif(milis < 11000) then
		player.m_mainTimer:reset();
	end;]]
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
	game.m_graphics:drawTexturedQuad(tx, col, game.m_player.m_animationData.m_imageID);
end;


--[[
these are so i can know what stuff was loaded, so i could reload them when the file was changed externally

SheetCache
AnimationCache
ImageCache
ScriptCache
]]