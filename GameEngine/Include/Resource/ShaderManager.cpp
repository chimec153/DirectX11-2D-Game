#include "ShaderManager.h"
#include "Shader.h"
#include "GraphicShader.h"
#include "../Device.h"
#include "ComputeShader.h"
#include "../Camera/CameraManager.h"
#include "../Component/Camera.h"

DEFINITION_SINGLE(CShaderManager)

CShaderManager::CShaderManager()	:
	m_iInputSize(0)
	, m_pBuffer(nullptr)
	, m_pInputSRV(nullptr)
	, m_pOutputBuffer(nullptr)
	, m_pOutputUAV(nullptr)
{

}

CShaderManager::~CShaderManager()
{
	SAFE_RELEASE_MAP(m_mapShader);
	SAFE_RELEASE_MAP(m_mapLayout);

	auto iter = m_mapCBuffer.begin();
	auto iterEnd = m_mapCBuffer.end();

	for (; iter != iterEnd; ++iter)
		SAFE_RELEASE(iter->second->pBuffer);

	SAFE_DELETE_MAP(m_mapCBuffer);

	SAFE_RELEASE(m_pBuffer);
	SAFE_RELEASE(m_pInputSRV);
	SAFE_RELEASE(m_pOutputBuffer);
	SAFE_RELEASE(m_pOutputUAV);
}

