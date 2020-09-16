#include "Material.h"
#include "Shader.h"
#include "ShaderManager.h"

CMaterial::CMaterial()	:
	m_pShader(nullptr)
{
	m_tCBuffer.vDif = Vector4::White;
	m_tCBuffer.vAmp = Vector4::White;
	m_tCBuffer.vSpe = Vector4::White;
}

CMaterial::~CMaterial()
{
	SAFE_RELEASE(m_pShader);
}

void CMaterial::SetShader(const std::string& strName)
{
	SAFE_RELEASE(m_pShader);

	m_pShader = GET_SINGLE(CShaderManager)->FindShader(strName);
}
