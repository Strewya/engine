
struct VOut
{
    float4 position : SV_POSITION;
    float4 diffuse : DIFFUSE;
	float2 texCoord : TEXCOORD;
};

cbuffer cbPerObject
{
	float4x4 WVP;
	float4 FillColor;
	float4 isTexture;
};

Texture2D ObjTexture;
SamplerState ObjSamplerState;

VOut VShader(float4 position : POSITION, float2 texCoord : TEXCOORD, float4 diffuse : DIFFUSE)
{
    VOut output;

    output.position = mul(position, WVP);
	output.texCoord = texCoord;
	output.diffuse = diffuse * FillColor;
	
    return output;
}


float4 PShader(VOut input) : SV_TARGET
{
    float4 diffuse = input.diffuse;
	if(isTexture.x != 0)
		diffuse = ObjTexture.Sample(ObjSamplerState, input.texCoord) * input.diffuse;
	clip(diffuse.a - 0.1f);
	return diffuse;
}