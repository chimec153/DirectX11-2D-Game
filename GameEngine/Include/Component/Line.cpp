#include "Line.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Mesh.h"
#include "../Resource/ShaderManager.h"

CLine::CLine()	:
	m_fAngle(0.f)
	, m_fAngleSpeed(45.f)
	, m_tCBuffer()
{
	m_eSceneComponentClassType = SCENECOMPONENT_CLASS_TYPE::LINE;
}

CLine::CLine(const CLine& com)	:
	CSceneComponent(com)
	, m_fAngle(com.m_fAngle)
	, m_fAngleSpeed(com.m_fAngleSpeed)
	, m_tCBuffer(com.m_tCBuffer)
{
}

CLine::~CLine()
{
}

void CLine::SetAngleSpeed(float fSpeed)
{
	m_fAngleSpeed = fSpeed;
}

void CLine::SetAngle(float fAngle)
{
	m_fAngle = fAngle;
}

bool CLine::Init()
{
	if (!CSceneComponent::Init())
		return false;

	SetMesh("Line");

	SetMaterial("Line");

	SetShader("LineShader");

	return true;
}

void CLine::Start()
{
	CSceneComponent::Start();
}

void CLine::Update(float fTime)
{
	CSceneComponent::Update(fTime);

	m_fAngle += fTime * m_fAngleSpeed;

	if (m_fAngle > 180.f)
	{
		m_fAngle -= 360.f;
	}

	else if (m_fAngle < -180.f)
	{
		m_fAngle += 360.f;
	}

	m_tCBuffer.fAngle = m_fAngle;
}

void CLine::PostUpdate(float fTime)
{
	CSceneComponent::PostUpdate(fTime);
}

void CLine::Collision(float fTime)
{
	CSceneComponent::Collision(fTime);
}

void CLine::PreRender(float fTime)
{
	CSceneComponent::PreRender(fTime);
}

void CLine::Render(float fTime)
{
	GET_SINGLE(CShaderManager)->UpdateCBuffer("Angle", &m_tCBuffer);

	CSceneComponent::Render(fTime);
}

void CLine::PostRender(float fTime)
{
	CSceneComponent::PostRender(fTime);
}

CLine* CLine::Clone()
{
	return new CLine(*this);
}

void CLine::Save(FILE* pFile)
{
	CSceneComponent::Save(pFile);
}

void CLine::Load(FILE* pFile)
{
	CSceneComponent::Load(pFile);
}
