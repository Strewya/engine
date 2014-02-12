
function paddleAI(game)
	local ball = game.m_ball;
	local paddle = game.m_rightPaddle;
	local targetVel = 0;
	local maxVel = paddle.m_maxVelocity;

	local py = paddle.m_tf.position.y;
	local phs = paddle.m_size.y/2;
	local by = ball.m_tf.position.y;
	local bvy = ball.m_velocity.y;
		
	if(by > py) then
		if(by > py+phs or by+bvy > py+phs+maxVel) then
			targetVel = maxVel;
		else
			targetVel = by-py;
		end;
	elseif(by < py) then
		if(by < py-phs or by+bvy < py-phs-maxVel) then
			targetVel = -maxVel;
		else
			targetVel = by-py;
		end;
	else
		targetVel = 0;
	end;

	if(targetVel > paddle.m_maxVelocity) then
		targetVel = paddle.m_maxVelocity;
	end;
	paddle.m_targetVelocity = targetVel;
end;