gProjDir = os.getenv("SGPROJECT");
exec = print;
exec = os.execute;

local lfs = require("lfs");
local copyCmd = [[copy /Y "%s" "%s"]];
local makeFolderCmd = [[xcopy "%s" "%s\" /T /Y]];
local deleteCmd = [[del "%s"]];

function printTable(t)
	local count = 0;
	print("table\n{");
	for key, value in pairs(t) do
		count = count + 1;
		local valueStr = "";
		if(type(value) ~= "table") then valueStr = tostring(value); end;
		print("   " .. tostring(key) .. " -> " .. type(value) .. "("..valueStr..")");
	end;
	print("}\nTotal count: " .. count);
end;

function mergeTables(destination, source, overwrite)
	overwrite = overwrite or true;
	for key, value in pairs(source) do
		if( destination[key] == nil or overwrite ) then
			destination[key] = value;
		end;
	end;
end;

function winPath(path)
	return path:gsub("/", "\\");
end;

function nixPath(path)
	return path:gsub("\\", "/");
end;

function ensure(check, errorTxt)
	if(not check) then
		print(errorTxt);
		os.exit();
	end;
end;

function checkFileExists(file)
	return (lfs.attributes(file) ~= nil);
end;

checkFolderExists = checkFileExists;

function isNewer(src, dst)
	local srcMod = lfs.attributes(src, "modification");
	local dstMod = lfs.attributes(dst, "modification");
	ensure(checkFileExists(src), ("File %s does not exist!"):format(src));
	if(dstMod == nil or srcMod > dstMod) then
		return true;
	end;
	return false;
end;

function copy(src, dst)
	src = winPath(src);
	dst = winPath(dst);
	local lastBackslash = dst:find('\\[^\\]*$');
	local folder = dst:sub(1, lastBackslash-1);
	if(not checkFolderExists(folder)) then
		exec(makeFolderCmd:format(src, folder));
	end;
	exec(copyCmd:format(src, dst));
	return nixPath(dst);
end;

function delete(dst)
	exec(deleteCmd:format(winPath(dst)));
end;

function copyIfNewer(src, dst)	
	if(isNewer(src, dst)) then
		return copy(src, dst);
	end;
	return nil;
end;

function forEachDir(path, fn, ...)
	for file in lfs.dir(path) do
        if(file ~= "." and file ~= "..") then
			local f = path .. '/' .. file;
			local mode = lfs.attributes(f, "mode");
			if(mode == "directory") then
                forEachDir(f, fn, ...);
            else
                fn(path, file, ...);
            end;
		end;
	end;
end;

function findLineContaining(filename, target)
	local f = io.open(filename, "r");
	local i = 1;
	local retval = "";
	for line in f:lines() do
		if(type(target) == "number" and i == target) then
			retval = line;
			break;
		elseif(type(target) == "string" and line:find(target) ~= nil) then
			retval = line;
			break;
		end;
		i = i+1;
	end;
	f:close();
	local _, i = retval:find(target);
	return retval, (i == nil and 0 or i);
end;

function findInTable(t, f)
	for _,v in pairs(t) do
		if(v == f) then
			return true;
		end;
	end;
	return false;
end;

function extractGameName()
	local selector = gProjDir .. "/src/Games/GameSelector.h";
	local gameDef = "#define CORE_GAME_BUILD";

	local game, defEnd = findLineContaining(selector, gameDef);
	ensure(game ~= nil, "Game not defined!");
	game = game:sub(defEnd+2);
	game = game:sub(1,1) .. game:sub(2):lower();
	
	return game;
end;

function assertResourcesPresent(resList, resRootDir)
	local resOK = true;
	if(resList ~= nil) then
		for _, res in pairs(resList) do
			if(not checkFileExists(resRootDir.."/"..res)) then
				print("Missing resource dependency " .. res);
				resOK = false;
			end;
		end;
	else
		print("No resource dependencies found, skipping...")
	end;
	return resOK;
end;