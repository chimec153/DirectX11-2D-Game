#include "pch.h"
#include "TitanFont.h"
#include "Scene/Scene.h"
#include "Component/SpriteComponent.h"
#include "Resource/Material.h"
#include "Component/Mesh2DComponent.h"
#include "Resource/ResourceManager.h"

CTitanFont::CTitanFont()	:
	m_iComCount(0)
	, m_strText()
{
}

CTitanFont::CTitanFont(const CTitanFont& obj)	:
	CObj(obj)
	, m_iComCount(obj.m_iComCount)
	, m_strText(obj.m_strText)
{
}

CTitanFont::~CTitanFont()
{
}

void CTitanFont::SetText(const std::string& strText, float fWidth, const Vector4& vColor)
{
	int iCurLength = (int)m_strText.length();
	int iLength = (int)strText.length();

	m_strText = strText;

	int iBigger = iCurLength > iLength ? iCurLength : iLength;

	for (int i = 0; i < iBigger; i++)
	{
		char strNum[32] = {};

		sprintf_s(strNum, "%d", i);

		if (i >= m_iComCount)
		{
			CSpriteComponent* pCom = CreateComponent<CSpriteComponent>(strNum, m_pLayer);

			pCom->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);
			pCom->SetWorldScale(1.f, 1.f, 1.f);
			pCom->SetPivot(0.5f, 0.5f, 0.f);
			pCom->SetRelativePos(i * fWidth - (iLength - 1) * fWidth/2.f, 0.f, 0.f);

			CMaterial* pMtrl = pCom->GetMaterial();

			pMtrl->SetDiffuseColor(vColor);

			SAFE_RELEASE(pMtrl);

			char pText[32] = {};

			strcpy_s(pText, "Titan");

			char strA[3] = {};

			strA[0] = strText[i];

			strcat_s(pText, strA);

			pCom->CreateSprite(pText, pText, LOOP_OPTION::LOOP);

			++m_iComCount;
			m_pRootComponent->AddChild(pCom);
			SAFE_RELEASE(pCom);
		}

		else if (i >= iLength)
		{
			CSpriteComponent* pCom = (CSpriteComponent*)FindSceneComponent(strNum);

			pCom->Enable(false);

			SAFE_RELEASE(pCom);
		}

		else
		{
			CSpriteComponent* pCom = (CSpriteComponent*)FindSceneComponent(strNum);

			char pText[32] = {};

			strcpy_s(pText, "Titan");

			char A[3] = {};

			A[0] = strText[i];

			strcat_s(pText, A);

			pCom->CreateSprite(pText, pText, LOOP_OPTION::LOOP);
			pCom->ChangeSequence(pText);
			pCom->Enable(true);
			pCom->SetRelativePos(i * fWidth - (iLength - 1) * fWidth/2.f, 0.f, 0.f);

			CMaterial* pMtrl = pCom->GetMaterial();

			pMtrl->SetDiffuseColor(vColor);

			SAFE_RELEASE(pMtrl);

			SAFE_RELEASE(pCom);
		}
	}
}

void CTitanFont::SetAlpha(float fAlpha)
{
	size_t iSz = m_strText.length();

	for (size_t i = 0; i < iSz; i++)
	{
		char strNum[32] = {};

		sprintf_s(strNum, "%d", (int)i);

		CSpriteComponent* pCom = (CSpriteComponent*)FindSceneComponent(strNum);

		CMaterial* pMtrl = pCom->GetMaterial();

		Vector4 vColor = pMtrl->GetDif();
		pMtrl->SetDiffuseColor(vColor.x, vColor.y, vColor.z, fAlpha);

		SAFE_RELEASE(pMtrl);

		SAFE_RELEASE(pCom);
	}
}

bool CTitanFont::Init()
{
	if (!CObj::Init())
		return false;

	CMesh2DComponent* pCom = CreateComponent<CMesh2DComponent>("Root", m_pLayer);

	pCom->SetWorldScale(16.f, 16.f, 0.f);

	SetRootComponent(pCom);

	SAFE_RELEASE(pCom);

	return true;
}

void CTitanFont::Start()
{
	CObj::Start();
}

void CTitanFont::Update(float fTime)
{
	CObj::Update(fTime);

}

void CTitanFont::PostUpdate(float fTime)
{
	CObj::PostUpdate(fTime);
}

void CTitanFont::Collision(float fTime)
{
	CObj::Collision(fTime);
}

void CTitanFont::PreRender(float fTime)
{
	CObj::PreRender(fTime);
}

void CTitanFont::Render(float fTime)
{
	CObj::Render(fTime);
}

void CTitanFont::PostRender(float fTime)
{
	CObj::PostRender(fTime);
}

CTitanFont* CTitanFont::Clone()
{
	return new CTitanFont(*this);
}
