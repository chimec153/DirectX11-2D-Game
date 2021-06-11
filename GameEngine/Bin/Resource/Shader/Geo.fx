#include "Share.fx"

struct VertexIn
{
	float3 vPos	:	POSITION;
	float2	vSize	:	SIZE;
	float4 vColor	:	COLOR;
};

struct VertexOut
{
	float3 vPos	:	POSITION;
	float2 vSize	:	SIZE;
	float4 vColor	:	COLOR;
};

struct GeoOut
{
	float4	vPos	:	SV_POSITION;
	float4	vColor	:	COLOR;
};

VertexOut VS(VertexIn input)
{
	VertexOut output = (VertexOut)0.f;

	output.vPos = input.vPos;
	output.vColor = input.vColor;
	output.vSize = input.vSize;

	return output;
}

[maxvertexcount(4)]
void GS(line VertexOut l[2],
	inout LineStream<GeoOut> lineStream)
{
	VertexOut vtOut[4];

	vtOut[0].vPos = l[0].vPos;
	vtOut[0].vColor = l[0].vColor;

	vtOut[1].vPos = l[1].vPos;
	vtOut[1].vColor = l[1].vColor;

	vtOut[2].vPos = l[0].vPos + float3(l[0].vSize, 0.f);
	vtOut[2].vColor = l[0].vColor;

	vtOut[3].vPos = l[1].vPos + float3(l[1].vSize, 0.f);
	vtOut[3].vColor = l[1].vColor;

	GeoOut gout;

	[unroll]
	for(int i=0;i<4;++i)
	{
		gout.vPos = mul(float4(vtOut[i].vPos, 1.f), g_matWVP);
		gout.vColor = vtOut[i].vColor;

		lineStream.Append(gout);
	}
}

PS_OUTPUT_COLOR PS(GeoOut input) : SV_TARGET
{
	PS_OUTPUT_COLOR output = (PS_OUTPUT_COLOR)0.f;

	output.vColor = input.vColor;

	return output;
}