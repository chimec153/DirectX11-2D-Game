#include "Share.fx"

struct tParticle
{
	float3	vWorldPos;
	float3	vMoveDir;
	float	fSpeed;
	float	fAddSpeed;
	float	fLifeTime;
	float	fMaxLifeTime;
	int		iLive;
	float	fAngle;
	int		iFrame;
};

struct tParticleShare
{
	uint	iMaxCount;
	uint	iCount;

float3	vStartScale;
float3	vEndScale;

float4 vStartColor;
float4 vEndColor;
};

cbuffer Particle	:	register(b11)
{
	float3	g_vPos;
	uint	g_iCount;
	float3	g_vStartScale;
	uint	g_iMaxCount;
	float3	g_vEndScale;
	float	g_fMinLifeTime;
	float3	g_vRange;
	float	g_fMaxLifeTime;
	float4	g_vStartColor;
	float4	g_vEndColor;
	float	g_fMinSpeed;
	float	g_fMaxSpeed;
	int		g_iMove;
	int		g_i2D;
	float3	g_vMoveDir;
	float	g_fTurnAngle;
	float3	g_vMoveAngle;
	float	g_fFrameTime;
	int		g_iFrameMax;
	float3	g_vAcc;
}

struct VS_INPUT_PARTICLE
{
	float3 vPos	:	POSITION;
	float2 vUV	:	TEXCOORD;
	float4 vColor	:	COLOR;
	uint iInstID	:	SV_InstanceID;
};

struct VS_OUTPUT_PARTICLE
{
	uint iInstID	:	SV_InstanceID;
};

struct GS_OUTPUT_PARTICLE
{
	float4 vPos	:	SV_POSITION;
	float2 vUV	:	TEXCOORD;
	float4 vColor	:	COLOR;
};

RWStructuredBuffer<tParticle>		gParticle	:	register(u0);
RWStructuredBuffer<tParticleShare>	gParticleShare	:	register(u1);

[numthreads(64, 1, 1)]
void ParticleMain(uint3 vThreadID	:	SV_DispatchThreadID)
{
	gParticleShare[0].iCount = g_iCount;
	gParticleShare[0].vStartScale = g_vStartScale;
	gParticleShare[0].vEndScale = g_vEndScale;
	gParticleShare[0].vStartColor = g_vStartColor;
	gParticleShare[0].vEndColor = g_vEndColor;

	if (vThreadID.x >= g_iMaxCount)
		return;

	if (!gParticle[vThreadID.x].iLive)
	{
		gParticle[vThreadID.x].vWorldPos = 0.f;
		gParticle[vThreadID.x].fAngle = 0.f;
		gParticle[vThreadID.x].fSpeed = 0.f;
		gParticle[vThreadID.x].fAddSpeed = 0.f;

		int iSpawnCount = gParticleShare[0].iCount;
		int iExchange = 0;

		if (gParticleShare[0].iCount >= 1)
		{
			int iInputValue = iSpawnCount - 1;

			InterlockedExchange(gParticleShare[0].iCount, iInputValue, iExchange);

			if (iExchange == iSpawnCount)
			{
				gParticle[vThreadID.x].iLive = true;

				float fKey = vThreadID.x / (float)g_iMaxCount;

				float3 vRand = float3(Rand(fKey), Rand(fKey * 2.f), Rand(fKey * 3.f));

				float3 vPosRange = (vRand - 0.5f) * g_vRange;

				gParticle[vThreadID.x].vWorldPos = g_vPos + vPosRange;

				gParticle[vThreadID.x].fMaxLifeTime = vRand.x * (g_fMaxLifeTime - g_fMinLifeTime) + g_fMinLifeTime;

				gParticle[vThreadID.x].fLifeTime = 0.f;

				gParticle[vThreadID.x].fAddSpeed = g_vAcc.x;

				if (g_iFrameMax != 0 && g_fFrameTime == 0.f)
				{
					gParticle[vThreadID.x].iFrame = vThreadID.x % g_iFrameMax;
				}

				if (g_iMove)
				{
					gParticle[vThreadID.x].fSpeed = vRand.z * (g_fMaxSpeed - g_fMinSpeed) + g_fMinSpeed;

					if (g_vMoveDir.x == 0.f &&
						g_vMoveDir.y == 0.f &&
						g_vMoveDir.z == 0.f)
					{
						gParticle[vThreadID.x].vMoveDir = normalize(vPosRange);
					}
					else
					{
						gParticle[vThreadID.x].vMoveDir = g_vMoveDir;
					}
				}
			}
		}
	}

	else
	{
		gParticle[vThreadID.x].fLifeTime += g_fDeltaTime;

		if (gParticle[vThreadID.x].fLifeTime >= gParticle[vThreadID.x].fMaxLifeTime)
		{
			gParticle[vThreadID.x].iLive = false;
			return;
		}

		if (g_fFrameTime != 0.f)
		{
			gParticle[vThreadID.x].iFrame = (int)(gParticle[vThreadID.x].fLifeTime / g_fFrameTime) % g_iFrameMax;
		}

		gParticle[vThreadID.x].fSpeed += gParticle[vThreadID.x].fAddSpeed * g_fDeltaTime;
		gParticle[vThreadID.x].vWorldPos += gParticle[vThreadID.x].vMoveDir * gParticle[vThreadID.x].fSpeed * g_fDeltaTime;
		gParticle[vThreadID.x].fAngle += g_fTurnAngle* g_fDeltaTime;
	}
}

