#include "Player.h"
#include "Component/Mesh2DComponent.h"
#include "Resource/Material.h"
#include "Bullet.h"
#include "Scene/Scene.h"
#include "Input.h"
#include "Component/Camera.h"
#include "Camera/CameraManager.h"
#include "InputObj.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderRect.h"
#include "Bow.h"
#include "Arrow.h"
#include "Component/ColliderOBB2D.h"
#include "Component/ColliderLine.h"
#include "Component/ColliderCircle.h"
#include "../UI/Missile.h"
#include "Scene/SceneManager.h"
#include "UI/UIFont.h"
#include "UI/Bar.h"
#include "UI/UIImage.h"
#include "Tile/TileMap.h"
#include "../GameMode/ColossusMode.h"
#include "Tile/Tile.h"

CPlayer::CPlayer() :
	m_pMesh(nullptr),
	m_pCam(nullptr),
	m_fSpeed(100.f),
	m_bIdleEnable(true),
	m_fScaleX(0.f),
	m_fScaleY(0.f),
	m_eDir(DIR_8::U),
	m_bCharge(true),
	m_bRolling(false),
	m_pBow(nullptr),
	m_pArrow(nullptr),
	m_pBullet(nullptr),
	m_fBowAngle(0.f),
	m_pTileMap(nullptr),
	m_pShadow(nullptr)
{
	m_iObjClassType = (int)OBJ_CLASS_TYPE::OCT_PLAYER;
}

CPlayer::CPlayer(const CPlayer& player) :
	CObj(player)
{
}

CPlayer::~CPlayer()
{
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pBullet);
	SAFE_RELEASE(m_pCam);
	SAFE_RELEASE(m_pBow);
	SAFE_RELEASE(m_pArrow);
	SAFE_RELEASE(m_pTileMap);
	SAFE_RELEASE(m_pCoord);
	SAFE_RELEASE(m_pShadow);
}

void CPlayer::SetMeshComponent(CSpriteComponent* pMesh)
{
	SAFE_RELEASE(m_pMesh);

	m_pMesh = pMesh;

	if (m_pMesh)
		m_pMesh->AddRef();
}

void CPlayer::Notify(const std::string& strTag)
{
	if (strTag == "Shot")
	{
		Fire1(1.f, 0.f);
	}
}

void CPlayer::Charge()
{
	//m_bCharge = true;
	m_pArrow->Enable(true);
	m_bIdleEnable = true;
}

