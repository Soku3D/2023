SamplerState samplerState : register(s0);
Texture2D basicTexture : register(t0);

struct psInput
{
    float4 pos : SV_POSITION;
    float2 texcoord : TEXCOORD;
};
float4 main(psInput input) : SV_Target
{
    return basicTexture.Sample(samplerState, input.texcoord);
    //return float4(1.0, 1.0, 0.0, 1.0);
}