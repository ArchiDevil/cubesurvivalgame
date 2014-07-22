cbuffer cbPerObject
{
	float4x4 matWorld;
};

cbuffer cbPerFrame
{
	float4x4 matView;
	float4x4 matProjection;
};

float4 Color = float4(1.0f, 0.7f, 0.7f, 0.3f);

struct VS_I
{
	float3 Position : POSITION;
	float3 Normal   : NORMAL;
	float2 Texcoord : TEXCOORD;
};

struct VS_O
{
	float4 Position : SV_POSITION;
};

VS_O VS (VS_I Input)
{
	VS_O Output;
	Output.Position = mul(float4(Input.Position, 1.0f), matWorld);
	Output.Position = mul(Output.Position, matView);
	Output.Position = mul(Output.Position, matProjection);
	return Output;
};

float4 PS (VS_O Input) : SV_TARGET
{
	return Color;
};