
local function enumerateDLLsForCopy(dllList, libDir, buildDir, buildName)
	local foundDLLs = {};
	for _, dll in pairs(dllList) do
		if( type(dll) == "string" ) then
			if(not dll:find(".dll$")) then
				dll = dll .. ".dll";
			end;
			if(checkFileExists(libDir.."/"..dll)) then
				foundDLLs[dll] = libDir.."/"..dll;
			elseif(checkFileExists(buildDir.."/"..dll)) then
				foundDLLs[dll] = buildDir.."/"..dll;
			else
				ensure(false, ("The dll %s could not be found!"):format(dll));
			end;
		end;
	end;
	if( type(dllList[buildName]) == "table" ) then
		local buildDependantDlls = enumerateDLLsForCopy(dllList[buildName], libDir, buildDir, buildName);
		mergeTables(foundDLLs, buildDependantDlls);
	end;
	return foundDLLs;
end;

function game_postbuild(sxRoot, buildName)
	local game = extractGameName(sxRoot);
	local resFile = sxRoot .. "/src/Games/" .. game .. "/resourceList.lua";
	local libDir = sxRoot .. "/lib";
	local buildLibDir = libDir .. "/" .. buildName;
	local buildOutputDir = sxRoot .. "/output/" .. buildName;
	local resDir = sxRoot .. "/output/resources";

	ensure(checkFileExists(resFile), ("Resource file %s does not exist!"):format(resFile));
	ensure(checkFolderExists(libDir), "Lib directory not found!");
	ensure(checkFolderExists(buildOutputDir), ("Output directory for %s not found!"):format(buildName));


	local resList, dllList = dofile(resFile);
	-- DLL list can be nil if there are no DLL dependencies!
	if(dllList ~= nil) then
		dllList = enumerateDLLsForCopy(dllList, libDir, buildLibDir, buildName);
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
	assertResourcesPresent(resList, resDir);


	if(buildName == "Deploy" or buildName == "Release") then
		allResourcesPresent = resOK;
		gameName = game;
		resourceList = resList;
		print(buildName);
		if(buildName == "Deploy") then
			doDeployment(sxRoot, dropboxTarget(), buildName);
		end;
		if(buildName == "Release") then
			doDeployment(sxRoot, deployTarget(sxRoot), buildName);
		end;
	end;
end;