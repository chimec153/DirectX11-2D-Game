#include "Share.fx"

struct VS_CUBEMAP_OUTPUT
{
	float4	vPos	:	SV_POSITION;
	float3	vLocalPos	:	POSITION;
};

TextureCube	gCubeMap	:	register(t20);

VS_CUBEMAP_OUTPUT VS(VS_IN input)
{
	VS_CUBEMAP_OUTPUT output = (VS_CUBEMAP_OUTPUT)0.f;

	output.vPos = mul(float4(input.vPos, 1.f),g_matWVP).xyww;
	output.vLocalPos = input.vPos;

	return output;
}

PS_OUTPUT_COLOR PS(VS_CUBEMAP_OUTPUT input)
{
	PS_OUTPUT_COLOR output = (PS_OUTPUT_COLOR)0.f;

	output.vColor = gCubeMap.Sample(g_sAnisotropic, input.vLocalPos);
	return output;
}
