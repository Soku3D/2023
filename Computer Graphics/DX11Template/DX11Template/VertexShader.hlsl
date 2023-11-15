struct vsInput
{
    float4 pos : POSITIONT;
    float2 texcoord : TEXCOORD;
};
struct psInput
{
    float4 pos : SV_POSITION;
    float2 texcoord : TEXCOORD;
};
psInput main(vsInput input)
{
    psInput output;
    output.pos = input.pos;
    output.texcoord = input.texcoord;
    return output;

}