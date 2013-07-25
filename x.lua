IncludeInfo = {};
IncludeInfo.__index = IncludeInfo;

function IncludeInfo.create(name)
	-- our new object
   local info = {};
   -- make Account handle lookup
   setmetatable(info,IncludeInfo)
   -- initialize our object
   info.name = name;
   info.ocurrences = 1;
   info.ocurrences_directly = 0;
   info.indent = 100;
   info.includes = 0;
   return info;
end;
IncludeInfo.__call = IncludeInfo.create;

local function cleanup_path(path)
    path = path:lower();
    path = path:gsub("\\", "/");
    path = path:gsub(" ", "");
    return path;
end;

local function open_file(filename)
	local f = io.open(filename, "r");
    if(f == nil) then
        print("File " .. filename .. " doesn't exist.");
        return nil;
	end;
    return f;
end;

local function ignore_file(inc, ignore_path)
    if(ignore_path ~= nil and ignore_path ~= "" and inc:find(ignore_path, 0, true) ~= nil) then
        return true;
	end;
    return false;
end;

local function parse_include (indent, inc, ignore_path, current_info, include_map)
    
    -- Update the number of includes caused by the current include (only the first time we encounter that include though).
    local direct_include = (indent <= current_info.indent);
    
    if(not direct_include) then
        if(current_info.ocurrences_directly == 1) then
            current_info.includes = current_info.includes + 1;
		end;
	end;
                    
    if(ignore_file(inc, ignore_path)) then
        --print("Skip ", indent, inc);
        current_info = IncludeInfo:create("");
        return current_info;
	end;
        
    if(include_map[inc] ~= nil) then
        local info = include_map[inc];
        info.ocurrences = info.ocurrences + 1;
        if(direct_include) then
            info.ocurrences_directly = info.ocurrences_directly + 1;
		end;
        --print("Add", indent, inc);
    else
        local info = IncludeInfo:create(inc);
        info.indent = indent;
        if(direct_include) then
            info.ocurrences_directly = info.ocurrences_directly + 1;
		end;
        include_map[inc] = info;
        --print("New", indent, direct_include, inc);
	end;
        
    if(direct_include) then
        current_info = info;
        --print("OK", indent, inc);
	end;
    return current_info;
end;

local function parse_includes(file, ignore_path)
    
    local include_map = {};
    local current_info = IncludeInfo:create("");
	
    for line in file:lines() do
		local first, last, capture1, capture2 = line:find('Note: including file:(%s+)(.*)');
        --print("Line is " .. line);
		--print("First is " .. tostring(first));
		--print("Last is " .. tostring(last));
		--print("Capture is " .. tostring(capture));
		if(first) then
            local indent = capture1:len() - 1;
            local inc = cleanup_path(capture2);
            local current_info = parse_include(indent, inc, ignore_path, current_info, include_map)
		end;
	end;
                                    
    return include_map;
end;

local function count_cpp_files(file)
    local cpp_count = 0
    file:seek("set");
	for line in file:lines() do
        f = line:find('%w+%.cpp%s*');
        if(f) then
            cpp_count = cpp_count + 1;
		end;
	end;
    return cpp_count 
end;

function sorter(a, b)
	print(type(a), type(b));
end;

local function print_top_includes( include_map, cpp_file_count, top_file_count )
    
    print("Cpp files: " .. cpp_file_count);
    print("(Header file, score, times included, includes caused by the header)");
    
    local result = {};
    for k,v in pairs(include_map) do
        if(v.ocurrences_directly > 0) then
            local score = v.ocurrences * (v.includes+1);
            result[score] = {key, score, v.ocurrences, v.includes};
		end;
	end;
    table.sort(result, sorter);
    for i,v in ipairs(result) do
        print(v);
	end;
end;



local fileName = arg[1];
local ignorePath = arg[2];

if(fileName == nil) then
	print("USAGE: lua x.lua fileToParse ignorePath");
	return;
end;

print("Counting includes in " .. fileName);
local fileHandle = open_file(fileName);
if(fileHandle == nil) then
	return;
end;
	
local include_map = parse_includes(fileHandle, ignore_path);
local cpp_file_count = count_cpp_files(fileHandle);
print_top_includes( include_map, cpp_file_count, 30 );

fileHandle:close();







