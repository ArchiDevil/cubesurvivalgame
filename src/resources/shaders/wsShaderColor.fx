cbuffer cbPerObject
{
	float4x4 matWorld;
};

cbuffer cbPerFrame
{
	float4x4 matView;
	float4x4 matProjection;
};

float3 LightDir = float3(-0.1f, 0.3f, -0.4f);	// directional light

struct VS_I
{
	float3 Position : POSITION;
	float3 Normal	: NORMAL;
	float2 Texcoord : TEXCOORD;
	float3 Color	: COLOR;
};

struct VS_O
{
	float4 Position : SV_POSITION;
	float3 Normal	: NORMAL;
	float3 Color	: COLOR;
};

VS_O VS (VS_I Input)
{
	VS_O Output;
	Output.Position = mul(float4(Input.Position, 1.0f), matWorld);
	Output.Position = mul(Output.Position, matView);
	Output.Position = mul(Output.Position, matProjection);
	Output.Normal = mul(float4(Input.Normal, 0.0f), matWorld).xyz;
	Output.Color = Input.Color;
	return Output;
};

float4 PS (VS_O Input) : SV_TARGET
{
	//there's should be Ambient + Diffuse + Specular components
	float3 AmbientColor = float3(0.2f, 0.2f, 0.2f);
	float lightInt = max( dot( normalize(-LightDir), normalize(Input.Normal) ), 0.0f);
	float3 result = float3(lightInt.xxx) + AmbientColor;
	result *= Input.Color;
	return float4(result, 1.0f);
};