bool CPlayer::Init()
{
	m_pMesh = CreateComponent<CSpriteComponent>("Mesh");

	CColliderRect* pRC = CreateComponent<CColliderRect>("PlayerBody");

	pRC->SetInheritScale(false);
	pRC->SetWorldScale(16.f, 16.f, 1.f);
	pRC->SetExtent(15.f, 15.f);
	pRC->SetProfile("Player");

	pRC->SetCallBack<CPlayer>(COLLISION_STATE::INIT, this, &CPlayer::ColStart);
	pRC->SetCallBack<CPlayer>(COLLISION_STATE::STAY, this, &CPlayer::ColStay);
	pRC->SetCallBack<CPlayer>(COLLISION_STATE::END, this, &CPlayer::ColEnd);

	m_pMesh->AddChild(pRC);

	m_pMesh->AddRenderState("Character");

	SAFE_RELEASE(pRC);

	m_pShadow = CreateComponent<CSpriteComponent>("Shadow");

	CMaterial* pMtrl = m_pShadow->GetMaterial();

	pMtrl->SetShader("Shadow");
	pMtrl->SetDiffuseColor(0.2f, 0.2f, 0.2f, 1.f);

	SAFE_RELEASE(pMtrl);

	m_pShadow->AddRenderState("Silhouette");
	m_pShadow->SetInheritScale(false);
	m_pShadow->SetRelativeScale(16.f, 16.f, 1.f);
	m_pShadow->SetPivot(0.5f, 0.5f, 0.f);

	m_pShadow->CreateSprite("IdleU", "ProIdleU", true);
	m_pShadow->CreateSprite("IdleRU", "ProIdleRU", true);
	m_pShadow->CreateSprite("IdleR", "ProIdleR", true);
	m_pShadow->CreateSprite("IdleRD", "ProIdleRD", true);
	m_pShadow->CreateSprite("IdleD", "ProIdleD", true);
	m_pShadow->CreateSprite("WalkU", "ProWalkU", true);
	m_pShadow->CreateSprite("WalkRU", "ProWalkRU", true);
	m_pShadow->CreateSprite("WalkR", "ProWalkR", true);
	m_pShadow->CreateSprite("WalkRD", "ProWalkRD", true);
	m_pShadow->CreateSprite("WalkD", "ProWalkD", true);
	m_pShadow->CreateSprite("AttackU", "ProAttackU", true);
	m_pShadow->CreateSprite("AttackRU", "ProAttackRU", true);
	m_pShadow->CreateSprite("AttackR", "ProAttackR", true);
	m_pShadow->CreateSprite("AttackRD", "ProAttackRD", true);
	m_pShadow->CreateSprite("AttackD", "ProAttackD", true);
	m_pShadow->CreateSprite("CallU", "ProCallU", true);
	m_pShadow->CreateSprite("CallRU", "ProCallRU", true);
	m_pShadow->CreateSprite("CallR", "ProCallR", true);
	m_pShadow->CreateSprite("CallRD", "ProCallRD", true);
	m_pShadow->CreateSprite("CallD", "ProCallD", true);
	m_pShadow->CreateSprite("RollU", "ProRollU", true);
	m_pShadow->CreateSprite("RollRU", "ProRollRU", true);
	m_pShadow->CreateSprite("RollR", "ProRollR", true);
	m_pShadow->CreateSprite("RollRD", "ProRollRD", true);
	m_pShadow->CreateSprite("RollD", "ProRollD", true);
	
	m_pMesh->AddChild(m_pShadow);

	m_pMesh->CreateSprite("IdleU", "ProIdleU", true);
	m_pMesh->CreateSprite("IdleRU", "ProIdleRU", true);
	m_pMesh->CreateSprite("IdleR", "ProIdleR", true);
	m_pMesh->CreateSprite("IdleRD", "ProIdleRD", true);
	m_pMesh->CreateSprite("IdleD", "ProIdleD", true);
	m_pMesh->CreateSprite("WalkU", "ProWalkU", true);
	m_pMesh->CreateSprite("WalkRU", "ProWalkRU", true);
	m_pMesh->CreateSprite("WalkR", "ProWalkR", true);
	m_pMesh->CreateSprite("WalkRD", "ProWalkRD", true);
	m_pMesh->CreateSprite("WalkD", "ProWalkD", true);
	m_pMesh->CreateSprite("AttackU", "ProAttackU", true);
	m_pMesh->CreateSprite("AttackRU", "ProAttackRU", true);
	m_pMesh->CreateSprite("AttackR", "ProAttackR", true);
	m_pMesh->CreateSprite("AttackRD", "ProAttackRD", true);
	m_pMesh->CreateSprite("AttackD", "ProAttackD", true);
	m_pMesh->CreateSprite("CallU", "ProCallU", true);
	m_pMesh->CreateSprite("CallRU", "ProCallRU", true);
	m_pMesh->CreateSprite("CallR", "ProCallR", true);
	m_pMesh->CreateSprite("CallRD", "ProCallRD", true);
	m_pMesh->CreateSprite("CallD", "ProCallD", true);
	m_pMesh->CreateSprite("RollU", "ProRollU", true);
	m_pMesh->CreateSprite("RollRU", "ProRollRU", true);
	m_pMesh->CreateSprite("RollR", "ProRollR", true);
	m_pMesh->CreateSprite("RollRD", "ProRollRD", true);
	m_pMesh->CreateSprite("RollD", "ProRollD", true);

	m_pMesh->SetEndFunc("RollU", this, &CPlayer::RollEnd);
	m_pMesh->SetEndFunc("RollRU", this, &CPlayer::RollEnd);
	m_pMesh->SetEndFunc("RollR", this, &CPlayer::RollEnd);
	m_pMesh->SetEndFunc("RollRD", this, &CPlayer::RollEnd);
	m_pMesh->SetEndFunc("RollD", this, &CPlayer::RollEnd);

	m_pMesh->SetRelativeScale(16.f, 16.f, 1.f);
	m_pMesh->SetPivot(0.5f, 0.5f, 0.f);

	SetRootComponent(m_pMesh);

	m_pInput->SetAxisFunc("MoveV", this, &CPlayer::MoveV);
	m_pInput->SetAxisFunc("MoveH", this, &CPlayer::MoveH);
	//m_pInput->SetAxisFunc("RotZ", this, &CPlayer::RotZ);
	m_pInput->SetAxisFunc("Z", this, &CPlayer::Fire1);
	m_pInput->SetActionFunc("Fire1", KT_DOWN, this, &CPlayer::Attack);
	m_pInput->SetActionFunc("Fire1", KT_PRESS, this, &CPlayer::ReturnArrow);
	m_pInput->SetActionFunc("Fire1", KT_UP, this, &CPlayer::AttackFire);
	m_pInput->SetActionFunc("Shift", KT_DOWN, this, &CPlayer::ShiftDown);
	m_pInput->SetActionFunc("X", KT_UP, this, &CPlayer::ShiftUp);
	m_pInput->SetActionFunc("O", KT_DOWN, this, &CPlayer::Option);
	m_pInput->SetActionFunc("TAB", KT_DOWN, this, &CPlayer::TileMatToggle);
	m_pInput->SetActionFunc("F1", KT_DOWN, this, &CPlayer::Stage1);
	m_pInput->SetActionFunc("C", KT_DOWN, this, &CPlayer::CloseUp);
	m_pInput->SetActionFunc("C", KT_UP, this, &CPlayer::CloseUpEnd);

	CLayer* pLayer = GET_SINGLE(CSceneManager)->GetNextScene()->FindLayer("Default");

	m_pBow = m_pScene->CreateObject<CBow>("Bow", pLayer);

	m_pArrow = m_pScene->CreateObject<CArrow>("Arrow", pLayer);

	m_pCoord = CreateComponent<CUIFont>("Coord");

	m_pCoord->SetRect(0.f, 20.f, 500.f, 0.f);

	m_pMesh->AddChild(m_pCoord);

	return true;
}

void CPlayer::Input(float fTime)
{
}

