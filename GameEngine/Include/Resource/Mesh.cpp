#include "Mesh.h"
#include "Material.h"

CMesh::CMesh()	:
	m_pMaterial(nullptr)
{

}

CMesh::~CMesh()
{
	SAFE_RELEASE(m_pMaterial);
}

void CMesh::SetMaterial(CMaterial* pMaterial)
{
	SAFE_RELEASE(m_pMaterial);

	m_pMaterial = pMaterial;

	if (m_pMaterial)
		m_pMaterial->AddRef();
}

bool CMesh::Init()
{
	return true;
}
