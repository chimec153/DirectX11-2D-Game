#include "Bullet.h"
#include "Component/Mesh2DComponent.h"
#include "Resource/Material.h"
#include "Component/Camera.h"
#include "Scene/Scene.h"
#include "Scene/CameraManager.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderRect.h"
#include "Player.h"
#include "Component/ColliderCircle.h"
#include "Tile/TileMap.h"
#include "Tile/Tile.h"
#include "Component/Sound.h"
#include "../GameMode/TileMode.h"
#include "Component/Particle.h"
#include "Engine.h"

CBullet::CBullet() :
	m_fDist(0.f),
	m_fLimitSpeed(1000.f),
	m_fSpeed(m_fLimitSpeed),
	m_pMesh(nullptr),
	m_pMap(nullptr),
	m_pSnd(nullptr),
	m_iCol(0),
	m_bFire(false),
	m_pFire(nullptr),
	m_bFixed(false),
	m_pFixingObj(nullptr),
	m_pMtrlMap(nullptr),
	m_fAddSpeed(1500.f),
	m_bInit(true)
{
	m_iObjClassType = (int)OBJ_CLASS_TYPE::OCT_BULLET;
}

CBullet::CBullet(const CBullet& bullet) :
 	CObj(bullet)
	, m_pSnd((CSound*)FindComByType<CSound>())
	, m_bFire(bullet.m_bFire)
	, m_pFire((CSpriteComponent*)FindSceneComponent("Fire"))
	, m_bFixed(bullet.m_bFixed)
	, m_pFixingObj(bullet.m_pFixingObj)
	, m_fAddSpeed(bullet.m_fAddSpeed)
	, m_bInit(true)
{
	m_fDist = 0.f;
	m_fLimitSpeed = bullet.m_fLimitSpeed;
	m_fSpeed = bullet.m_fSpeed;

	m_pMesh = (CSpriteComponent*)FindSceneComponent("Mesh");

	SetRootComponent(m_pMesh);

	CCollider* pRC = (CCollider*)FindSceneComponent("BulletBody");

	pRC->SetCallBack<CBullet>(COLLISION_STATE::INIT, this, &CBullet::ColInit);
	pRC->SetCallBack<CBullet>(COLLISION_STATE::STAY, this, &CBullet::ColStay);
	pRC->SetCallBack<CBullet>(COLLISION_STATE::END, this, &CBullet::ColEnd);

	SAFE_RELEASE(pRC);

	m_pMap = bullet.m_pMap;

	if (m_pMap)
		m_pMap->AddRef();

	m_iCol = 0;

	if (m_pFixingObj)
	{
		m_pFixingObj->AddRef();
	}

	m_pMtrlMap = bullet.m_pMtrlMap;

	if (m_pMtrlMap)
		m_pMtrlMap->AddRef();
}

CBullet::~CBullet()
{
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pMap);
	SAFE_RELEASE(m_pSnd);
	SAFE_RELEASE(m_pFire);
	SAFE_RELEASE(m_pFixingObj);
	SAFE_RELEASE(m_pMtrlMap);
}

void CBullet::ChangeSprite(const std::string& strTag)
{
	m_pMesh->ChangeSequence(strTag);
}

void CBullet::SetFire(bool bFire)
{
	m_bFire = bFire;
	m_pFire->Enable(m_bFire);

	CParticle* pParticle = (CParticle*)FindSceneComponent("FireParticle");

	if (m_bFire)
	{
		pParticle->Resume();

		m_pSnd->SetSoundAndPlay("ArrowIgnite");
	}

	else
	{
		pParticle->Stop();
	}

	SAFE_RELEASE(pParticle);

}

void CBullet::SetFix(bool bFix)
{
	if (bFix)
	{
		m_pSnd->SetSoundAndPlay("Impact_Boss");
	}

	m_bFixed = bFix;
}

