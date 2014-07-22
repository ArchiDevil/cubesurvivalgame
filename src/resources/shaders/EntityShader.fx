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
	float3 lightPos;
	float4 MaskColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
};

Texture2D tex;

SamplerState Sampler
{
	Filter = MIN_MAG_MIP_LINEAR;
};

struct vsInput
{
	float3 Position : POSITION;
	float3 Normal	: NORMAL;
	float2 Texcoord : TEXCOORD;
};

struct vsOutput
{
	float4 Position : SV_POSITION;
	float3 Normal 	: NORMAL;
	float2 Texcoord : TEXCOORD;
};

vsOutput VS (vsInput Input)
{
	vsOutput Output;
	Output.Position = mul(float4(Input.Position, 1.0f), matWorld);
	Output.Position = mul(Output.Position, matView);
	Output.Position = mul(Output.Position, matProjection);
	Output.Normal = mul(Input.Normal, (float3x3)matWorld);
	Output.Texcoord = Input.Texcoord;
	return Output;
};

float4 PS (vsOutput Input) : SV_TARGET
{
	float4 outw = tex.Sample(Sampler, Input.Texcoord);
	float lightIntensity = clamp(0.0f, 1.0f, dot(normalize(Input.Normal), normalize(lightPos)));
	outw.xyz = outw.xyz * lightIntensity;
	return outw * MaskColor;
};