void CPlayer::Update(float fTime)
{
	CObj::Update(fTime);

	TCHAR strPos[32] = {};

	Vector3 vPos = GetWorldPos();

	swprintf_s(strPos, TEXT("X: %.5f, Y: %.5f"), vPos.x, vPos.y);

	m_pCoord->SetText(strPos);
	m_pCoord->SetWorldPos(980.f, 5.f, 0.f);

	m_pBow->SetWorldPos(vPos);
	m_pArrow->SetWorldPos(vPos);

	if (!m_bRolling)
	{
		if (m_fScaleX == -1.f)	//	구르지 않고 걷는 상태다.
		{
			SetWorldRotYAll(180.f);

			if (m_fScaleY == 1.f)
			{
				m_eDir = DIR_8::LU;
				if (m_bIdleEnable)
				{
					ChangeSequenceAll("WalkRU");
					SetDefaultClipAll("IdleRU");
					m_fBowAngle = 45.f;
				}
				else
					Attack(0.f, fTime);
			}

			else if (m_fScaleY == -1.f)
			{
				m_eDir = DIR_8::LD;
				if (m_bIdleEnable)
				{
					ChangeSequenceAll("WalkRD");
					SetDefaultClipAll("IdleRD");
					m_fBowAngle = 135.f;
				}
				else
					Attack(0.f, fTime);
			}

			else
			{
				m_eDir = DIR_8::L;
				if (m_bIdleEnable)
				{
					ChangeSequenceAll("WalkR");
					SetDefaultClipAll("IdleR");
					m_fBowAngle = 90.f;
				}
				else
					Attack(0.f, fTime);
			}
		}

		else if (m_fScaleX == 1.f)
		{
			SetWorldRotYAll(0.f);

			if (m_fScaleY == 1.f)
			{
				m_eDir = DIR_8::RU;
				if (m_bIdleEnable)
				{
					ChangeSequenceAll("WalkRU");
					SetDefaultClipAll("IdleRU");
					m_fBowAngle = -45.f;
				}
				else
					Attack(0.f, fTime);
			}

			else if (m_fScaleY == -1.f)
			{
				m_eDir = DIR_8::RD;
				if (m_bIdleEnable)
				{
					ChangeSequenceAll("WalkRD");
					SetDefaultClipAll("IdleRD");
					m_fBowAngle = -135.f;
				}
				else
					Attack(0.f, fTime);
			}

			else
			{
				m_eDir = DIR_8::R;
				if (m_bIdleEnable)
				{
					ChangeSequenceAll("WalkR");
					SetDefaultClipAll("IdleR");
					m_fBowAngle = -90.f;
				}
				else
					Attack(0.f, fTime);
			}
		}

		else
		{
			if (m_fScaleY == 1.f)
			{
				m_eDir = DIR_8::U;
				if (m_bIdleEnable)
				{
					ChangeSequenceAll("WalkU");
					SetDefaultClipAll("IdleU");
					m_fBowAngle = 0.f;
				}
				else
					Attack(0.f, fTime);
			}

			else if (m_fScaleY == -1.f)
			{
				m_eDir = DIR_8::D;
				if (m_bIdleEnable)
				{
					ChangeSequenceAll("WalkD");
					SetDefaultClipAll("IdleD");
					m_fBowAngle = 180.f;
				}
				else
					Attack(0.f, fTime);
			}

			else
			{
				if (m_bIdleEnable)
				{
					m_pShadow->ReturnClip();
					m_pMesh->ReturnClip();
					m_pBow->ReturnClip();
					m_pArrow->ReturnClip();
				}
			}
		}
	}

	else // 구르고 있는 상태일 경우다.
	{
	float fSpeed = 150.f;

		switch (m_eDir)
		{
		case DIR_8::U:
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_Y) * fSpeed* fTime);
			break;
		case DIR_8::RU:
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_X) * fSpeed* fTime);
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_Y) * fSpeed* fTime);
			break;
		case DIR_8::R:
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_X) * fSpeed* fTime);
			break;
		case DIR_8::RD:
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_X)* fSpeed* fTime);
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_Y)* fSpeed* fTime * -1.f);
			break;
		case DIR_8::D:
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_Y)* fSpeed* fTime * -1.f);
			break;
		case DIR_8::LD:
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_X)* fSpeed* fTime);
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_Y)* fSpeed* fTime * -1.f);
			break;
		case DIR_8::L:
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_X)* fSpeed* fTime);
			break;
		case DIR_8::LU:
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_X)* fSpeed* fTime);
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_Y)* fSpeed* fTime);
			break;
		}
	}
}

void CPlayer::PostUpdate(float fTime)
{
	CObj::PostUpdate(fTime);

	if (m_fScaleX != 0.f || m_fScaleY != 0.f)	//	움직였을 경우다.
	{
		if (!m_pTileMap)
		{
			CLayer* pColLayer = m_pScene->FindLayer("1_COL");

			if (pColLayer)
				m_pTileMap = pColLayer->GetTileMap();
		}

		Vector3 vScale = GetWorldScale();
		Vector3 vPos = GetWorldPos() - GetPivot() * vScale;

		char cCol = m_pTileMap->GetTileCol(Vector2(vPos.x, vPos.y));

		if (cCol == 0)
		{
			cCol = m_pTileMap->GetTileCol(Vector2(vPos.x, vPos.y) + Vector2(0.f, vScale.y));

			if (cCol == 0)
			{
				cCol = m_pTileMap->GetTileCol(Vector2(vPos.x, vPos.y) + Vector2(vScale.x, 0.f));

				if (cCol == 0)
				{
					cCol = m_pTileMap->GetTileCol(Vector2(vPos.x, vPos.y) + Vector2(0.f, -vScale.y));
				}
			}
		}

		if (cCol != 0)
		{
			AddWorldPos(- m_fScaleX * m_fSpeed * fTime, -m_fScaleY * m_fSpeed * fTime, 0.f);
		}
	}

	m_fScaleX = 0.f;
	m_fScaleY = 0.f;
}

