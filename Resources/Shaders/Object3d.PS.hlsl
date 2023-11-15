#include "object3d.hlsli"

struct MaterialData
{
    float32_t4 color;
    int32_t enableLighting;
    float32_t4x4 uvTransform;
};
ConstantBuffer<MaterialData> gMaterialData : register(b0);

struct DirectionalLightData
{
    int32_t lightingType;
    float32_t4 color;
    float32_t3 direction;
    float intensity;
};
ConstantBuffer<DirectionalLightData> gDirectionalLightData : register(b1);

struct PointLightData
{
    float32_t4 color;
    float32_t3 position;
    float intensity;
};
ConstantBuffer<PointLightData> gPointLightData : register(b2);

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
    
    //ポイントライトの入射光
    //float32_t3 pointLightDirection = normalize(input.position - gPointLightData.position);
    
    if (gMaterialData.enableLighting != 0)
    {
        if (gDirectionalLightData.lightingType == 0)
        {
            output.color = gMaterialData.color * textureColor;
        }
        else if (gDirectionalLightData.lightingType == 1)
        {
            float cos = saturate(dot(normalize(input.normal), -gDirectionalLightData.direction));
            output.color.rgb = gMaterialData.color.rgb * textureColor.rgb * gDirectionalLightData.color.rgb * cos * gDirectionalLightData.intensity;
            output.color.a = gMaterialData.color.a * textureColor.a;
        }
        else if (gDirectionalLightData.lightingType == 2)
        {
            float NdotL = dot(normalize(input.normal), -gDirectionalLightData.direction);
            float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
            output.color.rgb = gMaterialData.color.rgb * textureColor.rgb * gDirectionalLightData.color.rgb * cos * gDirectionalLightData.intensity;
            output.color.a = gMaterialData.color.a * textureColor.a;
        }
    }
    else
    {
        output.color = gMaterialData.color * textureColor;
    }
    
    if (output.color.a == 0.0)
    {
        discard;
    }
    
    return output;
}