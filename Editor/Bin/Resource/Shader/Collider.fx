#include "Share.fx"

struct VS_COLLIDER_INPUT
{
	float3	vPos	:	POSITION;
};

struct VS_COLLIDER_OUTPUT
{
	float4	vPos	:	SV_POSITION;
};

VS_COLLIDER_OUTPUT ColliderVS(VS_COLLIDER_INPUT input)
{
	VS_COLLIDER_OUTPUT output = (VS_COLLIDER_OUTPUT)0;

	output.vPos = mul(float4(input.vPos, 1.f), g_matWVP);

	return output;
}

PS_OUTPUT_COLOR ColliderPS(VS_COLLIDER_OUTPUT input)
{
	PS_OUTPUT_COLOR output = (PS_OUTPUT_COLOR)0;

	output.vColor = g_vDif;

	return output;
}