#include "Share.fx"

struct VS_INPUT
{
	float3	vPos	:	POSITION;
};

struct VS_OUTPUT
{
	float4	vPos	:	SV_POSITION;
	float2	vUV : TEXCOORD;
};

Texture2D g_AlbedoTexture	:	register(t4);
Texture2D g_GBuffer1Texture	:	register(t5);
Texture2D g_GBuffer2Texture	:	register(t6);
Texture2D g_GBuffer3Texture	:	register(t7);
Texture2D g_ShadowTexture	:	register(t8);

Texture2D g_LightDifTexture	:	register(t11);
Texture2D g_LightSpcTexture	:	register(t12);
Texture2D g_LightEmvTexture	:	register(t13);

static const float4 gNullPos[4] =
{
	float4(-1.f, 1.f, 0.f, 1.f),
	float4(1.f, 1.f, 0.f, 1.f),
	float4(-1.f, -1.f, 0.f, 1.f),
	float4(1.f, -1.f, 0.f, 1.f)
};

static const float2 gNullUV[4] =
{
	float2(0.f, 0.f),
	float2(1.f, 0.f),
	float2(0.f, 1.f),
	float2(1.f, 1.f)
};

VS_OUTPUT VS(uint ID	:	SV_VertexID)
{
	VS_OUTPUT output = (VS_OUTPUT)0.f;

	output.vPos = gNullPos[ID];
	output.vUV = gNullUV[ID];

	return output;
}

PS_OUTPUT_LIGHT PS(VS_OUTPUT input)
{
	PS_OUTPUT_LIGHT output = (PS_OUTPUT_LIGHT)0.f;

	float4 GBuffer2 = g_GBuffer2Texture.Sample(g_sPoint, input.vUV);

	if (GBuffer2.w == 0.f)
		clip(-1);

	float4 GBuffer1 = g_GBuffer1Texture.Sample(g_sPoint, input.vUV);
	float4 GBuffer3 = g_GBuffer3Texture.Sample(g_sPoint, input.vUV);

	float3 vNormal = GBuffer1.xyz;
	float3 vViewPos = 0.f;
	float4 vProjPos = 0.f;

	vProjPos.x = input.vUV.x * 2.f - 1.f;
	vProjPos.y = input.vUV.y * -2.f + 1.f;
	vProjPos.z = GBuffer2.x;
	vProjPos.w = 1.f;

	vProjPos *= GBuffer2.w;

	vViewPos = mul(vProjPos, g_matProjInv).xyz;
	float3 vWorldPos = mul(vProjPos, g_matInvVP).xyz;
	float4 vShadowProj = mul(float4(vWorldPos, 1.f), g_matLightVP);

	float2 vShadowUV = vShadowProj.xy / vShadowProj.w;

	vShadowUV.y *= -1.f;
	vShadowUV += 1.f;
	vShadowUV *= 0.5f;
	
	float4 vMtrl = GBuffer3;
	float fSpecPower = GBuffer2.y;
	
	LightResult tResult = ComputeLight(vViewPos, vNormal, vMtrl, fSpecPower);

	float fPixelDepth = GBuffer1.w;
	//float fShadowDepth = g_ShadowTexture.Sample(g_sPoint, vShadowUV);	
	float fShadow = g_ShadowTexture.SampleCmpLevelZero(g_sShadow, vShadowUV, fPixelDepth);
	/*
	if (fPixelDepth > fShadowDepth + 0.0005f)
	{
		fShadow = 0.1f;
	}*/

	

	output.LightDif = fShadow * tResult.vDif + tResult.vAmb;
	output.LightSpc = fShadow * tResult.vSpc;
	output.LightEmv = tResult.vEmv;

	return output;
}

PS_OUTPUT_COLOR LightCombinePS(VS_OUTPUT input)
{
	PS_OUTPUT_COLOR output = (PS_OUTPUT_COLOR)0.f;

	float4 vAlbedo = g_AlbedoTexture.Sample(g_sPoint, input.vUV);
	float4 vDif = g_LightDifTexture.Sample(g_sPoint, input.vUV);
	float4 vSpc = g_LightSpcTexture.Sample(g_sPoint, input.vUV);
	float4 vEmv = g_LightEmvTexture.Sample(g_sPoint, input.vUV);

	output.vColor = vAlbedo * vDif + vSpc + vEmv;
	output.vColor.a = vAlbedo.a;

	return output;
}