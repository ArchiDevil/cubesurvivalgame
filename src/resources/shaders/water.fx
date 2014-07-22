cbuffer cbPerObject
{
	float4x4 matWorld;
};

cbuffer cbPerFrame
{
	float4x4 matView;
	float4x4 matProjection;
};

struct VS_IN
{
	float3 Pos  	: POSITION;
};

struct VS_OUT
{
	float4 Pos		: SV_POSITION;
};

VS_OUT VS(VS_IN Input)
{
	VS_OUT Output;
	float4 Position = float4(Input.Pos, 1.0f);
	Position = mul(Position, matWorld);
	Position = mul(Position, matView);
	Position = mul(Position, matProjection);
	Output.Pos = Position;
	return Output;
};

float4 PS(VS_OUT Input) : SV_TARGET
{
	return float4(118.0f/255, 212.0f/255, 247.0f/255, 0.3f);
};