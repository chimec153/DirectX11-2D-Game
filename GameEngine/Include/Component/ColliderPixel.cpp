#include "ColliderPixel.h"
#include "../Resource/ResourceManager.h"
#include "../Collision/Collision.h"
#include "../PathManager.h"
#include "ColliderRect.h"
#include "ColliderOBB2D.h"
#include "ColliderCircle.h"
#include "../Resource/Mesh2D.h"
#include "../Resource/Material.h"
#include "../Component/Transform.h"

CColliderPixel::CColliderPixel()
{
	m_eColType = COLLIDER_TYPE::CT_PIXEL;
	m_eSceneComponentClassType = SCENECOMPONENT_CLASS_TYPE::COLLIDERPIXEL;
	m_eSceneComponentType = SCENE_COMPONENT_TYPE::SCT_2D;
	m_b2D = true;

#ifdef _DEBUG
	m_pPixel = nullptr;
#endif
}

CColliderPixel::CColliderPixel(const CColliderPixel& col)	:
	CCollider(col)
{
	m_tInfo = col.m_tInfo;

	memcpy(m_tInfo.pPixel, col.m_tInfo.pPixel, m_tInfo.iWidth * m_tInfo.iHeight * sizeof(Color));

#ifdef _DEBUG
	m_pPixel = col.m_pPixel;

	if (m_pPixel)
		m_pPixel->AddRef();
#endif
}

CColliderPixel::~CColliderPixel()
{
#ifdef _DEBUG
	SAFE_RELEASE(m_pPixel);
#endif
}

const PixelInfo& CColliderPixel::GetInfo() const
{
	return m_tInfo;
}

void CColliderPixel::SetPixel(const TCHAR* pFileName, const std::string& strPathName)
{
	TCHAR strFullPath[MAX_PATH] = {};

	const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(strPathName);

	if (pPath)
		lstrcpy(strFullPath, pPath);

	lstrcat(strFullPath, pFileName);

	SetPixelFromFullPath(strFullPath);
}

void CColliderPixel::SetPixelFromFullPath(const TCHAR* pFullPath)
{
	TCHAR strExt[_MAX_EXT] = {};

	_wsplitpath_s(pFullPath, nullptr, 0, nullptr, 0, nullptr, 0, strExt, _MAX_EXT);

	char pExt[_MAX_EXT] = {};

	WideCharToMultiByte(CP_ACP, 0, strExt, -1, pExt, lstrlen(strExt), NULL, NULL);

	_strupr_s(pExt);

	ScratchImage* pImage = new ScratchImage;

	if (strcmp(pExt, ".DDS") == 0)
	{
		if (FAILED(LoadFromDDSFile(pFullPath, DDS_FLAGS_NONE, nullptr, *pImage)))
		{
			SAFE_DELETE(pImage);
			return;
		}
			
	}

	else if (strcmp(pExt, ".TGA") == 0)
	{
		if (FAILED(LoadFromTGAFile(pFullPath, nullptr, *pImage)))
		{
			SAFE_DELETE(pImage);	
			return;
		}
		
	}

	else
	{
		if (FAILED(LoadFromWICFile(pFullPath, WIC_FLAGS_NONE, nullptr, *pImage)))
		{
			SAFE_DELETE(pImage);
			return;
		}
	}

	int iSize = (int)pImage->GetPixelsSize();

	m_tInfo.iHeight = (int)pImage->GetImages()[0].height;
	m_tInfo.iWidth = (int)pImage->GetImages()[0].width;

	if (iSize / 4 != m_tInfo.iWidth * m_tInfo.iHeight)
		return;

	m_tInfo.pPixel = new Color[iSize / 4];

	memcpy(m_tInfo.pPixel, pImage->GetPixels(), iSize);

	if (strcmp(pExt, ".BMP") == 0)
	{
		PColor pColor = new Color[m_tInfo.iWidth];

		for (int i = 0; i < m_tInfo.iHeight / 2; ++i)
		{
			memcpy(pColor, m_tInfo.pPixel + size_t(i)* m_tInfo.iWidth, m_tInfo.iWidth * sizeof(Color));
			memcpy(m_tInfo.pPixel + size_t(i) * m_tInfo.iWidth, m_tInfo.pPixel + (size_t(m_tInfo.iHeight) - 1 - size_t(i)) * m_tInfo.iWidth, m_tInfo.iWidth * sizeof(Color));
			memcpy(m_tInfo.pPixel + (size_t(m_tInfo.iHeight) - 1 - size_t(i)) * m_tInfo.iWidth, pColor, m_tInfo.iWidth * sizeof(Color));
		}

		delete[] pColor;
	}

#ifdef _DEBUG
	CMaterial* pMat = m_pPixel->GetMaterial();

	pMat->SetTextureFromFullPath(REGISTER_TYPE::RT_DIF, "Pixel", pFullPath);

	SAFE_RELEASE(pMat);
#endif

	delete pImage;
}

