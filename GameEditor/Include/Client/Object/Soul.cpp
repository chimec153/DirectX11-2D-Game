#include "pch.h"
#include "Soul.h"
#include "Component/Particle.h"
#include "Resource/Material.h"
#include "Player.h"
#include "Scene/Scene.h"

float CSoul::m_fScale = 0.f;

CSoul::CSoul()	:
	m_pTarget(nullptr)
	, m_vInitVel()
	, m_vTermVel()
	, m_vSpeed()
	, m_fTime(0.f)
	, m_vTargetPos()
	, m_bArrive(false)
	, m_fOffset(0)
	, m_vPrevPos()
{
}

CSoul::CSoul(const CSoul& obj)	:
	CObj(obj)
	, m_pTarget(obj.m_pTarget)
	, m_vInitVel(obj.m_vInitVel)
	, m_vTermVel(obj.m_vTermVel)
	, m_vSpeed(obj.m_vSpeed)
	, m_fTime(obj.m_fTime)
	, m_vTargetPos(obj.m_vTargetPos)
	, m_bArrive(obj.m_bArrive)
	, m_fOffset(obj.m_fOffset)
	, m_vPrevPos(obj.m_vPrevPos)
{
	if (m_pTarget)
		m_pTarget->AddRef();
}

CSoul::~CSoul()
{
	SAFE_RELEASE(m_pTarget);
}

void CSoul::SetTarget(CObj* pObj)
{
	m_pTarget = pObj;

	if (m_pTarget)
		m_pTarget->AddRef();
}

bool CSoul::Init()
{
	if (!CObj::Init())
		return false;

	CParticle* pSouls = CreateComponent<CParticle>("Soul", m_pScene->FindLayer("Speacial"));

	pSouls->AddParticle("Souls");
	pSouls->AddParticle("SoulsShare");
	pSouls->AddRenderState("DepthNoWrite");
	pSouls->SetSpawnLimit(0.02f);
	pSouls->SetUVStart(192.f, 0.f);
	pSouls->SetUVEnd(256.f, 64.f);
	pSouls->SetUVSize(256.f, 256.f);
	pSouls->SetInheritScale(false);
	pSouls->SetRelativeScale(1.f, 1.f, 1.f);

	CMaterial* pParticleMtrl = pSouls->GetMaterial();

	pParticleMtrl->SetTexture(REGISTER_TYPE::RT_DIF, "Effect", 0, 1,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY | (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);

	SAFE_RELEASE(pParticleMtrl);

	SetRootComponent(pSouls);

	SAFE_RELEASE(pSouls);

	return true;
}

void CSoul::Start()
{
	CObj::Start();
}

void CSoul::Update(float fTime)
{
	CObj::Update(fTime);

	m_fTime += fTime;

	if (!m_bArrive)
	{
		Vector3 vPos = GetWorldPos();

		if (m_pTarget)
		{
			m_vTargetPos = m_pTarget->GetWorldPos() + Vector3(0.f, 16.f * m_fOffset, 0.f);
			m_vTargetPos.z = 0.f;
		}

		Vector3 vDir = m_vTargetPos - vPos;

		float fLength = vDir.Length();

		if (fLength <= 16.f)
		{
			m_bArrive = true;
			m_vPrevPos = vPos;

			if (m_fScale == 0.f)
			{
				m_fScale = 0.5f;
			}

			((CPlayer*)m_pTarget)->SetFly(true);
		}

		vDir/= fLength;

		Vector3 vAxis = GetWorldAxis(WORLD_AXIS::AXIS_Y);

		Vector3 vZAxis = vDir.Cross(vAxis);

		AddWorldRotZ(-vZAxis.z * 120.f * fTime);

		AddWorldPos(cosf(m_fTime) * 4.f * fTime, sinf(m_fTime) * 4.f * fTime, 0.f);

		AddWorldPos(vAxis * fTime * 64.f);
	}

	else
	{
		m_vPrevPos = m_pTarget->GetWorldPos() + 
			Vector3(cosf(m_fTime * 4.f) * 128.f * m_fScale,
				sinf(m_fTime * 4.f) * 64.f * m_fScale + 16.f * m_fOffset * m_fScale, 0.f);

		Vector3 vPos = GetWorldPos();

		Vector3 vDir = m_vPrevPos - vPos;

		vDir.Normalize();

		AddWorldPos(vDir * fTime * 512.f);

		m_fScale -= fTime * 0.0625f / 6.f;

		if (m_fScale <= 0.f)
		{
			m_fScale = 0.f;
			((CPlayer*)m_pTarget)->AddSoul();
			Destroy();
		}

		else if (m_fScale <= 0.125f)
		{
			SetRelativeScale(m_fScale * 8.f, m_fScale * 8.f, m_fScale * 8.f);
		}
	}
}

void CSoul::PostUpdate(float fTime)
{
	CObj::PostUpdate(fTime);
}

void CSoul::Collision(float fTime)
{
	CObj::Collision(fTime);
}

void CSoul::PreRender(float fTime)
{
	CObj::PreRender(fTime);
}

void CSoul::Render(float fTime)
{
	CObj::Render(fTime);
}

void CSoul::PostRender(float fTime)
{
	CObj::PostRender(fTime);
}

CSoul* CSoul::Clone()
{
	return new CSoul(*this);
}

void CSoul::Save(FILE* pFile)
{
	CObj::Save(pFile);
}

void CSoul::Load(FILE* pFile)
{
	CObj::Load(pFile);
}