void CPlayer::Collision(float fTime)
{
	CObj::Collision(fTime);

	//if (m_fScaleX != 0.f || m_fScaleY != 0.f)	//	움직였을 경우다.
	//{
	//	if (!m_pTileMap)
	//	{
	//		CLayer* pColLayer = m_pScene->FindLayer("1_COL");

	//		if (pColLayer)
	//			m_pTileMap = pColLayer->GetTileMap();
	//	}

	//	TileMapCol(m_pTileMap);

	//	CLayer* pColALayer = m_pScene->FindLayer("1_COLA");

	//	if (pColALayer)
	//	{
	//		CTileMap* pMap = pColALayer->GetTileMap();

	//		TileMapCol(pMap);

	//		SAFE_RELEASE(pMap);
	//	}
	//}
}

void CPlayer::PreRender(float fTime)
{
	m_fScaleX = 0.f;
	m_fScaleY = 0.f;

	CObj::PreRender(fTime);
}

void CPlayer::Render(float fTime)
{
	CObj::Render(fTime);
}

void CPlayer::PostRender(float fTime)
{
	CObj::PostRender(fTime);

}

CPlayer* CPlayer::Clone()
{
	return new CPlayer(*this);
}

void CPlayer::Save(FILE* pFile)
{
	CObj::Save(pFile);

	int iLength = (int)m_pMesh->GetName().length();

	fwrite(&iLength, 4, 1, pFile);
	fwrite(m_pMesh->GetName().c_str(), 1, iLength, pFile);
}

void CPlayer::Load(FILE* pFile)
{
	CObj::Load(pFile);

	SAFE_RELEASE(m_pMesh);

	int iLength = 0;
	char strTag[256] = {};

	fread(&iLength, 4, 1, pFile);
	fread(strTag, 1, iLength, pFile);

	m_pMesh = (CSpriteComponent*)FindSceneComponent(strTag);

	m_pInput->SetAxisFunc("MoveV", this, &CPlayer::MoveV);
	m_pInput->SetAxisFunc("MoveH", this, &CPlayer::MoveH);
	m_pInput->SetAxisFunc("RotZ", this, &CPlayer::RotZ);
	m_pInput->SetActionFunc("Fire1", KT_DOWN, this, &CPlayer::Fire1);
}

void CPlayer::MoveV(float fScale, float fTime)
{
	if (m_bIdleEnable && !m_bRolling)
		m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_Y) * m_fSpeed * fTime * fScale);
	m_fScaleY = fScale;
}

void CPlayer::MoveH(float fScale, float fTime)
{
	if (m_bIdleEnable && !m_bRolling)
	{
			m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_X) * m_fSpeed * fTime * fScale * fScale);

	}
	m_fScaleX = fScale;
}

void CPlayer::RotZ(float fScale, float fTime)
{
	m_pMesh->AddRelativeRotZ(180.f * fTime * fScale);
}

