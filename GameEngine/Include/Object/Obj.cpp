#include "Obj.h"

CObj::CObj()	:
	m_bStart(false),
	m_pScene(nullptr)
{
}

CObj::CObj(const CObj& obj)
{
	*this = obj;

	m_iRef = 1;
}

CObj::~CObj()
{
}

bool CObj::Init()
{
	return true;
}

void CObj::Start()
{
	m_bStart = true;
}

void CObj::Input(float fTime)
{
}

void CObj::Update(float fTime)
{
}

void CObj::PostUpdate(float fTime)
{
}

void CObj::Collision(float fTime)
{
}

void CObj::PreRender(float fTime)
{
}

void CObj::Render(float fTime)
{
}

void CObj::PostRender(float fTime)
{
}
