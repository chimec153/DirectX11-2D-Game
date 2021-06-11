#include "ComputeShader.h"
#include "../PathManager.h"
#include "../Device.h"

CComputeShader::CComputeShader()	:
	m_pCS(nullptr),
	m_pCSBlob(nullptr)
{
	m_eType = SHADER_TYPE::ST_COMPUTE;
}

CComputeShader::~CComputeShader()
{
	SAFE_RELEASE(m_pCS);
	SAFE_RELEASE(m_pCSBlob);
}

bool CComputeShader::LoadComputeShader(const char* pEntryName, const TCHAR* pFileName, const std::string& strRootName)
{
	SAFE_RELEASE(m_pCS);
	SAFE_RELEASE(m_pCSBlob);

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
		D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntryName, "cs_5_0",
		iFlag, 0, &m_pCSBlob, &pError)))
	{
		OutputDebugStringA((char*)pError->GetBufferPointer());
		OutputDebugStringA("\n");
		return false;
	}

	if (FAILED(DEVICE->CreateComputeShader(m_pCSBlob->GetBufferPointer(), m_pCSBlob->GetBufferSize(), nullptr, &m_pCS)))
		return false;

	return true;
}

void CComputeShader::SetShader()
{
	CONTEXT->CSSetShader(m_pCS, nullptr, 0);
}

void CComputeShader::Dispatch(unsigned int x, unsigned int y, unsigned int z)
{
	CONTEXT->Dispatch(x, y, z);
}

void CComputeShader::Clear()
{
	CONTEXT->CSSetShader(nullptr, nullptr, 0);
}
