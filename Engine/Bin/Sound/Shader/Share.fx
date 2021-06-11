struct PS_OUTPUT_COLOR
{
	float4 vColor	:	SV_TARGET;
};

struct PS_OUTPUT_MULTI
{
	float4 Albedo	:	SV_TARGET;
	float4 GBuffer1	:	SV_TARGET1;
	float4 GBuffer2	:	SV_TARGET2;
	float4 GBuffer3	:	SV_TARGET3;
};

struct PS_OUTPUT_LIGHT
{
	float4 LightDif	:	SV_TARGET;
	float4 LightSpc	:	SV_TARGET1;
	float4 LightEmv	:	SV_TARGET2;
};

struct VS_IN
{
	float3 vPos	:	POSITION;
	float2	vUV	:	TEXCOORD;
	float3	vNormal	:	NORMAL;
	float3	vTangent	:	TANGENT;
	float3	vBinormal	:	BINORMAL;
	float4	vBlendWeight	:	BLENDWEIGHT;
	float4	vBlendIndex	:	BLENDINDEX;
};

struct VS_OUT
{
	float4	vPos	:	SV_POSITION;
	float4	vProjPos	:	POSITION;
	float2	vUV	:	TEXCOORD;
	float3	vNormal	:	NORMAL;
	float3	vTangent	:	TANGENT;
	float3	vBinormal	:	BINORMAL;
	float4	vBlendWeight	:	BLENDWEIGHT;
	float4	vBlendIndex	:	BLENDINDEX;
	float3	vPosV	:	POSITION1;	//	¿œ∏”∏Æ
};

cbuffer Transform	:	register(b0)
{
	matrix	g_matWorld;
	matrix	g_matView;
	matrix	g_matProj;
	matrix	g_matWV;
	matrix	g_matWVP;
	matrix	g_matProjInv;
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
	float4	g_vEmi;
	int		g_iDif;
	int		g_iNrm;
	int		g_iSpe;
	int		g_iEmi;
	int		g_iBump;
	int		g_iAnimation;
	float	g_iSpecPower;
};

cbuffer Sprite	:	register(b2)
{
	int		g_iImageType;
	int		g_iAniType;
	float2	g_vStart;
	float2	g_vEnd;
	float2	g_vSize;
};

cbuffer Global	:	register(b3)
{
	float	g_fDeltaTime;
	float	g_fAccTime;
	float2	g_vNoiseSize;
	float2	g_vResolution;
};

cbuffer Paper	:	register(b4)
{
	float	g_fFilter;
	float	g_fInFilter;
	float	g_fMidFilter;
	float	g_fOutFilter;
	float4	g_vInColor;
	float4	g_vOutColor;
}

cbuffer Distortion	:	register(b5)
{
	float	g_fSpeed;
	float	g_fAmp;
}

cbuffer Light	:	register(b7)
{
	float4	g_vLightDif;
	float4	g_vLightAmb;
	float4	g_vLightSpc;
	float4	g_vLightEmv;
	float3	g_vLightDir;
	int		g_iLightType;
	float3	g_vLightPos;
	float	g_fLightRange;
	float	g_fLightAngleIn;
	float3	g_vLightAttn;
	float	g_fLightAngleOut;
	float3	E7;
}

cbuffer Terrain	:	register(b8)
{
	float2	g_vSplatSize;
	int		g_iSplatCount;
}

cbuffer Animation	:	register(b9)
{
	int		g_iFrame;
	float	g_fRatio;
	int		g_iAniFrameMax;
	int		g_iChange;
	float	g_fChangeRatio;
	int		g_iChangeFrameMax;
}


cbuffer Fade	:	register(b10)
{
	float4	g_vFade;
	float	g_fGray;
	float3	g_vE;
};
Texture2D gDiffuseMap	:		register(t0);
Texture2D gNormalMap	:		register(t1);
Texture2D gSpeculerMap	:		register(t2);
Texture2D gEmissiveMap	:		register(t3);

Texture2DArray gArrayTex	:	register(t9);
Texture2D gRandomTex	:		register(t10);

Texture2D g_PaperTex	:		register(t90);
Texture2D gSceneTex		:		register(t98);
Texture2D gDistortionTex	:	register(t99);
Texture2D gFilterTex	:		register(t100);

