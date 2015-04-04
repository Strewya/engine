
struct Vertex
{
   float4 position : POSITION;
   float4 diffuse : DIFFUSE;
   float2 texCoord : TEXCOORD;
};

struct Pixel
{
   float4 position : SV_POSITION;
   float4 diffuse : DIFFUSE;
   float2 texCoord : TEXCOORD;
};

Texture2D<float4> ObjTexture;
SamplerState ObjSamplerState;

Pixel VShader(Vertex vx)
{
   Pixel px;
   px.position = vx.position;
   px.texCoord = vx.texCoord;
   px.diffuse = vx.diffuse;

   return px;
}


float4 PShader(Pixel px) : SV_TARGET
{
   float4 diffuse = px.diffuse;
   if( px.texCoord.x >= 0 && px.texCoord.y >= 0 )
   {
      diffuse = ObjTexture.Sample(ObjSamplerState, px.texCoord) * px.diffuse;
   }
   return diffuse;
}
