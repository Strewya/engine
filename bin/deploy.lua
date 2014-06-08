local projDir = os.getenv("SGPROJECT");
dofile(projDir .. '/bin/binLib.lua');

local dofun = print;
dofun = os.execute;


function checkFolderExists(folder)
	local x = lfs.attributes(folder) ~= nil;
	return x;
end;

function copyIfNewer(src, dst)
	local srcMod = lfs.attributes(src, "modification");
	local dstMod = lfs.attributes(dst, "modification");
	if(dstMod == nil or srcMod > dstMod) then
		local index = dst:match('^.*()\\')
		local folder = dst:sub(1, index-1);
		if(not checkFolderExists(folder)) then
			dofun('xcopy "' .. src.. '" "' .. folder .. '\\" /T /Y');
		end;
		dofun('copy /Y "' .. src .. '" "' .. dst .. '"');
		return 1;
	end;
	return 0;
end;

function prefixNames(tbl, prefix)
	local newTbl = {};
	for i,v in ipairs(tbl) do
		table.insert(newTbl, prefix .. v);
	end;
	return newTbl;
end;

function findMatch(str, tbl)
	for i,v in ipairs(tbl) do
		if(str == v) then
			return true;
		end;
	end;
	return false;
end;

function cleanUnusedFiles(path, file, usedFilesList)
	local f = path..'/'..file;
	if(not findMatch(f, usedFilesList)) then
		dofun('del "' .. winPath(f) .. '"');
	end;
end;




local line = findLine(projDir .. "/src/Games/GameSelector.h", "#define CORE_GAME_BUILD");
if(line == "") then
	print("Game not detected!");
	os.exit();
end;
local game = line:sub(25);
game = game:sub(1,1) .. game:sub(2):lower();
line = findLine(os.getenv("USERPROFILE").."/AppData/Roaming/Dropbox/host.db", 2);


local dstRootDir = winPath(b64dec(line) .. "/Games/" .. game);
local dstBinDir = winPath(dstRootDir .. "/bin");
local dstResDir = winPath(dstRootDir .. "/resources");
local srcBinDir = winPath(projDir .. "/output/Final");
local srcResDir = winPath(projDir .. "/output/resources");

local resourceListFile = winPath(projDir .. "/src/Games/" .. game .. "/resourceList.lua");

must(checkFolderExists(srcBinDir), "Source dir '" .. srcBinDir .. "' doesn't exist.");
must(checkFolderExists(resourceListFile), "Resource list file doesn't exist!");

local resourceList = dofile(resourceListFile);
must(type(resourceList == "table"), "Resource list file has invalid format!");

print("Deploying " .. game .. " game to " .. dstRootDir);
local counter = 0;
--copy bin folder: exe and DLLs
forEachDir(srcBinDir, function(p, f)
	local src = winPath(p..'/'..f);
	local dst = winPath(dstBinDir..'/'..f);
	counter = counter + copyIfNewer(src, dst);
end);

--copy required resource files
for i,v in ipairs(resourceList) do
	local src = winPath(srcResDir..'/'..v);
	local dst = winPath(dstResDir..'/'..v);
	counter = counter + copyIfNewer(src, dst);
end;
print("Files copied: " .. counter);
--clean unused resource files
forEachDir(dstResDir, cleanUnusedFiles, prefixNames(resourceList, dstResDir.."/"));
