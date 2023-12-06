#include "Particle.hlsli"

struct MaterialData
{
    float32_t4 color;
    float32_t4x4 uvTransform;
};
ConstantBuffer<MaterialData> gMaterialData : register(b0);

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};
Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);
PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    float4 transformUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterialData.uvTransform);
    float32_t4 textureColor = gTexture.Sample(gSampler, transformUV.xy);
    output.color = gMaterialData.color * textureColor * input.color;
    
    if (output.color.a == 0.0)
    {
        discard;
    }
    
    return output;
}