#include "Share.fx"

struct VS_INPUT_INST
{
	float3	vPos	:	POSITION;
	float2	vUV	:	TEXCOORD;
	matrix	matWVP	:	WORLD;
	float3	vPivot	:	PIVOT;
	float3	vSize	:	SIZE;
	float2	vTexSize	:	TEXSIZE;
	float2	vStart	:	START;
	float2	vEnd	:	END;
	float4	vColor	:	COLOR;
};

struct VS_OUTPUT_INST
{
	float4	vPos	:	SV_POSITION;
	float2	vUV	:	TEXCOORD;
	float4	vColor	:	COLOR;
};

struct VS_INPUT_INST_COLLIDER
{
	float3	vPos	:	POSITION;
	matrix	matWVP	:	WORLD;
	float3	vPivot	:	PIVOT;
	float3	vSize	:	SIZE;
	float4	vColor	:	COLOR;
};

struct VS_OUTPUT_INST_COLLIDER
{
	float4	vPos	:	SV_POSITION;
	float4	vColor	:	COLOR;
};

VS_OUTPUT_INST VS_INST(VS_INPUT_INST input)
{
	VS_OUTPUT_INST output = (VS_OUTPUT_INST)0.f;

	float3 vPos = input.vPos - input.vPivot * input.vSize;

	output.vPos = mul(float4(vPos, 1.f), input.matWVP);
	output.vColor = input.vColor;

	if (input.vUV.x > 0.f)
		output.vUV.x = input.vEnd.x / input.vTexSize.x;

	else
		output.vUV.x = input.vStart.x / input.vTexSize.x;

	if (input.vUV.y > 0.f)
		output.vUV.y = input.vEnd.y / input.vTexSize.y;

	else
		output.vUV.y = input.vStart.y / input.vTexSize.y;

	return output;
}

PS_OUTPUT_COLOR PS_INST(VS_OUTPUT_INST input)
{
	PS_OUTPUT_COLOR output = (PS_OUTPUT_COLOR)0.f;

	output.vColor = gDiffuseMap.Sample(g_sPoint, input.vUV) * g_vDif * input.vColor;

	if (output.vColor.a == 0.f)
		clip(-1);

	float3 gray = dot(float3(output.vColor.r, output.vColor.g, output.vColor.b),
		float3(0.3333f, 0.3333f, 0.3333f));

	float4 vGrayColor = float4(gray, output.vColor.a);

	float4 vColor = lerp(output.vColor, vGrayColor, g_fGray);

	output.vColor = vColor;

	return output;
}

VS_OUTPUT_INST_COLLIDER VS_INST_COLLIDER(VS_INPUT_INST_COLLIDER input)
{
	VS_OUTPUT_INST_COLLIDER output = (VS_OUTPUT_INST_COLLIDER)0.f;

	float3 vPos = input.vPos - input.vPivot * input.vSize;

	output.vPos = mul(float4(vPos, 1.f), input.matWVP);
	output.vColor = input.vColor;

	return output;
}

PS_OUTPUT_COLOR PS_INST_COLLIDER(VS_OUTPUT_INST_COLLIDER input)
{
	PS_OUTPUT_COLOR output = (PS_OUTPUT_COLOR)0.f;

	output.vColor = input.vColor;

	return output;
}