void CBullet::SetFixObj(CObj* pObj)
{
	SAFE_RELEASE(m_pFixingObj);

	m_pFixingObj = pObj;

	if (m_pFixingObj)
	{
		m_pFixingObj->AddRef();
	}
}

CObj* CBullet::GetFixObj() const
{
	if (m_pFixingObj)
	{
		m_pFixingObj->AddRef();
	}

	return m_pFixingObj;
}

bool CBullet::Init()
{
	m_pMesh = CreateComponent<CSpriteComponent>("Mesh", m_pLayer);

	m_pMesh->SetWorldRotX(180.f);
	m_pMesh->CreateSprite("Arrow", "Arrow", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("ArrowStop", "ArrowStop", LOOP_OPTION::LOOP);
	m_pMesh->AddRenderState("NoCullBack");
	m_pMesh->AddRenderState("ForeGround");

	CMaterial* pMtrl = m_pMesh->GetMaterial();

	//pMtrl->SetGray(false);

	SAFE_RELEASE(pMtrl);

	CColliderCircle* pRC = CreateComponent<CColliderCircle>("BulletBody", m_pLayer);

	pRC->SetRadius(8.f);
	pRC->SetProfile("Base");

	pRC->SetCallBack<CBullet>(COLLISION_STATE::INIT, this, &CBullet::ColInit);
	pRC->SetCallBack<CBullet>(COLLISION_STATE::STAY, this, &CBullet::ColStay);
	pRC->SetCallBack<CBullet>(COLLISION_STATE::END, this, &CBullet::ColEnd);

	m_pMesh->AddChild(pRC);

	SAFE_RELEASE(pRC);

	CLayer* pGround = m_pScene->FindLayer("Ground");

	CSpriteComponent* pShadow = CreateComponent<CSpriteComponent>("Shadow", pGround);

	pShadow->SetWorldScale(16.f, 16.f, 1.f);
	pShadow->SetPivot(0.5f, 0.5f, 0.f);
	pShadow->CreateSprite("ArrowShadow", "ArrowShadow", LOOP_OPTION::LOOP);
	pShadow->SetInheritScale(false);
	pShadow->SetInheritRotZ(false);
	pShadow->SetRelativePos(0.f, -4.f, 0.f);
	pShadow->SetInheritPos(false);
	pShadow->AddRenderState("AlphaBlend");

	pMtrl = pShadow->GetMaterial();

	pMtrl->SetDiffuseColor(1.f, 1.f, 1.f, 0.5f);

	SAFE_RELEASE(pMtrl);

	m_pMesh->AddChild(pShadow);

	SAFE_RELEASE(pShadow);

	SetRootComponent(m_pMesh);

	m_pMesh->SetRelativeScale(16.f, 16.f, 1.f);
	m_pMesh->SetPivot(0.5f, 0.5f, 0.f);

	m_pSnd = CreateComponent<CSound>("Sound");

	m_pSnd->SetSound("ArrowImpact1");
	m_pSnd->SetMin(512.f);
	m_pSnd->SetMax(1024.f);
	m_pSnd->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);

	m_pMesh->AddChild(m_pSnd);

	m_pFire = CreateComponent<CSpriteComponent>("Fire", m_pLayer);

	m_pFire->SetWorldScale(16.f, 16.f, 0.);
	m_pFire->SetInheritScale(false);
	m_pFire->SetPivot(0.5f, 0.5f, 0.f);
	m_pFire->CreateSprite("Idle", "ArrowFire", LOOP_OPTION::LOOP);

	m_pMesh->AddChild(m_pFire);

	CParticle* pParticle = CreateComponent<CParticle>("FireParticle", m_pLayer);

	pParticle->AddParticle("ArrowFire");
	pParticle->AddParticle("ArrowFireShare");
	pParticle->SetUVStart(448.f, 32.f);
	pParticle->SetUVEnd(464.f, 48.f);
	pParticle->SetUVSize(512.f, 512.f);
	pParticle->SetWorldScale(1.f, 1.f, 1.f);
	pParticle->SetInheritScale(false);
	pParticle->SetSpawnLimit(0.01f);
	pParticle->SetInheritRotX(false);
	pParticle->AddRenderState("NoCullBack");
	pParticle->AddRenderState("DepthNoWrite");
	pParticle->AddRelativePos(0.f, -8.f, 0.f);
	pParticle->Stop();

	m_pMesh->AddChild(pParticle);

	pMtrl = pParticle->GetMaterial();

	pMtrl->SetTexture(REGISTER_TYPE::RT_DIF, "Pro", 0, 1, (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);

	SAFE_RELEASE(pMtrl);
	SAFE_RELEASE(pParticle);

	return true;
}

void CBullet::Start()
{
	CLayer* pMtrlLayer = m_pScene->FindLayer("1_MAT");

	if (pMtrlLayer)
	{
		SAFE_RELEASE(m_pMtrlMap);
		m_pMtrlMap = pMtrlLayer->GetTileMap();
	}
}

void CBullet::Update(float fTime)
{
	CObj::Update(fTime);

	if (!m_pMap)
	{
		CLayer* pLayer = m_pScene->FindLayer("1_COL");

		m_pMap = pLayer->GetTileMap();
	}

	Vector3 vPos = GetWorldPos()-GetWorldAxis(WORLD_AXIS::AXIS_Y) * GetWorldScale() * GetPivot();

	char cCol = m_pMap->GetTileCol(Vector2(vPos.x, vPos.y));

	if (cCol != 0)
	{
		CTile* pTile = m_pMap->GetTile(Vector2(vPos.x, vPos.y));

		Vector3 vDir = {};

		if (pTile)
		{
			Vector3 vTilePos = pTile->GetWorldPos();// -pTile->GetWorldScale() * 0.5f;

			vDir = vPos - vTilePos;

			vDir.z = 0.f;
		}			

		vDir.Normalize();

		Vector2 vDir2D = Vector2(vDir.x, vDir.y);

		Vector2 vAxis = Vector2(1.f, 0.f);

		float fAngle = RadToDeg(acosf(vDir2D.Dot(vAxis)));

		if (vDir2D.y < 0.f)
			fAngle *= -1.f;

		Vector3 vNorm = Vector3::Zero;

		if (fAngle > 45.f && fAngle < 135.f)	// ╩С
		{
			vNorm = Vector3(0.f, 1.f, 0.f);

			m_pSnd->SetSound("MinorArrowImpact1");
			m_pSnd->Play(fTime);
		}

		else if (fAngle > 135.f || fAngle < -135.f)	//	аб
		{
			vNorm = Vector3(-1.f, 0.f, 0.f);

			m_pSnd->SetSound("MinorArrowImpact4");
			m_pSnd->Play(fTime);
		}

		else if (fAngle < 45.f && fAngle > -45.f)	//	©Л
		{
			vNorm = Vector3(1.f, 0.f, 0.f);

			m_pSnd->SetSound("MinorArrowImpact2");
			m_pSnd->Play(fTime);
		}

		else	//	го
		{
			vNorm = Vector3(0.f, -1.f, 0.f);

			m_pSnd->SetSound("MinorArrowImpact3");
			m_pSnd->Play(fTime);
		}

		Vector3 vArrowDir = -GetWorldAxis(WORLD_AXIS::AXIS_Y);

		Vector3 vReflect = vArrowDir - vNorm.Dot(vArrowDir) * vNorm * 2;

		fAngle = RadToDeg(acosf(vReflect.Dot(vArrowDir)));

		if (vReflect.x * vReflect.y > 0.f)
			fAngle *= -1.f;

		if (vNorm.y == 0.f)
			fAngle *= -1.f;

		if (vArrowDir.y != 1.f && vArrowDir.y != -1.f)
		{
			fAngle = 180.f - fAngle;
		}

		AddWorldRotZ(-fAngle);
	}

	if (m_fSpeed > 0.f)
	{
		m_fSpeed -= fTime * m_fAddSpeed;
	}

	else if (m_fSpeed < 0.f)
	{
		m_fSpeed = 0.f;

		m_bInit = false;

		SetFire(false);

		m_pSnd->SetSound("Arrow_windloop");
		m_pSnd->Stop();
	}

	AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_Y)*
		m_fSpeed * -fTime);

	char cMtrl = m_pMtrlMap->GetTileCol(Vector2(vPos.x, vPos.y));

	char strTile[(int)MAT_TYPE::END] =
	{
		(char)atoi("4098"),
		(char)atoi("4099"),
		(char)atoi("4100"),
		(char)atoi("4101"),
		(char)atoi("4102"),
		(char)atoi("4103"),
		(char)atoi("4104"),
		(char)atoi("4105"),
		(char)atoi("4106"),
		(char)atoi("4107"),
		(char)atoi("4108"),
		(char)atoi("4109"),
		(char)atoi("4110"),
		(char)atoi("4111"),
		(char)atoi("4112"),
		(char)atoi("4116"),
		(char)atoi("4118"),
		(char)atoi("4119"),
		(char)atoi("4125"),
		(char)atoi("4145"),
		(char)atoi("4146"),
		(char)atoi("4147"),
		(char)atoi("4148"),
		(char)atoi("4149"),
		(char)atoi("4140"),
		(char)atoi("4141"),
		(char)atoi("4142"),
		(char)atoi("4153"),
		(char)atoi("4158")
	};

	if (cMtrl == strTile[(int)MAT_TYPE::WATER])
	{
		m_pSnd->SetSoundAndPlay("InWaterLoop");
	}
}

