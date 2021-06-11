#include "ResourceManager.h"
#include "Mesh2D.h"
#include "ShaderManager.h"
#include "Material.h"
#include "Texture.h"
#include "../Device.h"
#include "Animation2DSequence.h"
#include "StaticMesh.h"
#include "ParticleSystem.h"
#include "Skeleton.h"
#include "AnimationMesh.h"
#include "AnimationSequence.h"

DEFINITION_SINGLE(CResourceManager)

CResourceManager::CResourceManager()	:
	m_pDefaultMesh(nullptr),
	m_pDefaultUIMesh(nullptr),
	m_pNoiseTex(nullptr),
	m_pPoint(nullptr),
	m_pLinear(nullptr),
	m_pAnisotropic(nullptr)
{

}

CResourceManager::~CResourceManager()
{
	SAFE_RELEASE(m_pDefaultMesh);
	SAFE_RELEASE(m_pDefaultUIMesh);
	SAFE_RELEASE(m_pNoiseTex);
	SAFE_RELEASE_MAP(m_mapMesh);
	SAFE_RELEASE_MAP(m_mapMaterial);
	SAFE_RELEASE_MAP(m_mapAni2DSeq);
	SAFE_RELEASE_MAP(m_mapTexture);
	SAFE_RELEASE_MAP(m_mapParticle);
	SAFE_RELEASE_MAP(m_mapSkeleton);
	SAFE_RELEASE_MAP(m_mapSequence);
	DESTROY_SINGLE(CShaderManager);
	SAFE_RELEASE(m_pPoint);
	SAFE_RELEASE(m_pLinear);
	SAFE_RELEASE(m_pAnisotropic);
	Safe_Release_Map(m_mapSamplerStates);
}

CMesh2D* CResourceManager::GetDefaultMesh() const
{
	if (m_pDefaultMesh)
		m_pDefaultMesh->AddRef();

	return m_pDefaultMesh;
}

CMesh2D* CResourceManager::GetUIMesh() const
{
	if (m_pDefaultUIMesh)
		m_pDefaultUIMesh->AddRef();

	return m_pDefaultUIMesh;
}

CTexture* CResourceManager::GetNoiseTex() const
{
	if (m_pNoiseTex)
		m_pNoiseTex->AddRef();

	return m_pNoiseTex;
}