void CPlayer::Attack(float fPushTime, float fTime)
{
	m_bIdleEnable = false;

	if (m_bCharge)
	{
		if(m_pCam)
			m_pCam->SetZoom(true);

		if(!m_pBullet)
		{
			m_pBullet = (CBullet*)m_pScene->CreateCloneObj("Bullet", "Bullet",m_pLayer, m_pScene->GetSceneType());
			m_pBullet->SetSpeed(0.f);
		}

		float fDist = m_pBullet->GetWorldScale().x * m_pBullet->GetPivot().x + GetWorldScale().x * GetPivot().x + 1.f;

		if (m_pCam)
		m_pCam->SetTarget(m_pBullet);

		m_pBullet->SetRelativePos(GetWorldPos() + Vector3(fDist * -sinf(DirectX::XMConvertToRadians(m_fBowAngle)), fDist * cosf(DirectX::XMConvertToRadians(m_fBowAngle)), 0.f));
		m_pBullet->SetWorldRotZ(m_fBowAngle);

		if (m_fBowAngle > 180.f)
			m_fBowAngle -= 360.f;

		else if (m_fBowAngle < -180.f)
			m_fBowAngle += 360.f;

		switch (m_eDir)
		{
		case DIR_8::U:
			SetWorldRotYAll(0.f);
			ChangeSequenceAll("AttackU");
			SetDefaultClipAll("IdleU");

			if (m_fBowAngle > 0.f)
				m_fBowAngle -= fTime * 250.f;

			else
				m_fBowAngle += fTime * 250.f;

			break;
		case DIR_8::RU:
			SetWorldRotYAll(0.f);
			ChangeSequenceAll("AttackRU");
			SetDefaultClipAll("IdleRU");

			if (cosf(DirectX::XMConvertToRadians(m_fBowAngle-45.f)) > 0.f)
				m_fBowAngle -= fTime * 250.f;

			else
				m_fBowAngle += fTime * 250.f;

			break;
		case DIR_8::R:
			SetWorldRotYAll(0.f);
			ChangeSequenceAll("AttackR");
			SetDefaultClipAll("IdleR");

			if (cosf(DirectX::XMConvertToRadians(m_fBowAngle)) > 0.f)
				m_fBowAngle -= fTime * 250.f;

			else
				m_fBowAngle += fTime * 250.f;

			break;
		case DIR_8::RD:
			SetWorldRotYAll(0.f);
			ChangeSequenceAll("AttackRD");
			SetDefaultClipAll("IdleRD");

			if (cosf(DirectX::XMConvertToRadians(m_fBowAngle + 45.f)) > 0.f)
				m_fBowAngle -= fTime * 250.f;

			else 
				m_fBowAngle += fTime * 250.f;
			break;
		case DIR_8::D:
			SetWorldRotYAll(0.f);
			ChangeSequenceAll("AttackD");
			SetDefaultClipAll("IdleD");

			if (sinf(DirectX::XMConvertToRadians(m_fBowAngle)) < 0.f)
				m_fBowAngle -= fTime * 250.f;

			else
				m_fBowAngle += fTime * 250.f;

			break;
		case DIR_8::LD:
			SetWorldRotYAll(180.f);
			ChangeSequenceAll("AttackRD");
			SetDefaultClipAll("IdleRD");

			if (cosf(DirectX::XMConvertToRadians(m_fBowAngle - 45.f)) < 0.f)
				m_fBowAngle -= fTime * 250.f;

			else
				m_fBowAngle += fTime * 250.f;
			break;
		case DIR_8::L:
			SetWorldRotYAll(180.f);
			ChangeSequenceAll("AttackR");
			SetDefaultClipAll("IdleR");

			if (cosf(DirectX::XMConvertToRadians(m_fBowAngle)) < 0.f)
				m_fBowAngle -= fTime * 250.f;

			else 
				m_fBowAngle += fTime * 250.f;
			break;
		case DIR_8::LU:
			SetWorldRotYAll(180.f);
			ChangeSequenceAll("AttackRU");
			SetDefaultClipAll("IdleRU");

			if (cosf(DirectX::XMConvertToRadians(m_fBowAngle + 45.f)) < 0.f)
				m_fBowAngle -= fTime * 250.f;

			else
				m_fBowAngle += fTime * 250.f;
			break;
		}
	}

	else
	{
		switch (m_eDir)
		{
		case DIR_8::U:
			SetWorldRotYAll(0.f);
			ChangeSequenceAll("CallU");
			SetDefaultClipAll("IdleU");
			break;
		case DIR_8::RU:
			SetWorldRotYAll(0.f);
			ChangeSequenceAll("CallRU");
			SetDefaultClipAll("IdleRU");
			break;
		case DIR_8::R:
			SetWorldRotYAll(0.f);
			ChangeSequenceAll("CallR");
			SetDefaultClipAll("IdleR");
			break;
		case DIR_8::RD:
			SetWorldRotYAll(0.f);
			ChangeSequenceAll("CallRD");
			SetDefaultClipAll("IdleRD");
			break;
		case DIR_8::D:
			SetWorldRotYAll(0.f);
			ChangeSequenceAll("CallD");
			SetDefaultClipAll("IdleD");
			break;
		case DIR_8::LD:
			SetWorldRotYAll(180.f);
			ChangeSequenceAll("CallRD");
			SetDefaultClipAll("IdleRD");
			break;
		case DIR_8::L:
			SetWorldRotYAll(180.f);
			ChangeSequenceAll("CallR");
			SetDefaultClipAll("IdleR");
			break;
		case DIR_8::LU:
			SetWorldRotYAll(180.f);
			ChangeSequenceAll("CallRU");
			SetDefaultClipAll("IdleRU");
			break;
		}
	}
}

void CPlayer::AttackFire(float fPushTime, float fTime)
{
	m_bIdleEnable = true;

	if (m_pCam)
	{
		m_pCam->SetZoom(false);
		m_pCam->SetTarget(this);
	}

	if (!m_bCharge)
	{
		CLayer* pLayer = m_pScene->FindLayer("Default");

		CObj* pObj = pLayer->FindObj("Bullet");

		if (!pObj)
			m_bCharge = true;

		SAFE_RELEASE(pObj);

		return;
	}

	m_bCharge = false;
	m_pArrow->Enable(false);

#ifdef _DEBUG
	char strTime[32] = {};
	sprintf_s(strTime, "PT: %.5f\n", fPushTime);
	OutputDebugStringA(strTime);
#endif

	m_pBullet->SetLimitDist(fPushTime * 200.f);

	m_pBullet->SetRelativeRotZ(-45.f * (int)m_eDir);
/*
	switch (m_eDir)
	{
	case DIR_8::RU:
		m_pBullet->SetRelativeRotZ(-45.f);
		break;
	case DIR_8::R:
		m_pBullet->SetRelativeRotZ(-90.f);
		break;
	case DIR_8::RD:
		m_pBullet->SetRelativeRotZ(-135.f);
		break;
	case DIR_8::D:
		m_pBullet->SetRelativeRotZ(-180.f);
		break;
	case DIR_8::LD:
		m_pBullet->SetRelativeRotZ(135.f);
		break;
	case DIR_8::L:
		m_pBullet->SetRelativeRotZ(90.f);
		break;
	case DIR_8::LU:
		m_pBullet->SetRelativeRotZ(45.f);
		break;
	}*/

	m_pBullet->SetSpeed(1000.f);
	

	SAFE_RELEASE(m_pBullet);
}

void CPlayer::AttackEnd()
{
}

void CPlayer::Fire1(float fScale, float fTime)
{
	if (fScale != 0.f)
	{
		CObj* pMissile = m_pScene->CreateCloneObj("Missile", "Missile", m_pLayer, m_pScene->GetSceneType());

		if (pMissile)
		{
			pMissile->SetWorldPos(GetWorldPos());

			pMissile->SetWorldRot(GetWorldRot());

			pMissile->AddWorldPos(pMissile->GetWorldAxis(WORLD_AXIS::AXIS_Y) * 32.f);
		}

		SAFE_RELEASE(pMissile);
	}
}

