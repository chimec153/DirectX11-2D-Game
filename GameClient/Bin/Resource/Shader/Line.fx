#include "Share.fx"

cbuffer Angle	:	register(b12)
{
	float g_fAngle;
	float3 g_vE12;
}

struct VIN
{
	float3 vPos		:	POSITION;
	float2	vUV		:	TEXCOORD;
};

struct VOUT
{
	float4	vPos	:	SV_POSITION;
	float2	vUV	:	TEXCOORD;
	float3	vPosW	:	TEXCOORD1;
};

VOUT VS(VIN input)
{
	VOUT output = (VOUT)0.f;

	output.vPos = mul(float4(input.vPos - g_vMeshSize * g_vPivot, 1.f), g_matWVP);
	output.vUV = input.vUV;
	output.vPosW = mul(float4(input.vPos, 1.f), g_matWorld).xyz;

	return output;
}

PS_OUTPUT_COLOR PS(VOUT input)	:	SV_Target
{
	PS_OUTPUT_COLOR output = (PS_OUTPUT_COLOR)0.f;

	float2 vAxis = float2(1.f, 0.f);

	float2 vPos = input.vUV - float2(0.5f, 0.5f);

	vPos = normalize(vPos);

	float fAngle = acos(vPos.x * vAxis.x + vPos.y * vAxis.y) / 3.141592f * 180.f;

	if (0.f> vPos.y)
	{
		fAngle *= -1.f;
	}

	float fAngleUpper = g_fAngle + 2.f;
	float fAngleLower = g_fAngle - 2.f;

	if (fAngleUpper > 180.f)
	{
		fAngleUpper -= 360.f;

		if (fAngleUpper >= fAngle ||
			fAngleLower <= fAngle)
		{
			output.vColor = float4(1.f, 1.f, 1.f, 0.5f);
		}
		else
		{
			clip(-1);
		}
	}

	else if (fAngleLower < -180.f)
	{
		fAngleLower += 360.f;

		if (fAngleUpper >= fAngle ||
			fAngleLower <= fAngle)
		{
			output.vColor = float4(1.f, 1.f, 1.f, 0.5f);
		}
		else
		{
			clip(-1);
		}
	}

	else
	{
		if (fAngleUpper >= fAngle &&
			fAngleLower <= fAngle)
		{
			output.vColor = float4(1.f, 1.f, 1.f, 0.5f);
		}
		else
		{
			clip(-1);
		}
	}


	return output;
}

struct VS_LINE_INPUT
{
	float3	vPos	:	POSITION;
};

struct VS_LINE_OUTPUT
{
	float3	vPos	:	POSITION;
};

struct GS_OUT
{
	float4	vPos	:	SV_POSITION;
};

VS_LINE_OUTPUT LineVS(VS_LINE_INPUT input)
{
	VS_LINE_OUTPUT output = (VS_LINE_OUTPUT)0;

	output.vPos = input.vPos;

	return output;
}

[maxvertexcount(6)]
void LineGS(line VS_LINE_OUTPUT Line[2], 
	inout TriangleStream<GS_OUT> TriStream)
{
	GS_OUT output[4] = 
	{
		(GS_OUT)0.f,
		(GS_OUT)0.f,
		(GS_OUT)0.f,
		(GS_OUT)0.f
	};

	float fAngle = g_fAngle / 180.f * 3.141592f;

	output[0].vPos = mul(float4(float3(0.f, 0.f, 0.f) + float3(-sin(fAngle), cos(fAngle), 0.f) * 0.002f, 1.f), g_matWVP);
	output[1].vPos = mul(float4(float3(cos(fAngle), sin(fAngle), 0.f) + float3(-sin(fAngle), cos(fAngle), 0.f) * 0.04f, 1.f), g_matWVP);
	output[2].vPos = mul(float4(float3(0.f, 0.f, 0.f) - float3(-sin(fAngle), cos(fAngle), 0.f) * 0.002f, 1.f), g_matWVP);
	output[3].vPos = mul(float4(float3(cos(fAngle), sin(fAngle), 0.f) - float3(-sin(fAngle), cos(fAngle), 0.f) * 0.04f, 1.f), g_matWVP);

	TriStream.Append(output[0]);
	TriStream.Append(output[1]);
	TriStream.Append(output[3]);
	TriStream.RestartStrip();

	TriStream.Append(output[0]);
	TriStream.Append(output[3]);
	TriStream.Append(output[2]);
	TriStream.RestartStrip();
}

PS_OUTPUT_COLOR LinePS(GS_OUT input)	:	SV_TARGET
{
	PS_OUTPUT_COLOR output = (PS_OUTPUT_COLOR)0;

	output.vColor = g_vDif;

	return output;
}