bool CResourceManager::Init()
{
	// Shader Manager ÃÊ±âÈ­
	if (!GET_SINGLE(CShaderManager)->Init())
		return false;

	m_pNoiseTex = new CTexture;

	m_pNoiseTex->SetName("Noise");

	m_pNoiseTex->LoadTexture("Noise", TEXT("noise_01.png"));

	m_pNoiseTex->SetTexture(10, (int)SHADER_CBUFFER_TYPE::CBUFFER_COMPUTE |
		(int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);

	VertexColor v[4] = {};

	v[0].vPos = Vector3(0.f, 1.f, 0.f);
	v[0].vUV = Vector2(0.f, 0.f);
	v[0].vColor = Vector4::White;

	v[1].vPos = Vector3(1.f, 1.f, 0.f);
	v[1].vUV = Vector2(1.f, 0.f);
	v[1].vColor = Vector4::White;

	v[2].vPos = Vector3(0.f, 0.f, 0.f);
	v[2].vUV = Vector2(0.f, 1.f);
	v[2].vColor = Vector4::White;

	v[3].vPos = Vector3(1.f, 0.f, 0.f);
	v[3].vUV = Vector2(1.f, 1.f);
	v[3].vColor = Vector4::White;

	unsigned short iIdx[6] = {
		0, 1, 3,
		0, 3, 2
	};

	m_pDefaultMesh = new CMesh2D;

	m_pDefaultMesh->SetName("DefaultMesh");

	if (!m_pDefaultMesh->CreateMesh(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, v, sizeof(VertexColor), 4, D3D11_USAGE_IMMUTABLE, 
		iIdx, 2, 6, D3D11_USAGE_IMMUTABLE, DXGI_FORMAT_R16_UINT))
		return false;

	m_pDefaultMesh->AddRef();

	m_mapMesh.insert(std::make_pair("DefaultMesh", m_pDefaultMesh));

	CMaterial* pMaterial = CreateMaterial("Color");

	SAFE_RELEASE(pMaterial);

	m_pDefaultUIMesh = new CMesh2D;

	m_pDefaultUIMesh->SetName("DefaultUIMesh");

	if (!m_pDefaultUIMesh->CreateMesh(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, v, sizeof(VertexColor), 4, D3D11_USAGE_IMMUTABLE,
		iIdx, 2, 6, D3D11_USAGE_IMMUTABLE, DXGI_FORMAT_R16_UINT))
		return false;

	CreateSphere("Sphere", 0.5f, 20, 20);

	CMaterial* pMtrl = CreateMaterial("UI2D");

	SAFE_RELEASE(pMtrl);
	
	CMaterial* pParticleMtrl = CreateMaterial("Particle");

	SAFE_RELEASE(pParticleMtrl);

	m_pPoint = CreateSample(D3D11_FILTER_MIN_MAG_MIP_POINT);
	m_pLinear = CreateSample(D3D11_FILTER_MIN_MAG_MIP_LINEAR);
	m_pAnisotropic = CreateSample(D3D11_FILTER_ANISOTROPIC);
	float pBorder[4] = { 0.f, 0.f, 0.f, 1.f };
	CreateSamplerState("Shadow", 
		D3D11_TEXTURE_ADDRESS_CLAMP, 
		D3D11_TEXTURE_ADDRESS_CLAMP, 
		D3D11_TEXTURE_ADDRESS_CLAMP, 
		pBorder,
		D3D11_COMPARISON_LESS_EQUAL, 
		FLT_MAX, -FLT_MAX, 
		D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR);

	CONTEXT->VSSetSamplers(0, 1, &m_pPoint);
	CONTEXT->PSSetSamplers(0, 1, &m_pPoint);
	CONTEXT->HSSetSamplers(0, 1, &m_pPoint);
	CONTEXT->DSSetSamplers(0, 1, &m_pPoint);
	CONTEXT->GSSetSamplers(0, 1, &m_pPoint);
	CONTEXT->CSSetSamplers(0, 1, &m_pPoint);

	CONTEXT->VSSetSamplers(1, 1, &m_pLinear);
	CONTEXT->PSSetSamplers(1, 1, &m_pLinear);
	CONTEXT->HSSetSamplers(1, 1, &m_pLinear);
	CONTEXT->DSSetSamplers(1, 1, &m_pLinear);
	CONTEXT->GSSetSamplers(1, 1, &m_pLinear);
	CONTEXT->CSSetSamplers(1, 1, &m_pLinear);

	CONTEXT->VSSetSamplers(2, 1, &m_pAnisotropic);
	CONTEXT->PSSetSamplers(2, 1, &m_pAnisotropic);
	CONTEXT->HSSetSamplers(2, 1, &m_pAnisotropic);
	CONTEXT->DSSetSamplers(2, 1, &m_pAnisotropic);
	CONTEXT->GSSetSamplers(2, 1, &m_pAnisotropic);
	CONTEXT->CSSetSamplers(2, 1, &m_pAnisotropic);

	SetSamplerState("Shadow", 3);

	CMaterial* pColMat = CreateMaterial("Collider");

	SAFE_RELEASE(pColMat);

	Vector3 vVtx[5] =
	{
		Vector3(-0.5f, 0.5f, 0.f),
		Vector3(0.5f, 0.5f, 0.f),
		Vector3(0.5f, -0.5f, 0.f),
		Vector3(-0.5f, -0.5f, 0.f),
		Vector3(-0.5f, 0.5f, 0.f)
	};

	CreateMesh(MESH_TYPE::MT_2D, "Collider2D", D3D_PRIMITIVE_TOPOLOGY_LINESTRIP, vVtx, sizeof(Vector3), sizeof(vVtx) / sizeof(vVtx[0]),
		D3D11_USAGE_IMMUTABLE);

	Vector3 vLnVtx[2] =
	{
		Vector3(0.f, -0.5f, 0.f),
		Vector3(0.f, 0.5f, 0.f)
	};

	CreateMesh(MESH_TYPE::MT_2D, "Collider2DLine", D3D_PRIMITIVE_TOPOLOGY_LINELIST, vLnVtx, sizeof(Vector3), sizeof(vLnVtx) / sizeof(vLnVtx[0]),
		D3D11_USAGE_IMMUTABLE);

	Vector3 vLn[2] =
	{
		Vector3(0.f, 0.f, 0.f),
		Vector3(1.f, 0.f, 0.f)
	};

	CreateMesh(MESH_TYPE::MT_2D, "Line", D3D_PRIMITIVE_TOPOLOGY_LINELIST, vLn,
		sizeof(Vector3), sizeof(vLn) / sizeof(vLn[0]), D3D11_USAGE_IMMUTABLE);

	CMaterial* pLineMtrl = CreateMaterial("Line");

	SAFE_RELEASE(pLineMtrl);

	Vector3 vCircle[361] = {};

	for (int i = 0; i < 361; ++i)
	{
		float fAngle = DirectX::XMConvertToRadians((float)i);

		vCircle[i] = Vector3(cosf(fAngle) * 0.5f, sinf(fAngle) * 0.5f, 0.f);
	}

	CreateMesh(MESH_TYPE::MT_2D, "Circle2D", D3D_PRIMITIVE_TOPOLOGY_LINESTRIP,
		vCircle, sizeof(Vector3), sizeof(vCircle) / sizeof(vCircle[0]), D3D11_USAGE_IMMUTABLE);

	VertexColor* pCircle2 = new VertexColor[361];

	for (int i = 0; i < 361; ++i)
	{
		float fAngle = DirectX::XMConvertToRadians((float)i);

		pCircle2[i].vPos = Vector3(cosf(fAngle) * 0.5f, 0.f, sinf(fAngle) * 0.5f);
		pCircle2[i].vColor = Vector4((sinf(fAngle) + 1.f) / 2.f, (cosf(fAngle) + 1.f) / 2.f, (sinf(fAngle) + 1.f) / 2.f, 1.f);
		pCircle2[i].vUV = Vector2(0.f, -1.f);
	}

	CreateMesh(MESH_TYPE::MT_2D, "Circle", D3D_PRIMITIVE_TOPOLOGY_LINESTRIP,
		pCircle2, sizeof(VertexColor), 361, D3D11_USAGE_IMMUTABLE);

	delete[] pCircle2;

	CMaterial* pC2CMtrl = CreateMaterial("C2C");

	SAFE_RELEASE(pC2CMtrl);

	VertexColor tPoint = {};

	CreateMesh(MESH_TYPE::MT_2D, "Particle", D3D_PRIMITIVE_TOPOLOGY_POINTLIST,
		&tPoint, sizeof(VertexColor), 1, D3D11_USAGE_IMMUTABLE);

	Basic tVertex[4] = {};

	tVertex[0].vPos = Vector3(0.f, 1.f, 0.f);
	tVertex[0].vUV = Vector2(0.f, 0.f);

	tVertex[1].vPos = Vector3(1.f, 1.f, 0.f);
	tVertex[1].vUV = Vector2(1.f, 0.f);

	tVertex[2].vPos = Vector3(1.f, 0.f, 0.f);
	tVertex[2].vUV = Vector2(1.f, 1.f);

	tVertex[3].vPos = Vector3(0.f, 0.f, 0.f);
	tVertex[3].vUV = Vector2(0.f, 1.f);

	unsigned short pBasicIdx[6] =
	{
		0, 1, 2, 0, 2,3
	};

	CreateMesh(MESH_TYPE::MT_2D, "AngleMesh", D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		tVertex, sizeof(Basic), 4, D3D11_USAGE_IMMUTABLE, pBasicIdx, 2, 6, D3D11_USAGE_IMMUTABLE, DXGI_FORMAT_R16_UINT);

	CMaterial* pAngleMtrl = CreateMaterial("Angle");

	SAFE_RELEASE(pAngleMtrl);

	VERTEX3D vOBBVtx[8] = {};

	vOBBVtx[0].vPos = Vector3(-0.5f, 0.5f, -0.5f);
	vOBBVtx[0].vNormal = Vector3(-1.f, 1.f, -1.f);
	vOBBVtx[0].vNormal.Normalize();
	vOBBVtx[0].vTangent = Vector3(1.f, 2.f, 1.f);
	vOBBVtx[0].vTangent.Normalize();
	vOBBVtx[0].vBinormal = vOBBVtx[0].vNormal.Cross(vOBBVtx[0].vTangent);
	vOBBVtx[1].vPos = Vector3(-0.5f, 0.5f, +0.5f);
	vOBBVtx[1].vNormal = Vector3(1.f, 1.f, -1.f);
	vOBBVtx[1].vNormal.Normalize();
	vOBBVtx[1].vTangent = Vector3(1.f, 1.f, 2.f);
	vOBBVtx[1].vTangent.Normalize();
	vOBBVtx[1].vBinormal = vOBBVtx[1].vNormal.Cross(vOBBVtx[1].vTangent);
	vOBBVtx[2].vPos = Vector3(-0.5f, -0.5f, -0.5f);
	vOBBVtx[2].vNormal = Vector3(-1.f, -1.f, -1.f);
	vOBBVtx[2].vNormal.Normalize();
	vOBBVtx[2].vTangent = Vector3(1.f, 1.f, -2.f);
	vOBBVtx[2].vTangent.Normalize();
	vOBBVtx[2].vBinormal = vOBBVtx[2].vNormal.Cross(vOBBVtx[2].vTangent);
	vOBBVtx[3].vPos = Vector3(-0.5f, -0.5f, +0.5f);
	vOBBVtx[3].vNormal = Vector3(1.f, -1.f, -1.f);
	vOBBVtx[3].vNormal.Normalize();
	vOBBVtx[3].vTangent = Vector3(2.f, 1.f, 1.f);
	vOBBVtx[3].vTangent.Normalize();
	vOBBVtx[3].vBinormal = vOBBVtx[3].vNormal.Cross(vOBBVtx[3].vTangent);
	vOBBVtx[4].vPos = Vector3(0.5f, 0.5f, -0.5f);
	vOBBVtx[4].vNormal = Vector3(-1.f, 1.f, 1.f);
	vOBBVtx[4].vNormal.Normalize();
	vOBBVtx[4].vTangent = Vector3(2.f, 1.f, 1.f);
	vOBBVtx[4].vTangent.Normalize();
	vOBBVtx[4].vBinormal = vOBBVtx[4].vNormal.Cross(vOBBVtx[4].vTangent);
	vOBBVtx[5].vPos = Vector3(0.5f, 0.5f, +0.5f);
	vOBBVtx[5].vNormal = Vector3(1.f, 1.f, 1.f);
	vOBBVtx[5].vNormal.Normalize();
	vOBBVtx[5].vTangent = Vector3(-2.f, 1.f, 1.f);
	vOBBVtx[5].vTangent.Normalize();
	vOBBVtx[5].vBinormal = vOBBVtx[5].vNormal.Cross(vOBBVtx[5].vTangent);
	vOBBVtx[6].vPos = Vector3(0.5f, -0.5f, -0.5f);
	vOBBVtx[6].vNormal = Vector3(-1.f, -1.f, 1.f);
	vOBBVtx[6].vNormal.Normalize();
	vOBBVtx[6].vTangent = Vector3(2.f, -1.f, 1.f);
	vOBBVtx[6].vTangent.Normalize();
	vOBBVtx[6].vBinormal = vOBBVtx[6].vNormal.Cross(vOBBVtx[6].vTangent);
	vOBBVtx[7].vPos = Vector3(0.5f, -0.5f, +0.5f);
	vOBBVtx[7].vNormal = Vector3(1.f, -1.f, 1.f);
	vOBBVtx[7].vNormal.Normalize();
	vOBBVtx[7].vTangent = Vector3(-2.f, -1.f, 1.f);
	vOBBVtx[7].vTangent.Normalize();
	vOBBVtx[7].vBinormal = vOBBVtx[7].vNormal.Cross(vOBBVtx[7].vTangent);

	unsigned short pOBBIdx[36] =
	{
		0, 1, 3,
		0,3,2,
		1,5,3,
		5,7,3,
		0,5,1,
		0,4,5,
		4,6,5,
		5,6,7,
		4,0,6,
		0,2,6,
		2,3,6,
		3,7,6,
	};

	if (!CreateMesh(MESH_TYPE::MT_STATIC, "Cube", D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, &vOBBVtx[0], static_cast<int>(sizeof(VERTEX3D)), 8, D3D11_USAGE_IMMUTABLE,
		&pOBBIdx[0], 2, 36, D3D11_USAGE_IMMUTABLE, DXGI_FORMAT_R16_UINT))
		return false;

	std::vector<const TCHAR*> vecFileName;

	for (int i = 0; i < 13; ++i)
	{
		TCHAR* pFile = new TCHAR[MAX_PATH];

		swprintf_s(pFile, MAX_PATH, TEXT("mouse\\Mouse%d.png"), i);

		vecFileName.push_back(pFile);
	}

	LoadTexture("Mouse", vecFileName);

	SAFE_DELETE_VECLIST(vecFileName);

	LoadTexture("TopButton", TEXT("Up.png"));
	LoadTexture("BotButton", TEXT("Down.png"));
	LoadTexture("Slider", TEXT("Slider.png"));
	LoadTexture("SliderBack", TEXT("SliderBack.png"));

	LoadTexture("Filter", TEXT("DefaultDistortionFilter1.png"));
	LoadTexture("Distortion", TEXT("DefaultDistortionNormal1.png"));

	CTexture* pDistort = FindTexture("Distortion");

	pDistort->SetTexture(99, static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_PIXEL));

	SAFE_RELEASE(pDistort);

	CTexture* pFilter = FindTexture("Filter");

	pFilter->SetTexture(100, static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_PIXEL));

	SAFE_RELEASE(pFilter);

	return true;
}