void CPlayer::ShiftDown(float fPushTime, float fTime)
{
	if (m_bRolling)
		return;

	m_fSpeed = 150.f;
	m_bRolling = true;
	m_bIdleEnable = false;

	switch (m_eDir)
	{
	case DIR_8::U:
		SetWorldRotYAll(0.f);
		ChangeSequenceAll("RollU");
		break;
	case DIR_8::RU:
		SetWorldRotYAll(0.f);
		ChangeSequenceAll("RollRU");
		break;
	case DIR_8::R:
		SetWorldRotYAll(0.f);
		ChangeSequenceAll("RollR");
		break;
	case DIR_8::RD:
		SetWorldRotYAll(0.f);
		ChangeSequenceAll("RollRD");
		break;
	case DIR_8::D:
		SetWorldRotYAll(0.f);
		ChangeSequenceAll("RollD");
		break;
	case DIR_8::LD:
		SetWorldRotYAll(180.f);
		ChangeSequenceAll("RollRD");
		break;
	case DIR_8::L:
		SetWorldRotYAll(180.f);
		ChangeSequenceAll("RollR");
		break;
	case DIR_8::LU:
		SetWorldRotYAll(180.f);
		ChangeSequenceAll("RollRU");
		break;
	}
}

void CPlayer::ShiftUp(float fPushTIme, float fTime)
{
	m_fSpeed = 100.f;
}

float CPlayer::GetAngle()
{
	Vector3 vCurAxis = GetWorldAxis(WORLD_AXIS::AXIS_Y);
	Vector3 vAxis = Vector3::Axis[(int)WORLD_AXIS::AXIS_Y];

	float fCos = vAxis.Dot(vCurAxis);
	float fTheta = acosf(fCos);

	if (vCurAxis.x)
		fTheta *= -1.f;

	return fTheta;
}

void CPlayer::ReturnArrow(float fPushTime, float fTime)
{
	if (m_bCharge)
		return;

#ifdef _DEBUG
	char strTime[32] = {};
	sprintf_s(strTime, "PT: %.5f\n", fPushTime);
	OutputDebugStringA(strTime);
#endif
	CLayer* pLayer = m_pScene->FindLayer("Default");
	CObj* pArw = pLayer->FindObj("Bullet");

	if (pArw)
	{
		float fSpeed = ((CBullet*)pArw)->GetSpeed();

		if (fSpeed == 0.f)
		{
			Vector3 vArwPos = pArw->GetWorldPos();

			Vector3 vDir = GetWorldPos() - vArwPos;

			vDir.Normalize();

			Vector3 vAxis = pArw->GetWorldAxis(WORLD_AXIS::AXIS_Y);

			float fCos = vDir.Dot(vAxis);

			float fTheta = DirectX::XMConvertToDegrees(acosf(fCos));

			if (vDir.y > 0)
				fTheta *= -1.f;

			pArw->AddWorldRotZ(fTheta);

			pArw->AddWorldPos(fPushTime * 200.f * fTime * pArw->GetWorldAxis(WORLD_AXIS::AXIS_Y));
		}
	}

	SAFE_RELEASE(pArw);
}

void CPlayer::RollEnd()
{
	m_bIdleEnable = true;
	m_bRolling = false;

	if (m_fSpeed == 150.f)
		m_fSpeed = 100.f;
}

void CPlayer::ColStart(CCollider* pSrc, CCollider* pDst, float fTime)
{
}

void CPlayer::ColStay(CCollider* pSrc, CCollider* pDst, float fTime)
{
	if (pDst->GetName() == "LeftHandBody" ||
		pDst->GetName() == "RightHandBody")
	{
		RectInfo tInfo = ((CColliderRect*)pSrc)->GetInfo();

		Vector3 vCross = pSrc->GetCross();

		Vector3 vPos = GetWorldPos();

		if (m_fScaleX != 0.f)
		{
			if (vCross.x > vPos.x)
			{
				SetWorldPos(Vector3(vPos.x - (tInfo.fR - tInfo.fL) + (vCross.x - vPos.x) * 2.f - 1.f, vPos.y, vPos.z));
			}

			else
			{
				SetWorldPos(Vector3(vPos.x + (tInfo.fR - tInfo.fL) + (vCross.x - vPos.x) * 2.f + 1.f, vPos.y, vPos.z));
			}
		}

		if (m_fScaleY != 0.f)
		{
			if (vCross.y > vPos.y)
			{
				SetWorldPos(Vector3(vPos.x, vPos.y - (tInfo.fT - tInfo.fB) + (vCross.y - vPos.y) * 2.f - 1.f, vPos.z));
			}

			else
			{
				SetWorldPos(Vector3(vPos.x, vPos.y + (tInfo.fT - tInfo.fB) + (vCross.y - vPos.y) * 2.f + 1.f, vPos.z));
			}
		}
	}
}

void CPlayer::ColEnd(CCollider* pSrc, CCollider* pDst, float fTime)
{
}

void CPlayer::Option(float fPushTime, float fTime)
{
	CLayer * pUI = m_pScene->FindLayer("UI");

	CObj* pObj = pUI->FindObj("Button");

	pObj->Enable(!pObj->IsEnable());

	SAFE_RELEASE(pObj);
}

void CPlayer::Fire2(float fScale, float fTime)
{
}

