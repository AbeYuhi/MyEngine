#include "object3d.hlsli"

struct TransformationMatrix
{
    float32_t4x4 WVP;
    float32_t4x4 World;
    float32_t4x4 WorldInverseTranspose;
};
ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);

struct BoneNodeInfo
{
    string name;
    float32_t4x4 localMatrix;
    uint32_t boneNum;
};
ConstantBuffer<BoneNodeInfo> gBoneNodeInfo : register(b1);

struct VertexShaderInput
{
    float32_t4 position : POSITION0;
    float32_t2 texcoord : TEXCOORD0;
    float32_t3 normal : NORMAL0;
    string boneName : BONENAME0;
};

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    output.position = mul(input.position, gTransformationMatrix.WVP);
    output.texcoord = input.texcoord;
    output.normal = normalize(mul(input.normal, (float32_t3x3) gTransformationMatrix.WorldInverseTranspose));
    output.worldPosition = mul(input.position, gTransformationMatrix.World).xyz;
    
    if (input.boneName != "")
    {
        BoneNodeInfo boneNodeInfo = gBoneNodeInfo[0];
        for (int index = 0; index < boneNodeInfo.boneNum; index++)
        {
            
        }

    }
    
    return output;
}