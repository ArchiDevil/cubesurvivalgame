cbuffer cbPerFrame
{
	float4 Color = float4(0.0f, 0.0f, 0.0f, 1.0f);
	Texture2D FontTexture;
	float4x4 matOrtho;
	float4 Rect = float4(0.0f, 0.0f, 16000.0f, 16000.0f);	//LEFT, TOP, RIGHT, BOTTOM
};

SamplerState SS
{
    Filter = ANISOTROPIC;
};

struct VS_I
{
	float2 Position : POSITION;
	float2 Texcoord	: TEXCOORD;
};

struct VS_O
{
	float4 Position : SV_POSITION;
	float2 Pos		: POSITION;
	float2 Texcoord	: TEXCOORD;
};

VS_O VS (VS_I Input)
{
	VS_O Output;
	Output.Position = mul(float4(Input.Position, 0.0f, 1.0f), matOrtho);
	Output.Pos = Input.Position;
	Output.Texcoord = Input.Texcoord;
	return Output;
};

float4 PS (VS_O Input) : SV_TARGET
{
	float4 Output = FontTexture.Sample(SS, Input.Texcoord) * Color;

	if(Input.Position.x < Rect[0])
		clip(-1);

	if(Input.Position.x > Rect[2])
		clip(-1);

	if(Input.Position.y < Rect[1])
		clip(-1);

	if(Input.Position.y > Rect[3])
		clip(-1);

	return Output;
};