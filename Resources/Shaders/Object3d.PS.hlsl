#include "object3d.hlsli"

struct Material
{
    float32_t4 color;
    int32_t enableLighting;
    float32_t4x4 uvTransform;
};
ConstantBuffer<Material> gMaterial : register(b0);

struct DirectionalLightData
{
    float32_t4 color;
    float32_t3 direction;
    float intensity;
};
ConstantBuffer<DirectionalLightData> gDirectionalLightData : register(b1);

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};
Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);
PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    float4 transformUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float32_t4 textureColor = gTexture.Sample(gSampler, transformUV.xy);
    if (gMaterial.enableLighting != 0)
    {
        float NdotL = dot(normalize(input.normal), -gDirectionalLightData.direction);
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        output.color = gMaterial.color * textureColor * gDirectionalLightData.color * cos * gDirectionalLightData.intensity;
    }
    else
    {
        output.color = gMaterial.color * textureColor;
    }
    return output;
}