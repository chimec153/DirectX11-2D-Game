#include "Shader.h"

CShader::CShader()	:
	m_pVS(nullptr),
	m_pVBBlob(nullptr),
	m_pPS(nullptr),
	m_pIBBlob(nullptr)
{
}

CShader::~CShader()
{
	SAFE_RELEASE(m_pVS);
	SAFE_RELEASE(m_pVBBlob);
	SAFE_RELEASE(m_pPS);
	SAFE_RELEASE(m_pIBBlob);
}

bool CShader::Init()
{
	return true;
}
