#include "Share.fx"

Texture2D g_GBuffer2Texture	:	register(t6);
Texture2D g_GBuffer4Texture	:	register(t14);
Texture2D g_GBuffer5Texture	:	register(t15);

struct VS_DECAL_IN
{
	float3 vPos	:	POSITION;
};

struct VS_DECAL_OUT
{
	float4 vPos	:	SV_POSITION;
	float4 vProjPos	:	POSITION;
	float3 vLocalPos	:	POSITION1;
};

VS_DECAL_OUT VS(VS_DECAL_IN input)
{
	VS_DECAL_OUT output = (VS_DECAL_OUT)0.f;

	output.vPos = mul(float4(input.vPos,1.f),g_matWVP);
	output.vLocalPos = input.vPos;
	output.vProjPos = output.vPos;

	return output;
}

PS_OUTPUT_DECAL PS(VS_DECAL_OUT input)
{
	PS_OUTPUT_DECAL output = (PS_OUTPUT_DECAL)0.f;

	float2 vUV = input.vProjPos.xy / input.vProjPos.w;

	float4 vLocalPos = 0.f;

	vLocalPos.xy = vUV;

	vUV.y *= -1.f;

	vUV += 1.f;
	vUV *= 0.5f;

	float4 GBuffer2 = g_GBuffer2Texture.Sample(g_sPoint, vUV);
	float3 vTangent = g_GBuffer4Texture.Sample(g_sPoint, vUV).xyz;
	float3 vBinormal = g_GBuffer5Texture.Sample(g_sPoint, vUV).xyz;

	clip(GBuffer2.z - 0.5f);

	if (GBuffer2.w == 0.f)
		clip(-1);

	vLocalPos.z = GBuffer2.x;
	vLocalPos.w = 1.f;

	vLocalPos *= GBuffer2.w;

	vLocalPos = mul(vLocalPos, g_matDecalInvWVP);

	clip(0.5f - abs(vLocalPos.xyz));

	vLocalPos.xz += 0.5;

	vLocalPos.z = 1.f - vLocalPos.z;

	output.Albedo = gDiffuseMap.Sample(g_sAnisotropic, vLocalPos.xz);

	output.Albedo.a *= g_fDecalAlpha;

	clip(output.Albedo.a -0.33f);

	float3 vNormal = cross(vTangent, vBinormal);

	output.GBuffer1.xyz = ComputeBumpNormal(vNormal, 
		vTangent, vBinormal, vLocalPos.xz);
	output.GBuffer1.w = g_fDecalAlpha;

	output.GBuffer3.x = ConvertColor(g_vDif);
	output.GBuffer3.y = ConvertColor(g_vAmb);

	float4 vSpc = g_vSpe;
	if (g_iSpe)
	{
		vSpc = gSpeculerMap.Sample(g_sAnisotropic, vLocalPos.xz);
	}
	output.GBuffer3.z = ConvertColor(vSpc);
	float4 vEmv = g_vEmi;
	if (g_iEmi)
	{
		vEmv = gEmissiveMap.Sample(g_sAnisotropic, vLocalPos.xz);
	}
	output.GBuffer3.w = ConvertColor(vEmv);
	return output;
}

