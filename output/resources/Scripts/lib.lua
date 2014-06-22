
function parseInput(input)
	local cnt = input:getEventCount();
	for i=0, cnt-1 do
		local event = input:getEvent(i);
		local handled = false;
		local layer = #gInput.layerQueue;
		while(not handled and layer >= 1) do
			local l = gInput.layerQueue[layer];
			if(l ~= nil and l[event.m_type] ~= nil) then
				for k,v in pairs(l[event.m_type]) do
					if(v(event)) then
						handled = true;
						break;
					end;
				end;
			end;
			if(handled or l.blocks) then
				break;
			end;
			layer = layer - 1;
		end;
	end;
end;

function addInput(layer, eventType, fn)
	if(gInput[layer] == nil) then return; end;
	if(gInput[layer][eventType] == nil) then
		gInput[layer][eventType] = {};
	end;
	table.insert(gInput[layer][eventType], fn);
end;

function defineLayer(layerName, layerValue, blocksLowerLayers)
	gInput[layerName] = layerValue;
	gInput[layerValue] = {};
	gInput[layerValue].blocks = blocksLowerLayers;
end;

function pushLayer(layerID)
	if(type(gInput[layerID]) == "table") then
		table.insert(gInput.layerQueue, gInput[layerID]);
	end;
end;

function popLayer(layerID)
	if(gInput.layerQueue[#gInput.layerQueue] == gInput[layerID]) then
		table.remove(gInput.layerQueue);
	end;
end;