void CColliderPixel::SetChkColor(unsigned char r, unsigned char g, unsigned char b)
{
	m_tInfo.tChkColor.r = r;
	m_tInfo.tChkColor.g = g;
	m_tInfo.tChkColor.b = b;

	m_tInfo.eType = PIXEL_COLLISION_TYPE::COLOR_CONFIRM;
}

void CColliderPixel::SetAlpha(unsigned char a)
{
	m_tInfo.tChkColor.a = a;

	m_tInfo.eType = PIXEL_COLLISION_TYPE::LESS_EQUAL_ALPHA;
}

void CColliderPixel::SetIgnColor(unsigned char r, unsigned char g, unsigned char b)
{
	m_tInfo.tColor.r = r;
	m_tInfo.tColor.g = g;
	m_tInfo.tColor.b = b;

	m_tInfo.eType = PIXEL_COLLISION_TYPE::COLOR_IGN;
}

bool CColliderPixel::Init()
{
	if (!CCollider::Init())
		return false;

#ifdef _DEBUG
	m_pMesh = GET_SINGLE(CResourceManager)->FindMesh("Collider2D");
	m_pPixel = GET_SINGLE(CResourceManager)->GetDefaultMesh();
#endif

	SetInheritRotX(false);
	SetInheritRotY(false);
	SetInheritRotZ(false);

	return true;
}

void CColliderPixel::Start()
{
	CCollider::Start();
}

void CColliderPixel::Update(float fTime)
{
	CCollider::Update(fTime);
}

void CColliderPixel::PostUpdate(float fTime)
{
	CCollider::PostUpdate(fTime);

	Vector3 vPos = GetWorldPos();

	m_tInfo.tRI.fL = vPos.x - m_tInfo.iWidth / 2.f;
	m_tInfo.tRI.fR = vPos.x + m_tInfo.iWidth / 2.f;
	m_tInfo.tRI.fB = vPos.y - m_tInfo.iHeight / 2.f;
	m_tInfo.tRI.fT = vPos.y + m_tInfo.iHeight / 2.f;

	m_vMin.x = m_tInfo.tRI.fL;
	m_vMax.x = m_tInfo.tRI.fR;
	m_vMin.y = m_tInfo.tRI.fB;
	m_vMax.y = m_tInfo.tRI.fT;

	SetWorldScale((float)m_tInfo.iWidth, (float)m_tInfo.iHeight, 0.f);
}

void CColliderPixel::Collision(float fTime)
{
	CCollider::Collision(fTime);
}

void CColliderPixel::PreRender(float fTime)
{
	CCollider::PreRender(fTime);
}

void CColliderPixel::Render(float fTime)
{
	CCollider::Render(fTime);

#ifdef _DEBUG
	CMaterial* pMat = m_pPixel->GetMaterial();

	pMat->SetMaterial();

	SAFE_RELEASE(pMat);

	m_pPixel->Render(fTime);
#endif
}

void CColliderPixel::PostRender(float fTime)
{
	CCollider::PostRender(fTime);
}

CColliderPixel* CColliderPixel::Clone()
{
	return new CColliderPixel(*this);
}

void CColliderPixel::Save(FILE* pFile)
{
	CCollider::Save(pFile);
}

void CColliderPixel::Load(FILE* pFile)
{
	CCollider::Load(pFile);
}

bool CColliderPixel::Collision(CCollider* pDest)
{
	switch (pDest->GetColType())
	{
	case COLLIDER_TYPE::CT_RECT:
		return CCollision::CollisionPixelToRect((CColliderPixel*)this, (CColliderRect*)pDest);
	case COLLIDER_TYPE::CT_SPHERE:
		return CCollision::CollisionPixelToCircle((CColliderPixel*)this, (CColliderCircle*)pDest);
	case COLLIDER_TYPE::CT_OBB:
		return CCollision::CollisionPixelToOBB2D((CColliderPixel*)this, (CColliderOBB2D*)pDest);
	case COLLIDER_TYPE::CT_POINT:
		break;
	case COLLIDER_TYPE::CT_LINE:
		break;
	}

	return false;
}
