#include "Share.fx"

VS_OUT VS(VS_IN input)
{
	VS_OUT output = (VS_OUT)0.f;

	//float3 vPos = input.vPos - g_vPivot * g_vMeshSize;

	Skin tSkin = Skinning(input.vPos, input.vNormal, input.vTangent, input.vBinormal, input.vBlendWeight, input.vBlendIndex);

	output.vPos = mul(float4(tSkin.vPos, 1.f), g_matWVP);
	output.vProjPos = output.vPos;
	output.vPosV = mul(float4(tSkin.vPos, 1.f), g_matWV).xyz;
	output.vUV = input.vUV;
	output.vNormal = normalize(mul(tSkin.vNormal, (float3x3)g_matWV).xyz);
	output.vTangent = normalize(mul(tSkin.vTangent, (float3x3)g_matWV).xyz);
	output.vBinormal = normalize(mul(tSkin.vBinormal, (float3x3)g_matWV).xyz);
	output.vShadowPos = mul(float4(tSkin.vPos, 1.f), g_matLightWVP);

	return output;
}

PS_OUTPUT_MULTI PS(VS_OUT input)
{
	PS_OUTPUT_MULTI output =(PS_OUTPUT_MULTI)0.f;

	float4 vColor = g_vDif;
	if (g_iDif)
	{
		vColor = gDiffuseMap.Sample(g_sPoint, input.vUV);
	}

	output.Albedo = vColor;
	output.GBuffer1.xyz = ComputeBumpNormal(input.vNormal, input.vTangent, input.vBinormal, input.vUV);
	output.GBuffer1.w = input.vShadowPos.z / input.vShadowPos.w;
	output.GBuffer2.x = input.vProjPos.z / input.vProjPos.w;
	output.GBuffer2.y = g_vSpe.w;
	output.GBuffer2.z = g_iDecal;
	output.GBuffer2.w = input.vProjPos.w;
	output.GBuffer3.x = ConvertColor(g_vDif);
	output.GBuffer3.y = ConvertColor(g_vAmb);
	output.GBuffer4.xyz = input.vTangent;
	output.GBuffer5.xyz = input.vBinormal;

	float4 vSpc = g_vSpe;
	if (g_iSpe)
	{
		vSpc = gSpeculerMap.Sample(g_sAnisotropic, input.vUV).rrrr;
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

PS_OUTPUT_COLOR SinglePS(VS_OUT input)
{
	PS_OUTPUT_COLOR output = (PS_OUTPUT_COLOR)0.f;

	LightResult tResult = ComputeLight(input.vPosV, input.vNormal, 
		input.vTangent, input.vBinormal, input.vUV);

	float4 vColor = g_vDif;

	if (g_iDif)
	{
		vColor = gDiffuseMap.Sample(g_sPoint, input.vUV);
	}

	float4 vAmb = tResult.vAmb * vColor;
	output.vColor = vColor*
			(tResult.vDif+ vAmb) + tResult.vSpc + tResult.vEmv;

	return output;
}