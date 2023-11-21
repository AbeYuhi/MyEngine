#include "object3d.hlsli"

struct MaterialData
{
    float32_t4 color;
    int32_t enableLighting;
    int32_t isSpecularReflection;
    float32_t4x4 uvTransform;
    float shininess;
    float32_t3 shininessColor;
};
ConstantBuffer<MaterialData> gMaterialData : register(b0);

struct Camera
{
    float32_t3 worldPosition;
};
ConstantBuffer<Camera> gCamera : register(b1);

struct DirectionalLightData
{
    int32_t lightingType;
    float32_t4 color;
    float32_t3 direction;
    float intensity;
};
ConstantBuffer<DirectionalLightData> gDirectionalLightData : register(b2);

struct PointLightData
{
    int32_t lightingType;
    float32_t4 color;
    float32_t3 position;
    float intensity;
    float radius;
    float decay;
};
ConstantBuffer<PointLightData> gPointLightData : register(b3);

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};
Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);
PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    output.color.rgb = 0;
    float4 transformUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterialData.uvTransform);
    float32_t4 textureColor = gTexture.Sample(gSampler, transformUV.xy);
    
    if (gMaterialData.enableLighting != 0)
    {
        //平行光源
        if (gDirectionalLightData.lightingType == 0 && gPointLightData.lightingType == 0)
        {
            output.color = gMaterialData.color * textureColor;
        }
        
        if (gDirectionalLightData.lightingType == 1 || gDirectionalLightData.lightingType == 2)
        {
            float cos;
            if (gDirectionalLightData.lightingType == 1)
            {
                cos = saturate(dot(normalize(input.normal), -gDirectionalLightData.direction));
            }
            else if (gDirectionalLightData.lightingType == 2)
            {
                float NdotL = dot(normalize(input.normal), -gDirectionalLightData.direction);
                cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
            }
            //カメラへの方向を算出
            float32_t3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
            //内積
            float32_t3 halfVector = normalize(-gDirectionalLightData.direction + toEye);
            float NDotH = dot(normalize(input.normal), halfVector);
            //反射強度
            float specularPow = pow(saturate(NDotH), gMaterialData.shininess);
            //拡散反射
            float32_t3 diffuse = gMaterialData.color.rgb * textureColor.rgb * gDirectionalLightData.color.rgb * cos * gDirectionalLightData.intensity;
            //鏡面反射
            float32_t3 specular = gDirectionalLightData.color.rgb * gDirectionalLightData.intensity * specularPow * gMaterialData.shininessColor;
            
            if (gMaterialData.isSpecularReflection)
            {
                output.color.rgb += diffuse + specular;
            }
            else
            {
                output.color.rgb += diffuse;
            }
            
            output.color.a = gMaterialData.color.a * textureColor.a;
        }
        
        //ポイントライトの処理
        if (gPointLightData.lightingType == 1 || gPointLightData.lightingType == 2)
        {
            float cos;
            float32_t3 direction = normalize(input.worldPosition - gPointLightData.position);
            if (gPointLightData.lightingType == 1)
            {    
                cos = saturate(dot(normalize(input.normal), -direction));
            }
            else if (gPointLightData.lightingType == 2)
            {
                float NdotL = dot(normalize(input.normal), -direction);
                cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
            }
            //カメラへの方向を算出
            float32_t3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
            //内積
            float32_t3 halfVector = normalize(-direction + toEye);
            float NDotH = dot(normalize(input.normal), halfVector);
            //反射強度
            float specularPow = pow(saturate(NDotH), gMaterialData.shininess);
            //逆二乗
            float32_t distance = length(gPointLightData.position - input.worldPosition);
            float32_t factor = 1.0f / (distance * distance);
            //拡散反射
            float32_t3 diffuse = gMaterialData.color.rgb * textureColor.rgb * gPointLightData.color.rgb * cos * gPointLightData.intensity * factor;
            //鏡面反射
            float32_t3 specular = gPointLightData.color.rgb * gPointLightData.intensity * specularPow * gMaterialData.shininessColor * factor;
            
            if (gMaterialData.isSpecularReflection)
            {
                output.color.rgb += diffuse + specular;
            }
            else
            {
                output.color.rgb += diffuse;
            }
                    
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