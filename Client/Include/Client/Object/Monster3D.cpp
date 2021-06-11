#include "Monster3D.h"
#include "Resource/Skeleton.h"
#include "Resource/ResourceManager.h"
#include "Resource/AnimationSequence.h"
#include "Resource/Animation.h"
#include "Component/Mesh2DComponent.h"
#include "Scene/Scene.h"
#include "Component/Renderer.h"
#include "../../GeometryGenerator.h"
#include "Resource/Material.h"
#include "Component/ColliderOBB.h"

CMonster3D::CMonster3D()	:
	m_pAni(nullptr)
	, m_pCom(nullptr)
, m_pChild(nullptr)
{
}

CMonster3D::CMonster3D(const CMonster3D& mon)	:
	CObj(mon)
	, m_pAni(nullptr)
	, m_pCom(nullptr)
	, m_pChild(nullptr)
{
}

CMonster3D::~CMonster3D()
{

	SAFE_RELEASE(m_pAni);
	SAFE_RELEASE(m_pCom);
	SAFE_RELEASE(m_pChild);
}

CMesh2DComponent* CMonster3D::GetCom() const
{
	if (m_pCom)
		m_pCom->AddRef();

	return m_pCom;
}

CAnimation* CMonster3D::GetAni() const
{
	if (m_pAni)
		m_pAni->AddRef();

	return m_pAni;
}

bool CMonster3D::Init()
{
	if (!CObj::Init())
		return false;

	CMesh2DComponent* pCom = CreateComponent<CMesh2DComponent>("Monster");

	SetRootComponent(pCom);

	pCom->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_3D);
	pCom->SetMesh("Player_Default");

	m_pAni = CAnimation::CreateAnimation<CAnimation>("MonsterAni",m_pScene);

	m_pAni->SetSkeleton("Player_Default");
	m_pAni->AddSequence("Idle", "Player_Default");

	CRenderer* pRenderer = pCom->GetRenderer();
	SAFE_RELEASE(pCom);

	pRenderer->SetAnimation(m_pAni);

	SAFE_RELEASE(pRenderer);

	return true;
}

void CMonster3D::Start()
{
	CObj::Start();
}

void CMonster3D::Update(float fTime)
{
	CObj::Update(fTime);
}

void CMonster3D::PostUpdate(float fTime)
{
	CObj::PostUpdate(fTime);
}

void CMonster3D::Collision(float fTime)
{
	CObj::Collision(fTime);
}

void CMonster3D::PreRender(float fTime)
{
	CObj::PreRender(fTime);
}

void CMonster3D::Render(float fTime)
{
	CObj::Render(fTime);
}

void CMonster3D::PostRender(float fTime)
{
	CObj::PostRender(fTime);
}

CMonster3D* CMonster3D::Clone()
{
	return new CMonster3D(*this);
}

void CMonster3D::Save(FILE* pFile)
{
	CObj::Save(pFile);
}

void CMonster3D::Load(FILE* pFile)
{
	CObj::Load(pFile);
}
