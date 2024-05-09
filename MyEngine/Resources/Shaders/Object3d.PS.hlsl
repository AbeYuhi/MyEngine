#include "object3d.hlsli"

static const int kDirectionLightNum = 1;
static const int kPointLightNum = 3;
static const int kSpotLightNum = 3;

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

struct PointLightData
{
    int32_t lightingType;
    float32_t4 color;
    float32_t3 position;
    float32_t intensity;
    float32_t radius;
    float32_t decay;
};

struct SpotLightData
{
    int32_t lightingType;
    float32_t4 color;
    float32_t3 position;
    float32_t intensity;
    float32_t3 direction;
    float32_t distance;
    float32_t decay;
    float32_t cosAngle;
    float32_t falloffStart;
};

struct LightData
{
    DirectionalLightData directionalLight[kDirectionLightNum];
    PointLightData pointLight[kPointLightNum];
    SpotLightData spotLight[kSpotLightNum];
};
ConstantBuffer<LightData> gLightData : register(b2);

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
    bool isLightting = false;
    float4 transformUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterialData.uvTransform);
    float32_t4 textureColor = gTexture.Sample(gSampler, transformUV.xy);
    
    if (gMaterialData.enableLighting != 0)
    {
        //平行光源
        for (int directionalLightIndex = 0; directionalLightIndex < kDirectionLightNum; directionalLightIndex++)
        {
            if (gLightData.directionalLight[directionalLightIndex].lightingType == 1 || gLightData.directionalLight[directionalLightIndex].lightingType == 2)
            {
                isLightting = true;
                float cos;
                if (gLightData.directionalLight[directionalLightIndex].lightingType == 1)
                {
                    cos = saturate(dot(normalize(input.normal), -gLightData.directionalLight[directionalLightIndex].direction));
                }
                else if (gLightData.directionalLight[directionalLightIndex].lightingType == 2)
                {
                    float NdotL = dot(normalize(input.normal), -gLightData.directionalLight[directionalLightIndex].direction);
                    cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
                }
                //カメラへの方向を算出
                float32_t3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
                //内積
                float32_t3 halfVector = normalize(-gLightData.directionalLight[directionalLightIndex].direction + toEye);
                float NDotH = dot(normalize(input.normal), halfVector);
                //反射強度
                float specularPow = pow(saturate(NDotH), gMaterialData.shininess);
                //拡散反射
                float32_t3 diffuse = gMaterialData.color.rgb * textureColor.rgb * gLightData.directionalLight[directionalLightIndex].color.rgb * cos * gLightData.directionalLight[directionalLightIndex].intensity;
                //鏡面反射
                float32_t3 specular = gLightData.directionalLight[directionalLightIndex].color.rgb * gLightData.directionalLight[directionalLightIndex].intensity * specularPow * gMaterialData.shininessColor;
            
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
        
        for (int pointLightIndex = 0; pointLightIndex < kPointLightNum; pointLightIndex++)
        {
        //ポイントライトの処理
            if (gLightData.pointLight[pointLightIndex].lightingType == 1 || gLightData.pointLight[pointLightIndex].lightingType == 2)
            {
                isLightting = true;
                float cos;
                float32_t3 direction = normalize(input.worldPosition - gLightData.pointLight[pointLightIndex].position);
                if (gLightData.pointLight[pointLightIndex].lightingType == 1)
                {
                    cos = saturate(dot(normalize(input.normal), -direction));
                }
                else if (gLightData.pointLight[pointLightIndex].lightingType == 2)
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
                float32_t distance = length(gLightData.pointLight[pointLightIndex].position - input.worldPosition);
                float32_t factor = saturate(pow(-distance / gLightData.pointLight[pointLightIndex].radius + 1.0f, gLightData.pointLight[pointLightIndex].decay));
                //拡散反射
                float32_t3 diffuse = gMaterialData.color.rgb * textureColor.rgb * gLightData.pointLight[pointLightIndex].color.rgb * cos * gLightData.pointLight[pointLightIndex].intensity * factor;
                //鏡面反射
                float32_t3 specular = gLightData.pointLight[pointLightIndex].color.rgb * gLightData.pointLight[pointLightIndex].intensity * specularPow * gMaterialData.shininessColor * factor;
            
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
        
        for (int spotLightIndex = 0; spotLightIndex < kSpotLightNum; spotLightIndex++)
        {
        //スポットライト
            if (gLightData.spotLight[spotLightIndex].lightingType == 1 || gLightData.spotLight[spotLightIndex].lightingType == 2)
            {
                isLightting = true;
                float cos;
                float32_t3 directionOnSurface = normalize(input.worldPosition - gLightData.spotLight[spotLightIndex].position);
                if (gLightData.spotLight[spotLightIndex].lightingType == 1)
                {
                    cos = saturate(dot(normalize(input.normal), -directionOnSurface));
                }
                else if (gLightData.spotLight[spotLightIndex].lightingType == 2)
                {
                    float NdotL = dot(normalize(input.normal), -directionOnSurface);
                    cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
                }
                //カメラへの方向を算出
                float32_t3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
                //内積
                float32_t3 halfVector = normalize(-directionOnSurface + toEye);
                float NDotH = dot(normalize(input.normal), halfVector);
                //反射強度
                float specularPow = pow(saturate(NDotH), gMaterialData.shininess);
                //falloff
                float32_t cosAngle = dot(directionOnSurface, gLightData.spotLight[spotLightIndex].direction);
                float32_t falloffFactor = saturate((cosAngle - gLightData.spotLight[spotLightIndex].cosAngle) / (gLightData.spotLight[spotLightIndex].falloffStart - gLightData.spotLight[spotLightIndex].cosAngle));
                //逆二乗
                float32_t distance = length(gLightData.spotLight[spotLightIndex].position - input.worldPosition);
                float32_t attenuationFactor = saturate(pow(-distance / gLightData.spotLight[spotLightIndex].distance + 1.0f, gLightData.spotLight[spotLightIndex].decay));
                //拡散反射
                float32_t3 diffuse = gMaterialData.color.rgb * textureColor.rgb * gLightData.spotLight[spotLightIndex].color.rgb * cos * gLightData.spotLight[spotLightIndex].intensity * attenuationFactor * falloffFactor;
                //鏡面反射
                float32_t3 specular = gLightData.spotLight[spotLightIndex].color.rgb * gLightData.spotLight[spotLightIndex].intensity * specularPow * gMaterialData.shininessColor * attenuationFactor * falloffFactor;
            
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
        
        //すべてのライトデータがNoneだった場合
        if (!isLightting)
        {
            output.color = gMaterialData.color * textureColor;
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