bool CShaderManager::Init()
{
	//	gloval shader √ ±‚»≠
	CGraphicShader* pShader = CreateShader<CGraphicShader>("Standard2D");

	if (!LoadVertexShader("Standard2D", "StandardVS", TEXT("VertexShader.fx")))
		return false;

	if (!LoadPixelShader("Standard2D", "StandardPS", TEXT("VertexShader.fx")))
		return false;

	pShader->AddInputLayoutDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	pShader->AddInputLayoutDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	pShader->AddInputLayoutDesc("COLOR", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pShader->CreateInputLayout())
	{
		SAFE_RELEASE(pShader);
		return false;
	}

	SAFE_RELEASE(pShader);

	pShader = CreateShader<CGraphicShader>("Sprite");

	if (!LoadVertexShader("Sprite", "StandardSpriteVS", TEXT("VertexShader.fx")))
		return false;

	if (!LoadPixelShader("Sprite", "StandardSpritePS", TEXT("VertexShader.fx")))
		return false;

	pShader->AddInputLayoutDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	pShader->AddInputLayoutDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	pShader->AddInputLayoutDesc("COLOR", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pShader->CreateInputLayout())
	{
		SAFE_RELEASE(pShader);
		return false;
	}

	SAFE_RELEASE(pShader);

	pShader = CreateShader<CGraphicShader>("Collider");

	if (!LoadVertexShader("Collider", "ColliderVS", TEXT("Collider.fx")))
		return false;

	if (!LoadPixelShader("Collider", "ColliderPS", TEXT("Collider.fx")))
		return false;

	pShader->AddInputLayoutDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pShader->CreateInputLayout())
	{
		SAFE_RELEASE(pShader);
		return false;
	}

	SAFE_RELEASE(pShader);

	pShader = CreateShader<CGraphicShader>("UI2D");

	if (!LoadVertexShader("UI2D", "VS", TEXT("UI.fx")))
		return false;

	if (!LoadPixelShader("UI2D", "PS", TEXT("UI.fx")))
		return false;

	pShader->AddInputLayoutDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShader->AddInputLayoutDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShader->AddInputLayoutDesc("COLOR", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pShader->CreateInputLayout())
	{
		SAFE_RELEASE(pShader);
		return false;
	}

	SAFE_RELEASE(pShader);

	pShader = CreateShader<CGraphicShader>("Instance2D");

	if (!LoadVertexShader("Instance2D", "StandardVS", TEXT("VertexShader.fx")))
		return false;

	if (!LoadPixelShader("Instance2D", "StandardPS", TEXT("VertexShader.fx")))
		return false;

	pShader->AddInputLayoutDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_INSTANCE_DATA, 0);
	pShader->AddInputLayoutDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_INSTANCE_DATA, 0);
	pShader->AddInputLayoutDesc("WORLD", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pShader->AddInputLayoutDesc("WORLD", 1,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pShader->AddInputLayoutDesc("WORLD", 2,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pShader->AddInputLayoutDesc("WORLD", 3,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pShader->AddInputLayoutDesc("COLOR", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);

	if (!pShader->CreateInputLayout())
	{
		SAFE_RELEASE(pShader);
		return false;
	}

	SAFE_RELEASE(pShader);

	pShader = CreateShader<CGraphicShader>("Bar");

	if (!pShader->LoadVertexShader("BarVS", TEXT("UI.fx")))
	{
		SAFE_RELEASE(pShader);
		return false;
	}

	if (!pShader->LoadPixelShader("BarPS", TEXT("UI.fx")))
	{
		SAFE_RELEASE(pShader);
		return false;
	}

	pShader->AddInputLayoutDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShader->AddInputLayoutDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShader->AddInputLayoutDesc("COLOR", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pShader->CreateInputLayout())
	{
		SAFE_RELEASE(pShader);
		return false;
	}

	SAFE_RELEASE(pShader);

	pShader = CreateShader<CGraphicShader>("Inst");

	if (!pShader->LoadVertexShader("VS_INST", TEXT("Inst.fx")))
	{
		SAFE_RELEASE(pShader);
		return false;
	}

	if (!pShader->LoadPixelShader("PS_INST", TEXT("Inst.fx")))
	{
		SAFE_RELEASE(pShader);
		return false;
	}

	pShader->AddInputLayoutDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShader->AddInputLayoutDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShader->AddInputLayoutDesc("WORLD", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pShader->AddInputLayoutDesc("WORLD", 1,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pShader->AddInputLayoutDesc("WORLD", 2,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pShader->AddInputLayoutDesc("WORLD", 3,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pShader->AddInputLayoutDesc("PIVOT", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 1, 12,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pShader->AddInputLayoutDesc("SIZE", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 1, 12,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pShader->AddInputLayoutDesc("TEXSIZE", 0,
		DXGI_FORMAT_R32G32_FLOAT, 1, 8,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pShader->AddInputLayoutDesc("START", 0,
		DXGI_FORMAT_R32G32_FLOAT, 1, 8,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pShader->AddInputLayoutDesc("END", 0,
		DXGI_FORMAT_R32G32_FLOAT, 1, 8,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pShader->AddInputLayoutDesc("COLOR", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);

	if (!pShader->CreateInputLayout())
	{
		SAFE_RELEASE(pShader);
		return false;
	}

	SAFE_RELEASE(pShader);

	CGraphicShader* pInstCollider = CreateShader<CGraphicShader>("InstCollider");

	if (!pInstCollider->LoadVertexShader("VS_INST_COLLIDER", TEXT("Inst.fx")))
	{
		SAFE_RELEASE(pInstCollider);
		return false;
	}

	if (!pInstCollider->LoadPixelShader("PS_INST_COLLIDER", TEXT("Inst.fx")))
	{
		SAFE_RELEASE(pInstCollider);
		return false;
	}

	pInstCollider->AddInputLayoutDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	pInstCollider->AddInputLayoutDesc("WORLD", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pInstCollider->AddInputLayoutDesc("WORLD", 1,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pInstCollider->AddInputLayoutDesc("WORLD", 2,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pInstCollider->AddInputLayoutDesc("WORLD", 3,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pInstCollider->AddInputLayoutDesc("PIVOT", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 1, 12,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pInstCollider->AddInputLayoutDesc("SIZE", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 1, 12,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pInstCollider->AddInputLayoutDesc("COLOR", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
		D3D11_INPUT_PER_INSTANCE_DATA, 1);

	if (!pInstCollider->CreateInputLayout())
	{
		SAFE_RELEASE(pInstCollider);
		return false;
	}

	SAFE_RELEASE(pInstCollider);

	CGraphicShader* pShadow = CreateShader<CGraphicShader>("Shadow");

	if (!pShadow->LoadVertexShader("StandardSpriteVS", TEXT("VertexShader.fx")))
	{
		SAFE_RELEASE(pShadow);
		return false;
	}

	if (!pShadow->LoadPixelShader("ShadowPS", TEXT("VertexShader.fx")))
	{
		SAFE_RELEASE(pShadow);
		return false;
	}

	pShadow->AddInputLayoutDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShadow->AddInputLayoutDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShadow->AddInputLayoutDesc("COLOR", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pShadow->CreateInputLayout())
	{
		SAFE_RELEASE(pShadow);
		return false;
	}

	SAFE_RELEASE(pShadow);

	CGraphicShader* pParticle = CreateShader<CGraphicShader>("ParticleShader");

	if (!pParticle->LoadVertexShader("VS", TEXT("particle.fx")))
		return false;
	if (!pParticle->LoadGeometryShader("GS", TEXT("particle.fx")))
		return false;
	if (!pParticle->LoadPixelShader("PS", TEXT("particle.fx")))
		return false;

	pParticle->AddInputLayoutDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pParticle->AddInputLayoutDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pParticle->AddInputLayoutDesc("COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pParticle->CreateInputLayout())
		return false;

	SAFE_RELEASE(pParticle);

	CComputeShader* pComputeShader = CreateShader<CComputeShader>("ComputeShader");

	if (!pComputeShader->LoadComputeShader("ParticleMain", TEXT("Particle.fx")))
		return false;

	SAFE_RELEASE(pComputeShader);

	CGraphicShader* pCircle2CylinderShader = CreateShader<CGraphicShader>("C2CShader");

	if (!pCircle2CylinderShader->LoadVertexShader("VS", TEXT("Geo.fx")))
	{
		return false;
	}

	if (!pCircle2CylinderShader->LoadGeometryShader("GS", TEXT("Geo.fx")))
	{
		return false;
	}

	if (!pCircle2CylinderShader->LoadPixelShader("PS", TEXT("Geo.fx")))
	{
		return false;
	}

	pCircle2CylinderShader->AddInputLayoutDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	pCircle2CylinderShader->AddInputLayoutDesc("SIZE", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	pCircle2CylinderShader->AddInputLayoutDesc("COLOR", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pCircle2CylinderShader->CreateInputLayout())
	{
		return false;
	}

	SAFE_RELEASE(pCircle2CylinderShader);

	CGraphicShader* pLineShader = CreateShader<CGraphicShader>("LineShader");

	if (!pLineShader->LoadVertexShader("LineVS", L"Line.fx"))
	{
		return false;
	}

	if (!pLineShader->LoadGeometryShader("LineGS", L"Line.fx"))
	{
		return false;
	}

	if (!pLineShader->LoadPixelShader("LinePS", L"Line.fx"))
	{
		return false;
	}

	pLineShader->AddInputLayoutDesc("POSITION", 
		0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 12, 
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pLineShader->CreateInputLayout())
		return false;

	SAFE_RELEASE(pLineShader);

	D3D11_SO_DECLARATION_ENTRY pEntry[5] = {};

	pEntry[0].SemanticName = "POSITION";
	pEntry[0].SemanticIndex = 0;
	pEntry[0].StartComponent = 0;
	pEntry[0].ComponentCount = 3;
	pEntry[0].OutputSlot = 0;
	pEntry[0].Stream = 0;

	pEntry[1].SemanticName = "VELOCITY";
	pEntry[1].SemanticIndex = 0;
	pEntry[1].StartComponent = 0;
	pEntry[1].ComponentCount = 3;
	pEntry[1].OutputSlot = 0;
	pEntry[1].Stream = 0;

	pEntry[2].SemanticName = "SIZE";
	pEntry[2].SemanticIndex = 0;
	pEntry[2].StartComponent = 0;
	pEntry[2].ComponentCount = 2;
	pEntry[2].OutputSlot = 0;
	pEntry[2].Stream = 0;

	pEntry[3].SemanticName = "AGE";
	pEntry[3].SemanticIndex = 0;
	pEntry[3].StartComponent = 0;
	pEntry[3].ComponentCount = 1;
	pEntry[3].OutputSlot = 0;
	pEntry[3].Stream = 0;

	pEntry[4].SemanticName = "TYPE";
	pEntry[4].SemanticIndex = 0;
	pEntry[4].StartComponent = 0;
	pEntry[4].ComponentCount = 1;
	pEntry[4].OutputSlot = 0;
	pEntry[4].Stream = 0;

	CGraphicShader* pAngleShader = CreateShader<CGraphicShader>("AngleShader");

	if (!pAngleShader->LoadVertexShader("VS", L"Line.fx"))
		return false;
	if (!pAngleShader->LoadPixelShader("PS", L"Line.fx"))
		return false;

	pAngleShader->AddInputLayoutDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pAngleShader->AddInputLayoutDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pAngleShader->CreateInputLayout())
		return false;

	SAFE_RELEASE(pAngleShader);

	CGraphicShader* pGray = CreateShader<CGraphicShader>("GrayShader");

	if (!pGray->LoadVertexShader("StandardSpriteVS", TEXT("VertexShader.fx")))
		return false;
	if (!pGray->LoadPixelShader("GrayPS", TEXT("VertexShader.fx")))
		return false;

	pGray->AddInputLayoutDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pGray->AddInputLayoutDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pGray->AddInputLayoutDesc("COLOR", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pGray->CreateInputLayout())
		return false;

	SAFE_RELEASE(pGray);

	CGraphicShader* p3D = CreateShader<CGraphicShader>("Standard3DShader");

	if (!p3D->LoadVertexShader("VS", TEXT("3D.fx")))
		return false;

	if (!p3D->LoadPixelShader("PS", TEXT("3D.fx")))
		return false;

	p3D->AddInputLayoutDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3D->AddInputLayoutDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3D->AddInputLayoutDesc("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3D->AddInputLayoutDesc("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3D->AddInputLayoutDesc("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3D->AddInputLayoutDesc("BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3D->AddInputLayoutDesc("BLENDINDEX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!p3D->CreateInputLayout())
		return false;

	SAFE_RELEASE(p3D);

	p3D = CreateShader<CGraphicShader>("Single3DShader");

	if (!p3D->LoadVertexShader("VS", TEXT("3D.fx")))
		return false;

	if (!p3D->LoadPixelShader("SinglePS", TEXT("3D.fx")))
		return false;

	p3D->AddInputLayoutDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3D->AddInputLayoutDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3D->AddInputLayoutDesc("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3D->AddInputLayoutDesc("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3D->AddInputLayoutDesc("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3D->AddInputLayoutDesc("BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0);
	p3D->AddInputLayoutDesc("BLENDINDEX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!p3D->CreateInputLayout())
		return false;

	SAFE_RELEASE(p3D);

	CGraphicShader* pPaper2D = CreateShader<CGraphicShader>("PaperBurn2D");

	if (!pPaper2D->LoadVertexShader("PaperVS", TEXT("Post.fx")))
		return false;

	if (!pPaper2D->LoadPixelShader("PaperPS", TEXT("Post.fx")))
		return false;

	pPaper2D->AddInputLayoutDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	pPaper2D->AddInputLayoutDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pPaper2D->CreateInputLayout())
		return false;

	SAFE_RELEASE(pPaper2D);

	CGraphicShader* pFull = CreateShader<CGraphicShader>("Full");

	if (!pFull->LoadVertexShader("FullVS", TEXT("VertexShader.fx")))
		return false;

	if (!pFull->LoadPixelShader("FullPS", TEXT("VertexShader.fx")))
		return false;

	pFull->AddInputLayoutDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	pFull->AddInputLayoutDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pFull->CreateInputLayout())
		return false;

	SAFE_RELEASE(pFull);

	CGraphicShader* pDebug = CreateShader<CGraphicShader>("Debug");

	if (!pDebug->LoadVertexShader("DebugVS", TEXT("VertexShader.fx")))
		return false;

	if (!pDebug->LoadPixelShader("DebugPS", TEXT("VertexShader.fx")))
		return false;

	pDebug->AddInputLayoutDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	pDebug->AddInputLayoutDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pDebug->CreateInputLayout())
		return false;

	SAFE_RELEASE(pDebug);

	CGraphicShader* pDist = CreateShader<CGraphicShader>("DistortShader");

	if (!pDist->LoadVertexShader("DistortVS", TEXT("Post.fx")))
		return false;

	if (!pDist->LoadPixelShader("DistortPS", TEXT("Post.fx")))
		return false;

	pDist->AddInputLayoutDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	pDist->AddInputLayoutDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pDist->CreateInputLayout())
		return false;

	SAFE_RELEASE(pDist);

	CGraphicShader* pTest = CreateShader<CGraphicShader>("Test");

	if (!pTest->LoadVertexShader("VS", TEXT("Test.fx")))
		return false;
	if (!pTest->LoadPixelShader("PS", TEXT("Test.fx")))
		return false;

	pTest->AddInputLayoutDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pTest->AddInputLayoutDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pTest->AddInputLayoutDesc("NORMAL", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pTest->CreateInputLayout())
		return false;

	SAFE_RELEASE(pTest);

	CGraphicShader* pLightAcc = CreateShader<CGraphicShader>("LightAcc");

	if (!pLightAcc->LoadVertexShader("VS", TEXT("Light.fx")))
		return false;
	if (!pLightAcc->LoadPixelShader("PS", TEXT("Light.fx")))
		return false;

	pLightAcc->AddInputLayoutDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pLightAcc->CreateInputLayout())
		return false;
	SAFE_RELEASE(pLightAcc);

	CGraphicShader* pLightCombine = CreateShader<CGraphicShader>("LightCombine");

	if (!pLightCombine->LoadVertexShader("VS", TEXT("Light.fx")))
		return false;
	if (!pLightCombine->LoadPixelShader("LightCombinePS", TEXT("Light.fx")))
		return false;

	pLightCombine->AddInputLayoutDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pLightCombine->CreateInputLayout())
		return false;

	SAFE_RELEASE(pLightCombine);

	CComputeShader* pAni = CreateShader<CComputeShader>("AniUpdate");

	if (!pAni->LoadComputeShader("CS", TEXT("Animation.fx")))
		return false;

	SAFE_RELEASE(pAni);

	CGraphicShader* pTerrainShader = CreateShader<CGraphicShader>("TerrainShader");

	if (!pTerrainShader->LoadVertexShader("VS", TEXT("3D.fx")))
		return false;
	if (!pTerrainShader->LoadPixelShader("PS", TEXT("Terrain.fx")))
		return false;

	pTerrainShader->AddInputLayoutDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	pTerrainShader->AddInputLayoutDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0);
	pTerrainShader->AddInputLayoutDesc("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	pTerrainShader->AddInputLayoutDesc("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	pTerrainShader->AddInputLayoutDesc("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	pTerrainShader->AddInputLayoutDesc("BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0);
	pTerrainShader->AddInputLayoutDesc("BLENDINDEX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pTerrainShader->CreateInputLayout())
		return false;

	SAFE_RELEASE(pTerrainShader);

	pTerrainShader = CreateShader<CGraphicShader>("TerrainArrayShader");

	if (!pTerrainShader->LoadVertexShader("VS", TEXT("3D.fx")))
		return false;
	if (!pTerrainShader->LoadPixelShader("ArrayPS", TEXT("Terrain.fx")))
		return false;

	pTerrainShader->AddInputLayoutDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	pTerrainShader->AddInputLayoutDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0);
	pTerrainShader->AddInputLayoutDesc("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	pTerrainShader->AddInputLayoutDesc("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	pTerrainShader->AddInputLayoutDesc("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	pTerrainShader->AddInputLayoutDesc("BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0);
	pTerrainShader->AddInputLayoutDesc("BLENDINDEX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pTerrainShader->CreateInputLayout())
		return false;

	SAFE_RELEASE(pTerrainShader);

	CGraphicShader* pDecal = CreateShader<CGraphicShader>("DecalShader");

	if (!pDecal->LoadVertexShader("VS", L"Decal.fx"))
		return false;
	if (!pDecal->LoadPixelShader("PS", L"Decal.fx"))
		return false;

	pDecal->AddInputLayoutDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pDecal->CreateInputLayout())
		return false;

	SAFE_RELEASE(pDecal);

	CGraphicShader* pCubeMap = CreateShader<CGraphicShader>("CubeMapShader");

	if (!pCubeMap->LoadVertexShader("VS", L"Cube.fx"))
		return false;
	if (!pCubeMap->LoadPixelShader("PS", L"Cube.fx"))
		return false;

	pCubeMap->AddInputLayoutDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pCubeMap->AddInputLayoutDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pCubeMap->AddInputLayoutDesc("NORMAL", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pCubeMap->AddInputLayoutDesc("TANGENT", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pCubeMap->AddInputLayoutDesc("BINORMAL", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pCubeMap->AddInputLayoutDesc("BLENDWEIGHT", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pCubeMap->AddInputLayoutDesc("BLENDINDEX", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pCubeMap->CreateInputLayout())
		return false;

	SAFE_RELEASE(pCubeMap);

	CGraphicShader* pInst3D = CreateShader<CGraphicShader>("Inst3D");

	if (!pInst3D->LoadVertexShader("VS_Inst_3D", L"Inst.fx"))
	{
		SAFE_RELEASE(pInst3D);
		return false;
	}
	if (!pInst3D->LoadPixelShader("PS", L"3D.fx"))
	{
		SAFE_RELEASE(pInst3D);
		return false;
	}

	pInst3D->AddInputLayoutDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	pInst3D->AddInputLayoutDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0);
	pInst3D->AddInputLayoutDesc("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	pInst3D->AddInputLayoutDesc("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	pInst3D->AddInputLayoutDesc("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	pInst3D->AddInputLayoutDesc("BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0);
	pInst3D->AddInputLayoutDesc("BLENDINDEX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0);
	pInst3D->AddInputLayoutDesc("WV", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pInst3D->AddInputLayoutDesc("WV", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pInst3D->AddInputLayoutDesc("WV", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pInst3D->AddInputLayoutDesc("WV", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pInst3D->AddInputLayoutDesc("WVP", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pInst3D->AddInputLayoutDesc("WVP", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pInst3D->AddInputLayoutDesc("WVP", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pInst3D->AddInputLayoutDesc("WVP", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pInst3D->AddInputLayoutDesc("LIGHTWVP", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pInst3D->AddInputLayoutDesc("LIGHTWVP", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pInst3D->AddInputLayoutDesc("LIGHTWVP", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pInst3D->AddInputLayoutDesc("LIGHTWVP", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1);

	if (!pInst3D->CreateInputLayout())
	{
		SAFE_RELEASE(pInst3D);
		return false;
	}

	SAFE_RELEASE(pInst3D);

	CGraphicShader* pShadow3D = CreateShader<CGraphicShader>("ShadowShader");

	if (!pShadow3D->LoadVertexShader("VS", L"Shadow.fx"))
	{
		SAFE_RELEASE(pShadow3D);
		return false;
	}
	if (!pShadow3D->LoadPixelShader("PS", L"Shadow.fx"))
	{
		SAFE_RELEASE(pShadow3D);
		return false;
	}
	pShadow3D->AddInputLayoutDesc("POSITION", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShadow3D->AddInputLayoutDesc("TEXCOORD", 0,
		DXGI_FORMAT_R32G32_FLOAT, 0, 8,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShadow3D->AddInputLayoutDesc("NORMAL", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShadow3D->AddInputLayoutDesc("TANGENT", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShadow3D->AddInputLayoutDesc("BINORMAL", 0,
		DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShadow3D->AddInputLayoutDesc("BLENDWEIGHT", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);
	pShadow3D->AddInputLayoutDesc("BLENDINDEX", 0,
		DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16,
		D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!pShadow3D->CreateInputLayout())
	{
		SAFE_RELEASE(pShadow3D);
		return false;
	}

	SAFE_RELEASE(pShadow3D);

	CGraphicShader* pInstShadow = CreateShader<CGraphicShader>("InstShadowShader");

	if (!pInstShadow->LoadVertexShader("InstVS", L"Shadow.fx"))
	{
		SAFE_RELEASE(pInstShadow);
		return false;
	}
	if (!pInstShadow->LoadPixelShader("PS", L"Shadow.fx"))
	{
		SAFE_RELEASE(pInstShadow);
		return false;
	}

	pInstShadow->AddInputLayoutDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	pInstShadow->AddInputLayoutDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
		0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0);
	pInstShadow->AddInputLayoutDesc("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	pInstShadow->AddInputLayoutDesc("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	pInstShadow->AddInputLayoutDesc("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	pInstShadow->AddInputLayoutDesc("BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
		0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0);
	pInstShadow->AddInputLayoutDesc("BLENDINDEX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
		0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0);
	pInstShadow->AddInputLayoutDesc("WV", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
		1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pInstShadow->AddInputLayoutDesc("WV", 1, DXGI_FORMAT_R32G32B32A32_FLOAT,
		1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pInstShadow->AddInputLayoutDesc("WV", 2, DXGI_FORMAT_R32G32B32A32_FLOAT,
		1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pInstShadow->AddInputLayoutDesc("WV", 3, DXGI_FORMAT_R32G32B32A32_FLOAT,
		1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pInstShadow->AddInputLayoutDesc("WVP", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
		1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pInstShadow->AddInputLayoutDesc("WVP", 1, DXGI_FORMAT_R32G32B32A32_FLOAT,
		1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pInstShadow->AddInputLayoutDesc("WVP", 2, DXGI_FORMAT_R32G32B32A32_FLOAT,
		1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pInstShadow->AddInputLayoutDesc("WVP", 3, DXGI_FORMAT_R32G32B32A32_FLOAT,
		1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pInstShadow->AddInputLayoutDesc("LIGHTWVP", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
		1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pInstShadow->AddInputLayoutDesc("LIGHTWVP", 1, DXGI_FORMAT_R32G32B32A32_FLOAT,
		1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pInstShadow->AddInputLayoutDesc("LIGHTWVP", 2, DXGI_FORMAT_R32G32B32A32_FLOAT,
		1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1);
	pInstShadow->AddInputLayoutDesc("LIGHTWVP", 3, DXGI_FORMAT_R32G32B32A32_FLOAT,
		1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1);

	if (!pInstShadow->CreateInputLayout())
	{
		SAFE_RELEASE(pInstShadow);
		return false;
	}

	SAFE_RELEASE(pInstShadow);

	CreateCBuffer("Transform", sizeof(TransformCBuffer), 0, 
		(int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX
		|(int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY
		|static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_PIXEL));
	CreateCBuffer("Material", sizeof(ShaderCBuffer), 1,
		static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_VERTEX)
		|static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_PIXEL));
	CreateCBuffer("Sprite", sizeof(SpriteCBuffer), 2, 
		(int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX
		| (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL
		|(int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY);
	CreateCBuffer("Global", sizeof(GLOBALCBUFFER), 3,
		static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_VERTEX)
		| static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_COMPUTE));
	CreateCBuffer("Paper", sizeof(PAPERCBUFFER), 4, 
		static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_PIXEL));
	CreateCBuffer("Distortion", sizeof(DISTORTIONCBUFFER), 5, 
		static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_VERTEX) |
		static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_PIXEL));
	CreateCBuffer("Decal", sizeof(DECALCBUFFER), 6);
	CreateCBuffer("Light", sizeof(LIGHTCBUFFER), 7,
		static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_PIXEL));
	CreateCBuffer("Terrain", sizeof(TERRAINCBUFFER), 8,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);
	CreateCBuffer("Animation", sizeof(ANIMATIONCBUFFER), 9,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_COMPUTE | 
		static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_VERTEX));
	CreateCBuffer("Bar", sizeof(BARCBUFFER), 10,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);
	CreateCBuffer("Particle", sizeof(PARTICLECBUFFER), 11,
		(int)SHADER_CBUFFER_TYPE::CBUFFER_ALL);

	CreateCBuffer("Fade", sizeof(FADE), 10);

	CreateCBuffer("Angle", sizeof(ANGLE), 12, (int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY);

	FADE tFade = {};

	tFade.vFade = Vector4(1.f, 1.f, 1.f, 1.f);
	tFade.fGray = 0.f;

	UpdateCBuffer("Fade", (void*)&tFade);
	 struct _test
	{
		Vector4 ambDown;
		Vector4 ambRange;
		Vector4 lightDir;
		Vector4 lightColor;
	};

	CreateCBuffer("Hemi", sizeof(_test), 13);
	_test tHemi = {};
	tHemi.ambDown = Vector4(0.f, 0.5f, 0.f, 0.f);
	tHemi.ambRange = Vector4(1.f, 0.5f, 1.f, 0.f);
	tHemi.lightDir = Vector4(0.5f, -0.5f, 0.5f, 0.f);
	tHemi.lightColor = Vector4(0.8f, 0.f, 0.8f, 0.f);
	UpdateCBuffer("Hemi", &tHemi);
	 struct _tagCam
	{
		Vector4 EyePos;
	};

	_tagCam cam = {};

	CreateCBuffer("Cam", sizeof(_tagCam), 12);
	UpdateCBuffer("Cam", &cam);

	return true;
}

void CShaderManager::Update(float fTime)
{
	struct _tagCam
	{
		Vector4 EyePos;
	};

	_tagCam cam = {};

	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

	Vector3 Pos = pCam->GetWorldPos();

	memcpy_s(&cam.EyePos.x, 16, &Pos.x, 12);

	SAFE_RELEASE(pCam);

	UpdateCBuffer("Cam", &cam);
}

bool CShaderManager::CreateLayout(const std::string& strName)
{
	CShader* pShader = FindShader(strName);

	if (!pShader)
		return false;

	bool bResult = pShader->CreateInputLayout();

	SAFE_RELEASE(pShader);

	return bResult;
}

bool CShaderManager::CreateInputDesc(const std::string& strName, const char* pSemanticName, UINT iSemanticIndex, 
	DXGI_FORMAT eFormat, UINT iInputSlot, UINT iSize, 
	D3D11_INPUT_CLASSIFICATION eInputSlotClass, UINT iInstanceDataStepRate)
{
	CShader* pShader = FindShader(strName);

	if (!pShader)
		return false;

	bool bResult = pShader->AddInputLayoutDesc(pSemanticName, iSemanticIndex,
		eFormat, iInputSlot, iSize, eInputSlotClass, iInstanceDataStepRate);

	SAFE_RELEASE(pShader);

	return bResult;
}

CShader* CShaderManager::FindShader(const std::string& strName)
{
	std::unordered_map<std::string, CShader*>::iterator iter = m_mapShader.find(strName);

	if (iter == m_mapShader.end())
		return nullptr;		

	iter->second->AddRef();

	return iter->second;
}

void CShaderManager::ReleaseShader(const std::string& strName)
{
	std::unordered_map<std::string, CShader*>::iterator iter = m_mapShader.find(strName);

	if (iter == m_mapShader.end())
		return;

	if (iter->second->Release() == 0)
		m_mapShader.erase(iter);
}

ID3D11InputLayout* CShaderManager::FindLayout(const std::string& strName)
{
	std::unordered_map<std::string, ID3D11InputLayout*>::iterator iter = m_mapLayout.find(strName);

	if (iter == m_mapLayout.end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}

void CShaderManager::ReleaseLayout(const std::string& strName)
{
	std::unordered_map<std::string, ID3D11InputLayout*>::iterator iter = m_mapLayout.find(strName);

	if (iter == m_mapLayout.end())
		return;

	iter->second->Release();
	m_mapLayout.erase(iter);
}

bool CShaderManager::CreateCBuffer(const std::string& strTag, size_t iSize, int iRegister, int iType)
{
	PCBuffer pBuffer = FindCBuffer(strTag);

	if (pBuffer)
		return false;

	pBuffer = new CBuffer;

	pBuffer->iSize = (int)iSize;
	pBuffer->iType = iType;
	pBuffer->iRegister = iRegister;

	D3D11_BUFFER_DESC tDesc = {};

	tDesc.ByteWidth = (UINT)iSize;
	tDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	tDesc.Usage = D3D11_USAGE_DYNAMIC;
	tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, nullptr, &pBuffer->pBuffer)))
	{
		SAFE_DELETE(pBuffer);	
		return false;
	}	

	m_mapCBuffer.insert(std::make_pair(strTag, pBuffer));

	return true;
}

bool CShaderManager::UpdateCBuffer(const std::string& strTag, void* pData)
{
	PCBuffer pBuffer = FindCBuffer(strTag);

	if (!pBuffer)
		return false;

	D3D11_MAPPED_SUBRESOURCE tMap = {};

	CONTEXT->Map(pBuffer->pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tMap);

	memcpy(tMap.pData, pData, pBuffer->iSize);	

	CONTEXT->Unmap(pBuffer->pBuffer, 0);

	if (pBuffer->iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX)
		CONTEXT->VSSetConstantBuffers(pBuffer->iRegister, 1, &pBuffer->pBuffer);

	if (pBuffer->iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL)
		CONTEXT->PSSetConstantBuffers(pBuffer->iRegister, 1, &pBuffer->pBuffer);

	if (pBuffer->iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_HULL)
		CONTEXT->HSSetConstantBuffers(pBuffer->iRegister, 1, &pBuffer->pBuffer);

	if (pBuffer->iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_DOMAIN)
		CONTEXT->DSSetConstantBuffers(pBuffer->iRegister, 1, &pBuffer->pBuffer);

	if (pBuffer->iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY)
		CONTEXT->GSSetConstantBuffers(pBuffer->iRegister, 1, &pBuffer->pBuffer);

	if (pBuffer->iType & (int)SHADER_CBUFFER_TYPE::CBUFFER_COMPUTE)
		CONTEXT->CSSetConstantBuffers(pBuffer->iRegister, 1, &pBuffer->pBuffer);

	return true;
}

PCBuffer CShaderManager::FindCBuffer(const std::string& strTag)
{
	std::unordered_map<std::string, PCBuffer>::iterator iter = m_mapCBuffer.find(strTag);

	if (iter == m_mapCBuffer.end())
		return nullptr;

	return iter->second;
}


bool CShaderManager::LoadVertexShader(const std::string& strName, const char* pEntryName, const TCHAR* pFileName, const std::string& strRootPath)
{
	CShader* pShader = FindShader(strName);

	if (!pShader)
		return false;

	else if (pShader->GetShaderType() == SHADER_TYPE::ST_COMPUTE)
	{
		SAFE_RELEASE(pShader);
		return false;
	}

	bool bResult = ((CGraphicShader*)pShader)->LoadVertexShader(pEntryName, pFileName, strRootPath);

	SAFE_RELEASE(pShader);

	return bResult;
}

bool CShaderManager::LoadPixelShader(const std::string& strName, const char* pEntryName, const TCHAR* pFileName, const std::string& strRootPath)
{
	CShader* pShader = FindShader(strName);

	if (!pShader)
		return false;

	else if (pShader->GetShaderType() == SHADER_TYPE::ST_COMPUTE)
	{
		SAFE_RELEASE(pShader);
		return false;
	}

	bool bResult = ((CGraphicShader*)pShader)->LoadPixelShader(pEntryName, pFileName, strRootPath);

	SAFE_RELEASE(pShader);

	return bResult;
}

bool CShaderManager::LoadHullShader(const std::string& strName, const char* pEntryName, const TCHAR* pFileName, const std::string& strRootPath)
{
	CShader* pShader = FindShader(strName);

	if (!pShader)
		return false;

	else if (pShader->GetShaderType() == SHADER_TYPE::ST_COMPUTE)
	{
		SAFE_RELEASE(pShader);
		return false;
	}

	bool bResult = ((CGraphicShader*)pShader)->LoadHullShader(pEntryName, pFileName, strRootPath);

	SAFE_RELEASE(pShader);

	return bResult;
}

bool CShaderManager::LoadGeometryShader(const std::string& strName, const char* pEntryName, const TCHAR* pFileName, const std::string& strRootPath)
{
	CShader* pShader = FindShader(strName);

	if (!pShader)
		return false;

	else if (pShader->GetShaderType() == SHADER_TYPE::ST_COMPUTE)
	{
		SAFE_RELEASE(pShader);
		return false;
	}

	bool bResult = ((CGraphicShader*)pShader)->LoadGeometryShader(pEntryName, pFileName, strRootPath);

	SAFE_RELEASE(pShader);

	return bResult;
}

bool CShaderManager::LoadGeometryShaderWithSO(const std::string& strName, const char* pEntryName, 
	D3D11_SO_DECLARATION_ENTRY* pEntry, UINT iNumEntries, UINT iStride, UINT iNumStrides, 
	const TCHAR* pFileName, const std::string& strRootName)
{
	CShader* pShader = FindShader(strName);

	if (!pShader)
		return false;

	else if (pShader->GetShaderType() == SHADER_TYPE::ST_COMPUTE)
	{
		SAFE_RELEASE(pShader);
		return false;
	}

	bool bResult = ((CGraphicShader*)pShader)->LoadGeometryShaderWithSO(pEntryName, pEntry, 
		iNumEntries, iStride, iNumStrides, pFileName, strRootName);

	SAFE_RELEASE(pShader);

	return bResult;
}

bool CShaderManager::LoadDomainShader(const std::string& strName, const char* pEntryName, const TCHAR* pFileName, const std::string& strRootPath)
{
	CShader* pShader = FindShader(strName);

	if (!pShader)
		return false;

	else if (pShader->GetShaderType() == SHADER_TYPE::ST_COMPUTE)
	{
		SAFE_RELEASE(pShader);
		return false;
	}

	bool bResult = ((CGraphicShader*)pShader)->LoadDomainShader(pEntryName, pFileName, strRootPath);

	SAFE_RELEASE(pShader);

	return bResult;
}
