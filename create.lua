function copy(source, dest)
	for line in source:lines() do
		dest:write(line .. "\n");
	end;
end;


local filename = arg[1];
local ext1 = arg[2];
local ext2 = arg[3];

if(not filename and ext1 ~= ext2) then
	print("Invalid usage, arguments expected are name and path of file to create, and the extensions to create (h and cpp)");
	print("lua create.lua path/to/file/name h|cpp [cpp|h]");
	exit(1);
end;

local cpp = (ext1 == "cpp" or ext2 == "cpp");
local h = (ext1 == "h" or ext2 == "h");

if(h) then
	local file = io.open(filename .. ".h", "w");
	local template = io.open(os.getenv("SGPROJECT") .. "/docs/tHeader.txt", "r");
	copy(template, file);
	file:close();
	template:close();
end;

if(cpp) then
	local file = io.open(filename .. ".cpp", "w");
	local template = io.open(os.getenv("SGPROJECT") .. "/docs/tSource.txt", "r");
	copy(template, file);
	file:close();
	template:close();
end;