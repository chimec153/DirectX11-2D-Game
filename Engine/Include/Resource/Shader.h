#pragma once
#include "../Ref.h"
class CShader :
	public CRef
{
	friend class CShaderManager;
	friend class CSceneResource;

protected:
	CShader();
	virtual ~CShader() = 0;

protected:
	SHADER_TYPE									m_eType;
	std::vector<D3D11_INPUT_ELEMENT_DESC>		m_vecInputDesc;
	int											m_iInputSize;
	int										m_iInstSize;
	ID3D11InputLayout*							m_pInputLayout;

public:
	SHADER_TYPE GetShaderType()	const;

public:
	bool Init();
	bool AddInputLayoutDesc(const char* pSemanticName, UINT iSemanticIndex,
		DXGI_FORMAT eFormat, UINT iInputSlot, UINT iSize,
		D3D11_INPUT_CLASSIFICATION eInputSlotClass, UINT iInstanceDataStepRate);
	virtual bool CreateInputLayout();
	virtual void SetShader();

	void CreateGeometryShader();

};

