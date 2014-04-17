require('lfs');

function attrdir (path, storage)
    for file in lfs.dir(path) do
        if file ~= "." and file ~= ".." then
            local f = path..'/'..file
            --print ("\t "..f)
            local attr = lfs.attributes (f)
            assert (type(attr) == "table")
            if attr.mode == "directory" then
                attrdir (f, storage)
            else
                if(f:sub(f:len()-2, f:len()) == "pkg") then
					table.insert(storage, f);
				end;
            end
        end
    end
end

local sgprojDir = os.getenv("SGPROJECT");
local d = lfs.chdir(sgprojDir);

local pkgFiles = {};
attrdir(sgprojDir .. "/src", pkgFiles);
local cppAttr = lfs.attributes(sgprojDir .. "/src/Scripting/luaBinding.cpp");
local rebuild = true;
if(cppAttr ~= nil) then
	rebuild = false;
	for k,v in pairs(pkgFiles) do
		local fileTS = lfs.attributes(v, "modification");
		if(fileTS > cppAttr.modification) then
			rebuild = true;
			break;
		end;
	end;
end;

if(rebuild) then
	print("Generating Lua bindings...");
	lfs.chdir(sgprojDir .. "/bin");
	os.execute(string.format("cl.exe /EP %s/src/Games/Game.cpp > %s/src/Games/Game.pkg", sgprojDir, sgprojDir));
	os.execute(string.format("tolua++.exe -n core -H %s/src/Scripting/luaBinding.h -o %s/src/Scripting/luaBinding.cpp %s/src/mainExport.pkg", sgprojDir, sgprojDir, sgprojDir));
else
	print("Bindings up to date...");
end;