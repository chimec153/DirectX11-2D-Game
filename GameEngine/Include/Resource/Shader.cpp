#include "Shader.h"

CShader::CShader()	:
	m_eType(SHADER_TYPE::ST_COMPUTE),
	m_iInputSize(0),
	m_iInstSize(0),
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

	if (eInputSlotClass == D3D11_INPUT_PER_VERTEX_DATA)
	{
		tDesc.AlignedByteOffset = m_iInputSize;
		m_iInputSize += iSize;
	}

	else
	{
		tDesc.AlignedByteOffset = m_iInstSize;
		m_iInstSize += iSize;
	}
		
	tDesc.InputSlotClass = eInputSlotClass;
	tDesc.InstanceDataStepRate = iInstanceDataStepRate;

	m_vecInputDesc.push_back(tDesc);

	return true;
}

bool CShader::CreateInputLayout()
{
	return false;
}

void CShader::SetShader()
{
}
