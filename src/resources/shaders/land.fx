cbuffer cbPerObject
{
	float4x4 matWorld;
};

cbuffer cbPerFrame
{
	float4x4 matView;
	float4x4 matProjection;
	float3 SunPosition = float3(0.0f, 0.0f, 90000.0f);
};

cbuffer cbRarely
{
	float3 DiffuseColor = float3(1.0f, 1.0f, 1.0f);
	float3 AmbientColor = float3(0.1f, 0.1f, 0.15f);
};

SamplerState testSS;

struct VS_IN
{
	float3 Pos  	: POSITION;
	float3 Normal 	: NORMAL;
	float3 Color    : COLOR;
};

struct VS_OUT
{
	float4 Pos		: SV_POSITION;
	float3 Normal 	: NORMAL;
	float3 Color    : COLOR;
};

VS_OUT VS(VS_IN Input)
{
	VS_OUT Output;
	float4 Position = float4(Input.Pos, 1.0f);
	Position = mul(Position, matWorld);
	Position = mul(Position, matView);
	Position = mul(Position, matProjection);
	Output.Pos = Position;
	Output.Normal = Input.Normal;
	Output.Color = Input.Color;
	return Output;
};

float4 PS(VS_OUT Input) : SV_TARGET
{
	float3 LightDir = normalize(SunPosition);
	float3 color = Input.Color;
	float DiffuseIntensity = max(0.0f, dot(LightDir, normalize(Input.Normal)));
	color *= DiffuseIntensity * DiffuseColor;
    color += AmbientColor;
	return float4(color, 1.0f);
};
