struct ConstantData
{
    float4 offset;
    float4 color;
};

cbuffer ConstBuff : register(b0) {
    ConstantData ConstData;
};

struct VS_IN
{
    float4 pos : POSITION0;
    float4 color : COLOR0;
};

struct PS_IN
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

PS_IN VSMain(VS_IN input, uint vId : SV_VertexID)
{
    PS_IN output = (PS_IN) 0;
	
    output.pos = float4(input.pos.xyz + ConstData.offset.xyz, 1.0f);
    output.color = input.color + ConstData.color;
	
    return output;
}

float4 PSMain(PS_IN input) : SV_Target
{
    float4 col = input.color;
#ifdef TEST
	//if (input.pos.x > 400) col = TCOLOR;
#endif
    return col;
}