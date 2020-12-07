#include "Share.fx"

struct VS_IN
{
	float3 vPos	:	POSITION;
	float2 vUV	:	TEXCOORD;
	float4 vColor	:	COLOR;
};

struct VS_OUT
{
	float4 vPos	:	SV_POSITION;
	float2 vUV	:	TEXCOORD;
	float4 vColor	:	COLOR;
};

cbuffer Bar	:	register(b10)
{
	float	g_fPercent;
	int		g_iDir;
	float2	g_E;
}

#define	LEFT	0
#define	TOP		1
#define	RIGHT	2
#define	BOT		3

VS_OUT VS(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	float3 vPos = input.vPos - g_vPivot * g_vMeshSize;

	output.vPos = mul(float4(vPos, 1.f), g_matWVP);
	output.vUV = input.vUV;
	output.vColor = input.vColor;

	return output;
}

PS_OUTPUT_COLOR PS(VS_OUT input)
{
	PS_OUTPUT_COLOR output = (PS_OUTPUT_COLOR)0;

	output.vColor = gDiffuseMap.Sample(g_sPoint, input.vUV) * g_vDif;

	if (output.vColor.r == 1.f &&
		output.vColor.g == 1.f &&
		output.vColor.b == 1.f &&
		output.vColor.a != 1.f)
		clip(-1);

	return output;
}

VS_OUT BarVS(VS_IN input)
{
	VS_OUT output = (VS_OUT)0.f;

	float3 vPos = input.vPos - g_vPivot * g_vMeshSize;

	output.vPos = mul(float4(vPos, 1.f), g_matWVP);
	output.vColor = input.vColor;
	output.vUV = input.vUV;

	return output;	
}

PS_OUTPUT_COLOR BarPS(VS_OUT input)
{
	PS_OUTPUT_COLOR output = (PS_OUTPUT_COLOR)0.f;

	if (g_iDir == LEFT)
	{
		if (input.vUV.x < 1.f - g_fPercent)
			clip(-1);
	}

	else if (g_iDir == TOP)
	{
		if (input.vUV.y < 1.f - g_fPercent)
			clip(-1);
	}

	else if (g_iDir == RIGHT)
	{
		if (input.vUV.x > g_fPercent)
			clip(-1);
	}

	else if (g_iDir == BOT)
	{
		if (input.vUV.y > g_fPercent)
			clip(-1);
	}

	output.vColor = gDiffuseMap.Sample(g_sPoint, input.vUV) * g_vDif;

	return output;
}