bool CResourceManager::CreateMesh(MESH_TYPE eType, const std::string& strName, 
	D3D_PRIMITIVE_TOPOLOGY eTop, void* pVtxData, int iVtxSz, int iVtxCnt, 
	D3D11_USAGE eVtxUsg, void* pIdxData, int iIdxSz, int iIdxCnt, 
	D3D11_USAGE eIdxUsg, DXGI_FORMAT eFmt)
{
	CMesh* pMesh = FindMesh(strName);

	if (pMesh)
	{
		SAFE_RELEASE(pMesh);
		return false;
	}
		
	switch (eType)
	{
	case MESH_TYPE::MT_2D:
		pMesh = new CMesh2D;
		break;
	case MESH_TYPE::MT_STATIC:
		pMesh = new CStaticMesh;
		break;
	case MESH_TYPE::MT_ANIMATION:
		pMesh = new CAnimationMesh;
		break;
	}

	pMesh->SetName(strName);

	if (!pMesh->CreateMesh(eTop, pVtxData, iVtxSz, iVtxCnt, eVtxUsg, 
		pIdxData, iIdxSz, iIdxCnt, eIdxUsg, eFmt))
	{
		SAFE_RELEASE(pMesh);
		return false;
	}

	m_mapMesh.insert(std::make_pair(strName, pMesh));

	return true;
}

