struct VOut
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD;
};

cbuffer cbPerObject
{
	float4x4 WVP;
};

Texture2D ObjTexture;
SamplerState ObjSamplerState;

VOut VShader(float4 position : POSITION, float2 texCoord : TEXCOORD)
{
    VOut output;

    output.position = mul(position, WVP);
    output.texCoord = texCoord;

    return output;
}


float4 PShader(VOut input) : SV_TARGET
{
    float4 diffuse = ObjTexture.Sample(ObjSamplerState, input.texCoord);
	clip(diffuse.a - 0.25f);
	return diffuse;
}
