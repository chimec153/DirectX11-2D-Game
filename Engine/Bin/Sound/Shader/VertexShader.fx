#include "Share.fx"

struct VS_INPUT
{
	float3 vPos	:	POSITION;
	float2 vUV	:	TEXCOORD;
	float4 vColor	:	COLOR;
};

struct VS_OUTPUT
{
	float4 vPos	:	SV_POSITION;
	float2 vUV	:	TEXCOORD;
	float4 vColor	:	COLOR;
};

VS_OUTPUT StandardVS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float3 vPos = input.vPos - g_vPivot * g_vMeshSize;

	output.vPos = mul(float4(vPos, 1.f), g_matWVP);
	output.vColor = input.vColor;
	output.vUV = input.vUV;

	return output;
}

PS_OUTPUT_COLOR StandardPS(VS_OUTPUT input)
{
	PS_OUTPUT_COLOR output = (PS_OUTPUT_COLOR)0;

	if (!g_iDif)
		output.vColor = input.vColor * g_vDif;

	else
		output.vColor = gDiffuseMap.Sample(g_sAnisotropic, input.vUV) * g_vDif;

	if (output.vColor.a == 0.f)
		clip(-1);

	return output;
}

VS_OUTPUT StandardSpriteVS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float3 vPos = input.vPos - g_vPivot * g_vMeshSize;

	output.vPos = mul(float4(vPos, 1.f), g_matWVP);
	output.vColor = input.vColor;

	output.vUV = ComputeUV(input.vUV);

	return output;
}

PS_OUTPUT_COLOR StandardSpritePS(VS_OUTPUT input)
{
	PS_OUTPUT_COLOR output = (PS_OUTPUT_COLOR)0;

	output.vColor = gDiffuseMap.Sample(g_sPoint, input.vUV) * g_vDif;

	return output;
}

PS_OUTPUT_COLOR ShadowPS(VS_OUTPUT input)
{
	PS_OUTPUT_COLOR output = (PS_OUTPUT_COLOR)0;

	output.vColor = gDiffuseMap.Sample(g_sPoint, input.vUV);

	if (output.vColor.a != 0.f)
		output.vColor = g_vDif;

	return output;
}

struct VertexIn
{
	float3 vPos	:	POSITION;
	float2 vUV	:	TEXCOORD;
	row_major float4x4 matWVP	:	WORLD;
	float4 vColor	:	COLOR;
	uint InstanceId	:	SV_InstanceID;
};

struct VertexOut
{
	float4 vPos	:	SV_POSITION;
	float2 vUV	:	TEXCOORD;
	float4 vColor	:	COLOR;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	vout.vPos = mul(float4(vin.vPos, 1.f), g_matWVP);
	vout.vUV = ComputeUV(vin.vUV);
	vout.vColor = vin.vColor;

	return vout;
}

PS_OUTPUT_COLOR PS(VertexOut input)
{
	PS_OUTPUT_COLOR output = (PS_OUTPUT_COLOR)0;

	output.vColor = gDiffuseMap.Sample(g_sAnisotropic, input.vUV) * g_vDif;

	return output;
}
