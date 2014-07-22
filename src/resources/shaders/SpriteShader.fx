float4x4 matRes;
Texture2D Texture;
float4 MaskColor;

SamplerState SS
{
    Filter = MIN_MAG_MIP_POINT;
};

struct VS_I
{
	float2 Position : POSITION;
	float2 Texcoord	: TEXCOORD;
};

struct VS_O
{
	float4 Position : SV_POSITION;
	float2 Texcoord	: TEXCOORD;
};

VS_O VS(VS_I Input)
{
	VS_O Output;
	Output.Position = mul(float4(Input.Position, 0.0f, 1.0f), matRes);
	Output.Texcoord = Input.Texcoord;
	return Output;
};

float4 PS(VS_O Input) : SV_TARGET
{
	//return float4(Input.Texcoord.x, Input.Texcoord.y, 0.0, 1.0);
	return MaskColor * Texture.Sample(SS, Input.Texcoord);
};