dofile(os.getenv("SGPROJECT").."/bin/cmn.lua");

local function findPkgFiles(path, file, storage)
	if(file:sub(#file - 2) == "pkg") then
		local fp = nixPath(path..'/'..file);
		table.insert(storage, fp);
	end;
end;

local function removeWhitelines(filename)
	local fstr = "";
	local file = io.open(filename, "r");
	for line in file:lines() do
		if(#line > 0) then
			fstr = fstr .. "\n" .. line .. "\n";
		end;
	end;
	file:close();
	file = io.open(filename, "w");
	file:write(fstr);
	file:close();
end;

local pkgFiles = {};
forEachDir(gProjDir .. "/src", findPkgFiles, pkgFiles);

local binding = winPath(gProjDir .. "/src/Scripting/luaBinding.cpp");
local bindingHeader = winPath(gProjDir .. "/src/Scripting/luaBinding.h");
local selector = winPath(gProjDir .. "/src/Games/GameSelector.h");
local docs = winPath(gProjDir .. "/docs.html");
local gamePkg = winPath(gProjDir .. "/src/Games/Game.pkg");
local gameCpp = winPath(gProjDir .. "/src/Games/Game.cpp");
local mainExportPkg = winPath(gProjDir .. "/src/mainExport.pkg");

ensure(checkFileExists(selector), ("ERROR: Selector file %s does not exist!"):format(selector));
ensure(checkFileExists(gameCpp), ("ERROR: Compile file %s does not exist!"):format(gameCpp));
ensure(checkFileExists(mainExportPkg), ("ERROR: Main export file %s does not exist!"):format(mainExportPkg));

local rebuild = isNewer(selector, binding);
if(not rebuild) then
	for _, file in ipairs(pkgFiles) do
		if(isNewer(file, binding)) then
			rebuild = true;
			--break;
		end;
	end;
end;

if(rebuild) then
	print("Generating Lua bindings...");
	local makeGamePkg = ("cl.exe /EP %s > %s");
	exec(makeGamePkg:format(gameCpp, gamePkg));
	removeWhitelines(gamePkg);
	-- the following command depends on being in a specific directory when executed
	-- it's because of the format in which the main pkg file is written
	local makeBindings = ("%s/bin/tolua++.exe -n core -H %s -o %s %s");
	lfs.chdir(gProjDir .. "/src");
	exec(makeBindings:format(gProjDir, bindingHeader, binding, mainExportPkg));
else
	print("Lua bindings up to date...");
end;

if(isNewer(binding, docs)) then
	print("Generating Lua API docs...");
	local gen = dofile(gProjDir.."/bin/docsGenerator.lua");
	gen:generateAPIDocs(pkgFiles, docs);
else
	print("Lua API docs up to date...")
end;