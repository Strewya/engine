local lfs = require('lfs');

function countLines(path)
	local file = io.open(path, "r");
	local count = 0;
	local comment = false;
	for line in file:lines() do
		local mcStart = line:find("/*", 1, true);
		local mcEnd = line:find("*/", 1, true);
		--print("original " .. line);
		if(mcStart ~= nil or mcEnd ~= nil) then
			mcStart = mcStart or 1;
			mcEnd = mcEnd or line:len();
			if(mcEnd == nil) then comment = true; end;
			if(mcStart == nil) then comment = false; end;
			line = line:sub(1, mcStart-1) .. line:sub(mcEnd+2);
		end;
		local scStart = line:find("//", 1, true);
		if(scStart ~= nil) then
			line = line:sub(1, scStart-1);
		end;
		--print("w/o comment " .. line);
		line = line:gsub("%s", "");
		--print("w/o ws " .. line);
		if(comment == false and line:len() > 0) then
			count = count + 1;
			--print("considered");
		end;
	end;
	file:close();
	return count;
end;

function isExcluded(path, exclude)
	for i,v in ipairs(exclude) do
		if(path == v) then
			return true;
		end;
	end;
	return false;
end;

function iterateDirectory(path, exclude, linesPerFile)
	local count = 0;
	for file in lfs.dir(path) do
		if(file ~= "." and file ~= "..") then
			local f = path .. '/' .. file;
			if(not isExcluded(f, exclude)) then
				local mode = lfs.attributes(f, "mode");
				if(mode == "directory") then
					count = count + iterateDirectory(f, exclude, linesPerFile);
				else
					local lpf = countLines(f);
					count = count + lpf;
					linesPerFile[f] = lpf;
				end;
			end;
		end;
	end;
	return count, linesPerFile;
end;

local path = "w:/src";
local exclude = {path .. "/box2D", path .. "/to_lua++"};
local lpf = {};
local count, lpf = iterateDirectory(path, exclude, lpf);
local maxLen = 0;
for k,v in pairs(lpf) do
	if(k:len() > maxLen) then
		maxLen = k:len();
	end;
end;
local nlpf = {};
for k,v in pairs(lpf) do
	while(k:len() < maxLen + 2) do
		k = k .. " ";
	end;
	table.insert(nlpf, k .. tostring(v));
end;
table.sort(nlpf);
for i,v in ipairs(nlpf) do
	print(v);
end;
print(count);