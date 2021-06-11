#include "UIButton.h"
#include "../Resource/Texture.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Mesh2D.h"
#include "../Resource/Material.h"
#include "../Component/Collider.h"
#include "../Input.h"
#include "../Component/ColliderRect.h"
#include "../Object/Obj.h"
#include "../Component/Sound.h"

CUIButton::CUIButton()	:
	m_eState(BUTTON_STATE::OFF),
	m_pRC(nullptr),
	m_pSnd(nullptr)
{
	m_vecInfo.resize((int)BUTTON_STATE::END);
	m_eSceneComponentClassType = SCENECOMPONENT_CLASS_TYPE::UI_BUTTON;

	m_strSnd[(int)BUTTON_STATE::OFF] = "";
	m_strSnd[(int)BUTTON_STATE::ON] = "";
	m_strSnd[(int)BUTTON_STATE::PUSH] = "";
	m_strSnd[(int)BUTTON_STATE::DISABLE] = "";
}

CUIButton::CUIButton(const CUIButton& ui)	:
	CUIControl(ui)
{
	m_eState = ui.m_eState;

	size_t iSz = ui.m_vecInfo.size();

	for (size_t i = 0; i < iSz; ++i)
	{
		PBUTTONINFO pInfo = new BUTTONINFO;

		*pInfo = *ui.m_vecInfo[i];

		m_vecInfo.push_back(pInfo);
	}

	m_pRC = FindComByType<CColliderRect>();

	m_pSnd = FindComByType<CSound>();
}

CUIButton::~CUIButton()
{
	size_t iSz = m_vecInfo.size();

	for (size_t i = 0; i < iSz; ++i)
	{
		SAFE_RELEASE(m_vecInfo[i]->pTex);
		SAFE_DELETE(m_vecInfo[i]);
	}

	SAFE_RELEASE(m_pRC);
	SAFE_RELEASE(m_pSnd);
}

void CUIButton::SetTexture(BUTTON_STATE eStat, const std::string& strKey)
{
	SAFE_RELEASE(m_vecInfo[(int)eStat]->pTex);

	m_vecInfo[(int)eStat]->pTex = GET_SINGLE(CResourceManager)->FindTexture(strKey);

	if (m_vecInfo[(int)eStat]->pTex)
	{
		m_pRC->SetExtent((float)m_vecInfo[(int)eStat]->pTex->GetWidth(),
			(float)m_vecInfo[(int)eStat]->pTex->GetHeigth());
	}
}

void CUIButton::SetTexture(BUTTON_STATE eStat, CTexture* pTex)
{
	SAFE_RELEASE(m_vecInfo[(int)eStat]->pTex);

	m_vecInfo[(int)eStat]->pTex = pTex;

	if (m_vecInfo[(int)eStat]->pTex)
	{
		m_pRC->SetExtent((float)m_vecInfo[(int)eStat]->pTex->GetWidth(),
			(float)m_vecInfo[(int)eStat]->pTex->GetHeigth());

		m_vecInfo[(int)eStat]->pTex->AddRef();
	}
}

void CUIButton::SetColor(BUTTON_STATE eStat, const Vector4& vClr)
{
	m_vecInfo[(int)eStat]->vClr = vClr;

	//m_pMaterial->SetDiffuseColor(m_vecInfo[(int)eStat]->vClr);
}

void CUIButton::SetColor(BUTTON_STATE eStat, float r, float g, float b, float a)
{
	SetColor(eStat, Vector4(r, g, b, a));

	//m_pMaterial->SetDiffuseColor(Vector4(r, g, b, a));
}

void CUIButton::SetColor(BUTTON_STATE eStat, BYTE r, BYTE g, BYTE b, BYTE a)
{
	SetColor(eStat, Vector4(r /255.f, g / 255.f, b / 255.f, a / 255.f));

	//m_pMaterial->SetDiffuseColor(Vector4(r / 255.f, g / 255.f, b / 255.f, a / 255.f));
}

void CUIButton::SetSound(BUTTON_STATE eStat, const std::string& strKey)
{
	m_strSnd[(int)eStat] = strKey;
}

BUTTON_STATE CUIButton::GetState() const
{
	return m_eState;
}

void CUIButton::SetCallBack(BUTTON_STATE eState, void(*pFunc)(float))
{
	m_vecCallBack[(int)eState].push_back(std::bind(pFunc, std::placeholders::_1));
}

void CUIButton::Call(float fTime)
{
	m_pSnd->SetSound(m_strSnd[(int)m_eState]);

	size_t iSz = m_vecCallBack[(int)m_eState].size();

	for (size_t i = 0; i < iSz; ++i)
		m_vecCallBack[(int)m_eState][i](fTime);
}

