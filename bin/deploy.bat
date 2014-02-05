@ECHO OFF
set project=%1
set config=%2
set myPath=%~dp0

if /I "%config%"=="release" (
	copy /Y "%myPath%\..\lib\tolua++.dll" "%myPath%
	copy /Y "%myPath%\tolua++.dll" "%myPath%\..\output\Debug\"
	copy /Y "%myPath%\tolua++.dll" "%myPath%\..\output\Release\"
	copy /Y "%myPath%\tolua++.dll" "%myPath%\..\output\Final\"
)
del "%myPath%\..\lib\tolua++.dll"