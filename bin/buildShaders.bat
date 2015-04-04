@echo off

pushd w:\output\resources\Shaders

REM shader
fxc /Tvs_4_0 /EVShader /nologo /O3 /Fo shader_vs.cso shader.hlsl
fxc /Tps_4_0 /EPShader /nologo /O3 /Fo shader_ps.cso shader.hlsl

REM health
fxc /Tvs_4_0 /EVShader /nologo /O3 /Fo health_vs.cso shader.hlsl
fxc /Tps_4_0 /EHealthbar /nologo /O3 /Fo health_ps.cso shader.hlsl

popd