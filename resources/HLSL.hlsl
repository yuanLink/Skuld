Texture2D gImage : register(t0);
SamplerState gSampler : register(s0);

cbuffer Camera : register(b0)
{
	matrix gWorld;
	matrix gView;
	matrix gProjection;
	float4 gEye;
}

cbuffer TransformMatrix : register(b1)
{
	matrix gTransform;
}

struct PixelShaderInput
{
	float4 Pos : SV_POSITION;
};

struct BufferStruct
{
    uint4 color;
};
#define thread_group_size_x 4
#define thread_group_size_y 4
RWStructuredBuffer<BufferStruct> g_OutBuff;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

PixelShaderInput vs_main(float4 Position : POSITION)
{
	PixelShaderInput ret;
	ret.Pos = mul(gProjection, mul(gView, mul(gWorld, Position)));
	ret.Pos = mul(gTransform, ret.Pos);
	return ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float4 ps_main(PixelShaderInput Input) : SV_TARGET
{
	return float4(0.0f, 1.0f, 0.0f, 1.0f);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

[numthreads(thread_group_size_x, thread_group_size_y, 1)]
void ComputeShaderMain(uint3 threadIDInGroup : SV_GroupThreadID, uint3 groupID : SV_GroupID, uint groupIndex : SV_GroupIndex, uint3 dispatchThreadID : SV_DispatchThreadID)
{
    int N_THREAD_GROUPS_X = 16;

    int stride = thread_group_size_x * N_THREAD_GROUPS_X;
    int idx = dispatchThreadID.y * stride + dispatchThreadID.x;

    float4 color = float4(groupID.x, groupID.y, dispatchThreadID.x, dispatchThreadID.y);

    g_OutBuff[idx].color = color;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////