cbuffer cbPerObject
{
	float4x4 matWorld;
};

cbuffer cbPerFrame
{
	float4x4 matView;
	float4x4 matProjection;
	float3 PlayerPosition = float3(0.0f, 0.0f, 90.0f);
	float3 SunPosition = float3(0.0f, 0.0f, 90000.0f);
};

cbuffer cbRarely
{
	float3 DiffuseColor = float3(0.8f, 0.8f, 0.8f);
	float3 AmbientColor = float3(0.5f, 0.5f, 0.5f);
	float fogEnd = 200.0f;
	float fogStart = 66.0f;
	float3 fogColor = float3(0.81568f, 0.93333f, 0.97255f);
	float SunIntensity = 1.0f;
};

SamplerState testSS;

Texture2DArray diffuseMap;

struct VS_IN
{
	float3 Pos  	: POSITION; 	//позиция вершины
	float3 Texc		: TEXCOORD; 	//текстура вершины
	float3 Normal 	: NORMAL;		//нормали
	float Lightness : COLOR;		//освещенность
};

struct VS_OUT
{
	float4 Pos		: SV_POSITION;	//выходная - умноженная позиция
	float3 Texc		: TEXCOORD;
	float3 Normal 	: NORMAL;
	float Lightness : LIGHTNESS;
	float FogFactor : FOGFACTOR;
};

VS_OUT VS(VS_IN Input)
{
	VS_OUT Output;
	float4 Position = float4(Input.Pos, 1.0f);
	Position = mul(Position, matWorld);
	float d = distance(Position.xyz, PlayerPosition);
	Output.FogFactor = saturate( (d - fogStart) / fogEnd );
	Position = mul(Position, matView);
	Position = mul(Position, matProjection);
	Output.Pos = Position;
	Output.Texc = Input.Texc;
	Output.Normal = Input.Normal;
	Output.Lightness = Input.Lightness / 10;
	return Output;
};

float4 PS(VS_OUT Input) : SV_TARGET
{
	float3 LightDir = SunPosition;
	float d = length(LightDir);
	float3 color = AmbientColor * Input.Lightness;
	float DiffuseIntensity = dot( normalize(LightDir), normalize(Input.Normal) ) * Input.Lightness;

	if(DiffuseIntensity > 0.0f)
		color += DiffuseIntensity * DiffuseColor;

	float3 TexColor = diffuseMap.Sample(testSS, Input.Texc).xyz * color * SunIntensity;
	//return float4(fogColor, 1.0f);
	//return float4(Input.FogFactor, 0.0f, 0.0f, 1.0f);
	return float4(TexColor, 1.0f);
};