
function game_init(game)
	
end;

function game_tick(game)
	game.m_window:resize(800,600);
	game.m_timeScale = 1;
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