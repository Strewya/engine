struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

cbuffer cbPerObject
{
	float4x4 WVP;
};

VOut VShader(float4 position : POSITION, float4 color : COLOR)
{
    VOut output;

    output.position = mul(position, WVP);
    output.color = color;

    return output;
}


float4 PShader(VOut input) : SV_TARGET
{
    return input.color;
}
