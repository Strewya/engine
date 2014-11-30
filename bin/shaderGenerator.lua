
function rebuildShadersIfNeccessary()
	local shaderList =
	{
		{ "/Shaders/shader.hlsl", "ps_4_0", "PShader", "pshader.h" },
		{ "/Shaders/shader.hlsl", "vs_4_0", "VShader", "vshader.h" }
	};

	local pwd = lfs.currentdir();
	lfs.chdir(os.getenv("DXSDK_DIR")..[[\Utilities\bin\x86]]);
	local shaderCompilerOptions = '/T%s /E%s /O3 /Fh%s %s';
	local shaderSourceDir = "/output/resources";
	local shaderDestinationDir = "/src/Graphics/";
	local shaderCompilerCmds = {};
	for i=1, #shaderList do
		local shaderFile = winPath(gProjDir .. shaderSourceDir .. shaderList[i][1]);
		local shaderProfile = shaderList[i][2];
		local shaderFunction = shaderList[i][3];
		local shaderHeader = winPath(gProjDir .. shaderDestinationDir .. shaderList[i][4]);
		if(isNewer(shaderFile, shaderHeader)) then
			local cmd = string.format(shaderCompilerOptions, shaderProfile, shaderFunction, shaderHeader, shaderFile);
			table.insert(shaderCompilerCmds, "fxc " .. cmd);
		end;
	end;
	
	if(#shaderCompilerCmds > 0) then
		for i, cmd in ipairs(shaderCompilerCmds) do
			exec(cmd);
		end;
	else
		print("Shaders up to date...");
	end;
	lfs.chdir(pwd);
end;