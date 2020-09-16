#pragma once

#include "../Ref.h"

class CMaterial :
	public CRef
{
	friend class CResourceManager;

private:
	CMaterial();
	~CMaterial();

private:
	class CShader*		m_pShader;	
	ShaderCBuffer		m_tCBuffer;

public:
	void SetShader(const std::string& strName);

};

