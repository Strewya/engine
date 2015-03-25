
function winPath(path)
	return path:gsub("/", "\\");
end;

function nixPath(path)
	return path:gsub("\\", "/");
end;

function normalizePath(path)
	path = nixPath(path);
	local s = 0;
	repeat
		path = string.gsub(path, "//", "/");
		path, s = string.gsub(path, "[%w]*/%.%.", "", 1);
	until s == 0;
	path = string.gsub(path, "/$", "", 1);
	return path;
end;

lfs = require('lfs');
sxRoot = normalizePath(arg[1]);
exec = os.execute;
--exec = print; --for debug purposes

--load all other scripts
dofile(sxRoot .. "/bin/cmn.lua");
dofile(sxRoot .. "/bin/docsGenerator.lua");
dofile(sxRoot .. "/bin/shaderGenerator.lua");

dofile(sxRoot .. "/bin/game_prebuild.lua");
dofile(sxRoot .. "/bin/game_postbuild.lua");
dofile(sxRoot .. "/bin/game_deploy.lua");
dofile(sxRoot .. "/bin/tolua_postbuild.lua");

local event = arg[2];

local func = _G[event];

if(type(func) ~= "function") then
	print("Invalid event name: " .. event);
	os.exit();
end;

func(sxRoot, arg[3]);