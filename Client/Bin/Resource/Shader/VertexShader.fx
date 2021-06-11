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

	/*else if (g_iGray)
	{
		float3 gray = dot(float3(output.vColor.r, output.vColor.g, output.vColor.b),
			float3(0.3333f, 0.3333f, 0.3333f));

		float4 vGrayColor = float4(gray, output.vColor.a);

		float4 vColor = lerp(output.vColor, vGrayColor, g_fGray);

		output.vColor = vColor;
	}*/

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

	output.vColor = (gDiffuseMap.Sample(g_sPoint, input.vUV) + g_vAmb) * g_vDif;

	if (output.vColor.a == 0.f)
	{
		clip(-1);
	}

	/*else if (g_iGray)
	{
		float3 gray = dot(float3(output.vColor.r, output.vColor.g, output.vColor.b),
			float3(0.3333f, 0.3333f, 0.3333f));

		float4 vGrayColor = float4(gray, output.vColor.a);

		float4 vColor = lerp(output.vColor, vGrayColor, g_fGray);

		output.vColor = vColor;
	}*/

	return output;
}

PS_OUTPUT_COLOR ShadowPS(VS_OUTPUT input)
{
	PS_OUTPUT_COLOR output = (PS_OUTPUT_COLOR)0;

	output.vColor = gDiffuseMap.Sample(g_sPoint, input.vUV);

	if (output.vColor.a != 0.f)
		output.vColor = g_vDif;

	float3 gray = dot(float3(output.vColor.r, output.vColor.g, output.vColor.b),
		float3(0.3333f, 0.3333f, 0.3333f));

	float4 vGrayColor = float4(gray, output.vColor.a);

	float4 vColor = lerp(output.vColor, vGrayColor, g_fGray);

	output.vColor = vColor;

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

	float3 gray = dot(float3(output.vColor.r, output.vColor.g, output.vColor.b),
		float3(0.3333f, 0.3333f, 0.3333f));

	float4 vGrayColor = float4(gray, output.vColor.a);

	float4 vColor = lerp(output.vColor, vGrayColor, g_fGray);

	output.vColor = vColor;

	return output;
}

PS_OUTPUT_COLOR GrayPS(VS_OUTPUT input)
{
	PS_OUTPUT_COLOR output = (PS_OUTPUT_COLOR)0;

	output.vColor = gDiffuseMap.Sample(g_sPoint, input.vUV) * g_vDif;

	if (output.vColor.a == 0.f)
		clip(-1);

		float3 gray = dot(float3(output.vColor.r, output.vColor.g, output.vColor.b),
			float3(0.3333f, 0.3333f, 0.3333f));

		float4 vGrayColor = float4(gray, output.vColor.a);

		float4 vColor = lerp(output.vColor, vGrayColor, 1.f);

		output.vColor = vColor;

	return output;
}

struct VS_F_IN
{
	float3	vPos	:	POSITION;
	float2	vUV	:	TEXCOORD;
};

struct VS_F_OUT
{
	float4	vPos	:	SV_POSITION;
	float2	vUV	:	TEXCOORD;
};

static const float4 gFull[4] =
{
	float4(-1.f, 1.f,0.f, 1.f),
	float4(1.f, 1.f,0.f, 1.f),
	float4(-1.f, -1.f,0.f, 1.f),
	float4(1.f, -1.f,0.f, 1.f)
};

static const float2 gFullUV[4] =
{
	float2(0.f, 0.f),
	float2(1.f, 0.f),
	float2(0.f, 1.f),
	float2(1.f, 1.f)
};

VS_F_OUT FullVS(uint iID	:	SV_VertexID)
{
	VS_F_OUT output = (VS_F_OUT)0.f;

	output.vPos = gFull[iID];
	output.vUV = gFullUV[iID];

	return output;
}

PS_OUTPUT_COLOR FullPS(VS_F_OUT input)
{
	PS_OUTPUT_COLOR output = (PS_OUTPUT_COLOR)0.f;

	output.vColor = gSceneTex.Sample(g_sPoint, input.vUV);

	return output;
}

VS_F_OUT DebugVS(VS_F_IN input)
{
	VS_F_OUT output = (VS_F_OUT)0.f;

	output.vPos = mul(float4(input.vPos, 1.f), g_matWVP);
	output.vUV = input.vUV;

	return output;
}

PS_OUTPUT_COLOR DebugPS(VS_F_OUT input)
{
	PS_OUTPUT_COLOR output = (PS_OUTPUT_COLOR)0.f;
	output.vColor = gDiffuseMap.Sample(g_sPoint, input.vUV);

	return output;
}