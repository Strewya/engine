
local generator = {};

function generator:generateAPIDocs(pkgFiles, location)
	local data = {};
	for i, file in ipairs(pkgFiles) do
		local tejbl = {vars={}, funcs={}, enums={}};
		self:parseFileForAPI(file, tejbl);
		tejbl.name = file:match("/([^/]*)$"):lower();
		table.insert(data, tejbl);
	end;
	local output = {f = io.open(location, "w")};
	function output:write(...) self.f:write(...); end;
	function output:wl(...) self.f:write(...); self.f:write("\n"); end;
	
	self:makeHtml(output, data);
	output.f:close();
end;

function generator:parseFileForAPI(file, data)
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
			local func = self:parseFunction(line, level-1, blocks);
			local var = self:parseVariable(line, level-1, blocks, enum);
			local val = func or var;
			if(val) then
				table.insert(data[val.id], val);
			end;
		end;
	end;
	pkg:close();
end;

function generator:parseFunction(line, level, blocks)
	local rn, args, const = line:match("%s*([^(]*)%(([^)]*)%)(.*);");
	if(rn == nil) then
		return nil;
	end;
	
	local space = rn:find(" ([^ ]*)$");
	local ret = space and rn:sub(1, space) or "";
	local func = space and rn:sub(space+1) or "";
	ret = ret:gsub("inline ", "");
	
	local full = ret;
	local loc = self:getLocation(level, blocks);
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

function generator:parseVariable(line, level, blocks, isEnum)
	
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
	local loc = self:getLocation(level, blocks);
	full = full .. loc .. "." .. vn .. ";";
	
	return {Type=vt, Name=vn, Location=loc, Delimiter=".", Full = full, id=(isEnum and "enums" or "vars")};
end;

function generator:getLocation(level, blocks)
	local str = "";
	for i=1, level do
		str = str .. blocks[i];
		str = str .. ".";
	end;
	return str:sub(1, #str-1);
end;

function generator:makeHtml(output, data)
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
		self:drawSelection(output, v);
	end;
	output:wl("</div>");
	output:wl("<div id='content'>");
	for i,v in ipairs(data) do
		self:drawContent(output, v);
	end;
	output:wl("</div>");
	
	output:wl("</body>");
	output:wl("</html>");
end;

function generator:drawSelection(output, data)
	if(#data.enums == 0 and #data.vars == 0 and #data.funcs == 0) then
		return;
	end;
	local uniqueID = data.name;
	output:wl("<button type=\"button\" onclick=\"toggle('"..uniqueID.."')\">");
	output:wl(uniqueID);
	output:wl("</button>");
end;

function generator:drawContent(output, data)
	local uniqueID = data.name;
	output:wl("<div id='" .. uniqueID .. "' style='display:none'>");
	self:drawButtonContent(output, "ENUMS", data.enums);
	self:drawButtonContent(output, "VARIABLES", data.vars);
	self:drawButtonContent(output, "FUNCTIONS", data.funcs);
	output:wl("</div>");
end;

function generator:drawButtonContent(output, header, data)
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

return generator;