#include "StaticMesh.h"

CStaticMesh::CStaticMesh()
{
}

CStaticMesh::~CStaticMesh()
{
}

CStaticMesh* CStaticMesh::Clone()
{
	return nullptr;
}

void CStaticMesh::Save(FILE* pFile)
{
	CMesh::Save(pFile);
}

void CStaticMesh::Load(FILE* pFile)
{
	CMesh::Load(pFile);
}
