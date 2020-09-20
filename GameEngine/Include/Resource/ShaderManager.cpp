#include "ShaderManager.h"
#include "Shader.h"
#include "GraphicShader.h"
#include "../Device.h"

DEFINITION_SINGLE(CShaderManager)

CShaderManager::CShaderManager()	:
	m_iInputSize(0)
{

}

CShaderManager::~CShaderManager()
{
	SAFE_RELEASE_MAP(m_mapShader);
	SAFE_RELEASE_MAP(m_mapLayout);

	auto iter = m_mapCBuffer.begin();
	auto iterEnd = m_mapCBuffer.end();

	for (; iter != iterEnd; ++iter)
		SAFE_RELEASE(iter->second->pBuffer);

	SAFE_DELETE_MAP(m_mapCBuffer);
}

bool CShaderManager::Init()
{
	//	gloval shader √ ±‚»≠
	CGraphicShader* pShader = CreateShader<CGraphicShader>("Standard2D");

	if (!LoadVertexShader("Standard2D", "StandardVS", TEXT("VertexShader.fx")))
		return false;

	if (!LoadPixelShader("Standard2D", "StandardPS", TEXT("VertexShader.fx")))
		return false;

	pShader->AddInputLayoutDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	pShader->AddInputLayoutDesc("COLOR", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pShader->CreateInputLayout())
	{
		SAFE_RELEASE(pShader);
		return false;
	}

	SAFE_RELEASE(pShader);

	CreateCBuffer("Transform", sizeof(TransformCBuffer), 0);
	CreateCBuffer("Material", sizeof(ShaderCBuffer), 1,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);

	return true;
}

bool CShaderManager::CreateLayout(const std::string& strName)
{
	CShader* pShader = FindShader(strName);

	if (!pShader)
		return false;

	bool bResult = pShader->CreateInputLayout();

	SAFE_RELEASE(pShader);

	return bResult;
}

bool CShaderManager::CreateInputDesc(const std::string& strName, const char* pSemanticName, UINT iSemanticIndex, 
	DXGI_FORMAT eFormat, UINT iInputSlot, UINT iSize, 
	D3D11_INPUT_CLASSIFICATION eInputSlotClass, UINT iInstanceDataStepRate)
{
	CShader* pShader = FindShader(strName);

	if (!pShader)
		return false;

	bool bResult = pShader->AddInputLayoutDesc(pSemanticName, iSemanticIndex,
		eFormat, iInputSlot, iSize, eInputSlotClass, iInstanceDataStepRate);

	SAFE_RELEASE(pShader);

	return bResult;
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

bool CShaderManager::CreateCBuffer(const std::string& strTag, int iSize, int iRegister, int iType)
{
	PCBuffer pBuffer = FindCBuffer(strTag);

	if (pBuffer)
		return false;

	pBuffer = new CBuffer;

	pBuffer->iSize = iSize;
	pBuffer->iType = iType;
	pBuffer->iRegister = iRegister;

	D3D11_BUFFER_DESC tDesc = {};

	tDesc.ByteWidth = iSize;
	tDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	tDesc.Usage = D3D11_USAGE_DYNAMIC;
	tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, nullptr, &pBuffer->pBuffer)))
	{
		SAFE_DELETE(pBuffer);	
		return false;
	}	

	m_mapCBuffer.insert(std::make_pair(strTag, pBuffer));

	return true;
}

