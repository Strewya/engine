set pth=%~dp0
echo %pth%
echo %pth%tolua++.exe
%pth%tolua++.exe -n core -H %pth%..\src\Scripting\luaBinding.h -o %pth%..\src\Scripting\luaBinding.cpp %pth%..\src\Scripting\luaExport.pkg