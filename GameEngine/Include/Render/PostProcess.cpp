#include "PostProcess.h"
#include "../Component/SceneComponent.h"

CPostProcess::CPostProcess()	:
	m_pCom(nullptr)
{
}

CPostProcess::CPostProcess(const CPostProcess& pro)	:
	CRef(pro)
	, m_pCom(pro.m_pCom)
{
}

CPostProcess::~CPostProcess()
{
}

bool CPostProcess::Init()
{
	return true;
}

void CPostProcess::Start()
{
}

void CPostProcess::Update(float fTime)
{
}

void CPostProcess::PostUpdate(float fTime)
{
}

void CPostProcess::Collision(float fTime)
{
}

void CPostProcess::PreRender(float fTime)
{
}

void CPostProcess::Render(float fTime)
{
}

void CPostProcess::PostRender(float fTime)
{
}

CPostProcess* CPostProcess::Clone()
{
	return nullptr;
}

void CPostProcess::Save(FILE* pFile)
{
}

void CPostProcess::Load(FILE* pFile)
{
}
