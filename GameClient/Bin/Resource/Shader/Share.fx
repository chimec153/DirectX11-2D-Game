struct PS_OUTPUT_COLOR
{
	float4 vColor	:	SV_TARGET;
};

cbuffer Transform	:	register(b0)
{
	matrix	g_matWorld;
	matrix	g_matView;
	matrix	g_matProj;
	matrix	g_matWV;
	matrix	g_matWVP;
	float3	g_vPivot;
	float3	g_vMeshSize;
};

cbuffer Material	:	register(b1)
{
	float4	g_vDif;
	float4	g_vAmb;
	float4	g_vSpe;
};