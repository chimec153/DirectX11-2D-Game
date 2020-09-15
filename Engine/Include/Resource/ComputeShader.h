#pragma once
#include "Shader.h"
class CComputeShader :
	public CShader
{
	friend class CShaderManager;
	friend class CSceneResource;

private:
	CComputeShader();
	virtual ~CComputeShader();


};