StructuredBuffer<matrix>	g_matBones	:	register(t104);

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

	output.x = ((g_vEnd.x - g_vStart.x) * vUV.x + g_vStart.x) / g_vSize.x;
	output.y = ((g_vEnd.y - g_vStart.y) * vUV.y + g_vStart.y) / g_vSize.y;

	return output;
}

static float gaussian5x5[25] =	
{
	0.003765, 0.015019, 0.023792, 0.015019, 0.003765,
	0.015019, 0.059912, 0.094907, 0.059912, 0.015019,
	0.023792, 0.094907, 0.150342, 0.094907, 0.023792,
	0.015019, 0.059912, 0.094907, 0.059912, 0.015019,
	0.003765, 0.015019, 0.023792, 0.015019, 0.003765,
};

float gaussian5x5Sample(in int2 vUV, in texture2D RandomTex)
{
	float4 vRand = (float4)0.f;

	vRand += RandomTex[vUV];

	//for (int i = 0; i < 5; ++i)
	//{
	//	for (int j = 0; j < 5; ++j)
	//	{
	//		vRand += RandomTex[vUV + int2(i-2, j-2)] * gaussian5x5[i * 5 + j];
	//	}
	//}

	return vRand.x;
}

float Rand(in float fRand)
{
	float2 vUV = float2(fRand + g_fAccTime, g_fAccTime);

	vUV.y += sin(vUV.x * 6.283184f);

	if (vUV.x < 0)
	{
		vUV.x= -(int)vUV.x + 1 + vUV.x;
	}

	else
	{
		vUV.x = frac(vUV.x);
	}

	if (vUV.y < 0)
	{
		vUV.y = -(int)vUV.y + 1 + vUV.y;
	}

	else
	{
		vUV.y = frac(vUV.y);
	}
	
	vUV = vUV * g_vNoiseSize;

	return gaussian5x5Sample(vUV, gRandomTex);
}

float3 RandUnitVec3(float offset)
{
	float u = (g_fDeltaTime + offset);

	float3 v = gRandomTex.SampleLevel(g_sLinear, u, 0).xyz;

	return normalize(v);
}

float4 PaperBurn(in float4 vColor, in float2 vUV)
{
	float2 _vUV = float2(vUV.x / 1280.f, vUV.y / 720.f);

	float4 vFilterColor = g_PaperTex.Sample(g_sPoint, _vUV);

	if (vFilterColor.x < g_fFilter - g_fOutFilter)
	{
		clip(-1);
		return float4(0.f, 0.f, 0.f, 0.f);
	}
	else if (vFilterColor.x < g_fFilter - g_fMidFilter)
	{
		return g_vOutColor;
	}
	else if (vFilterColor.x < g_fFilter - g_fInFilter)
	{
		return g_vInColor;
	}

	return vColor;
}

struct LightResult
{
	float4 vDif;
	float4 vAmb;
	float4 vSpc;
	float4 vEmv;
};

float3 ComputeBumpNormal(float3 vNormal, float3 vTangent, float3 vBinormal, float2 vUV)
{
	float3 vBumpNormal = vNormal;

	if (g_iBump)
	{
		float4 vColor = gNormalMap.Sample(g_sAnisotropic, vUV);

		vColor = vColor * 2.f - 1.f;

		vBumpNormal = normalize(vColor.xyz);

		float3x3 mat =
		{
			vTangent,
			vBinormal,
			vNormal
		};

		vBumpNormal = normalize(mul(vBumpNormal, mat));
	}

	return vBumpNormal;
}

#define DIR_LIGHT	0
#define POINT_LIGHT	1
#define	SPOT_LIGHT	2

