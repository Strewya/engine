dofile(os.getenv("SGPROJECT").."/bin/cmn.lua");

local src = winPath(gProjDir .. "/lib/tolua++.dll");
local dst = winPath(gProjDir .. "/bin/tolua++.dll");

ensure(checkFileExists(src), ("ERROR: Source file '%s' does not exist!"):format(src));

local f = copy(src, dst);
print("Copy OK: "..f);