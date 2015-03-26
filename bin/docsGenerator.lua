
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

function rebuildLuaDocsIfNeccessary(sxRoot)
	local pkgFiles = {};
	forEachDir(sxRoot .. "/src", findPkgFiles, pkgFiles);

	local binding = winPath(sxRoot .. "/src/Scripting/luaBinding.cpp");
	local bindingHeader = winPath(sxRoot .. "/src/Scripting/luaBinding.h");
	local selector = winPath(sxRoot .. "/src/Games/GameList.h");
	local docs = winPath(sxRoot .. "/docs.html");
	local gamePkg = winPath(sxRoot .. "/src/Games/GamePkg.pkg");
	local gameCpp = winPath(sxRoot .. "/src/Games/GamePkg.cpp");
	local mainExportPkg = winPath(sxRoot .. "/src/mainExport.pkg");

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
		lfs.chdir(sxRoot .. "/src");
		exec(makeBindings:format(sxRoot, bindingHeader, binding, mainExportPkg));
	else
		print("Lua bindings up to date...");
	end;

	if(isNewer(binding, docs)) then
		print("Generating Lua API docs...");
		generateAPIDocs(pkgFiles, docs);
	else
		print("Lua API docs up to date...")
	end;
end;

function generateAPIDocs(pkgFiles, location)
	local data = {};
	for i, file in ipairs(pkgFiles) do
		local tejbl = {vars={}, funcs={}, enums={}};
		parseFileForAPI(file, tejbl);
		tejbl.name = file:match("/([^/]*)$"):lower();
		table.insert(data, tejbl);
	end;
	local output = {f = io.open(location, "w")};
	function output:write(...) self.f:write(...); end;
	function output:wl(...) self.f:write(...); self.f:write("\n"); end;
	
	makeHtml(output, data);
	output.f:close();
end;

function parseFileForAPI(file, data)
	local pkg = io.open(file, "r");
	local blocks = {};
	local level = 1;
	local enum = false;
	for line in pkg:lines() do
		local _, n = line:find("namespace");
		local _, c = line:find("class");
		local _, s = line:find("struct");
		local _, e = line:find("enum");
		if(e) then enum = true; end;
		local i = n or c or s or e;
		if(i) then
			blocks[level] = line:sub(i + 2);
		elseif(line:find("{")) then
			level = level + 1;
		elseif(line:find("}")) then
			level = level - 1;
			if(enum) then enum = false; end;
		else
			local func = parseFunction(line, level-1, blocks);
			local var = parseVariable(line, level-1, blocks, enum);
			local val = func or var;
			if(val) then
				table.insert(data[val.id], val);
			end;
		end;
	end;
	pkg:close();
end;

function parseFunction(line, level, blocks)
	local rn, args, const = line:match("%s*([^(]*)%(([^)]*)%)(.*);");
	if(rn == nil) then
		return nil;
	end;
	
	local space = rn:find(" ([^ ]*)$");
	local ret = space and rn:sub(1, space) or "";
	local func = space and rn:sub(space+1) or "";
	ret = ret:gsub("inline ", "");
	
	local full = ret;
	local loc = getLocation(level, blocks);
	full = full .. loc .. ":";
	if(not space) then
		full = full:sub(1, #full-1);
	end;
	full = full .. func .. "(" .. args .. ")";
	full = const ~= "" and full .. const or full;
	full = full .. ";";
	
	--print(full);
	return {Type=ret, Name=func, Location=loc, Delimiter=":", Full=full, id="funcs", Args=args};
end;

function parseVariable(line, level, blocks, isEnum)
	
	line = line:match("%s*(.*)%s*$")
	local mustEndWith = isEnum and ",$" or ";$";
	if(line:find("typedef") or line:find("%(") or line == "" or not line:find(mustEndWith)) then
		return nil;
	end;
	
	local space = line:find(" ");
	local vt, vn = "", "";
	while(space) do
		vt = vt .. line:sub(1, space);
		line = line:sub(space+1);
		space = line:find(" ");
	end;
	vt = vt:sub(1, #vt-1):gsub("tolua_", "");
	vn = line:sub(1, #line-1);
	
	local full = vt .. (isEnum and "" or " ");
	local loc = getLocation(level, blocks);
	full = full .. loc .. "." .. vn .. ";";
	
	return {Type=vt, Name=vn, Location=loc, Delimiter=".", Full = full, id=(isEnum and "enums" or "vars")};
end;

function getLocation(level, blocks)
	local str = "";
	for i=1, level do
		str = str .. blocks[i];
		str = str .. ".";
	end;
	return str:sub(1, #str-1);
end;

function makeHtml(output, data)
	output:wl("<html>");
	output:wl("<head>");
	output:wl("<style>");
	output:wl('body {font-family:"Courier"; overflow-y:scroll}');
	output:wl('td {font-size:small}');
	output:wl('button {width:100%; font-size:large}');
	output:wl('#selection {width:10%;position:fixed;left:0}');
	output:wl('#content {width:85%;float:left;margin-left:10%}');
	output:wl("</style>");
	output:write([[
	<script>
		var activeElement = ""
		function toggle(id)
		{
			if(activeElement != "")
			{
				document.getElementById(activeElement).style.display = "none";
			}
			document.getElementById(id).style.display = "block";
			activeElement = id;
		}
	</script>]]);
	output:wl("</head>");
	output:wl("<body>");
	
	output:wl("<div id='selection'>");
	for i,v in ipairs(data) do
		drawSelection(output, v);
	end;
	output:wl("</div>");
	output:wl("<div id='content'>");
	for i,v in ipairs(data) do
		drawContent(output, v);
	end;
	output:wl("</div>");
	
	output:wl("</body>");
	output:wl("</html>");
end;

function drawSelection(output, data)
	if(#data.enums == 0 and #data.vars == 0 and #data.funcs == 0) then
		return;
	end;
	local uniqueID = data.name;
	output:wl("<button type=\"button\" onclick=\"toggle('"..uniqueID.."')\">");
	output:wl(uniqueID);
	output:wl("</button>");
end;

function drawContent(output, data)
	local uniqueID = data.name;
	output:wl("<div id='" .. uniqueID .. "' style='display:none'>");
	drawButtonContent(output, "ENUMS", data.enums);
	drawButtonContent(output, "VARIABLES", data.vars);
	drawButtonContent(output, "FUNCTIONS", data.funcs);
	output:wl("</div>");
end;

function drawButtonContent(output, header, data)
	if(#data == 0) then
		return;
	end;
	output:wl("<h4>"..header.."</h4>");
	output:wl("<table>");
	local prevLoc = data[1].Location;
	for i,t in ipairs(data) do
		if(prevLoc ~= t.Location) then
			output:wl("<tr style='height:1em'><td></td></tr>");
			prevLoc = t.Location;
		end;
		output:wl("<tr>");
		output:wl("<td>")
		output:wl(t.Type);
		output:wl("</td>");
		output:wl("<td>");
		if(t.Name ~= "") then
			output:write(t.Location .. t.Delimiter .. t.Name);
		else
			output:write(t.Location);
		end;
		if(t.Args) then output:write("("..t.Args..")"); end;
		output:wl("\n</td>");
		
		output:wl("</tr>");
	end;
	output:wl("</table>");
end;
