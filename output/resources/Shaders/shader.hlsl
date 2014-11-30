
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

Pixel VShader(Vertex vx)
{
    Pixel px;
	px.position = mul(vx.position, vx.world);
	px.texCoord = vx.texCoord;
	px.diffuse = vx.diffuse * vx.fill;
	
	return px;
}


float4 PShader(Pixel px) : SV_TARGET
{
	float4 diffuse = px.diffuse;
	if( px.texCoord.x >= 0 && px.texCoord.y >= 0 )
		diffuse = ObjTexture.Sample(ObjSamplerState, px.texCoord) * px.diffuse;
	return diffuse;
}