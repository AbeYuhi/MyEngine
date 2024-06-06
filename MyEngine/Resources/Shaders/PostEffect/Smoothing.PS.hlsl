#include "FullScreen.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float32_t4 color : SV_Target0;
};

struct SmoothingInfo
{
    int32_t kernelSize;
    int32_t type;
    float blurStrength;
};
ConstantBuffer<SmoothingInfo> gSmoothingInfo : register(b0);

static const float32_t PI = 3.14159265f;

float32_t4 LinearFilter(VertexShaderOutput input)
{
    uint32_t width, height;
    gTexture.GetDimensions(width, height);
    float32_t2 uvStepSize = float32_t2(rcp(width), rcp(height));
    
    float32_t4 color;
    color.rgb = float32_t3(0.0f, 0.0f, 0.0f);
    color.a = 1.0f;
    
    for (int x = -gSmoothingInfo.kernelSize; x <= gSmoothingInfo.kernelSize; x++)
    {
        for (int y = -gSmoothingInfo.kernelSize; y <= gSmoothingInfo.kernelSize; y++)
        {
            float32_t2 index = float32_t2(x, y);
            float32_t2 texcoord = input.texcoord + index * uvStepSize;
            float32_t3 fetchColor = gTexture.Sample(gSampler, texcoord).rgb;
            color.rgb += fetchColor * rcp(pow(gSmoothingInfo.kernelSize * 2 + 1, 2.0f));
        }
    }
    
    return color;
}

float gauss(float x, float y, float sigma)
{
    float exponent = -(x * x + y * y) * rcp(2.0f * sigma * sigma);
    float denominator = 2.0f * PI * sigma * sigma;
    return exp(exponent) * rcp(denominator);
}

float32_t4 GaussFilter(VertexShaderOutput input)
{
    uint32_t width, height;
    gTexture.GetDimensions(width, height);
    float32_t2 uvStepSize = float32_t2(rcp(width), rcp(height));
    float32_t weight = 0.0f;
    
    float32_t4 color;
    color.rgb = float32_t3(0.0f, 0.0f, 0.0f);
    color.a = 1.0f;
    
    for (int x = -gSmoothingInfo.kernelSize; x <= gSmoothingInfo.kernelSize; x++)
    {
        for (int y = -gSmoothingInfo.kernelSize; y <= gSmoothingInfo.kernelSize; y++)
        {
            float32_t2 index = float32_t2(x, y);
            float32_t2 texcoord = input.texcoord + index * uvStepSize;
            float32_t3 fetchColor = gTexture.Sample(gSampler, texcoord).rgb;
            color.rgb += fetchColor * gauss(x, y, gSmoothingInfo.blurStrength);
            weight += gauss(x, y, gSmoothingInfo.blurStrength);
        }
    }
    
    color.rgb *= rcp(weight);
    return color;
}

PixelShaderOutput main(VertexShaderOutput input)
{   
    
    PixelShaderOutput output;
    
    if (gSmoothingInfo.type == 0)
    {
        output.color = LinearFilter(input);
    }
    else
    {
        output.color = GaussFilter(input);
    }
    
    return output;
}