VS_OUTPUT_PARTICLE VS(VS_INPUT_PARTICLE input)
{
	VS_OUTPUT_PARTICLE output = (VS_OUTPUT_PARTICLE)0;

	output.iInstID = input.iInstID;

	return output;
}

static float4 g_vWorldPos[4] =
{
	{-0.5f, 0.5f, 0.f, 0.f},
	{0.5f, 0.5f, 0.f, 0.f},
	{0.5f, -0.5f, 0.f, 0.f},
	{-0.5f, -0.5f, 0.f, 0.f}
};

StructuredBuffer<tParticle>	gParticleInput	:	register(t30);
StructuredBuffer<tParticleShare>	gParticleShareInput	:	register(t31);

[maxvertexcount(6)]
void GS(point VS_OUTPUT_PARTICLE input[1],
	inout TriangleStream<GS_OUTPUT_PARTICLE> stream)
{
	uint iInstID = input[0].iInstID;

	GS_OUTPUT_PARTICLE output[4] = 
	{
		(GS_OUTPUT_PARTICLE)0, 
		(GS_OUTPUT_PARTICLE)0, 
		(GS_OUTPUT_PARTICLE)0, 
		(GS_OUTPUT_PARTICLE)0
	};

	output[0].vUV = ComputeUV(float2(gParticleInput[iInstID.x].iFrame, 0.f));
	output[1].vUV = ComputeUV(float2(1.f+ gParticleInput[iInstID.x].iFrame, 0.f));
	output[2].vUV = ComputeUV(float2(1.f+ gParticleInput[iInstID.x].iFrame, 1.f));
	output[3].vUV = ComputeUV(float2(gParticleInput[iInstID.x].iFrame, 1.f));

	float4 vViewPos = mul(float4(gParticleInput[iInstID].vWorldPos, 1.f), g_matView);

	float3x3 matRot =
	{
		{cos(gParticleInput[iInstID].fAngle), sin(gParticleInput[iInstID].fAngle), 0},
		{-sin(gParticleInput[iInstID].fAngle), cos(gParticleInput[iInstID].fAngle), 0},
		{0, 0, 1}
	};

	float fRatio = gParticleInput[iInstID].fLifeTime / 
		gParticleInput[iInstID].fMaxLifeTime;

	float3 vScale = lerp(gParticleShareInput[0].vStartScale, 
		gParticleShareInput[0].vEndScale,
		float3(fRatio, fRatio, fRatio));

	float4 vColor = lerp(gParticleShareInput[0].vStartColor, 
		gParticleShareInput[0].vEndColor,
		float4(fRatio, fRatio, fRatio, fRatio));

	for (int i = 0; i < 4; ++i)
	{
		matrix mat = g_matWorld;

		mat[3][0] = 0.f;
		mat[3][1] = 0.f;
		mat[3][2] = 0.f;

		float4 vScaled = mul(g_vWorldPos[i].xyz, mat);

		float3 vWorld = float3(vScaled.x, vScaled.y, vScaled.z) * vScale;
		
		vWorld = mul(vWorld, matRot);

		output[i].vPos = float4(vWorld + vViewPos.xyz, 1.f);

		output[i].vPos = mul(output[i].vPos, g_matProj);

		output[i].vColor = vColor;
	}
	
	stream.Append(output[0]);
	stream.Append(output[1]);
	stream.Append(output[2]);
	stream.RestartStrip();

	stream.Append(output[0]);
	stream.Append(output[2]);
	stream.Append(output[3]);
	stream.RestartStrip();
}

PS_OUTPUT_COLOR PS(GS_OUTPUT_PARTICLE input) : SV_TARGET
{
	PS_OUTPUT_COLOR output = (PS_OUTPUT_COLOR)0.f;

	output.vColor = gDiffuseMap.Sample(g_sPoint, input.vUV)* g_vDif;

	output.vColor *= input.vColor;

	return output;
}
