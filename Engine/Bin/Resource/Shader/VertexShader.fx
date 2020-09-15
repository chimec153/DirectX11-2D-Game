#include "Share.fx"

struct VS_INPUT
{
	float3 vPos	:	POSITION;
	float4 vColor	:	COLOR;
};

struct VS_OUTPUT
{
	float4 vPos	:	SV_POSITION;
	float4 vColor	:	COLOR;
};

VS_OUTPUT StandardVS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.vPos = float4(input.vPos, 1.f);
	output.vColor = input.vColor;

	return output;
}

PS_OUTPUT_COLOR StandardPS(VS_OUTPUT input)
{
	PS_OUTPUT_COLOR output = (PS_OUTPUT_COLOR)0;

	output.vColor = input.vColor;

	return output;
}