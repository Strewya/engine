
struct Vertex
{
	float4 position : POSITION;
	float2 texCoord : TEXCOORD;
	float4 diffuse : DIFFUSE;
	matrix world : WORLD;
	float4 fill : FILL;
};

struct Pixel
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

Pixel VShader(Vertex input)
{
    Pixel output;
    output.position = mul(input.position, input.world);
	output.texCoord = input.texCoord;
	output.diffuse = input.diffuse * input.fill;
	
    return output;
}


float4 PShader(Pixel input) : SV_TARGET
{
    float4 diffuse = input.diffuse;
	if(input.texCoord.x >= 0 && input.texCoord.y >= 0)
		diffuse = ObjTexture.Sample(ObjSamplerState, input.texCoord) * input.diffuse;
	return diffuse;
}