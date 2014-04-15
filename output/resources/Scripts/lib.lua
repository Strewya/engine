
function parseInput(input)
	local cnt = input:getEventCount();
	for i=0, cnt-1 do
		local event = input:getEvent(i);
		if(event.m_type == Core.WE_KEYBOARDKEY) then
			for k,v in pairs(gInputMap) do
				if(event.m_keyboard.m_keyCode == k) then
					v(event);
					break;
				end;
			end;
		end;
	end;
end;

