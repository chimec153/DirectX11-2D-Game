#include "GraphicShader.h"
#include "../PathManager.h"
#include "../Device.h"

CGraphicShader::CGraphicShader() :
	m_pVS(nullptr),
	m_pVSBlob(nullptr),
	m_pPS(nullptr),
	m_pPSBlob(nullptr),
	m_pHS(nullptr),
	m_pHSBlob(nullptr),
	m_pGS(nullptr),
	m_pGSBlob(nullptr),
	m_pDS(nullptr),
	m_pDSBlob(nullptr)
{
	m_eType = SHADER_TYPE::ST_GRAPHIC;
}

CGraphicShader::~CGraphicShader()
{
	SAFE_RELEASE(m_pVS);
	SAFE_RELEASE(m_pVSBlob);
	SAFE_RELEASE(m_pPS);
	SAFE_RELEASE(m_pPSBlob);
	SAFE_RELEASE(m_pHS);
	SAFE_RELEASE(m_pHSBlob);
	SAFE_RELEASE(m_pGS);
	SAFE_RELEASE(m_pGSBlob);
	SAFE_RELEASE(m_pDS);
	SAFE_RELEASE(m_pDSBlob);
}

bool CGraphicShader::LoadVertexShader(const char* pEntryName, const TCHAR* pFileName, const std::string& strRootName)
{
	SAFE_RELEASE(m_pVS);
	SAFE_RELEASE(m_pVSBlob);

	unsigned int iFlag = 0;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif

	TCHAR strFullPath[MAX_PATH] = {};

	const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(strRootName);

	if (pPath)
		lstrcpy(strFullPath, pPath);

	lstrcat(strFullPath, pFileName);

	ID3DBlob* pError = nullptr;

	if(FAILED(D3DCompileFromFile(strFullPath, 0,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntryName, "vs_5_0",
		iFlag, 0, &m_pVSBlob, &pError)))
	{
		OutputDebugStringA((char*)pError->GetBufferPointer());
		return false;
	}

	if (FAILED(DEVICE->CreateVertexShader(m_pVSBlob->GetBufferPointer(), m_pVSBlob->GetBufferSize(), nullptr, &m_pVS)))
		return false;

	return true;
}

bool CGraphicShader::LoadPixelShader(const char* pEntryName, const TCHAR* pFileName, const std::string& strRootName)
{
	SAFE_RELEASE(m_pPS);
	SAFE_RELEASE(m_pPSBlob);

	unsigned int iFlag = 0;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif

	TCHAR strFullPath[MAX_PATH] = {};

	const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(strRootName);

	if (pPath)
		lstrcpy(strFullPath, pPath);

	lstrcat(strFullPath, pFileName);

	ID3DBlob* pError = nullptr;

	if (FAILED(D3DCompileFromFile(strFullPath, 0,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntryName, "ps_5_0",
		iFlag, 0, &m_pPSBlob, &pError)))
	{
		OutputDebugStringA((char*)pError->GetBufferPointer());
		return false;
	}

	if (FAILED(DEVICE->CreatePixelShader(m_pPSBlob->GetBufferPointer(), m_pPSBlob->GetBufferSize(), nullptr, &m_pPS)))
		return false;

	return true;
}

bool CGraphicShader::LoadHullShader(const char* pEntryName, const TCHAR* pFileName, const std::string& strRootName)
{
	SAFE_RELEASE(m_pHS);
	SAFE_RELEASE(m_pHSBlob);

	unsigned int iFlag = 0;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif

	TCHAR strFullPath[MAX_PATH] = {};

	const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(strRootName);

	if (pPath)
		lstrcpy(strFullPath, pPath);

	lstrcat(strFullPath, pFileName);

	ID3DBlob* pError = nullptr;

	if (FAILED(D3DCompileFromFile(strFullPath, 0,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntryName, "hs_5_0",
		iFlag, 0, &m_pHSBlob, &pError)))
	{
		OutputDebugStringA((char*)pError->GetBufferPointer());
		return false;
	}

	if (FAILED(DEVICE->CreateHullShader(m_pHSBlob->GetBufferPointer(), m_pHSBlob->GetBufferSize(), nullptr, &m_pHS)))
		return false;

	return true;
}

