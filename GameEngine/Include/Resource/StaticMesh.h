#pragma once
#include "Mesh.h"
class CStaticMesh :
	public CMesh
{
	friend class CResourceManager;
	friend class CSceneResource;

private:
	CStaticMesh();
	~CStaticMesh();

public:
	virtual CStaticMesh* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

