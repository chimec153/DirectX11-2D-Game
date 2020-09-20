#pragma once

#include "../Ref.h"

class CMaterial :
	public CRef
{
	friend class CResourceManager;

private:
	CMaterial();
	CMaterial(const CMaterial& material);
	~CMaterial();

private:
	class CShader*		m_pShader;	
	ShaderCBuffer		m_tCBuffer;

public:
	void SetShader(const std::string& strName);
	void SetDiffuseColor(const Vector4& v);
	void SetDiffuseColor(float x, float y, float z, float w);
	void SetAmbientColor(const Vector4& v);
	void SetAmbientColor(float x, float y, float z, float w);
	void SetSpecularColor(const Vector4& v);
	void SetSpecularColor(float x, float y, float z, float w);
	void SetMaterial();
	CMaterial* Clone();

};

