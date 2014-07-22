cbuffer cbPerObject
{
	float4x4 matWorld;
};

cbuffer cbPerFrame
{
	float4x4 matView;
	float4x4 matProjection;
	float3 sunPos;
	float3 eyePos;
};

cbuffer cbRarely
{
	float turbidity = 2.0f;
	float3 sunColor = float3(1.0f, 1.0f, 0.8f);
};

struct VS_INPUT 
{
	float3 Position : POSITION;
};

struct VS_OUTPUT 
{
	float4 Position : SV_POSITION;
	float4 Pos		: POSITION;
};

VS_OUTPUT VS( VS_INPUT Input )
{
	VS_OUTPUT Output;
	Output.Position		= mul( float4(Input.Position, 1.0f), matWorld );
	Output.Pos			= Output.Position;
	Output.Position 	= mul( Output.Position, matView );
	Output.Position 	= mul( Output.Position, matProjection );
	return Output;
}

//
// Perez zenith func
//

float3 perezZenith ( float t, float thetaSun )
{
	const float pi = 3.1415926;
	const float4 cx1 = float4 ( 0,       0.00209, -0.00375, 0.00165  );
	const float4 cx2 = float4 ( 0.00394, -0.03202, 0.06377, -0.02903 );
	const float4 cx3 = float4 ( 0.25886, 0.06052, -0.21196, 0.11693  );
	const float4 cy1 = float4 ( 0.0,     0.00317, -0.00610, 0.00275  );
	const float4 cy2 = float4 ( 0.00516, -0.04153, 0.08970, -0.04214 );
	const float4 cy3 = float4 ( 0.26688, 0.06670, -0.26756, 0.15346  );

	float t2 = t*t;
	float chi = (4.0 / 9.0 - t / 120.0 ) * (pi - 2.0 * thetaSun );
	float4 theta = float4 ( 1, thetaSun, thetaSun * thetaSun, thetaSun * thetaSun * thetaSun );

	float Y = (4.0453 * t - 4.9710) * tan ( chi ) - 0.2155 * t + 2.4192;
	float x = t2 * dot ( cx1, theta ) + t * dot ( cx2, theta ) + dot ( cx3, theta );
	float y = t2 * dot ( cy1, theta ) + t * dot ( cy2, theta ) + dot ( cy3, theta );

	return float3 ( Y, x, y );
}

//
// Perez allweather func (turbidity, cosTheta, cosGamma)
//

float3 perezFunc ( float t, float cosTheta, float cosGamma )
{
	float  gamma      = acos ( cosGamma );
	float  cosGammaSq = cosGamma * cosGamma;
	float  aY =  0.17872 * t - 1.46303;
	float  bY = -0.35540 * t + 0.42749;
	float  cY = -0.02266 * t + 5.32505;
	float  dY =  0.12064 * t - 2.57705;
	float  eY = -0.06696 * t + 0.37027;
	float  ax = -0.01925 * t - 0.25922;
	float  bx = -0.06651 * t + 0.00081;
	float  cx = -0.00041 * t + 0.21247;
	float  dx = -0.06409 * t - 0.89887;
	float  ex = -0.00325 * t + 0.04517;
	float  ay = -0.01669 * t - 0.26078;
	float  by = -0.09495 * t + 0.00921;
	float  cy = -0.00792 * t + 0.21023;
	float  dy = -0.04405 * t - 1.65369;
	float  ey = -0.01092 * t + 0.05291;

	return float3 ( (1.0 + aY * exp(bY/cosTheta)) * (1.0 + cY * exp(dY * gamma) + eY*cosGammaSq),
		(1.0 + ax * exp(bx/cosTheta)) * (1.0 + cx * exp(dx * gamma) + ex*cosGammaSq),
		(1.0 + ay * exp(by/cosTheta)) * (1.0 + cy * exp(dy * gamma) + ey*cosGammaSq) );
}

float3 perezSky ( float t, float cosTheta, float cosGamma, float cosThetaSun )
{
	float thetaSun = acos        ( cosThetaSun );
	float3  zenith   = perezZenith ( t, thetaSun );
	float3  clrYxy   = zenith * perezFunc ( t, cosTheta, cosGamma ) / perezFunc ( t, 1.0, cosThetaSun );

	clrYxy [0] *= smoothstep ( -0.3, 0.1, cosThetaSun );

	return clrYxy;
}

float3 convertColor (float3 colorYxy)
{
	float3  clrYxy = colorYxy;
		// now rescale Y component
	clrYxy [0] = 1.0 - exp ( -clrYxy [0] / 25.0 );

	float ratio = clrYxy [0] / clrYxy [2];  // Y / y = X + Y + Z
	float3  XYZ;

	XYZ.x = clrYxy [1] * ratio;             // X = x * ratio
	XYZ.y = clrYxy [0];                     // Y = Y
	XYZ.z = ratio - XYZ.x - XYZ.y;          // Z = ratio - X - Y

	const float3 rCoeffs = float3 ( 3.240479, -1.53715, -0.49853  );
	const float3 gCoeffs = float3 ( -0.969256, 1.875991, 0.041556 );
	const float3 bCoeffs = float3 ( 0.055684, -0.204043, 1.057311 );

	return float3 ( dot ( rCoeffs, XYZ ), dot ( gCoeffs, XYZ ), dot ( bCoeffs, XYZ ) );
}


float4 PS(VS_OUTPUT Input) : SV_TARGET
{   
	float3 v				= normalize( Input.Pos.xyz - eyePos );
	float3 l				= normalize( sunPos );
	float dotLV				= dot(l, v);
	float dotTV				= dot(float3(0.0, 0.0, 1.0), v);
	float3 perezColor		= perezSky( turbidity, max( v.z, 0.0 ) + 0.05, dotLV, l.z );

	dotLV					= clamp(dotLV, 0.0, 1.0);
	dotTV					= clamp(dotTV + 0.25, 0.0, 1.0);
	float sunFactor			= pow(dotLV, 100) * pow(dotTV, 0.4);

	float3 convertedColor	= convertColor( perezColor );
	float4 Color			= float4( clamp(convertedColor, 0.0, 1.0) + float3(1.0, 1.0, 1.0) * sunFactor, 1.0 );
	return Color;
}