void CBullet::PostUpdate(float fTime)
{
	CObj::PostUpdate(fTime);
}

void CBullet::Collision(float fTime)
{
	CObj::Collision(fTime);
}

void CBullet::PreRender(float fTime)
{
	CObj::PreRender(fTime);

	if (GET_SINGLE(CEngine)->IsImgui())
	{
		SpawnWindow();
	}
}

void CBullet::Render(float fTime)
{
	CObj::Render(fTime);
}

void CBullet::PostRender(float fTime)
{
	CObj::PostRender(fTime);
}

CBullet* CBullet::Clone()
{
	return new CBullet(*this);
}

void CBullet::ColInit(CCollider* pSrc, CCollider* pDest, float fTime)
{
	++m_iCol;

	std::string strDst = pDest->GetName();

	if (strDst == "PlayerBody")
	{
		CPlayer* pObj = (CPlayer*)pDest->GetObj();

		if (!pObj->IsCharged() && !m_bFixed)
		{
			CObj* pCharge = m_pScene->CreateCloneObj("Charge", "Charge", m_pLayer, m_pScene->GetSceneType());

			pCharge->SetWorldPos(pSrc->GetCross());

			CSpriteComponent* pCom = pCharge->FindComByType<CSpriteComponent>();

			pCom->SetEndFunc<CObj>("Charge", pCharge, &CObj::Destroy);

			SAFE_RELEASE(pCom);

			SAFE_RELEASE(pCharge);

			pObj->Charge();

			m_pSnd->SetSound("Arrow_windloop");
			m_pSnd->Stop();

			m_pSnd->SetSoundAndPlay("Pickuparrow");

			Destroy();
		}
	}

	else if (strDst == "HeadBody" ||
		strDst == "LeftHandBody" ||
		strDst == "RightHandBody" ||
		strDst == "CubeBody" ||
		strDst == "IceBody" ||
		strDst == "BrainBody")
	{
		Vector3 vSrcPos = pSrc->GetWorldPos();

		Vector3 vDstPos = pDest->GetWorldPos();

		Vector2 vExtent = ((CColliderRect*)pDest)->GetExtent();

		Vector3 vCrs = pSrc->GetCross();

		Vector3 vNrm = {};

		if (vExtent.y / vExtent.x * (vCrs.x - vDstPos.x) + vDstPos.y < vCrs.y)
		{
			if (-vExtent.y / vExtent.x * (vCrs.x - vDstPos.x) + vDstPos.y < vCrs.y)
			{
				vNrm = Vector3(0.f, 1.f, 0.f);

				m_pSnd->SetSoundAndPlay("MinorArrowImpact1");
			}

			else
			{
				vNrm = Vector3(-1.f, 0.f, 0.f);

				m_pSnd->SetSoundAndPlay("MinorArrowImpact4");
			}
		}

		else
		{
			if (-vExtent.y / vExtent.x * (vCrs.x - vDstPos.x) + vDstPos.y < vCrs.y)
			{
				vNrm = Vector3(1.f, 0.f, 0.f);

				m_pSnd->SetSoundAndPlay("MinorArrowImpact2");
			}

			else
			{
				vNrm = Vector3(0.f, -1.f, 0.f);

				m_pSnd->SetSoundAndPlay("MinorArrowImpact3");
			}
		}

		m_pSnd->SetSound("Arrow_windloop");
		m_pSnd->Stop();

		m_pSnd->SetSound("arrow_retrieval");
		m_pSnd->Stop();

		Vector3 vReflect = {};

		Vector3 v = GetWorldAxis(WORLD_AXIS::AXIS_Y);

		vReflect = v - 2 * v.Dot(vNrm) * vNrm;

		float fAngle = RadToDeg(acosf(v.Dot(vReflect)));

		AddWorldRotZ(fAngle);
	}

}

