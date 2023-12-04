struct VSInput
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD;
};
struct PSInput
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

PSInput main(VSInput input)
{
    PSInput output;
    output.pos = input.pos;
    output.uv = input.uv;
    
    return output;
}