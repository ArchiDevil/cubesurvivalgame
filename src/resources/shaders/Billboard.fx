float4x4 World;
float4x4 View;
float4x4 Projection;

Texture2D Texture;

SamplerState Sampler
{
	Filter = MIN_MAG_MIP_LINEAR;
};

struct VS_I
{
	float3 Position : POSITION;
	float2 Texcoord : TEXCOORD;
	float3 Normal	: NORMAL;
};

struct VS_O
{
	float4 Position : SV_POSITION;
	float2 Texcoord : TEXCOORD;
	float3 Normal	: NORMAL;
};

VS_O VS (VS_I Input)
{
	VS_O Output;
	Output.Position = mul(float4(Input.Position, 1.0f), World);
	Output.Position = mul(Output.Position, View);
	Output.Position = mul(Output.Position, Projection);
	Output.Texcoord = Input.Texcoord;
	Output.Normal = mul(float4(Input.Normal), 1.0f), World);
	return Output;
};

float4 PS (VS_O Input) : SV_TARGET
{
	float4 outw = tex.Sample(Sampler, Input.Texcoord);
	return outw;
};