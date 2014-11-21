
struct VIn
{
	float4 position : POSITION;
	float2 texCoord : TEXCOORD;
	float4 diffuse : DIFFUSE;
	float4 fill : FILL;
	matrix world : WORLD;
};

struct VOut
{
    float4 position : SV_POSITION;
    float4 diffuse : DIFFUSE;
	float2 texCoord : TEXCOORD;
};

cbuffer dataPerScene
{
	matrix view;
	matrix projection;
};

Texture2D ObjTexture;
SamplerState ObjSamplerState;

VOut VShader(VIn input)
{
    VOut output;
    output.position = mul(input.position, input.world);
	//output.position = mul(output.position, view);
	//output.position = mul(output.position, projection);
	output.texCoord = input.texCoord;
	output.diffuse = input.diffuse * input.fill;
	
    return output;
}


float4 PShader(VOut input) : SV_TARGET
{
    float4 diffuse = input.diffuse;
	if(input.texCoord.x >= 0 && input.texCoord.y >= 0)
		diffuse = ObjTexture.Sample(ObjSamplerState, input.texCoord) * input.diffuse;
	return diffuse;
}