LightResult ComputeLight(float3 vPos, float3 vNormal, 
	float3 vTangent, float3 vBinormal, float2 vUV)
{
	LightResult tResult = (LightResult)0.f;

	float3 vLightDir = (float3)0.f;

	float fAttn = 1.f;

	if (g_iLightType == DIR_LIGHT)
	{
		vLightDir = -g_vLightDir;
	}

	else if (g_iLightType == POINT_LIGHT)
	{
		vLightDir = g_vLightPos - vPos;

		float fDist = length(vLightDir);

		vLightDir /= fDist;

		if (fDist > g_fLightRange)
		{
			fAttn = 0.f;
		}

		else
		{
			fAttn = 1.f / dot(float3(1.f, fDist, fDist * fDist), float3(1.f, 2.f / g_fLightRange, 1.f / g_fLightRange / g_fLightRange));
		}

	}

	else if (g_iLightType == SPOT_LIGHT)
	{
	}

	float3 toEye = -vPos;

	float fLength = length(toEye);

	toEye /= fLength;

	float3 vBumpNormal = ComputeBumpNormal(vNormal, vTangent, vBinormal,vUV);

	float3 vReflect = reflect(-vLightDir, vBumpNormal);

	float fDiffuse = max(0.f, dot(vBumpNormal, vReflect));

	tResult.vDif = g_vDif * g_vLightDif * fDiffuse * fAttn;

	float up = vNormal.y * 0.5f + 0.5f;

	tResult.vAmb = (g_vAmb + g_vLightAmb * up) * fAttn;

	tResult.vSpc = g_vSpe;

	if (g_iSpe)
	{
		tResult.vSpc = gSpeculerMap.Sample(g_sAnisotropic, vUV);
	}

	tResult.vSpc.w = 1.f;

	tResult.vSpc = tResult.vSpc* g_vLightSpc* pow(max(0.f, dot(toEye, vReflect)), g_vSpe.w)* fAttn;

	tResult.vEmv = g_vEmi;
	if (g_iEmi)
	{
		tResult.vEmv = gEmissiveMap.Sample(g_sAnisotropic, vUV);
	}

	tResult.vEmv *= g_vLightEmv;

	return tResult;
}

float ConvertColor(float4 vColor)
{
	uint4 _vColor = 0.f;

	_vColor.x = uint(vColor.x * 255.f);
	_vColor.y = uint(vColor.y * 255.f);
	_vColor.z = uint(vColor.z * 255.f);
	_vColor.w = uint(vColor.w * 255.f);

	uint iColor = 0;

	iColor = _vColor.x;
	iColor = (iColor << 8) | _vColor.y;
	iColor = (iColor << 8) | _vColor.z;
	iColor = (iColor << 8) | _vColor.w;

	return asfloat(iColor);
}

float4 ConvertColor(float fColor)
{
	uint iColor = asuint(fColor);

	float4 vColor = (float4)0.f;

	vColor.x = ((iColor >> 24) & 0xff) / 255.f;
	vColor.y = ((iColor >> 16) & 0xff) / 255.f;
	vColor.z = ((iColor >> 8) & 0xff) / 255.f;
	vColor.w = (iColor & 0xff) / 255.f;

	return vColor;
}

LightResult ComputeLight(float3 vPos, float3 vNormal, float4 vMtrl,float fSpecPower)
{
	LightResult tResult = (LightResult)0.f;

	float3 vLightDir = (float3)0.f;

	float fAttn = 1.f;

	float4 vDiffuseColor = ConvertColor(vMtrl.r);
	float4 vAmbientColor = ConvertColor(vMtrl.g);
	float4 vSpecularColor = ConvertColor(vMtrl.b);
	float4 vEmissiveColor = ConvertColor(vMtrl.a);

	if (g_iLightType == DIR_LIGHT)
	{
		vLightDir = -g_vLightDir;
	}

	else if (g_iLightType == POINT_LIGHT)
	{
		vLightDir = g_vLightPos - vPos;

		float fDist = length(vLightDir);

		vLightDir /= fDist;

		if (fDist > g_fLightRange)
		{
			fAttn = 0.f;
		}

		else
		{
			fAttn = 1.f / dot(float3(1.f, fDist, fDist * fDist), 
				float3(1.f, 2.f / g_fLightRange, 1.f / g_fLightRange / g_fLightRange));
		}

	}

	else if (g_iLightType == SPOT_LIGHT)
	{
	}

	float3 toEye = normalize(-vPos);

	float3 vBumpNormal = vNormal;

	float3 vReflect = -vLightDir - dot(vBumpNormal, -vLightDir) * vBumpNormal * 2.f;// reflect(-vLightDir, vBumpNormal);

	float fDiffuse = max(0.f, dot(vBumpNormal, vReflect));

	tResult.vDif = vDiffuseColor * g_vLightDif * fDiffuse * fAttn;

	float up = vNormal.y * 0.5f + 0.5f;

	tResult.vAmb = (vAmbientColor + g_vLightAmb * up) * fAttn;

	tResult.vSpc = vSpecularColor* g_vLightSpc* pow(max(0.f, dot(toEye, vReflect)), fSpecPower)* fAttn;

	tResult.vEmv = vEmissiveColor * g_vLightEmv;

	return tResult;
}