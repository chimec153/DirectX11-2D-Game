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
	float	g_fTime;
	float3	g_vMeshSize;
	float	g_f2;
};

cbuffer Material	:	register(b1)
{
	float4	g_vDif;
	float4	g_vAmb;
	float4	g_vSpe;
	int		g_iDif;
	int		g_iNrm;
	int		g_iSpe;
	int		g_iEmpty;
};

cbuffer Sprite	:	register(b2)
{
	int		g_iImageType;
	int		g_iAniType;
	float2	g_vStart;
	float2	g_vEnd;
	float2	g_vSize;
};

Texture2D gDiffuseMap	:	register(t0);
Texture2D gRandomTex	:	register(t1);
Texture2DArray gArrayTex	:	register(t9);

SamplerState g_sPoint	:	register(s0);
SamplerState g_sLinear	:	register(s1);
SamplerState g_sAnisotropic	:	register(s2);

#define ATLAS	0
#define ARRAY	1

float2 ComputeUV(float2 vUV)
{
	if (g_iImageType == ARRAY)
		return vUV;

	float2 output = (float2)0.f;

	if (vUV.x > 0.f)
		output.x = g_vEnd.x / g_vSize.x;

	else
		output.x = g_vStart.x / g_vSize.x;

	if (vUV.y > 0.f)
		output.y = g_vEnd.y / g_vSize.y;

	else
		output.y = g_vStart.y / g_vSize.y;

	return output;
}


// ========================연습용 셰이더 코드======================== //

float3 RandUnitVec3(float offset)
{
	float u = (g_fTime + offset);

	float3 v = gRandomTex.SampleLevel(g_sAnisotropic, u, 0).xyz;

	return normalize(v);
}



// ========================연습용 셰이더 코드======================== //