bool CShaderManager::UpdateCBuffer(const std::string& strTag, void* pData)
{
	PCBuffer pBuffer = FindCBuffer(strTag);

	if (!pBuffer)
		return false;

	D3D11_MAPPED_SUBRESOURCE tMap = {};

	CONTEXT->Map(pBuffer->pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tMap);

	memcpy(tMap.pData, pData, pBuffer->iSize);	

	CONTEXT->Unmap(pBuffer->pBuffer, 0);

	if (pBuffer->iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX)
		CONTEXT->VSSetConstantBuffers(pBuffer->iRegister, 1, &pBuffer->pBuffer);

	if (pBuffer->iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL)
		CONTEXT->PSSetConstantBuffers(pBuffer->iRegister, 1, &pBuffer->pBuffer);

	if (pBuffer->iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_HULL)
		CONTEXT->HSSetConstantBuffers(pBuffer->iRegister, 1, &pBuffer->pBuffer);

	if (pBuffer->iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_DOMAIN)
		CONTEXT->DSSetConstantBuffers(pBuffer->iRegister, 1, &pBuffer->pBuffer);

	if (pBuffer->iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY)
		CONTEXT->GSSetConstantBuffers(pBuffer->iRegister, 1, &pBuffer->pBuffer);

	if (pBuffer->iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_COMPUTE)
		CONTEXT->CSSetConstantBuffers(pBuffer->iRegister, 1, &pBuffer->pBuffer);

	return true;
}

PCBuffer CShaderManager::FindCBuffer(const std::string& strTag)
{
	std::unordered_map<std::string, PCBuffer>::iterator iter = m_mapCBuffer.find(strTag);

	if (iter == m_mapCBuffer.end())
		return nullptr;

	return iter->second;
}

bool CShaderManager::LoadVertexShader(const std::string& strName, const char* pEntryName, const TCHAR* pFileName, const std::string& strRootPath)
{
	CShader* pShader = FindShader(strName);

	if (!pShader)
		return false;

	else if (pShader->GetShaderType() == SHADER_TYPE::ST_COMPUTE)
	{
		SAFE_RELEASE(pShader);
		return false;
	}

	bool bResult = ((CGraphicShader*)pShader)->LoadVertexShader(pEntryName, pFileName, strRootPath);

	SAFE_RELEASE(pShader);

	return bResult;
}

bool CShaderManager::LoadPixelShader(const std::string& strName, const char* pEntryName, const TCHAR* pFileName, const std::string& strRootPath)
{
	CShader* pShader = FindShader(strName);

	if (!pShader)
		return false;

	else if (pShader->GetShaderType() == SHADER_TYPE::ST_COMPUTE)
	{
		SAFE_RELEASE(pShader);
		return false;
	}

	bool bResult = ((CGraphicShader*)pShader)->LoadPixelShader(pEntryName, pFileName, strRootPath);

	SAFE_RELEASE(pShader);

	return bResult;
}

bool CShaderManager::LoadHullShader(const std::string& strName, const char* pEntryName, const TCHAR* pFileName, const std::string& strRootPath)
{
	CShader* pShader = FindShader(strName);

	if (!pShader)
		return false;

	else if (pShader->GetShaderType() == SHADER_TYPE::ST_COMPUTE)
	{
		SAFE_RELEASE(pShader);
		return false;
	}

	bool bResult = ((CGraphicShader*)pShader)->LoadHullShader(pEntryName, pFileName, strRootPath);

	SAFE_RELEASE(pShader);

	return bResult;
}

bool CShaderManager::LoadGeometryShader(const std::string& strName, const char* pEntryName, const TCHAR* pFileName, const std::string& strRootPath)
{
	CShader* pShader = FindShader(strName);

	if (!pShader)
		return false;

	else if (pShader->GetShaderType() == SHADER_TYPE::ST_COMPUTE)
	{
		SAFE_RELEASE(pShader);
		return false;
	}

	bool bResult = ((CGraphicShader*)pShader)->LoadGeometryShader(pEntryName, pFileName, strRootPath);

	SAFE_RELEASE(pShader);

	return bResult;
}

bool CShaderManager::LoadDomainShader(const std::string& strName, const char* pEntryName, const TCHAR* pFileName, const std::string& strRootPath)
{
	CShader* pShader = FindShader(strName);

	if (!pShader)
		return false;

	else if (pShader->GetShaderType() == SHADER_TYPE::ST_COMPUTE)
	{
		SAFE_RELEASE(pShader);
		return false;
	}

	bool bResult = ((CGraphicShader*)pShader)->LoadDomainShader(pEntryName, pFileName, strRootPath);

	SAFE_RELEASE(pShader);

	return bResult;
}
