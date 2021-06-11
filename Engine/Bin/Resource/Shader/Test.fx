#include "Share.fx"

cbuffer HemiConstants	:	register(b13)
{
	float3 AmbientDown	:	packoffset(c0);
	float3 AmbiendRange	:	packoffset(c1);
	float3	DirToLight	:	packoffset(c2);
	float3	DirLightColor	:	packoffset(c3);
	//float3 PointLightPos	:	packoffset(c4);
	//float3 PointLightRangeRcp	:	packoffset(c4.w);
}

cbuffer Camera	:	register(b12)
{
	float3 EyePosition	:	packoffset(c0);
}

float3 CalcAmbient(float3 normal, float3 color)
{
	// [-1, 1]에서 [0,1]으로 변환
	float up = normal.y * 0.5 + 0.5;

	// 앰비언트 값 계산
	float3 Ambient = AmbientDown + up * AmbiendRange;

	// 색상 값에 앰비언트 값 적용
	return Ambient* color;
}

struct VS_IN_TEST
{
	float3	Pos	:	POSITION;
	float3	Normal	:	NORMAL;
	float2	UV	:	TEXCOORD;
};

struct VS_OUT_TEST
{
	float4	Pos	:	SV_POSITION;
	float3	PosW	:	POSITION;
	float2	UV	:	TEXCOORD;
	float3	Normal	:	NORMAL;
};

struct Material
{
	float3 normal;
	float4 diffuseColor;
	float specExp;
	float specIntensity;
};

VS_OUT_TEST VS(VS_IN_TEST input)
{
	VS_OUT_TEST output = (VS_OUT_TEST)0.f;

	// 위치를 오브젝트 공간에서 프로젝션 공간으로 변환
	output.Pos = mul(float4(input.Pos, 1.f), g_matWVP);

	// 전달할 텍스쳐 좌표를 복사
	output.UV = input.UV;

	output.PosW = mul(input.Pos, (float3x3)g_matWorld);
	output.Normal = mul(input.Normal, (float3x3)g_matWorld);

	return output;
}

float4 PS(VS_OUT_TEST input)	:	SV_TARGET
{
	PS_OUTPUT_COLOR output = (PS_OUTPUT_COLOR)0.f;

	float4 color = g_vDif;// gDiffuseMap.Sample(g_sPoint, input.UV);
	
	float3 normal = normalize(input.Normal);

	color = float4(color.rgb * color.rgb, color.a);

	float NDotL = dot(DirToLight, normal);
	float3 finalColor = DirLightColor.rgb * saturate(NDotL);
	float3 ToEye = EyePosition.xyz - input.PosW;
	ToEye = normalize(ToEye);
	float3 HalfWay = normalize(ToEye + DirToLight);
	float NDotH = saturate(dot(HalfWay, normal));
	finalColor += DirLightColor.rgb * pow(NDotH, g_vSpe.w) *
		2.f;


	float3 specColor = finalColor * color.rgb;
	float3 ambColor = CalcAmbient(normal, color.xyz);
	return float4(ambColor + specColor, color.w);
}

//
//float3 CalcPoint(float3 position, float3 normal)
//{
//	float3 ToLight = PointLightPos.xyz - position;
//	float3 ToEye = EyePosition.xyz - position;
//	float DistToLight = length(ToLight);
//
//	ToLight /= DistToLight;
//	float NDotL = saturate(dot(ToLight, normal));
//	float3 finalColor = PointColor.rgb * NDotL;
//
//	ToEye = normalize(ToEye);
//	float3 HalfWay = normalize(ToEye + ToLight);
//	float NDotH = saturate(dot(HalfWay, normal));
//	finalColor += PointColor.rgb * pow(NDotH, material.g_vSpe.w) *
//		g_iSpecPower;
//
//	float DistToLightNorm = 1.0 - saturate(DistToLight *
//		PointLightRangeRcp);
//	float Attn = DistToLightNorm * DistToLightNorm;
//	finalColor *= g_vDif * Attn;
//
//	return finalColor;
//}