#pragma once

#include "../GameEngine.h"

class CShaderManager
{
private:
	std::unordered_map<std::string, class CShader*>			m_mapShader;
	std::unordered_map<std::string, ID3D11InputLayout*>		m_mapLayout;
	std::unordered_map<std::string, PCBuffer>				m_mapCBuffer;
	std::vector<D3D11_INPUT_ELEMENT_DESC>					m_vecInputDesc;
	int														m_iInputSize;
	ID3D11Buffer* m_pBuffer;
	ID3D11Buffer* m_pOutputBuffer;
	ID3D11UnorderedAccessView* m_pInputSRV;
	ID3D11UnorderedAccessView* m_pOutputUAV;

public:
	bool Init();
	void Update(float fTime);
	bool CreateLayout(const std::string& strName);
	bool CreateInputDesc(const std::string& strName, const char* pSemanticName, UINT iSemanticIndex,
	DXGI_FORMAT eFormat, UINT iInputSlot, UINT iSize,
		D3D11_INPUT_CLASSIFICATION eInputSlotClass, UINT iInstanceDataStepRate);

	class CShader* FindShader(const std::string& strName);
	void ReleaseShader(const std::string& strName);

	ID3D11InputLayout* FindLayout(const std::string& strName);
	void ReleaseLayout(const std::string& strName);

public:
	bool CreateCBuffer(const std::string& strTag, size_t iSize, int iRegister, 
		int iType = (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX | (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);
	bool UpdateCBuffer(const std::string& strTag, void* pData);

private:
	PCBuffer FindCBuffer(const std::string& strTag);

public:
	template <typename T>
	T* CreateShader(const std::string& strName)
	{
		T* pShader = (T*)FindShader(strName);

		if (pShader)
		{
			SAFE_RELEASE(pShader);
			return nullptr;
		}

		pShader = new T;

		pShader->SetName(strName);

		pShader->AddRef();

		m_mapShader.insert(std::make_pair(strName, pShader));

		return pShader;
	}

public:
	bool LoadVertexShader(const std::string& strName, const char* pEntryName,
		const TCHAR* pFileName, const std::string& strRootPath = SHADER_PATH);
	bool LoadPixelShader(const std::string& strName, const char* pEntryName,
		const TCHAR* pFileName, const std::string& strRootPath = SHADER_PATH);
	bool LoadHullShader(const std::string& strName, const char* pEntryName,
		const TCHAR* pFileName, const std::string& strRootPath = SHADER_PATH);
	bool LoadGeometryShader(const std::string& strName, const char* pEntryName,
		const TCHAR* pFileName, const std::string& strRootPath = SHADER_PATH);
	bool LoadGeometryShaderWithSO(const std::string& strName, const char* pEntryName,
		D3D11_SO_DECLARATION_ENTRY* pEntry, UINT iNumEntries, UINT iStride, UINT iNumStrides,
		const TCHAR* pFileName, const std::string& strRootName = SHADER_PATH);
	bool LoadDomainShader(const std::string& strName, const char* pEntryName,
		const TCHAR* pFileName, const std::string& strRootPath = SHADER_PATH);

	DECLARE_SINGLE(CShaderManager)
};

