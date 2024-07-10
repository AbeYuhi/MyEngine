#include "FullScreen.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct Material
{
    float32_t hue;
    float32_t saturation;
    float32_t value;
};
ConstantBuffer<Material> gMaterial : register(b0);

struct PixelShaderOutput
{
    float32_t4 color : SV_Target0;
};

float32_t3 RGBToHSV(float32_t3 rgb)
{
    float max = rgb.x > rgb.y ? rgb.x : rgb.y;
    max = max > rgb.z ? max : rgb.z;
    float min = rgb.x < rgb.y ? rgb.x : rgb.y;
    min = min < rgb.z ? min : rgb.z;
    
    float32_t3 hsv;
    
    hsv.x = max - min;
    if (hsv.x > 0.0f)
    {
        if (max == rgb.x)
        {
            hsv.x = (rgb.y - rgb.z) / hsv.x;
            if (hsv.x < 0.0f)
            {
                hsv.x += 6.0f;
            }
        }
        else if (max == rgb.y)
        {
            hsv.x = 2.0f + (rgb.z - rgb.x) / hsv.x;
        }
        else
        {
            hsv.x = 4.0f + (rgb.x - rgb.y) / hsv.x;
        }
    }
    hsv.x /= 6.0f;
    hsv.y = (max - min);
    if (max != 0.0f)
        hsv.y /= max;
    hsv.z = max;
    
    return hsv;
}

float32_t3 HSVToRGB(float32_t3 hsv)
{
    float32_t3 rgb;
    rgb.x = hsv.z;
    rgb.y = hsv.z;
    rgb.z = hsv.z;
    if (hsv.y > 0.0f)
    {
        hsv.x *= 6.0f;
        const int i = (int)hsv.x;
        const float f = hsv.x - (float)i;
        switch (i)
        {
            default:
            case 0:
                rgb.y *= 1 - hsv.y * (1 - f);
                rgb.z *= 1 - hsv.y;
                break;
            case 1:
                rgb.x *= 1 - hsv.y * f;
                rgb.z *= 1 - hsv.y;
                break;
            case 2:
                rgb.x *= 1 - hsv.y;
                rgb.z *= 1 - hsv.y * (1 - f);
                break;
            case 3:
                rgb.x *= 1 - hsv.y;
                rgb.y *= 1 - hsv.y * f;
                break;
            case 4:
                rgb.x *= 1 - hsv.y * (1 - f);
                rgb.y *= 1 - hsv.y;
                break;
            case 5:
                rgb.y *= 1 - hsv.y;
                rgb.z *= 1 - hsv.y * f;
                break;
        }
    }
    
    return rgb;
}

float32_t WrapValue(float32_t value, float32_t minRange, float32_t maxRange)
{
    float32_t range = maxRange - minRange;
    float32_t modValue = fmod(value - minRange, range);
    if (modValue < 0)
    {
        modValue += range;
    }
    
    return minRange + modValue;
}

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    
    float32_t4 textureColor = gTexture.Sample(gSampler, input.texcoord);
    float32_t3 hsv = RGBToHSV(textureColor.rgb);
    
    hsv.x += gMaterial.hue;
    hsv.y += gMaterial.saturation;
    hsv.z += gMaterial.value;
    
    hsv.x = WrapValue(hsv.x, 0.0f, 1.0f);
    hsv.y = saturate(hsv.y);
    hsv.z = saturate(hsv.z);
    
    float32_t3 rgb = HSVToRGB(hsv);
    
    output.color.rgb = rgb;
    output.color.a = textureColor.a;
    return output;
}