CMesh* CResourceManager::FindMesh(const std::string& strName)
{
	std::unordered_map<std::string, CMesh*>::iterator iter = m_mapMesh.find(strName);

	if (iter == m_mapMesh.end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}

void CResourceManager::ReleaseMesh(const std::string& strName)
{
	std::unordered_map<std::string, CMesh*>::iterator iter = m_mapMesh.find(strName);

	if (iter == m_mapMesh.end())
		return;
		
	if (iter->second->Release() == 0)
		m_mapMesh.erase(iter);
}

bool CResourceManager::LoadMesh(MESH_TYPE eType, const std::string& strKey, const TCHAR* pFilePath, const std::string& strPathKey)
{
	CMesh* pMesh = nullptr;

	switch (eType)
	{
	case MESH_TYPE::MT_2D:
		pMesh = new CMesh2D;
		break;
	case MESH_TYPE::MT_STATIC:
		pMesh = new CStaticMesh;
		break;
	case MESH_TYPE::MT_ANIMATION:
		pMesh = new CAnimationMesh;
		break;
	}

	if (!pMesh->LoadMesh(strKey, pFilePath, strPathKey))
	{
		delete pMesh;
		return false;
	}

	m_mapMesh.insert(std::make_pair(strKey, pMesh));

	return true;
}

void CResourceManager::CreateSphere(const std::string& strKey, float fRadius, UINT iSliceCount, UINT iStackCount)
{
	std::vector<VERTEX3D>	vecVtx;
	std::vector<UINT>	vecIdx;
	//
	// Compute the vertices stating at the top pole and moving down the stacks.
	//

	// Poles: note that there will be texture coordinate distortion as there is
	// not a unique point on the texture map to assign to the pole when mapping
	// a rectangular texture onto a sphere.
	VERTEX3D topVertex =
	{
		Vector3(0.f, fRadius, 0.f),
	Vector2(0.f, 0.f),
	Vector3(0.f, 1.f, 0.f),
	Vector3(1.f, 0.f, 0.f),
	Vector3(0.f, 0.f, -1.f),
	Vector4(),
	Vector4()
	};
	VERTEX3D bottomVertex = 
	{
		Vector3(0.f, -fRadius, 0.f),
	Vector2(0.f, 1.f),
	Vector3(0.f, -1.f, 0.f),
	Vector3(1.f, 0.f, 0.f),
	Vector3(0.f, 0.f, 1.f),
	Vector4(),
	Vector4() 
	};

	vecVtx.push_back(topVertex);

	float phiStep = XM_PI / iStackCount;
	float thetaStep = 2.0f * XM_PI / iSliceCount;

	// Compute vertices for each stack ring (do not count the poles as rings).
	for (UINT i = 1; i <= iStackCount - 1; ++i)
	{
		float phi = i * phiStep;

		// Vertices of ring.
		for (UINT j = 0; j <= iSliceCount; ++j)
		{
			float theta = j * thetaStep;

			VERTEX3D v;

			// spherical to cartesian
			v.vPos.x = fRadius * sinf(phi) * cosf(theta);
			v.vPos.y = fRadius * cosf(phi);
			v.vPos.z = fRadius * sinf(phi) * sinf(theta);

			// Partial derivative of P with respect to theta
			v.vTangent.x = -fRadius * sinf(phi) * sinf(theta);
			v.vTangent.y = 0.0f;
			v.vTangent.z = +fRadius * sinf(phi) * cosf(theta);

			v.vTangent.Normalize();
			v.vPos.Normalize();

			v.vUV.x = theta / XM_2PI;
			v.vUV.y = phi / XM_PI;

			vecVtx.push_back(v);
		}
	}

	vecVtx.push_back(bottomVertex);

	//
	// Compute indices for top stack.  The top stack was written first to the vertex buffer
	// and connects the top pole to the first ring.
	//

	for (UINT i = 1; i <= iSliceCount; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 1);
		vecIdx.push_back(i);
	}

	//
	// Compute indices for inner stacks (not connected to poles).
	//

	// Offset the indices to the index of the first vertex in the first ring.
	// This is just skipping the top pole vertex.
	UINT baseIndex = 1;
	UINT ringVertexCount = iSliceCount + 1;
	for (UINT i = 0; i < iStackCount - 2; ++i)
	{
		for (UINT j = 0; j < iSliceCount; ++j)
		{
			vecIdx.push_back(baseIndex + i * ringVertexCount + j);
			vecIdx.push_back(baseIndex + i * ringVertexCount + j + 1);
			vecIdx.push_back(baseIndex + (i + 1) * ringVertexCount + j);

			vecIdx.push_back(baseIndex + (i + 1) * ringVertexCount + j);
			vecIdx.push_back(baseIndex + i * ringVertexCount + j + 1);
			vecIdx.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
		}
	}

	//
	// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
	// and connects the bottom pole to the bottom ring.
	//

	// South pole vertex was added last.
	UINT southPoleIndex = (UINT)vecVtx.size() - 1;

	// Offset the indices to the index of the first vertex in the last ring.
	baseIndex = southPoleIndex - ringVertexCount;

	for (UINT i = 0; i < iSliceCount; ++i)
	{
		vecIdx.push_back(southPoleIndex);
		vecIdx.push_back(baseIndex + i);
		vecIdx.push_back(baseIndex + i + 1);
	}

	CreateMesh(MESH_TYPE::MT_STATIC, strKey, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, &vecVtx[0], 
		static_cast<int>(sizeof(VERTEX3D)), 
		static_cast<int>(vecVtx.size()),
		D3D11_USAGE_IMMUTABLE, &vecIdx[0], 4, 
		static_cast<int>(vecIdx.size()), D3D11_USAGE_IMMUTABLE, DXGI_FORMAT_R32_UINT);
}

