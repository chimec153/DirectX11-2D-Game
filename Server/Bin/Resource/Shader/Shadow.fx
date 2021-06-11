#include "Share.fx"

struct VS_SHADOW_OUT
{
	float4 vPos	:	SV_POSITION;
	float4 vProjPos	:	POSITION;
};

VS_SHADOW_OUT VS(VS_IN input)
{
	VS_SHADOW_OUT output = (VS_SHADOW_OUT)0.f;

	Skin tSkin = Skinning(input.vPos, input.vNormal, input.vTangent,
		input.vBinormal, input.vBlendWeight, input.vBlendIndex);

	output.vPos = mul(float4(tSkin.vPos, 1.f), g_matLightWVP);
	output.vProjPos = output.vPos;

	return output;
}

float4 PS(VS_SHADOW_OUT input) : SV_TARGET
{
	return input.vProjPos.z / input.vProjPos.w;
}

VS_SHADOW_OUT InstVS(VS_INPUT_INST_3D input,
	uint iId	:	SV_InstanceID)
{
	VS_SHADOW_OUT output = (VS_SHADOW_OUT)0.f;

	Skin tSkin = Skinning(input.vPos, input.vNormal, input.vTangent, input.vBinormal,
		input.vBlendWeight, input.vBlendIndex, iId);

	output.vPos = mul(float4(tSkin.vPos, 1.f), input.matLightWVP);
	output.vProjPos = output.vPos;

	return output;
}