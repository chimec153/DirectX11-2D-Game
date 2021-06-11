#include "Share.fx"

struct Skin
{
	float3 vPos;
	float3 vNormal;
	float3 vTangent;
	float3 vBinormal;
};

Skin Skinning(float3 vPos, float3 vNormal, float3 vTangent, float3 vBinormal, float4 vBlendWeight, float4 vBlendIndex)
{
	Skin skin = (Skin)0.f;

	if (!g_iAnimation)
	{
		skin.vPos = vPos;
		skin.vNormal = vNormal;
		skin.vTangent = vTangent;
		skin.vBinormal = vBinormal;

		return skin;
	}

	for (int i = 0; i < 4; i++)
	{
		matrix matBone = g_matBones[vBlendIndex[i]];

		skin.vPos += (mul(float4(vPos, 1.f), matBone) * vBlendWeight[i]).xyz;
		skin.vNormal += mul(vNormal, (float3x3)matBone) * vBlendWeight[i];
		skin.vTangent += mul(vTangent, (float3x3)matBone) * vBlendWeight[i];
		skin.vBinormal += mul(vBinormal, (float3x3)matBone) * vBlendWeight[i];
	}

	return skin;
}


VS_OUT VS(VS_IN input)
{
	VS_OUT output = (VS_OUT)0.f;

	float3 vPos = input.vPos - g_vPivot * g_vMeshSize;

	Skin tSkin = Skinning(vPos, input.vNormal, input.vTangent, input.vBinormal, input.vBlendWeight, input.vBlendIndex);

	output.vPos = mul(float4(tSkin.vPos, 1.f), g_matWVP);
	output.vProjPos = output.vPos;
	output.vPosV = mul(float4(tSkin.vPos, 1.f), g_matWV).xyz;
	output.vUV = input.vUV;
	output.vNormal = normalize(mul(tSkin.vNormal, (float3x3)g_matWV).xyz);
	output.vTangent = normalize(mul(tSkin.vTangent, (float3x3)g_matWV).xyx);
	output.vBinormal = normalize(mul(tSkin.vBinormal, (float3x3)g_matWV).xyz);

	return output;
}

PS_OUTPUT_MULTI PS(VS_OUT input)
{
	PS_OUTPUT_MULTI output =(PS_OUTPUT_MULTI)0.f;

	//LightResult tResult = ComputeLight(input.vPosV, input.vNormal, 
		//input.vTangent, input.vBinormal, input.vUV);

	float4 vColor = g_vDif;
	if (g_iDif)
	{
		vColor = gDiffuseMap.Sample(g_sPoint, input.vUV);
	}
/*
	float4 vAmb = tResult.vAmb * vColor;
	output.vColor = vColor*
		(tResult.vDif+ vAmb) + tResult.vSpc + tResult.vEmv;*/

	output.Albedo = vColor;
	output.GBuffer1.xyz = ComputeBumpNormal(input.vNormal, input.vTangent, input.vBinormal, input.vUV);
	output.GBuffer2.x = input.vProjPos.z / input.vProjPos.w;
	output.GBuffer2.y = g_vSpe.w;
	output.GBuffer2.w = input.vProjPos.w;
	output.GBuffer3.x = ConvertColor(g_vDif);
	output.GBuffer3.y = ConvertColor(g_vAmb);

	float4 vSpc = g_vSpe;
	if (g_iSpe)
	{
		vSpc = gSpeculerMap.Sample(g_sAnisotropic, input.vUV);
	}
	output.GBuffer3.z = ConvertColor(vSpc);
	float4 vEmv = g_vEmi;
	if (g_iEmi)
	{
		vEmv = gEmissiveMap.Sample(g_sAnisotropic, input.vUV);
	}
	output.GBuffer3.w = ConvertColor(vEmv);

	return output;
}