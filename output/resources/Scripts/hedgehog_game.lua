
function game_init(game)
	
end;

function game_tick(game)
	game.m_window:resize(1024,768);
end;

function game_render(game)
	local tx = Core.Transform();
	tx.position:set(0,0);
	local col = Core.Color();
	col:set(1,1,0.5);
	game.m_graphics:drawText("hello from lua", tx, col, 1);
end;