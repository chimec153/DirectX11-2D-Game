#include "Share.fx"

Texture2DArray	g_DiffuseMaps	:	register(t30);
Texture2DArray	g_NormalMaps	:	register(t31);
Texture2DArray	g_SpecularMaps	:	register(t32);
Texture2DArray	g_EmissiveMaps	:	register(t33);

Texture2DArray	g_AlphaMaps	:	register(t40);

float3 ComputeBumpNormalArray(float3 vNormal, float3 vTangent, float3 vBinormal,
	float2 vUV, float vAlphas[8])
{
	float3 vBumpNormal = vNormal;

	if (g_iBump)
	{
		float4 vColor = g_NormalMaps.SampleLevel(g_sAnisotropic, float3(vUV, 0),0.f);

		for (int i = 1; i < g_iSplatCount; i++)
		{
			float4 vNormal = g_NormalMaps.SampleLevel(g_sAnisotropic, float3(vUV, i), 0.f);

			vColor = lerp(vColor, vNormal, vAlphas[i - 1]);
		}

		vColor = vColor * 2.f - 1.f;

		vBumpNormal = normalize(vColor.xyz);

		float3x3 mat =
		{
			vTangent,
			vBinormal,
			vNormal
		};

		vBumpNormal = normalize(mul(vBumpNormal, mat));
	}

	return vBumpNormal;
}

PS_OUTPUT_MULTI PS(VS_OUT input)
{
	PS_OUTPUT_MULTI output = (PS_OUTPUT_MULTI)0.f;

	float2 vUV = input.vUV * g_vSplatSize;
	float4 vColor = g_vDif;
	if (g_iDif)
	{
		vColor = gDiffuseMap.Sample(g_sPoint, vUV);
	}

	output.Albedo = vColor;
	output.GBuffer1.xyz = ComputeBumpNormal(input.vNormal, input.vTangent, input.vBinormal, vUV) * 0.5f + 0.5f;
	output.GBuffer2.x = input.vProjPos.z / input.vProjPos.w;
	output.GBuffer2.y = g_vSpe.w;
	output.GBuffer2.w = input.vProjPos.w;
	output.GBuffer3.x = ConvertColor(g_vDif);
	output.GBuffer3.y = ConvertColor(g_vAmb);
	float4 vSpc = g_vSpe;
	if (g_iSpe)
	{
		vSpc = gSpeculerMap.Sample(g_sAnisotropic, vUV);
	}
	output.GBuffer3.z = ConvertColor(vSpc);
	float4 vEmv = g_vEmi;
	if (g_iEmi)
	{
		vEmv = gEmissiveMap.Sample(g_sAnisotropic, vUV);
	}
	output.GBuffer3.w = ConvertColor(vEmv);

	return output;
}

PS_OUTPUT_MULTI ArrayPS(VS_OUT input)
{
	PS_OUTPUT_MULTI output = (PS_OUTPUT_MULTI)0.f;

	float2 vUV = input.vUV * g_vSplatSize;

	float vAlphas[8];

	for (int i = 0; i < g_iSplatCount; i++)
	{
		vAlphas[i] = g_AlphaMaps.SampleLevel(g_sAnisotropic, float3(input.vUV, i), 0.f).r;
	}

	float4 vColor = g_vDif;
	if (g_iDif)
	{
		vColor = g_DiffuseMaps.SampleLevel(g_sAnisotropic, float3(vUV,0.f),0.f);

		for (int i = 1; i < g_iSplatCount; i++)
		{
			float4 vDiffuse = g_DiffuseMaps.SampleLevel(g_sAnisotropic, float3(vUV, i), 0.f);

			vColor = lerp(vColor, vDiffuse, vAlphas[i-1]);
		}
	}

	output.Albedo = vColor;
	output.GBuffer1.xyz = ComputeBumpNormalArray(input.vNormal, input.vTangent, input.vBinormal, vUV, vAlphas) * 0.5f + 0.5f;
	output.GBuffer2.x = input.vProjPos.z / input.vProjPos.w;
	output.GBuffer2.y = g_vSpe.w;
	output.GBuffer2.w = input.vProjPos.w;
	output.GBuffer3.x = ConvertColor(g_vDif);
	output.GBuffer3.y = ConvertColor(g_vAmb);
	float4 vSpc = g_vSpe;
	if (g_iSpe)
	{
		vSpc = g_SpecularMaps.SampleLevel(g_sPoint, float3(vUV, 0.f), 0.f);

		for (int i = 1; i < g_iSplatCount; i++)
		{
			float4 vSpecular = g_DiffuseMaps.SampleLevel(g_sAnisotropic, float3(vUV, i), 0.f);

			vSpc = lerp(vSpc, vSpecular, vAlphas[i - 1]);
		}
	}
	output.GBuffer3.z = ConvertColor(vSpc);
	float4 vEmv = g_vEmi;
	if (g_iEmi)
	{
		vEmv = g_EmissiveMaps.SampleLevel(g_sPoint, float3(vUV, 0.f), 0.f);

		for (int i = 1; i < g_iSplatCount; i++)
		{
			float4 vEmissive = g_EmissiveMaps.SampleLevel(g_sAnisotropic, float3(vUV, i), 0.f);

			vEmv = lerp(vEmv, vEmissive, vAlphas[i - 1]);
		}
	}
	output.GBuffer3.w = ConvertColor(vEmv);

	return output;
}