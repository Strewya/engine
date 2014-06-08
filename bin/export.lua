local projDir = os.getenv("SGPROJECT");
dofile(projDir .. '/bin/binLib.lua');

local dofun = print;
dofun = os.execute;

function append(path, file, storage)
	if(file:sub(#file - 2, #file) == "pkg") then
		table.insert(storage, path..'/'..file);
	end;
end;

local d = lfs.chdir(projDir);

local pkgFiles = {};
forEachDir(nixPath(projDir .. "/src"), append, pkgFiles);
local cppMod = lfs.attributes(projDir .. "/src/Scripting/luaBinding.cpp", "modification");
local selectorMod = lfs.attributes(projDir .. "/src/Games/GameSelector.h", "modification");
local rebuild = true;
if(cppMod ~= nil and selectorMod < cppMod) then
	rebuild = false;
	for k,v in pairs(pkgFiles) do
		local fileTS = lfs.attributes(v, "modification");
		if(fileTS > cppMod) then
			rebuild = true;
			break;
		end;
	end;
end;

if(rebuild) then
	print("Generating Lua bindings...");
	lfs.chdir(projDir .. "/bin");
	dofun(string.format("cl.exe /EP %s/src/Games/Game.cpp > %s/src/Games/Game.pkg", projDir, projDir));
	dofun(string.format("tolua++.exe -n core -H %s/src/Scripting/luaBinding.h -o %s/src/Scripting/luaBinding.cpp %s/src/mainExport.pkg", projDir, projDir, projDir));
else
	print("Bindings up to date...");
end;