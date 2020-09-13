#pragma once

#include "../GameEngine.h"

class CShaderManager
{
private:
	std::unordered_map<std::string, class CShader*>			m_mapShader;
	std::unordered_map<std::string, ID3D11InputLayout*>		m_mapLayout;
	std::vector<D3D11_INPUT_ELEMENT_DESC>					m_vecInputDesc;
	int														m_iInputSize;

public:
	bool Init();
	bool CreateLayout(const std::string strName, const std::string strShaderName);
	bool CreateInputDesc(const char* pSemanticName, UINT iSemanticIndex,
	DXGI_FORMAT eFormat, UINT iInputSlot, UINT iSize,
		D3D11_INPUT_CLASSIFICATION eInputSlotClass, UINT iInstanceDataStepRate);

	class CShader* FindShader(const std::string& strName);
	void ReleaseShader(const std::string& strName);

	ID3D11InputLayout* FindLayout(const std::string& strName);
	void ReleaseLayout(const std::string& strName);

	DECLARE_SINGLE(CShaderManager)
};

