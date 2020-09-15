#pragma once

#include "Mesh.h"

class CMesh2D :
	public CMesh
{
	friend class CResourceManager;
	friend class CSceneResource;

private:
	CMesh2D();
	~CMesh2D();

private:
	D3D_PRIMITIVE_TOPOLOGY	m_ePrimitive;
	VertexBuffer		m_tVB;
	IndexBuffer			m_tIB;

public:
	bool Init();
	bool CreateMesh();
	virtual void Render(float fTime);
};

