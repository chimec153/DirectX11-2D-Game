#include "Slider.h"
#include "UIButton.h"
#include "../Component/ColliderRect.h"
#include "../Object/Obj.h"
#include "../Input.h"
#include "../Device.h"

CSlider::CSlider()	:
	m_pTopBtn(nullptr),
	m_pBotBtn(nullptr),
	m_pBackBtn(nullptr),
	m_pSlider(nullptr)
{
}

CSlider::CSlider(const CSlider& ui)	:
	CUIControl(ui)
{
	
}

CSlider::~CSlider()
{
	SAFE_RELEASE(m_pTopBtn);
	SAFE_RELEASE(m_pBotBtn);
	SAFE_RELEASE(m_pBackBtn);
	SAFE_RELEASE(m_pSlider);
}

void CSlider::SetMax(float fMax)
{
	m_pBackBtn->SetWorldScale(20.f, fMax , 1.f);
}

float CSlider::GetPercent() const
{
	return m_fPercent;
}

bool CSlider::Init()
{
	if (!CUIControl::Init())
		return false;

	m_pTopBtn = m_pObj->CreateComponent<CUIButton>("TopButton", m_pLayer);

	m_pTopBtn->SetTexture(BUTTON_STATE::OFF, "TopButton");
	m_pTopBtn->SetTexture(BUTTON_STATE::ON, "TopButton");
	m_pTopBtn->SetTexture(BUTTON_STATE::PUSH, "TopButton");
	m_pTopBtn->SetColor(BUTTON_STATE::OFF, Vector4::White);
	m_pTopBtn->SetColor(BUTTON_STATE::ON, Vector4::White);
	m_pTopBtn->SetColor(BUTTON_STATE::PUSH, Vector4::White);
	m_pTopBtn->SetCallBack<CSlider>(BUTTON_STATE::PUSH, this, &CSlider::Up);

	m_pTopBtn->SetWorldScale(20.f, 20.f, 1.f);
	m_pTopBtn->SetPivot(0.5f, 0.5f, 0.f);
	m_pTopBtn->SetInheritScale(false);

	m_pBotBtn = m_pObj->CreateComponent<CUIButton>("BotButton", m_pLayer);

	m_pBotBtn->SetTexture(BUTTON_STATE::OFF, "BotButton");
	m_pBotBtn->SetTexture(BUTTON_STATE::ON, "BotButton");
	m_pBotBtn->SetTexture(BUTTON_STATE::PUSH, "BotButton");
	m_pBotBtn->SetColor(BUTTON_STATE::OFF, Vector4::White);
	m_pBotBtn->SetColor(BUTTON_STATE::ON, Vector4::White);
	m_pBotBtn->SetColor(BUTTON_STATE::PUSH, Vector4::White);
	m_pBotBtn->SetCallBack<CSlider>(BUTTON_STATE::PUSH, this, &CSlider::Down);

	m_pBotBtn->SetWorldScale(20.f, 20.f, 1.f);
	m_pBotBtn->SetPivot(0.5f, 0.5f, 0.f);
	m_pBotBtn->SetInheritScale(false);

	m_pBackBtn = m_pObj->CreateComponent<CUIButton>("BackButton", m_pLayer);

	m_pBackBtn->SetTexture(BUTTON_STATE::OFF, "SliderBack");
	m_pBackBtn->SetTexture(BUTTON_STATE::ON, "SliderBack");
	m_pBackBtn->SetTexture(BUTTON_STATE::PUSH, "SliderBack");
	m_pBackBtn->SetColor(BUTTON_STATE::OFF, Vector4::White);
	m_pBackBtn->SetColor(BUTTON_STATE::ON, Vector4::White);
	m_pBackBtn->SetColor(BUTTON_STATE::PUSH, Vector4::White);
	m_pBackBtn->SetZOrder(1);
	m_pBackBtn->SetCallBack<CSlider>(BUTTON_STATE::PUSH, this, &CSlider::Jump);

	m_pBackBtn->SetWorldScale(20.f, 160.f, 1.f);
	m_pBackBtn->SetPivot(0.5f, 0.5f, 0.f);
	m_pBackBtn->SetInheritScale(false);

	m_pSlider = m_pObj->CreateComponent<CUIButton>("Slider", m_pLayer);

	m_pSlider->SetTexture(BUTTON_STATE::OFF, "Slider");
	m_pSlider->SetTexture(BUTTON_STATE::ON, "Slider");
	m_pSlider->SetTexture(BUTTON_STATE::PUSH, "Slider");
	m_pSlider->SetColor(BUTTON_STATE::OFF, Vector4::White);
	m_pSlider->SetColor(BUTTON_STATE::ON, Vector4::White);
	m_pSlider->SetColor(BUTTON_STATE::PUSH, Vector4::White);

	m_pSlider->SetWorldScale(20.f, 40.f, 1.f);
	m_pSlider->SetPivot(0.5f, 0.5f, 0.f);
	m_pSlider->SetInheritScale(false);

	m_pBackBtn->AddChild(m_pSlider);
	m_pBackBtn->AddChild(m_pBotBtn);
	m_pBackBtn->AddChild(m_pTopBtn);

	AddChild(m_pBackBtn);

	return true;
}

