
struct Vertex
{
	float4 position : POSITION; 
	float2 texCoord : TEXCOORD;
	float4 diffuse : DIFFUSE;
	matrix world : WORLD;
	float4 fill : FILL;
	float health : HEALTH;
};

struct Pixel
{
    float4 position : SV_POSITION;
    float4 diffuse : DIFFUSE;
	float2 texCoord : TEXCOORD;
	float health : HEALTH;
};

cbuffer dataPerScene
{
	matrix view;
	matrix projection;
};

Texture2D<float4> ObjTexture;
SamplerState ObjSamplerState;

Pixel VShader(Vertex vx)
{
    Pixel px;
	px.position = mul(vx.position, vx.world);
	px.texCoord = vx.texCoord;
	px.diffuse = vx.diffuse * vx.fill;
	px.health = vx.health;
	
	return px;
}


float4 PShader(Pixel px) : SV_TARGET
{
	float4 diffuse = px.diffuse;
	if( px.texCoord.x >= 0 && px.texCoord.y >= 0 )
		diffuse = ObjTexture.Sample(ObjSamplerState, px.texCoord) * px.diffuse;
	return diffuse;
}

float4 Healthbar(Pixel px) : SV_TARGET
{
	float4 gradient = ObjTexture.Sample(ObjSamplerState, px.texCoord);
	float greyscale = (gradient.r + gradient.g + gradient.b)/3.0f;
	if(greyscale < (1.0f - px.health)) discard;
	float4 diffuse = px.diffuse;
	diffuse.a = gradient.a;
	return diffuse;
}