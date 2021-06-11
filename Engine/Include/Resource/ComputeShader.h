#pragma once
#include "Shader.h"
class CComputeShader :
	public CShader
{
	friend class CShaderManager;

private:
	CComputeShader();
	virtual ~CComputeShader();

private:
	ID3D11ComputeShader* m_pCS;
	ID3DBlob* m_pCSBlob;

public:
	bool LoadComputeShader(const char* pEntryName,
		const TCHAR* pFileName, const std::string& strRootName = SHADER_PATH);
	virtual void SetShader();
	void Dispatch(unsigned int x, unsigned int y, unsigned int z);
	void Clear();
};

