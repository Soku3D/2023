SamplerState Sampler : register(s0);
Texture2D canvasTexture : register(t0);

struct PSInput
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

float4 main(PSInput input) : SV_Target
{
    return canvasTexture.Sample(Sampler, input.uv);
}