void CSlider::Start()
{
	CUIControl::Start();
}

void CSlider::Update(float fTime)
{
	CUIControl::Update(fTime);

	BUTTON_STATE eState = m_pSlider->GetState();

	if (eState == BUTTON_STATE::PUSH)
	{
		DIMOUSESTATE tState = GET_SINGLE(CInput)->GetMouseState();

		Vector2 vRatio = RATIO;

		m_pSlider->AddWorldPos(0.f, -tState.lY * vRatio.y, 0.f);
	}

	if (m_pTopBtn->GetWorldPos().y - m_pTopBtn->GetWorldScale().y / 2.f <
		m_pSlider->GetWorldPos().y + m_pSlider->GetWorldScale().y / 2.f)
		m_pSlider->SetRelativePos(0.f, m_pTopBtn->GetRelativePos().y - m_pTopBtn->GetWorldScale().y / 2.f - m_pSlider->GetWorldScale().y /2.f, 0.f);

	if (m_pBotBtn->GetWorldPos().y + m_pBotBtn->GetWorldScale().y / 2.f >
		m_pSlider->GetWorldPos().y - m_pSlider->GetWorldScale().y / 2.f)
		m_pSlider->SetRelativePos(0.f, m_pBotBtn->GetRelativePos().y + m_pBotBtn->GetWorldScale().y / 2.f + m_pSlider->GetWorldScale().y / 2.f, 0.f);

	m_fPercent = (-m_pSlider->GetRelativePos().y) / (m_pBackBtn->GetWorldScale().y - m_pSlider->GetWorldScale().y) + 0.5f;

	m_pTopBtn->SetRelativePos(0.f, m_pBackBtn->GetWorldScale().y / 2.f + 10.f, 0.f);
	m_pBotBtn->SetRelativePos(0.f, -m_pBackBtn->GetWorldScale().y / 2.f - 10.f, 0.f);
}

void CSlider::PostUpdate(float fTime)
{
	CUIControl::PostUpdate(fTime);
}

void CSlider::Collision(float fTime)
{
	CUIControl::Collision(fTime);
}

void CSlider::PreRender(float fTime)
{
	CUIControl::PreRender(fTime);
}

void CSlider::Render(float fTime)
{
	CUIControl::Render(fTime);
}

void CSlider::PostRender(float fTime)
{
	CUIControl::PostRender(fTime);
}

CSlider* CSlider::Clone()
{
	return new CSlider(*this);
}

void CSlider::Save(FILE * pFile)
{
	CUIControl::Save(pFile);
}

void CSlider::Load(FILE * pFile)
{
	CUIControl::Load(pFile);
}

void CSlider::Up(float fTime)
{
	m_pSlider->AddRelativePos(0.f, 10.f, 0.f);
}

void CSlider::Down(float fTime)
{
	m_pSlider->AddRelativePos(0.f, -10.f, 0.f);
}

void CSlider::Jump(float fTime)
{
	Vector2 vMousePos = GET_SINGLE(CInput)->GetMousePos();

	Vector2 vRatio = RATIO;

	vMousePos *= vRatio;

	if (vMousePos.y > m_pSlider->GetWorldPos().y)
		m_pSlider->AddRelativePos(0.f, 50.f, 0.f);

	else
		m_pSlider->AddRelativePos(0.f, -50.f, 0.f);
}
