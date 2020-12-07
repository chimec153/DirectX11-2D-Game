struct GS_INPUT
{
	float4 vPos	:	SV_POSITION;
	float3 vNorm : TEXCOORD0;
	float2 vTex	:	TEXCOORD1;
};

[maxvertexcount(12)]
void GS(triangle GS_INPUT input[3], inout TriangleStream<GS_INPUT> TriStream)
{
	GS_INPUT output;

	float3 faceEdgeA = input[1].Pos - input[0].Pos;
	float3 faceEdgeB = input[2].Pos - input[0].Pos;
	float3 faceNormal = normalize(cross(faceEdgeA, faceEdgeB));
	float3 ExplodeAmt = faceNormal * Explode;

	float3 centerPos = (input[0].Pos.xyz + input[1].Pos.xyz + input[2].Pos.xyz) / 3.f;

	float2 centerTex = (input[0].Tex + input[1].Tex + input[2].Tex) / 3.f;

	centerPos += faceNormal * Explode;

	for (int i = 0; i < 3; ++i)
	{
		output.Pos = input[i].Pos + float4(ExplodeAmt, 0);
		output.Pos = mul(output.Pos, View);
		output.Pos = mul(output.Pos, Projection);
		output.Norm = input[i].Norm;
		output.Tex = input[i].Tex;

		TriStream.Append(output);

		int iNext = (i + 1) % 3;
		output.Pos = input[iNext].Pos + float4(ExplodeAmt, 0);
		output.Pos = mul(output.Pos, View);
		output.Pos = mul(output.Pos, Projection);
		output.Norm = input[iNext].Norm;
		output.Tex = input[iNext].Tex;
		TriStream.Append(output);

		output.Pos = float4(centerPos, 1) + float4(ExplodeAmt, 0);
		output.Pos = mul(output.Pos, View);
		output.Pos = mul(output.Pos, Projection);
		output.Norm = faceNormal;
		output.Tex = centerTex;

		Tristream.Append(output);

		TriStream.RestartStrip();
	}

	for (int i = 2; i >= 0; i--)
	{
		output.Pos = input[i].Pos + float4(ExplodeAmt, 0);
		output.Pos = mul(output.Pos, View);
		output.Pos = mul(output.Pos, Projection);
		output.Norm = -input[i].Norm;
		output.Tex = input[i].Tex;
		TriStream.Append(output);
	}
	TriStream.RestartStrip();
}