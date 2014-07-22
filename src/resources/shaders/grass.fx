cbuffer cbPerObject
{
	float4x4 matWorld;
};

cbuffer cbPerFrame
{
	float4x4 matView;
	float4x4 matProjection;
};

cbuffer cbRarely
{
	float Time;
	float sunIntensity;
};

Texture2D Diffuse;

SamplerState SS
{
	Filter = MIN_MAG_MIP_LINEAR;
};

struct VS_I
{
	float3 Position : POSITION;
	float2 Texcoord : TEXCOORD;
	float Animation : ANIMVALUE;
};

struct VS_O
{
	float4 Position : SV_POSITION;
	float2 Texcoord : TEXCOORD;
};

VS_O VS (VS_I Input)
{
	VS_O Output;
	float3 Pos = Input.Position;
	Pos.x += sin(Pos.x + Time) * 0.25f * Input.Animation;
	Pos.y += sin(Pos.y + Time) * 0.21f * Input.Animation;
	Output.Position = mul(float4(Pos, 1.0f), matWorld);
	Output.Position = mul(Output.Position, matView);
	Output.Position = mul(Output.Position, matProjection);
	Output.Texcoord = Input.Texcoord;
	return Output;
};

float4 PS (VS_O Input) : SV_TARGET
{
	float4 Output;
	Output = Diffuse.Sample(SS, Input.Texcoord);
	clip(Output.w - 0.5f);
	return float4(Output.xyz * sunIntensity, Output.w);
};