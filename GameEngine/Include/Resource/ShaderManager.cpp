#include "ShaderManager.h"
#include "Shader.h"

DEFINITION_SINGLE(CShaderManager)

CShaderManager::CShaderManager()
{

}

CShaderManager::~CShaderManager()
{
	SAFE_RELEASE_MAP(m_mapShader);
	SAFE_RELEASE_MAP(m_mapLayout);
}

bool CShaderManager::Init()
{
	return true;
}

bool CShaderManager::CreateLayout(const std::string strName, const std::string strShaderName)
{
	ID3D11InputLayout* pLayout = FindLayout(strName);

	if (pLayout)
	{
		SAFE_RELEASE(pLayout);
		return false;
	}	

	CShader* pShader = FindShader(strShaderName);

	return true;
}

bool CShaderManager::CreateInputDesc(const char* pSemanticName, UINT iSemanticIndex, 
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

CShader* CShaderManager::FindShader(const std::string& strName)
{
	std::unordered_map<std::string, CShader*>::iterator iter = m_mapShader.find(strName);

	if (iter == m_mapShader.end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}

void CShaderManager::ReleaseShader(const std::string& strName)
{
	std::unordered_map<std::string, CShader*>::iterator iter = m_mapShader.find(strName);

	if (iter == m_mapShader.end())
		return;

	if (iter->second->Release() == 0)
		m_mapShader.erase(iter);
}

ID3D11InputLayout* CShaderManager::FindLayout(const std::string& strName)
{
	std::unordered_map<std::string, ID3D11InputLayout*>::iterator iter = m_mapLayout.find(strName);

	if (iter == m_mapLayout.end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}

void CShaderManager::ReleaseLayout(const std::string& strName)
{
	std::unordered_map<std::string, ID3D11InputLayout*>::iterator iter = m_mapLayout.find(strName);

	if (iter == m_mapLayout.end())
		return;

	iter->second->Release();
	m_mapLayout.erase(iter);
}
