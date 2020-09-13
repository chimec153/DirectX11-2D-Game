#pragma once
#include "../Ref.h"
class CShader :
	public CRef
{
	friend class CShaderManager;
	friend class CSceneResource;

private:
	CShader();
	virtual ~CShader();

private:
	ID3D11VertexShader* m_pVS;
	ID3DBlob*			m_pVBBlob;
	ID3D11PixelShader*	m_pPS;
	ID3DBlob*			m_pIBBlob;

public:
	bool Init();

};

