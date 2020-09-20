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

CMaterial::CMaterial(const CMaterial& material)
{
	*this = material;

	m_iRef = 1;

	if (m_pShader)
		m_pShader->AddRef();
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

void CMaterial::SetDiffuseColor(const Vector4& v)
{
	m_tCBuffer.vDif = v;
}

void CMaterial::SetDiffuseColor(float x, float y, float z, float w)
{
	SetDiffuseColor(Vector4(x, y, z, w));
}

void CMaterial::SetAmbientColor(const Vector4& v)
{
	m_tCBuffer.vAmp = v;
}

void CMaterial::SetAmbientColor(float x, float y, float z, float w)
{
	SetAmbientColor(Vector4(x, y, z, w));
}

void CMaterial::SetSpecularColor(const Vector4 & v)
{
	m_tCBuffer.vSpe = v;
}

void CMaterial::SetSpecularColor(float x, float y, float z, float w)
{
	SetSpecularColor(Vector4(x, y, z, w));
}

void CMaterial::SetMaterial()
{
	if (m_pShader)
		m_pShader->SetShader();

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Material", &m_tCBuffer);
}

CMaterial* CMaterial::Clone()
{
	return new CMaterial(*this);
}
