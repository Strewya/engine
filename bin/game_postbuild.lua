dofile(os.getenv("SGPROJECT").."/bin/cmn.lua");

local function extractGameName()
	local selector = gProjDir .. "/src/Games/GameSelector.h";
	local gameDef = "#define CORE_GAME_BUILD";

	local game, defEnd = findLineContaining(selector, gameDef);
	ensure(game ~= nil, "Game not defined!");
	game = game:sub(defEnd+2);
	game = game:sub(1,1) .. game:sub(2):lower();
	
	return game;
end;

local function enumerateDLLsForCopy(dllList, lib, build)
	local foundDLLs = {};
	for _, dll in ipairs(dllList) do
		if(not dll:find(".dll$")) then
			dll = dll .. ".dll";
		end;
		if(checkFileExists(lib.."/"..dll)) then
			foundDLLs[dll] = lib.."/"..dll;
		elseif(checkFileExists(build.."/"..dll)) then
			foundDLLs[dll] = build.."/"..dll;
		else
			ensure(false, ("The dll %s could not be found!"):format(dll));
		end;
		
	end;
	return foundDLLs;
end;

local buildName = arg[1];
local game = extractGameName();
local resFile = gProjDir .. "/src/Games/" .. game .. "/resourceList.lua";
local libDir = gProjDir .. "/lib";
local buildLibDir = libDir .. "/" .. buildName;
local buildOutputDir = gProjDir .. "/output/" .. buildName;
local resDir = gProjDir .. "/output/resources";


ensure(checkFileExists(resFile), ("Resource file %s does not exist!"):format(resFile));
ensure(checkFolderExists(libDir), "Lib directory not found!");
ensure(checkFolderExists(buildOutputDir), ("Output directory for %s not found!"):format(buildName));

local dllList, resList = dofile(resFile);
-- DLL list can be nil if there are no DLL dependencies!
if(dllList ~= nil) then
	dllList = enumerateDLLsForCopy(dllList, libDir, buildLibDir);
	for dll, dllPath in pairs(dllList) do
		local file = copyIfNewer(dllPath, buildOutputDir.."/"..dll);
		if(file) then
			print("Copy OK: "..file);
		end;
	end;
else
	print("No DLL dependencies found, skipping...")
end;

-- make sure all resource dependencies are present, and log if any are missing
-- as before, the game might not have any resource dependencies
local resOK = true;
if(resList ~= nil) then
	for _, res in pairs(resList) do
		if(not checkFileExists(resDir.."/"..res)) then
			print("Missing resource dependency " .. res);
			resOK = false;
		end;
	end;
else
	print("No resource dependencies found, skipping...")
end;



if(buildName == "Final") then
	ensure(resOK, "Some resource dependencies are missing! Fix those first!");
	local deployer = dofile(gProjDir.."/bin/game_deploy.lua");
	deployer:deploy(game, resList);
end;
