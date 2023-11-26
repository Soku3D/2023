SamplerState Sampler : register(s0);
Texture2D canvasTexture0 : register(t0);
Texture2D canvasTexture1 : register(t1);



struct PSInput
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

float4 main(PSInput input) : SV_Target
{   
    float2 center = float2(0.5f);
    
    return canvasTexture0.Sample(Sampler, input.uv);
}