void CBullet::ColStay(CCollider* pSrc, CCollider* pDest, float fTime)
{
	std::string strDst = pDest->GetName();

	if (strDst == "PlayerBody")
	{
		CPlayer* pObj = (CPlayer*)pDest->GetObj();

		if (!pObj->IsCharged() && !m_bFixed)
		{
			CObj* pCharge = m_pScene->CreateCloneObj("Charge", "Charge", m_pLayer, m_pScene->GetSceneType());

			pCharge->SetWorldPos(pSrc->GetCross());

			CSpriteComponent* pCom = pCharge->FindComByType<CSpriteComponent>();

			pCom->SetEndFunc<CObj>("Charge", pCharge, &CObj::Destroy);

			SAFE_RELEASE(pCom);

			SAFE_RELEASE(pCharge);

			pObj->Charge();

			m_pSnd->SetSound("Arrow_windloop");
			m_pSnd->Stop();

			m_pSnd->SetSound("Pickuparrow");
			m_pSnd->Play(fTime);

			Destroy();
		}
	}

	else if (strDst == "HeadBody" ||
		strDst == "LeftHandBody" ||
		strDst == "RightHandBody" ||
		strDst == "CubeBody" ||
		strDst == "IceBody")
	{
		if (!m_bInit)
		{
			AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_Y) *
				m_fSpeed * fTime);

			m_fSpeed = 100.f;
		}
	}
}

void CBullet::ColEnd(CCollider* pSrc, CCollider* pDest, float fTime)
{
	--m_iCol;
}

void CBullet::SpawnWindow()
{
	if (ImGui::Begin("Bullet"))
	{
		ImGui::SliderFloat("Speed", &m_fSpeed, 0.f, 1000.f);
		ImGui::SliderFloat("MaxSpeed", &m_fLimitSpeed, 0.f, 1000.f);
		ImGui::Checkbox("Fire", &m_bFire);
		Vector3 vRot = GetWorldRot();
		ImGui::SliderFloat("Rot Z", &vRot.z, -180.f, 180.f);
		SetWorldRotZ(vRot.z);
		ImGui::SliderInt("Col", &m_iCol, 0, 10);
	}
	ImGui::End();
}
