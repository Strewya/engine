
function tolua_postbuild(sxRoot)
	local src = winPath(sxRoot .. "/lib/tolua++.dll");
	local dst = winPath(sxRoot .. "/bin/tolua++.dll");

	ensure(checkFileExists(src), ("ERROR: Source file '%s' does not exist!"):format(src));

	local f = copy(src, dst);
	print("Copy OK: "..f);
end;