bool CUIButton::Init()
{
	if (!CUIControl::Init())
		return false;

	for (int i = 0; i < (int)BUTTON_STATE::END; ++i)
		m_vecInfo[i] = new BUTTONINFO;

	m_pImage = GET_SINGLE(CResourceManager)->GetUIMesh();

	CMaterial* pMat = GET_SINGLE(CResourceManager)->FindMaterial("UI2D");

	//m_pMaterial = pMat->Clone();

	SAFE_RELEASE(pMat);

	m_pRC = m_pObj->CreateComponent<CColliderRect>("ButtonBody", m_pLayer);
	m_pRC->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);
	m_pRC->SetUI();
	m_pRC->SetInheritScale(false);

	m_pRC->SetCallBack<CUIButton>(COLLISION_STATE::INIT, this, &CUIButton::ColEnter);
	m_pRC->SetCallBack<CUIButton>(COLLISION_STATE::STAY, this, &CUIButton::ColStay);
	m_pRC->SetCallBack<CUIButton>(COLLISION_STATE::END, this, &CUIButton::ColEnd);

	AddChild(m_pRC);

	m_pSnd = m_pObj->CreateComponent<CSound>("ButtonSound", m_pLayer);

	m_pSnd->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);

	SetCallBack<CSound>(BUTTON_STATE::PUSH, m_pSnd, &CSound::Play);

	AddChild(m_pSnd);

	return true;
}

void CUIButton::Start()
{
	CUIControl::Start();
}

void CUIButton::Update(float fTime)
{
	CUIControl::Update(fTime);

	Vector3 vScale = GetWorldScale();

	m_pRC->SetExtent(vScale.x, vScale.y);

	if (m_eState == BUTTON_STATE::ON)
	{
		if (GET_SINGLE(CInput)->IsMouseDown(MOUSE_BUTTON_TYPE::LEFT))
		{
			Call(fTime);

			m_eState = BUTTON_STATE::PUSH;

			//m_pMaterial->SetDiffuseColor(m_vecInfo[(int)m_eState]->vClr);
		}			
	}

	else if (m_eState == BUTTON_STATE::PUSH)
	{
		if (GET_SINGLE(CInput)->IsMouseUp(MOUSE_BUTTON_TYPE::LEFT))
		{
			Call(fTime);

			m_eState = BUTTON_STATE::OFF;

			//m_pMaterial->SetDiffuseColor(m_vecInfo[(int)m_eState]->vClr);
		}
	}

	m_pRC->SetZOrder(GetZOrder());
}

void CUIButton::PostUpdate(float fTime)
{
	CUIControl::PostUpdate(fTime);
}

void CUIButton::Collision(float fTime)
{
	CUIControl::Collision(fTime);
}

void CUIButton::PreRender(float fTime)
{
	CUIControl::PreRender(fTime);
}

void CUIButton::Render(float fTime)
{
	CUIControl::Render(fTime);

	//m_pMaterial->SetMaterial();

	if(m_vecInfo[(int)m_eState]->pTex)
	m_vecInfo[(int)m_eState]->pTex->SetTexture(0, (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);

	m_pImage->Render(fTime);
}

void CUIButton::PostRender(float fTime)
{
	CUIControl::PostRender(fTime);
}

CUIButton* CUIButton::Clone()
{
	return new CUIButton(*this);
}

void CUIButton::Save(FILE* pFile)
{
	CUIControl::Save(pFile);
}

void CUIButton::Load(FILE* pFile)
{
	CUIControl::Load(pFile);
}

void CUIButton::ColEnter(CCollider* pSrc, CCollider* pDst, float fTime)
{
	if (pDst->GetName() == "MouseBody")
	{
		if (m_eState == BUTTON_STATE::OFF)
		{
			Call(fTime);

			m_eState = BUTTON_STATE::ON;

			//m_pMaterial->SetDiffuseColor(m_vecInfo[(int)m_eState]->vClr);
		}
			
	}
}

void CUIButton::ColStay(CCollider* pSrc, CCollider* pDst, float fTime)
{
	if (pDst->GetName() == "MouseBody")
	{
		if (m_eState == BUTTON_STATE::OFF)
		{
			Call(fTime);

			m_eState = BUTTON_STATE::ON;

			//m_pMaterial->SetDiffuseColor(m_vecInfo[(int)m_eState]->vClr);
		}
	}
}

void CUIButton::ColEnd(CCollider* pSrc, CCollider* pDst, float fTime)
{
	if (pDst->GetName() == "MouseBody")
	{
		if (m_eState == BUTTON_STATE::ON)
		{
			Call(fTime);

			m_eState = BUTTON_STATE::OFF;

			//m_pMaterial->SetDiffuseColor(m_vecInfo[(int)m_eState]->vClr);
		}
	}
}
