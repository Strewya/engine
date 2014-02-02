
local name = "variable";

oldFile = io.open(name..".lua", "r");
newFile = io.open(name.."2.lua", "w");

content = oldFile:read("*a");
char = 1;
for i=1, #content do
	local s = content:byte(i);
	if(s < 100) then
		s = " " .. tostring(s);
	end;
	if(i < #content) then
		s = s .. ",";
	end;
	if(char == 15) then
		char = 0;
		s = s .. "\n";
	end;
	newFile:write(s);
	char = char+1;
end;

oldFile:close();
newFile:close();
	