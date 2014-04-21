
function parseInput(input)
	local cnt = input:getEventCount();
	for i=0, cnt-1 do
		local event = input:getEvent(i);
		if(gInput[event.m_type] ~= nil) then
			for k,v in pairs(gInput[event.m_type]) do
				if(v(event)) then
					break;
				end;
			end;
		end;
	end;
end;

function addInput(eventType, fn)
	if(gInput[eventType] == nil) then
		gInput[eventType] = {};
	end;
	table.insert(gInput[eventType], fn);
end;