void CPlayer::TileMatToggle(float fScale, float fTime)
{
	CLayer* pLayer = m_pScene->FindLayer("1_COL");

	if (pLayer)
	{
		CObj* pObj = pLayer->FindObj("1_COL");

		CSceneComponent* pCom = pObj->FindSceneComponent("1_COL");

		pCom->Enable(!pCom->IsEnable());

		SAFE_RELEASE(pCom);

		SAFE_RELEASE(pObj);
	}

	pLayer = m_pScene->FindLayer("1_COLA");

	if (pLayer)
	{
		CObj* pObj = pLayer->FindObj("1_COLA");

		CSceneComponent* pCom = pObj->FindSceneComponent("1_COLA");

		pCom->Enable(!pCom->IsEnable());

		SAFE_RELEASE(pCom);

		SAFE_RELEASE(pObj);
	}

	pLayer = m_pScene->FindLayer("2_COL");

	if (pLayer)
	{
		CObj* pObj = pLayer->FindObj("2_COL");

		CSceneComponent* pCom = pObj->FindSceneComponent("2_COL");

		pCom->Enable(!pCom->IsEnable());

		SAFE_RELEASE(pCom);

		SAFE_RELEASE(pObj);
	}

	pLayer = m_pScene->FindLayer("1_MAT");

	if (pLayer)
	{
		CObj* pObj = pLayer->FindObj("1_MAT");

		CSceneComponent* pCom = pObj->FindSceneComponent("1_MAT");

		pCom->Enable(!pCom->IsEnable());

		SAFE_RELEASE(pCom);

		SAFE_RELEASE(pObj);
	}
}

void CPlayer::Stage1(float fScale, float fTime)
{
	GET_SINGLE(CSceneManager)->CreateNextScene();

	GET_SINGLE(CSceneManager)->SetGameMode<CColossusMode>(false);
}

void CPlayer::CloseUp(float fScale, float fTime)
{
	if (!m_pCam)
	{
		m_pCam = GET_SINGLE(CCameraManager)->GetMainCam();
	}

	m_pCam->SetZoom(true);

	Vector3 vPos = GetWorldPos();

	switch (m_eDir)
	{
	case DIR_8::U:
		m_pCam->SetMovePos(vPos + Vector3(0.f, 100.f, 0.f));
		break;
	case DIR_8::RU:
		m_pCam->SetMovePos(vPos + Vector3(70.7f, 70.7f, 0.f));
		break;
	case DIR_8::R:
		m_pCam->SetMovePos(vPos + Vector3(100.f, 0.f, 0.f));
		break;
	case DIR_8::RD:
		m_pCam->SetMovePos(vPos + Vector3(70.7f, -70.7f, 0.f));
		break;
	case DIR_8::D:
		m_pCam->SetMovePos(vPos + Vector3(0.f, -100.f, 0.f));
		break;
	case DIR_8::LD:
		m_pCam->SetMovePos(vPos + Vector3(-70.7f, -70.7f, 0.f));
		break;
	case DIR_8::L:
		m_pCam->SetMovePos(vPos + Vector3(-100.f, 0.f, 0.f));
		break;
	case DIR_8::LU:
		m_pCam->SetMovePos(vPos + Vector3(-70.7f, 70.7f, 0.f));
		break;
	}
}

void CPlayer::CloseUpEnd(float fScale, float fTime)
{
	if (!m_pCam)
	{
		m_pCam = GET_SINGLE(CCameraManager)->GetMainCam();
	}

	m_pCam->SetZoom(false);
}

