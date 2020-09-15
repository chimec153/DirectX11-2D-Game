#include "Shader.h"

CShader::CShader()	:
	m_eType(SHADER_TYPE::ST_COMPUTE),
	m_iInputSize(0),
	m_pInputLayout(nullptr)
{
}

CShader::~CShader()
{
	SAFE_RELEASE(m_pInputLayout);
}

SHADER_TYPE CShader::GetShaderType() const
{
	return m_eType;
}

bool CShader::Init()
{
	return true;
}

bool CShader::AddInputLayoutDesc(const char* pSemanticName, UINT iSemanticIndex,
	DXGI_FORMAT eFormat, UINT iInputSlot, UINT iSize,
	D3D11_INPUT_CLASSIFICATION eInputSlotClass, UINT iInstanceDataStepRate)
{
	D3D11_INPUT_ELEMENT_DESC tDesc = {};

	tDesc.SemanticName = pSemanticName;
	tDesc.SemanticIndex = iSemanticIndex;
	tDesc.Format = eFormat;
	tDesc.InputSlot = iInputSlot;
	tDesc.AlignedByteOffset = m_iInputSize;
	tDesc.InputSlotClass = eInputSlotClass;
	tDesc.InstanceDataStepRate = iInstanceDataStepRate;

	m_vecInputDesc.push_back(tDesc);

	m_iInputSize += iSize;

	return true;
}

bool CShader::CreateInputLayout()
{
	return false;
}

void CShader::SetShader()
{
}