CMaterial* CResourceManager::CreateMaterial(const std::string& strName)
{
	CMaterial* pMaterial = FindMaterial(strName);
	
	if (pMaterial)
		return pMaterial;

	pMaterial = new CMaterial;

	pMaterial->SetName(strName);

	pMaterial->AddRef();

	m_mapMaterial.insert(std::make_pair(strName, pMaterial));

	return pMaterial;
}

CMaterial* CResourceManager::FindMaterial(const std::string& strName)
{
	std::unordered_map<std::string, CMaterial*>::iterator iter = m_mapMaterial.find(strName);

	if (iter == m_mapMaterial.end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}

void CResourceManager::ReleaseMaterial(const std::string& strName)
{
	std::unordered_map<std::string, CMaterial*>::iterator iter = m_mapMaterial.find(strName);

	if (iter == m_mapMaterial.end())
		return;

	if (iter->second->Release() == 0)
		m_mapMaterial.erase(iter);
}

bool CResourceManager::LoadTexture(const std::string& strTag, const TCHAR* pFileName, const std::string& strRootPath)
{
	CTexture* pTex = FindTexture(strTag);

	if (pTex)
	{
		SAFE_RELEASE(pTex);
		return false;
	}

	pTex = new CTexture;

	if (!pTex->LoadTexture(strTag, pFileName, strRootPath))
	{
		SAFE_RELEASE(pTex);
		return false;
	}

	m_mapTexture.insert(std::make_pair(strTag, pTex));

	return true;
}

bool CResourceManager::LoadTextureFromFullPath(const std::string& strTag, const TCHAR* pFullPath)
{
	CTexture* pTex = FindTexture(strTag);

	if (pTex)
	{
		SAFE_RELEASE(pTex);
		return true;
	}

	pTex = new CTexture;

	if (!pTex->LoadTextureFromFullPath(strTag, pFullPath))
	{
		SAFE_RELEASE(pTex);
		return false;
	}

	m_mapTexture.insert(std::make_pair(strTag, pTex));

	return true;
}

bool CResourceManager::LoadTexture(const std::string& strTag, const std::vector<const TCHAR*>& vecFileName, const std::string& strRootPath)
{
	CTexture* pTex = FindTexture(strTag);

	if (pTex)
	{
		SAFE_RELEASE(pTex);
		return true;
	}

	pTex = new CTexture;

	if (!pTex->LoadTexture(strTag, vecFileName, strRootPath))
	{
		SAFE_RELEASE(pTex);
		return false;
	}

	m_mapTexture.insert(std::make_pair(strTag, pTex));

	return true;
}

bool CResourceManager::LoadTextureFromFullPath(const std::string& strTag, const std::vector<const TCHAR*>& vecFullPath)
{
	CTexture* pTex = FindTexture(strTag);

	if (pTex)
	{
		SAFE_RELEASE(pTex);
		return true;
	}

	pTex = new CTexture;

	if (!pTex->LoadTextureFromFullPath(strTag, vecFullPath))
	{
		SAFE_RELEASE(pTex);
		return false;
	}

	m_mapTexture.insert(std::make_pair(strTag, pTex));

	return true;
}

bool CResourceManager::LoadTextureArray(const std::string& strTag, 
	const std::vector<const TCHAR*>& vecFileName, const std::string& strRootPath)
{
	CTexture* pTexture = FindTexture(strTag);

	if (pTexture)
	{
		SAFE_RELEASE(pTexture);
		return false;
	}

	pTexture = new CTexture;

	if (!pTexture->LoadTextureArray(strTag, vecFileName, strRootPath))
	{
		SAFE_RELEASE(pTexture);
		return false;
	}

	m_mapTexture.insert(std::make_pair(strTag, pTexture));

	return true;
}

bool CResourceManager::LoadTextureArrayFromFullPath(const std::string& strTag, 
	const std::vector<const TCHAR*>& vecFullPath)
{
	CTexture* pTexture = FindTexture(strTag);

	if (pTexture)
	{
		SAFE_RELEASE(pTexture);
		return false;
	}

	pTexture = new CTexture;

	if (!pTexture->LoadTextureArrayFromFullPath(strTag, vecFullPath))
	{
		SAFE_RELEASE(pTexture);
		return false;
	}

	m_mapTexture.insert(std::make_pair(strTag, pTexture));

	return true;
}

void CResourceManager::ReleaseTexture(const std::string& strTag)
{
	std::unordered_map<std::string, CTexture*>::iterator iter = m_mapTexture.find(strTag);

	if (iter == m_mapTexture.end())
		return;

	if (iter->second->Release() == 0)
		m_mapTexture.erase(iter);
}

CTexture* CResourceManager::FindTexture(const std::string& strTag)
{
	std::unordered_map<std::string, CTexture*>::iterator iter = m_mapTexture.find(strTag);

	if (iter == m_mapTexture.end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}

CTexture* CResourceManager::LoadTexture(FILE* pFile)
{
	CTexture* pTex = new CTexture;

	pTex->Load(pFile);

	return pTex;
}

bool CResourceManager::CreateAni2DSeq(const std::string& strTag, CTexture* pTex, float fMaxTime, float fPlayRate)
{
	CAnimation2DSequence* pSeq = FindAni2DSeq(strTag);

	if (pSeq)
	{
		SAFE_RELEASE(pSeq);
		return false;
	}

	pSeq = new CAnimation2DSequence;

	pSeq->SetName(strTag);

	if (!pSeq->Init(pTex, fMaxTime, fPlayRate))
	{
		SAFE_RELEASE(pSeq);
		return false;
	}

	m_mapAni2DSeq.insert(std::make_pair(strTag, pSeq));
	return true;
}

bool CResourceManager::CreateAni2DSeq(const std::string& strTag, const std::string& strKey, float fMaxTime, float fPlayRate)
{
	CAnimation2DSequence* pSeq = FindAni2DSeq(strTag);

	if (pSeq)
	{
		SAFE_RELEASE(pSeq);
		return false;
	}

	pSeq = new CAnimation2DSequence;

	pSeq->SetName(strTag);

	if (!pSeq->Init(strKey, fMaxTime, fPlayRate))
	{
		SAFE_RELEASE(pSeq);
		return false;
	}

	m_mapAni2DSeq.insert(std::make_pair(strTag, pSeq));
	return true;
}

bool CResourceManager::LoadAni2DSeq(const std::string& strTag, const TCHAR* pFileName, const std::string& strRootPath, float fMaxTime, float fPlayRate)
{
	CAnimation2DSequence* pSeq = FindAni2DSeq(strTag);

	if (pSeq)
	{
		SAFE_RELEASE(pSeq);
		return false;
	}

	pSeq = new CAnimation2DSequence;

	pSeq->SetName(strTag);

	if (!pSeq->Init(pFileName, strRootPath, fMaxTime, fPlayRate))
	{
		SAFE_RELEASE(pSeq);
		return false;
	}

	m_mapAni2DSeq.insert(std::make_pair(strTag, pSeq));

	return true;
}

void CResourceManager::SaveAni2DSeqFromFullPath(const std::string& strTag, const char* pFullPath)
{
	CAnimation2DSequence* pSeq = FindAni2DSeq(strTag);

	if (!pSeq)
		return;

	FILE* pFile = nullptr;

	fopen_s(&pFile, pFullPath, "wb");

	if (pFile)
	{
		pSeq->Save(pFile);

		fclose(pFile);
	}

	SAFE_RELEASE(pSeq);
}

void CResourceManager::AddFrame(const std::string& strTag, const Vector2& vStart, const Vector2& vEnd)
{
	CAnimation2DSequence* pSeq = FindAni2DSeq(strTag);

	if (!pSeq)
		return;

	pSeq->AddFrame(vStart, vEnd);

	SAFE_RELEASE(pSeq);
}

void CResourceManager::AddNotify(const std::string& strTag, const std::string& strNot, int iFrame)
{
	CAnimation2DSequence* pSeq = FindAni2DSeq(strTag);

	if (!pSeq)
		return;

	pSeq->AddNotify(strNot, iFrame);

	SAFE_RELEASE(pSeq);
}

void CResourceManager::AddNotify(const std::string& strTag, const std::string& strNot, float fTime)
{
	CAnimation2DSequence* pSeq = FindAni2DSeq(strTag);

	if (!pSeq)
		return;

	pSeq->AddNotify(strNot, fTime);

	SAFE_RELEASE(pSeq);
}

CAnimation2DSequence* CResourceManager::FindAni2DSeq(const std::string& strTag)
{
	std::unordered_map<std::string, CAnimation2DSequence*>::iterator iter = m_mapAni2DSeq.find(strTag);

	if (iter == m_mapAni2DSeq.end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}

void CResourceManager::ReleaseAni2DSeq(const std::string& strTag)
{
	std::unordered_map<std::string, CAnimation2DSequence*>::iterator iter = m_mapAni2DSeq.find(strTag);

	if (iter == m_mapAni2DSeq.end())
		return;

	if (iter->second->Release() == 0)
		m_mapAni2DSeq.erase(iter);
}

void CResourceManager::LoadSequence(const std::string& strKey, const char* pFilePath, const std::string& strPathKey)
{
	CAnimationSequence* pSeq = FindSequence(strKey);

	if (pSeq)
	{
		SAFE_RELEASE(pSeq);
		return;
	}

	pSeq = new CAnimationSequence;
	pSeq->SetName(strKey);

	if (!pSeq->Init())
	{
		SAFE_RELEASE(pSeq);
		return;
	}

	if (!pSeq->LoadMultiByte(pFilePath, strPathKey))
	{
		SAFE_RELEASE(pSeq);
		return;
	}

	m_mapSequence.insert(std::make_pair(strKey, pSeq));
}

void CResourceManager::LoadSequence(const std::string& strKey, bool bLoop, _tagFbxAnimationClip* clip)
{
	CAnimationSequence* pSeq = FindSequence(strKey);

	if (pSeq)
	{
		SAFE_RELEASE(pSeq);
		return;
	}

	pSeq = new CAnimationSequence;

	if (!pSeq->CreateSequence(clip, bLoop))
	{
		SAFE_RELEASE(pSeq);
		return;
	}

	m_mapSequence.insert(std::make_pair(strKey, pSeq));
}

CAnimationSequence* CResourceManager::FindSequence(const std::string& strKey)
{
	std::unordered_map<std::string, CAnimationSequence*>::iterator iter = m_mapSequence.find(strKey);

	if (iter == m_mapSequence.end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}

bool CResourceManager::CreateSequence(const std::string& strKey, bool bLoop, int iStartFrame, int iEndFrame, float fPlayTime, const std::vector<_tagBoneKeyFrame*>& vecFrame)
{
	CAnimationSequence* pSeq = FindSequence(strKey);
	
	if (pSeq)
	{
		SAFE_RELEASE(pSeq);
		return false;
	}

	pSeq = new CAnimationSequence;

	if (!pSeq->Init())
	{
		SAFE_RELEASE(pSeq);
		return false;
	}

	if (!pSeq->CreateSequence(strKey, bLoop, iStartFrame, iEndFrame, fPlayTime, vecFrame))
	{
		SAFE_RELEASE(pSeq);
		return false;
	}

	m_mapSequence.insert(std::make_pair(strKey, pSeq));

	return true;
}

ID3D11SamplerState* CResourceManager::CreateSample(D3D11_FILTER eFilter)
{
	D3D11_SAMPLER_DESC tDesc = {};

	tDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	tDesc.MaxLOD = FLT_MAX;
	tDesc.MinLOD = -FLT_MAX;
	tDesc.Filter = eFilter;

	ID3D11SamplerState* pSampler = nullptr;

	DEVICE->CreateSamplerState(&tDesc, &pSampler);

	return pSampler;
}

void CResourceManager::CreateSamplerState(
	const std::string& strKey,
	D3D11_TEXTURE_ADDRESS_MODE eAddressU, 
	D3D11_TEXTURE_ADDRESS_MODE eAddressV, 
	D3D11_TEXTURE_ADDRESS_MODE eAddressW, 
	FLOAT pBorderColor[4],
	D3D11_COMPARISON_FUNC eCompareFunc, 
	FLOAT fMaxLod, FLOAT fMinLod, 
	D3D11_FILTER eFilter)
{
	ID3D11SamplerState* pState = FindSampler(strKey);

	if (pState)
	{
		SAFE_RELEASE(pState);
		return;
	}

	D3D11_SAMPLER_DESC tDesc = {};

	tDesc.AddressU = eAddressU;
	tDesc.AddressV = eAddressV;
	tDesc.AddressW = eAddressW;
	memcpy_s(tDesc.BorderColor, 16, pBorderColor, 16);
	tDesc.ComparisonFunc = eCompareFunc;
	tDesc.MaxLOD = fMaxLod;
	tDesc.MinLOD = fMinLod;
	tDesc.Filter = eFilter;

	if (FAILED(DEVICE->CreateSamplerState(&tDesc, &pState)))
		return;

	m_mapSamplerStates.insert(std::make_pair(strKey,pState));
}

ID3D11SamplerState* CResourceManager::FindSampler(const std::string& strKey)
{
	std::unordered_map<std::string, ID3D11SamplerState*>::iterator	iter = m_mapSamplerStates.find(strKey);

	if (iter == m_mapSamplerStates.end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}

void CResourceManager::SetSamplerState(const std::string& strKey, int iRegister, SHADER_CBUFFER_TYPE eType)
{
	ID3D11SamplerState* pState = FindSampler(strKey);

	if (!pState)
	{
		return;
	}

	if (static_cast<int>(eType) & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_VERTEX))
	{
		CONTEXT->VSSetSamplers(iRegister, 1, &pState);
	}
	if (static_cast<int>(eType) & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_PIXEL))
	{
		CONTEXT->PSSetSamplers(iRegister, 1, &pState);
	}
	if (static_cast<int>(eType) & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_HULL))
	{
		CONTEXT->HSSetSamplers(iRegister, 1, &pState);
	}
	if (static_cast<int>(eType) & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_DOMAIN))
	{
		CONTEXT->DSSetSamplers(iRegister, 1, &pState);
	}
	if (static_cast<int>(eType) & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY))
	{
		CONTEXT->GSSetSamplers(iRegister, 1, &pState);
	}
	if (static_cast<int>(eType) & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_COMPUTE))
	{
		CONTEXT->CSSetSamplers(iRegister, 1, &pState);
	}

	SAFE_RELEASE(pState);
}