void CPlayer::TileMapCol(CTileMap* pMap)
{
	Vector3 vScale = GetWorldScale();
	Vector3 vPivot = GetPivot();
	Vector3 vWorldPos = GetWorldPos();
	Vector3 vPos = vWorldPos - vPivot * vScale;

	if (pMap)
	{
		if (m_fScaleX == -1.f)
		{
			char cCol = pMap->GetTileCol(Vector2(vPos.x, vPos.y));

			if (cCol != 0)	//	좌하단이 충돌 했을 경우다.
			{
				CTile* pTile = pMap->GetTile(Vector2(vPos.x, vPos.y));

				Vector3 vTilePos = pTile->GetWorldPos();
				Vector3 vTileScale = pTile->GetWorldScale();
				Vector3 vTilePivot = pTile->GetPivot();

				Vector3 _vPos = vTilePos + vTileScale * vTilePivot + vScale * vPivot;

				m_pMesh->SetWorldPos(_vPos.x, vWorldPos.y, 0.f);
			}

			else
			{
				vPos = vWorldPos - vPivot * vScale;
				vPos += Vector3(0.f, vScale.y, 0.f);

				char cCol = pMap->GetTileCol(Vector2(vPos.x, vPos.y));

				if (cCol != 0)	//	좌상단이 충돌 했을 경우다.
				{
					CTile* pTile = pMap->GetTile(Vector2(vPos.x, vPos.y));

					Vector3 vTilePos = pTile->GetWorldPos();
					Vector3 vTileScale = pTile->GetWorldScale();
					Vector3 vTilePivot = pTile->GetPivot();

					Vector3 _vPos = vTilePos + vTileScale * vTilePivot + vScale * vPivot;

					m_pMesh->SetWorldPos(_vPos.x, vWorldPos.y, 0.f);
				}
			}
		}

		else if (m_fScaleX == 1.f)
		{
			vPos = vWorldPos - vPivot * vScale;
			vPos += Vector3(vScale.x, 0.f, 0.f);

			char cCol = pMap->GetTileCol(Vector2(vPos.x, vPos.y));

			if (cCol != 0)	//	우하단이 충돌 했을 경우다.
			{
				CTile* pTile = pMap->GetTile(Vector2(vPos.x, vPos.y));

				Vector3 vTilePos = pTile->GetWorldPos();
				Vector3 vTileScale = pTile->GetWorldScale();
				Vector3 vTilePivot = pTile->GetPivot();

				Vector3 _vPos = vTilePos - vTileScale * vTilePivot - vScale * vPivot;

				m_pMesh->SetWorldPos(_vPos.x, vWorldPos.y, 0.f);
			}

			else
			{
				vPos = vWorldPos + vPivot * vScale;

				char cCol = pMap->GetTileCol(Vector2(vPos.x, vPos.y));

				if (cCol != 0)	//	우상단이 충돌 했을 경우다.
				{
					CTile* pTile = pMap->GetTile(Vector2(vPos.x, vPos.y));

					Vector3 vTilePos = pTile->GetWorldPos();
					Vector3 vTileScale = pTile->GetWorldScale();
					Vector3 vTilePivot = pTile->GetPivot();

					Vector3 _vPos = vTilePos - vTileScale * vTilePivot - vScale * vPivot;

					m_pMesh->SetWorldPos(_vPos.x, vWorldPos.y, 0.f);
				}
			}
		}

		if (m_fScaleY == -1.f)
		{
			vWorldPos = GetWorldPos();
			vPos = vWorldPos - vPivot * vScale;

			char cCol = pMap->GetTileCol(Vector2(vPos.x, vPos.y));

			if (cCol != 0)	//	좌하단이 충돌 했을 경우다.
			{
				CTile* pTile = pMap->GetTile(Vector2(vPos.x, vPos.y));

				Vector3 vTilePos = pTile->GetWorldPos();
				Vector3 vTileScale = pTile->GetWorldScale();
				Vector3 vTilePivot = pTile->GetPivot();

				Vector3 _vPos = vTilePos + vTileScale * vTilePivot + vScale * vPivot;

				m_pMesh->SetWorldPos(vWorldPos.x, _vPos.y, 0.f);
			}

			else
			{
				vPos = vWorldPos - vPivot * vScale;

				vPos += Vector3(vScale.x, 0.f, 0.f);

				char cCol = pMap->GetTileCol(Vector2(vPos.x, vPos.y));

				if (cCol != 0)	//	우하단이 충돌 했을 경우다.
				{
					CTile* pTile = pMap->GetTile(Vector2(vPos.x, vPos.y));

					Vector3 vTilePos = pTile->GetWorldPos();
					Vector3 vTileScale = pTile->GetWorldScale();
					Vector3 vTilePivot = pTile->GetPivot();

					Vector3 _vPos = vTilePos + vTileScale * vTilePivot + vScale * vPivot;

					m_pMesh->SetWorldPos(vWorldPos.x, _vPos.y, 0.f);
				}
			}
		}

		else if (m_fScaleY == 1.f)
		{
			vWorldPos = GetWorldPos();
			vPos = vWorldPos - vPivot * vScale;
			vPos += Vector3(0.f, vScale.y, 0.f);

			char cCol = pMap->GetTileCol(Vector2(vPos.x, vPos.y));

			if (cCol != 0)	//	좌상단이 충돌 했을 경우다.
			{
				CTile* pTile = pMap->GetTile(Vector2(vPos.x, vPos.y));

				Vector3 vTilePos = pTile->GetWorldPos();
				Vector3 vTileScale = pTile->GetWorldScale();
				Vector3 vTilePivot = pTile->GetPivot();

				Vector3 _vPos = vTilePos - vTileScale * vTilePivot - vScale * vPivot;

				m_pMesh->SetWorldPos(vWorldPos.x, _vPos.y, 0.f);
			}

			else
			{
				vPos += Vector3(vScale.x, 0.f, 0.f);

				char cCol = pMap->GetTileCol(Vector2(vPos.x, vPos.y));

				if (cCol != 0)	//	우상단이 충돌 했을 경우다.
				{
					CTile* pTile = pMap->GetTile(Vector2(vPos.x, vPos.y));

					Vector3 vTilePos = pTile->GetWorldPos();
					Vector3 vTileScale = pTile->GetWorldScale();
					Vector3 vTilePivot = pTile->GetPivot();

					Vector3 _vPos = vTilePos - vTileScale * vTilePivot - vScale * vPivot;

					m_pMesh->SetWorldPos(vWorldPos.x, _vPos.y, 0.f);
				}
			}
		}
	}
}

void CPlayer::ChangeSequenceAll(const char* pSeq)
{
	m_pShadow->ChangeSequence(pSeq);
	m_pMesh->ChangeSequence(pSeq);
	m_pBow->ChangeClip(pSeq);
	m_pArrow->ChangeClip(pSeq);
}

void CPlayer::SetDefaultClipAll(const char* pSeq)
{
	m_pShadow->SetDefaultSeq(pSeq);
	m_pMesh->SetDefaultSeq(pSeq);
	m_pBow->SetDefaultClip(pSeq);
	m_pArrow->SetDefaultClip(pSeq);
}

void CPlayer::SetWorldRotYAll(float fRot)
{
	m_pShadow->SetWorldRotY(fRot);
	m_pMesh->SetWorldRotY(fRot);
	m_pBow->SetWorldRotY(fRot);
	m_pArrow->SetWorldRotY(fRot);
}
