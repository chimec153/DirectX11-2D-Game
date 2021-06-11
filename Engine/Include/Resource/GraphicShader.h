#pragma once
#include "Shader.h"
class CGraphicShader :
	public CShader
{
	friend class CShaderManager;
	friend class CSceneResource;

private:
	CGraphicShader();
	virtual ~CGraphicShader();

private:
	ID3D11VertexShader*		m_pVS;
	ID3DBlob*				m_pVSBlob;
	ID3D11PixelShader*		m_pPS;
	ID3DBlob*				m_pPSBlob;
	ID3D11HullShader*		m_pHS;
	ID3DBlob*				m_pHSBlob;
	ID3D11GeometryShader*	m_pGS;
	ID3DBlob*				m_pGSBlob;
	ID3D11DomainShader*		m_pDS;
	ID3DBlob*				m_pDSBlob;

public:
	bool LoadVertexShader(const char* pEntryName,
		const TCHAR* pFileName, const std::string& strRootName = SHADER_PATH);
	bool LoadPixelShader(const char* pEntryName,
		const TCHAR* pFileName, const std::string& strRootName = SHADER_PATH);
	bool LoadHullShader(const char* pEntryName,
		const TCHAR* pFileName, const std::string& strRootName = SHADER_PATH);
	bool LoadGeometryShader(const char* pEntryName,
		const TCHAR* pFileName, const std::string& strRootName = SHADER_PATH);
	bool LoadGeometryShaderWithSO(const char* pEntryName, 
		D3D11_SO_DECLARATION_ENTRY* pEntry,	UINT iNumEntries, UINT iStride, UINT iNumStrides,
		const TCHAR* pFileName, const std::string& strRootName = SHADER_PATH);
	bool LoadDomainShader(const char* pEntryName,
		const TCHAR* pFileName, const std::string& strRootName = SHADER_PATH);

public:
	virtual bool CreateInputLayout();
	virtual void SetShader();
	virtual void ResetShader(SHADER_CBUFFER_TYPE eType);

};

