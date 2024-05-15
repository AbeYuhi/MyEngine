#include "FullScreen.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float32_t4 color : SV_Target0;
};

struct KernelSize
{
    int size;
};
ConstantBuffer<KernelSize> gKernelSize : register(b0);

static const float32_t2 kIndex3x3[3][3] =
{
    { { -1.0f, -1.0f }, { 0.0f, -1.0f }, { 1.0f, -1.0f } },
    { { -1.0f, 0.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f } },
    { { -1.0f, 1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f } },
};

static const float32_t kKernel3x3[3][3] =
{
    { 1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f },
    { 1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f },
    { 1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f },
};

PixelShaderOutput main(VertexShaderOutput input)
{
    uint32_t width, height;
    gTexture.GetDimensions(width, height);
    float32_t2 uvStepSize = float32_t2(rcp(width), rcp(height));
    
    PixelShaderOutput output;
    output.color.rgb = float32_t3(0.0f, 0.0f, 0.0f);
    output.color.a = 1.0f;
    for (int x = 0; x < gKernelSize.size; x++)
    {
        for (int y = 0; y < gKernelSize.size; y++)
        {
            float32_t2 index = float32_t2(x - (gKernelSize.size - 1.0f) / 2.0f, y - (gKernelSize.size - 1.0f) / 2.0f);
            float32_t2 texcoord = input.texcoord + index * uvStepSize;
            float32_t3 fetchColor = gTexture.Sample(gSampler, texcoord).rgb;
            output.color.rgb += fetchColor * rcp(pow(gKernelSize.size, 2.0f));
            
            //float32_t2 texcoord = input.texcoord + kIndex3x3[x][y] * uvStepSize;
            //float32_t3 fetchColor = gTexture.Sample(gSampler, texcoord).rgb;
            //output.color.rgb += fetchColor * kKernel3x3[x][y];
        }

    }
    
    return output;
}