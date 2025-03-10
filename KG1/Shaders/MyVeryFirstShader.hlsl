struct ConstantData
{
    matrix World;
    matrix View;
    matrix Projection;
    float4 offset;
    float4 color;
    float scale;
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
    float4 depthPos : TEXTURE0;
};

PS_IN VSMain(VS_IN input, uint vId : SV_VertexID)
{
    PS_IN output = (PS_IN) 0;
	
    input.pos.w = 1.0f;

    float4 scaledPos = input.pos * ConstData.scale;

    output.pos = mul( input.pos, ConstData.World );
    output.pos = mul( output.pos, ConstData.View );
    output.pos = mul( output.pos, ConstData.Projection );    

    output.pos = output.pos * ConstData.scale;

    output.pos = float4(output.pos.xyz + ConstData.offset.xyz, output.pos.w);
    output.color = input.color + ConstData.color;
	
    output.depthPos = output.pos;

    return output;
}

float4 PSMain(PS_IN input) : SV_Target
{
    float depthValue;
    float4 col;

    // Get the depth value of the pixel by dividing the Z pixel depth by the homogeneous W coordinate.
    depthValue = input.depthPos.z / input.depthPos.w;

    // First 10% of the depth buffer color red.
    if(depthValue < 0.9f)
    {
        col = float4(1.0, 0.0f, 0.0f, 1.0f);
    }
	
    // The next 0.025% portion of the depth buffer color green.
    if(depthValue > 0.9f)
    {
        col = float4(0.0, 1.0f, 0.0f, 1.0f);
    }

    // The remainder of the depth buffer color blue.
    if(depthValue > 0.925f)
    {
        col = float4(0.0, 0.0f, 1.0f, 1.0f);
    }

    return input.color;
}