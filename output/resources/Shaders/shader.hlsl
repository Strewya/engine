
struct Vertex
{
   float4 position : POSITION; 
   float4 diffuse : DIFFUSE;
   float2 texCoord : TEXCOORD;
   float2 health : HEALTH;
};

struct Pixel
{
   float4 position : SV_POSITION;
   float4 diffuse : DIFFUSE;
   float2 texCoord : TEXCOORD;
   float health : HEALTH;
};

Texture2D<float4> ObjTexture;
SamplerState ObjSamplerState;

Pixel VShader(Vertex vx)
{
   Pixel px;
   px.position = vx.position;
   px.texCoord = vx.texCoord;
   px.diffuse = vx.diffuse;
   px.health = vx.health.x;
	
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