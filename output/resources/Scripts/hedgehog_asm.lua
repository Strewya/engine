
function makeStates(asm)
	local s = State("idle");
	function s.fenter(game)
		game.m_player.m_imageID = game.m_imageCache:getImageID("idle_00");
		game.m_player.m_collisionRect.center:set(-0.25,0.1);
		game.m_player.m_collisionRect.halfWidth = 0.5;
		game.m_player.m_collisionRect.halfHeight = 0.7;
	end;
	function s.fevent(event, asm)
		if(event == "walk") then
			asm:setState("idle_to_walk");
		end;
	end;
	asm:addState(s);
	
	s = State("idle_to_walk");
	function s.fenter(game)
		game.m_animation:startAnimation(game.m_player.m_animationPlayerID, game.m_animationCache:getAnimationID("idle_to_walk"), 3);
	end;
	function s.fupdate(game, asm)
		if(game.m_animation:isRunning(game.m_player.m_animationPlayerID) == false) then
			asm:setState("walk");
		end;
	end;
	function s.fevent(event, asm)
		if(event == "idle") then
			asm:setState("idle");
		end;
	end;
	asm:addState(s);
	
	s = State("walk");
	function s.fenter(game)
		game.m_animation:startAnimation(game.m_player.m_animationPlayerID, game.m_animationCache:getAnimationID("walk"), 1);
		game.m_player.m_collisionRect.center:set(0,-0.25);
		game.m_player.m_collisionRect.halfWidth = 0.7;
		game.m_player.m_collisionRect.halfHeight = 0.4;
	end;
	function s.fevent(event, asm)
		if(event == "idle") then
			asm:setState("walk_to_idle");
		end;
	end;
	asm:addState(s);
	
	s = State("walk_to_idle");
	function s.fenter(game)
		game.m_animation:startAnimation(game.m_player.m_animationPlayerID, game.m_animationCache:getAnimationID("idle_to_walk"), -3);
	end;
	function s.fupdate(game, asm)
		if(not game.m_animation:isRunning(game.m_player.m_animationPlayerID)) then
			asm:setState("idle");
		end;
	end;
	function s.fevent(event, asm)
		if(event == "walk") then
			asm:setState("walk");
		end;
	end;
	asm:addState(s);
	
end;

makeStates(gState.asm);