local name = arg[1];
local from = arg[3] or "D:/My Documents/Visual Studio 2010/Projects/StrujaGames/Design/";
local dest = arg[2] or "D:/My Documents/Visual Studio 2010/Projects/StrujaGames/Snake/actions/";
local in_h = io.open(from .. "headers_action.txt", "r");
local in_cpp = io.open(from .. "sources_action.txt", "r");
local out_h = io.open(dest .. "SG" .. name .. ".h", "w");
local out_cpp = io.open(dest .. "SG" .. name .. ".cpp", "w");

for line in in_h:lines() do
    line = line:gsub("#header_name#", name);
    out_h:write(line .. "\n");
end;
for line in in_cpp:lines() do
    line = line:gsub("#header_name#", name);
    out_cpp:write(line .. "\n");
end;

in_h:close();
out_h:close();
in_cpp:close();
out_cpp:close();