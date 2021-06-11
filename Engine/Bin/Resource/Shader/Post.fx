#include "Share.fx"

struct VS_IN_PAPER
{
	float3	vPos	:	POSITION;
	float2	vUV	:	TEXCOORD;
};

struct VS_OUT_PAPER
{
	float4	vPos	:	SV_POSITION;
	float3	vProjPos	:	POSITION;
	float2	vUV	:	TEXCOORD;
};

VS_OUT_PAPER PaperVS(VS_IN_PAPER input)
{
	VS_OUT_PAPER output = (VS_OUT_PAPER)0.f;

	float3 vPos = input.vPos - g_vPivot * g_vMeshSize;

	output.vPos = mul(float4(vPos,1.f), g_matWVP);
	output.vUV = input.vUV;

	return output;
}

PS_OUTPUT_COLOR PaperPS(VS_OUT_PAPER input, float4 vPos	:	SV_POSITION)
{
	PS_OUTPUT_COLOR output = (PS_OUTPUT_COLOR)0.f;
	output.vColor = gDiffuseMap.Sample(g_sPoint, input.vUV) * g_vDif;
	output.vColor = PaperBurn(output.vColor, vPos.xy);
	return output;
}

VS_OUT_PAPER DistortVS(VS_IN_PAPER input)
{
	VS_OUT_PAPER output = (VS_OUT_PAPER)0.f;

	float3 vPos = input.vPos - g_vPivot * g_vMeshSize;

	output.vPos = mul(float4(vPos, 1.f), g_matWVP);
	output.vUV = input.vUV + float2(0.f, g_fAmp * sin(g_fAccTime * g_fSpeed));
	output.vProjPos.xy = output.vPos.xy / output.vPos.w;
	output.vProjPos.xy += 1.f;
	output.vProjPos.xy /= 2.f;
	output.vProjPos.y = 1.f - output.vProjPos.y;
	return output;
}

PS_OUTPUT_COLOR DistortPS(VS_OUT_PAPER input, float4 vPos :	SV_POSITION)
{
	PS_OUTPUT_COLOR output = (PS_OUTPUT_COLOR)0.f;

	float4 vFilter = gFilterTex.Sample(g_sPoint, input.vUV);
	float4 vDistort = gDistortionTex.Sample(g_sPoint, input.vUV) * vFilter;
	output.vColor = gSceneTex.Sample(g_sPoint, input.vProjPos.xy + vDistort.xy/2.f);

	return output;
}