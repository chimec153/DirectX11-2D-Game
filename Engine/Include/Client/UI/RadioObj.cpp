#include "RadioObj.h"
#include "UI/Radio.h"
#include "UI/Slider.h"

CRadioObj::CRadioObj()
{
}

CRadioObj::CRadioObj(const CRadioObj& obj)	:
	CObj(obj)
{
}

CRadioObj::~CRadioObj()
{
}

bool CRadioObj::Init()
{
	if (!CObj::Init())
		return false;

	CRadio* pRadio1 = CreateComponent<CRadio>("Radio1", m_pLayer);

	pRadio1->SetTexture(BUTTON_STATE::OFF, "Radio");
	pRadio1->SetTexture(BUTTON_STATE::ON, "Radio");
	pRadio1->SetTexture(BUTTON_STATE::PUSH, "Radio");
	pRadio1->SetColor(BUTTON_STATE::OFF, Vector4::White);
	pRadio1->SetColor(BUTTON_STATE::ON, Vector4::White);
	pRadio1->SetColor(BUTTON_STATE::PUSH, Vector4::White);
	pRadio1->SetWorldScale(50.f, 50.f, 1.f);
	pRadio1->SetPivot(0.5f, 0.5f, 0.f);
	pRadio1->SetWorldPos(100.f, 100.f, 0.f);
	pRadio1->SetSelTexture("RadioSel");

	CSlider* pSlider = CreateComponent<CSlider>("Slider", m_pLayer);

	pRadio1->AddChild(pSlider);

	SAFE_RELEASE(pSlider);

	SetRootComponent(pRadio1);


	CRadio* pRadio2 = CreateComponent<CRadio>("Radio2", m_pLayer);

	pRadio2->SetTexture(BUTTON_STATE::OFF, "Radio");
	pRadio2->SetTexture(BUTTON_STATE::ON, "Radio");
	pRadio2->SetTexture(BUTTON_STATE::PUSH, "Radio");
	pRadio2->SetColor(BUTTON_STATE::OFF, Vector4::White);
	pRadio2->SetColor(BUTTON_STATE::ON, Vector4::White);
	pRadio2->SetColor(BUTTON_STATE::PUSH, Vector4::White);
	pRadio2->SetWorldScale(50.f, 50.f, 1.f);
	pRadio2->SetPivot(0.5f, 0.5f, 0.f);
	pRadio2->SetRelativePos(100.f, 0.f, 0.f);
	pRadio2->SetSelTexture("RadioSel");
	pRadio2->SetInheritScale(false);

	pRadio1->AddChild(pRadio2);

	CRadio* pRadio3 = CreateComponent<CRadio>("Radio3", m_pLayer);

	pRadio3->SetTexture(BUTTON_STATE::OFF, "Radio");
	pRadio3->SetTexture(BUTTON_STATE::ON, "Radio");
	pRadio3->SetTexture(BUTTON_STATE::PUSH, "Radio");
	pRadio3->SetColor(BUTTON_STATE::OFF, Vector4::White);
	pRadio3->SetColor(BUTTON_STATE::ON, Vector4::White);
	pRadio3->SetColor(BUTTON_STATE::PUSH, Vector4::White);
	pRadio3->SetWorldScale(50.f, 50.f, 1.f);
	pRadio3->SetPivot(0.5f, 0.5f, 0.f);
	pRadio3->SetRelativePos(200.f, 0.f, 0.f);
	pRadio3->SetSelTexture("RadioSel");
	pRadio3->SetInheritScale(false);

	pRadio1->AddChild(pRadio3);

	pRadio1->AddGroup(pRadio2);
	pRadio1->AddGroup(pRadio3);

	SAFE_RELEASE(pRadio1);
	SAFE_RELEASE(pRadio2);
	SAFE_RELEASE(pRadio3);

	return true;
}

void CRadioObj::Start()
{
	CObj::Start();
}

void CRadioObj::Update(float fTime)
{
	CObj::Update(fTime);
}

void CRadioObj::PostUpdate(float fTime)
{
	CObj::PostUpdate(fTime);
}

void CRadioObj::Collision(float fTime)
{
	CObj::Collision(fTime);
}

void CRadioObj::PreRender(float fTime)
{
	CObj::PreRender(fTime);
}

void CRadioObj::Render(float fTime)
{
	CObj::Render(fTime);
}

void CRadioObj::PostRender(float fTime)
{
	CObj::PostRender(fTime);
}

CRadioObj* CRadioObj::Clone()
{
	return new CRadioObj(*this);
}

void CRadioObj::Save(FILE * pFile)
{
	CObj::Save(pFile);
}

void CRadioObj::Load(FILE * pFile)
{
	CObj::Load(pFile);
}
