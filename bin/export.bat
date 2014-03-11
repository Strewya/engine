set pth=%~dp0

cl.exe /EP %pth%..\src\Games\luaExport.cpp > %pth%..\src\Games\luaExport.pkg

%pth%tolua++.exe -n core -H %pth%..\src\Scripting\luaBinding.h -o %pth%..\src\Scripting\luaBinding.cpp %pth%..\src\luaExportMaster.pkg