CParticleSystem* CResourceManager::CreateParticle(const std::string& strTag, bool bDynamic, int iCount, int iSize,int iRegister, int iType)
{
	CParticleSystem* pParticle = FindParticle(strTag);

	if (pParticle)
	{
		SAFE_RELEASE(pParticle);
		return nullptr;
	}

	pParticle = new CParticleSystem;

	pParticle->SetName(strTag);

	if (!pParticle->Init( bDynamic, iCount, iSize, iRegister, iType))
	{
		SAFE_RELEASE(pParticle);
		return nullptr;
	}

	m_mapParticle.insert(std::make_pair(strTag, pParticle));

	pParticle->AddRef();

	return pParticle;
}

CParticleSystem* CResourceManager::FindParticle(const std::string& strTag)
{
	std::unordered_map<std::string, CParticleSystem*>::iterator iter = m_mapParticle.find(strTag);

	if (iter == m_mapParticle.end())
	{
		return nullptr;
	}

	iter->second->AddRef();

	return iter->second;
}

CSkeleton* CResourceManager::FindSkeleton(const std::string& strKey)
{
	std::unordered_map<std::string, class CSkeleton*>::iterator iter = m_mapSkeleton.find(strKey);

	if (iter == m_mapSkeleton.end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}

void CResourceManager::SetSkeleton(const std::string& strKey, CSkeleton* pSkel)
{
	class CSkeleton* _pSkel = FindSkeleton(strKey);

	if (_pSkel)
	{
		SAFE_RELEASE(_pSkel);
		return;
	}

	pSkel->AddRef();

	m_mapSkeleton.insert(std::make_pair(strKey, pSkel));

	return;
}

bool CResourceManager::LoadSkeleton(const std::string& strKey, const char* pFilePath, const std::string& strPathKey)
{
	CSkeleton* pSkel = FindSkeleton(strKey);

	if (pSkel)
	{
		SAFE_RELEASE(pSkel);
		return false;
	}

	pSkel = new CSkeleton;

	if (!pSkel->Load(strKey, pFilePath, strPathKey))
	{
		SAFE_RELEASE(pSkel);
		return false;
	}

	m_mapSkeleton.insert(std::make_pair(strKey, pSkel));

	return true;
}

ID3D11ShaderReflectionVariable* InputA;
ID3D11ShaderReflectionVariable* InputB;
ID3D11ShaderReflectionVariable* Output;