bool CGraphicShader::LoadGeometryShader(const char* pEntryName, const TCHAR* pFileName, const std::string& strRootName)
{
	SAFE_RELEASE(m_pGS);
	SAFE_RELEASE(m_pGSBlob);

	unsigned int iFlag = 0;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif

	TCHAR strFullPath[MAX_PATH] = {};

	const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(strRootName);

	if (pPath)
		lstrcpy(strFullPath, pPath);

	lstrcat(strFullPath, pFileName);

	ID3DBlob* pError = nullptr;

	if (FAILED(D3DCompileFromFile(strFullPath, 0,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntryName, "gs_5_0",
		iFlag, 0, &m_pGSBlob, &pError)))
	{
		OutputDebugStringA((char*)pError->GetBufferPointer());
		return false;
	}

	if (FAILED(DEVICE->CreateGeometryShader(m_pGSBlob->GetBufferPointer(), m_pGSBlob->GetBufferSize(), nullptr, &m_pGS)))
		return false;
	
	return true;
}

bool CGraphicShader::LoadGeometryShaderWithSO(const char* pEntryName, D3D11_SO_DECLARATION_ENTRY* pEntry,
	UINT iNumEntries, UINT iStride, UINT iNumStrides,
	const TCHAR* pFileName, const std::string& strRootName)
{
	SAFE_RELEASE(m_pGS);
	SAFE_RELEASE(m_pGSBlob);

	unsigned int iFlag = 0;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif

	TCHAR strFullPath[MAX_PATH] = {};

	const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(strRootName);

	if (pPath)
		lstrcpy(strFullPath, pPath);

	lstrcat(strFullPath, pFileName);

	ID3DBlob* pError = nullptr;

	if (FAILED(D3DCompileFromFile(strFullPath, 0,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntryName, "gs_5_0",
		iFlag, 0, &m_pGSBlob, &pError)))
	{
		OutputDebugStringA((char*)pError->GetBufferPointer());
		return false;
	}

	if(FAILED(DEVICE->CreateGeometryShaderWithStreamOutput(m_pGSBlob->GetBufferPointer(),
		m_pGSBlob->GetBufferSize(), pEntry, iNumEntries, &iStride, iNumStrides, 
		D3D11_SO_NO_RASTERIZED_STREAM, nullptr, &m_pGS)))
	{
		return false;
	}

	return true;
}

bool CGraphicShader::LoadDomainShader(const char* pEntryName, const TCHAR* pFileName, const std::string& strRootName)
{
	SAFE_RELEASE(m_pDS);
	SAFE_RELEASE(m_pDSBlob);

	unsigned int iFlag = 0;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif

	TCHAR strFullPath[MAX_PATH] = {};

	const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(strRootName);

	if (pPath)
		lstrcpy(strFullPath, pPath);

	lstrcat(strFullPath, pFileName);

	ID3DBlob* pError = nullptr;

	if (FAILED(D3DCompileFromFile(strFullPath, 0,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntryName, "ds_5_0",
		iFlag, 0, &m_pDSBlob, &pError)))
	{
		OutputDebugStringA((char*)pError->GetBufferPointer());
		return false;
	}

	if (FAILED(DEVICE->CreateDomainShader(m_pDSBlob->GetBufferPointer(), m_pDSBlob->GetBufferSize(), nullptr, &m_pDS)))
		return false;

	return true;
}


bool CGraphicShader::CreateInputLayout()
{
	if (FAILED(DEVICE->CreateInputLayout(&m_vecInputDesc[0], (UINT)m_vecInputDesc.size(),
		m_pVSBlob->GetBufferPointer(), m_pVSBlob->GetBufferSize(), &m_pInputLayout)))
		return false;

	return true;
}

void CGraphicShader::SetShader()
{
	CONTEXT->VSSetShader(m_pVS, nullptr, 0);
	CONTEXT->HSSetShader(m_pHS, nullptr, 0);
	CONTEXT->GSSetShader(m_pGS, nullptr, 0);
	CONTEXT->DSSetShader(m_pDS, nullptr, 0);
	CONTEXT->PSSetShader(m_pPS, nullptr, 0);

	CONTEXT->IASetInputLayout(m_pInputLayout);
}

void CGraphicShader::ResetShader(SHADER_CBUFFER_TYPE eType)
{
	if ((int)eType & (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX)
	{
		CONTEXT->VSSetShader(nullptr, nullptr, 0);
	}

	if ((int)eType & (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL)
	{
		CONTEXT->PSSetShader(nullptr, nullptr, 0);
	}

	if ((int)eType & (int)SHADER_CBUFFER_TYPE::CBUFFER_HULL)
	{
		CONTEXT->HSSetShader(nullptr, nullptr, 0);
	}

	if ((int)eType & (int)SHADER_CBUFFER_TYPE::CBUFFER_DOMAIN)
	{
		CONTEXT->DSSetShader(nullptr, nullptr, 0);
	}

	if ((int)eType & (int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY)
	{
		CONTEXT->GSSetShader(nullptr, nullptr, 0);
	}
}
