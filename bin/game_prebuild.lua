dofile(os.getenv("SGPROJECT").."/bin/cmn.lua");

local shaderGen = dofile(gProjDir.."/bin/shaderGenerator.lua");
rebuildShadersIfNeccessary(shaderGen);


local docsGen = dofile(gProjDir.."/bin/docsGenerator.lua");
rebuildLuaDocsIfNeccessary(docsGen);