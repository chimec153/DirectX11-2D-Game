#include "TileMode.h"
#include "../../tinyxml.h"
#include "PathManager.h"
#include "Scene/Scene.h"
#include "Tile/TileMap.h"
#include "Resource/ResourceManager.h"
#include "Component/Camera.h"
#include "Camera/CameraManager.h"
#include "Component/Sound.h"
#include "SoundManager.h"
#include "Component/ColliderRect.h"
#include "Scene/SceneManager.h"
#include "ColossusMode.h"
#include "MainGameMode.h"
#include "../Object/Colossus.h"
#include "../Object/Player.h"
#include "../Object/Effect.h"
#include "Component/SpriteComponent.h"
#include "Resource/Material.h"
#include "../UI/Missile.h"
#include "../Object/Bullet.h"
#include "SoundManager.h"
#include "Component/Renderer.h"
#include "Component/Mesh2DComponent.h"
#include "Device.h"
#include "Component/Particle.h"
#include "../Object/Soul.h"
#include "../Object/Arrow.h"
#include "../Object/Bow.h"
#include "../Object/BrainFreeze.h"
#include "../Object/Eyecube.h"
#include "Engine.h"
#include "../Object/TitanFont.h"

TILE_TEX_TYPE CTileMode::m_eTexType = TILE_TEX_TYPE::TEX;
const char* CTileMode::m_pTex[(int)TILE_TEX_TYPE::END] = {};

CTileMode::CTileMode() :
	m_fShakeTime(0.f),
	m_fShakeLimit(0.25f),
	m_vPrevCamPos(),
	m_iShake(1),
	m_fAmplitude(5.f),
	m_fDist(0.f),
	m_fSpeed(150.f),
	m_fFade(0.f),
	m_bLock(false),
	m_pCam(nullptr),
	m_pFadeObj(nullptr),
	m_pFade(nullptr),
	m_fFadeStart(0.f),
	m_fFadeEnd(0.f),
	m_fFadeScale(0.f),
	m_bFade(false),
	m_bHub(false),
	m_eType(BOSS_TYPE::NONE),
	m_vShakeDir(0.f, 1.f, 0.f)
{
	m_vDoorDir = Vector3(0.f, -1.f, 0.f);
}

CTileMode::~CTileMode()
{
	SAFE_RELEASE(m_pCam);
	SAFE_RELEASE(m_pFadeObj);
	SAFE_RELEASE(m_pFade);
}

void CTileMode::Lock(bool bLock)
{
	m_bLock = bLock;

	CLayer* pLayer = m_pScene->FindLayer("Default");

	if (pLayer)
	{

		CObj* pObj = pLayer->FindObj("internal");

		if (pObj)
		{
			pObj->Enable(!m_bLock);

			pObj->Release();
		}
	}
}

bool CTileMode::Init()
{
	GET_SINGLE(CSoundManager)->Stop("Hub1");

	m_pScene->SortInstText();

	CLayer* pLayer = m_pScene->FindLayer("Default");

	CPlayer* pPlayer = m_pScene->CreateObject<CPlayer>("player", pLayer);

	pPlayer->SetWorldPos(488.f, 500.f, 0.f);

	SetPlayer(pPlayer);

	CBow* pBow = m_pScene->CreateObject<CBow>("Bow", pLayer);

	CArrow* pArrow = m_pScene->CreateObject<CArrow>("Arrow", pLayer);

	pPlayer->SetBow(pBow);
	pPlayer->SetArrow(pArrow);

	SAFE_RELEASE(pBow);
	SAFE_RELEASE(pArrow);

	SAFE_RELEASE(pPlayer);

	CEffect* pChargeEffect = CScene::CreateProtoObj<CEffect>("Charge", m_pScene, m_pScene->GetSceneType());

	CSpriteComponent* pCharge = pChargeEffect->CreateComponent<CSpriteComponent>("Charge", m_pScene->FindLayer("Default"));

	pCharge->CreateSprite("Charge", "Charge", LOOP_OPTION::ONCE_DESTROY);

	pCharge->SetPivot(0.5f, 0.5f, 0.f);
	pCharge->SetRelativeScale(16.f, 16.f, 1.f);
	pCharge->AddNotify("Charge", "FireEnd", 1.f);

	pChargeEffect->SetRootComponent(pCharge);
	pChargeEffect->SetSpeed(0.f);

	SAFE_RELEASE(pCharge);

	SAFE_RELEASE(pChargeEffect);

	CEffect* pWaterEffect = CScene::CreateProtoObj<CEffect>("Water", m_pScene, m_pScene->GetSceneType());

	CSpriteComponent* pWater = pWaterEffect->CreateComponent<CSpriteComponent>("Water");

	pWater->CreateSprite("Water", "Water", LOOP_OPTION::LOOP);

	pWater->SetPivot(0.5f, 0.5f, 0.f);
	pWater->SetRelativeScale(32.f, 16.f, 1.f);
	pWater->AddNotify("Water", "Water", 1.f);
	pWater->AddCallBack<CEffect>("Water", "Water", pWaterEffect, &CEffect::Destroy);
	pWater->AddRenderState("AlphaBlend");

	CMaterial* pMtrl = pWater->GetMaterial();

	pMtrl->SetDiffuseColor(1.f, 1.f, 1.f, 0.3f);

	SAFE_RELEASE(pMtrl);

	pWaterEffect->SetRootComponent(pWater);
	pWaterEffect->SetSpeed(0.f);
	pWaterEffect->SetScaleSpeed(Vector3(16.f, 8.f, 0.f));

	SAFE_RELEASE(pWater);

	SAFE_RELEASE(pWaterEffect);

	CSoul* pSoulEffect = CScene::CreateProtoObj<CSoul>("Soul", m_pScene, m_pScene->GetSceneType());

	SAFE_RELEASE(pSoulEffect);

	CMissile* pMissile = CScene::CreateProtoObj<CMissile>("Missile", m_pScene, m_pScene->GetSceneType());

	SAFE_RELEASE(pMissile);

	CBullet* pBullet = CScene::CreateProtoObj<CBullet>("Bullet", m_pScene, m_pScene->GetSceneType());

	SAFE_RELEASE(pBullet);

	m_pCam = GET_SINGLE(CCameraManager)->GetMainCam();

	m_pFadeObj = m_pScene->CreateObject<CObj>("Fade", m_pScene->FindLayer("Fore"));

	m_pFade = m_pFadeObj->CreateComponent<CMesh2DComponent>("FadeCom", m_pScene->FindLayer("Speacial"));

	Resolution tRS = RESOLUTION;

	m_pFade->SetWorldScale((float)tRS.iWidth, (float)tRS.iHeight, 0.f);
	m_pFade->SetPivot(0.5f, 0.5f, 0.f);
	//m_pFade->SetTexture(REGISTER_TYPE::RT_DIF, "Pro");	
	//m_pFade->SetTexture(REGISTER_TYPE::RT_DIF, "util", TEXT("IMG\\UTIL\\util.png"), DATA_PATH, 0, 1);
	m_pFade->SetWorldPos(0.f, 0.f, -520.f);
	m_pFade->AddRenderState("DepthNoWrite");
	m_pFade->AddRenderState("NoCullBack");

	CMaterial* pFadeMtrl = m_pFade->GetMaterial();

	pFadeMtrl->SetDiffuseColor(0.f, 0.f, 0.f, 0.f);
	//pFadeMtrl->SetGray(false);

	SAFE_RELEASE(pFadeMtrl);

	m_pFadeObj->SetRootComponent(m_pFade);

	CTitanFont* pFont = m_pScene->CreateObject<CTitanFont>("font", m_pScene->FindLayer("UI"), m_pScene->GetSceneType());

	pFont->SetText("GOL-IATH", 16.f);
	pFont->SetWorldPos((float)tRS.iWidth / 2.f, (float)tRS.iHeight / 3.f, 0.f);
	pFont->Enable(false);

	SAFE_RELEASE(pFont);

	CTitanFont* pDesc = m_pScene->CreateObject<CTitanFont>("desc", m_pScene->FindLayer("UI"), m_pScene->GetSceneType());

	pDesc->SetText("GUARDIAN~OF~THE~FIRST~GATE", 8.f, Vector4(0.5f, 0.5f, 0.5f, 1.f));
	pDesc->SetWorldPos((float)tRS.iWidth / 2.f, (float)tRS.iHeight / 3.f - 16.f, 0.f);
	pDesc->SetWorldScale(8.f, 8.f, 0.f);
	pDesc->Enable(false);

	SAFE_RELEASE(pDesc);

	CObj* pObj = m_pScene->CreateObject<CObj>("Back", m_pScene->FindLayer("Speacial"), m_pScene->GetSceneType());

	VertexColor pVertex[4] = {};

	pVertex[0].vPos = Vector3(0.f, 1.f, 0.f);
	pVertex[0].vUV = Vector2(1.f, 0.f);
	pVertex[1].vPos = Vector3(1.f, 1.f, 0.f);
	pVertex[1].vUV = Vector2(1.f, 1.f);
	pVertex[2].vPos = Vector3(0.f, 0.f, 0.f);
	pVertex[2].vUV = Vector2(0.f, 0.f);
	pVertex[3].vPos = Vector3(1.f, 0.f, 0.f);
	pVertex[3].vUV = Vector2(0.f, 1.f);

	unsigned short pIdx[6] = { 0, 1, 3, 0, 3, 2 };

	GET_SINGLE(CResourceManager)->CreateMesh(MESH_TYPE::MT_2D, "UVCCW", D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		pVertex, sizeof(VertexColor), 4, D3D11_USAGE::D3D11_USAGE_IMMUTABLE, pIdx, 2, 6, D3D11_USAGE_IMMUTABLE, DXGI_FORMAT_R16_UINT);

	CSpriteComponent* pBack = pObj->CreateComponent<CSpriteComponent>("Back", m_pScene->FindLayer("Speacial"));

	pBack->CreateSprite("Idle", "DoorGradient", LOOP_OPTION::LOOP);
	pBack->SetWorldScale(64.f, 16.f, 0.f);
	pBack->SetPivot(0.5f, 0.5f, 0.f);
	pBack->SetInheritScale(false);
	pBack->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);
	pBack->AddRenderState("DepthNoWrite");
	pBack->SetMesh("UVCCW");
	pBack->SetShader("Sprite");

	pMtrl = pBack->GetMaterial();

	pMtrl->SetDiffuseColor(0.f, 0.f, 0.f, 1.f);

	SAFE_RELEASE(pMtrl);

	pObj->SetRootComponent(pBack);

	pVertex[0].vUV = Vector2(0.f, 1.f);
	pVertex[1].vUV = Vector2(0.f, 0.f);
	pVertex[2].vUV = Vector2(1.f, 1.f);
	pVertex[3].vUV = Vector2(1.f, 0.f);

	GET_SINGLE(CResourceManager)->CreateMesh(MESH_TYPE::MT_2D, "UVCW", D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		pVertex, sizeof(VertexColor), 4, D3D11_USAGE::D3D11_USAGE_IMMUTABLE, pIdx, 2, 6, D3D11_USAGE_IMMUTABLE, DXGI_FORMAT_R16_UINT);

	CSpriteComponent* pBackR = pObj->CreateComponent<CSpriteComponent>("BackR", m_pScene->FindLayer("Speacial"));

	pBackR->CreateSprite("Idle", "DoorGradient", LOOP_OPTION::LOOP);
	pBackR->SetWorldScale(64.f, 16.f, 0.f);
	pBackR->AddRelativePos(64.f, 0.f, 0.f);
	pBackR->SetInheritScale(false);
	pBackR->SetPivot(0.5f, 0.5f, 0.f);
	pBackR->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);
	pBackR->AddRenderState("DepthNoWrite");
	pBackR->SetMesh("UVCW");
	pBackR->SetShader("Sprite");

	pMtrl = pBackR->GetMaterial();

	pMtrl->SetDiffuseColor(0.f, 0.f, 0.f, 1.f);

	SAFE_RELEASE(pMtrl);

	pBack->AddChild(pBackR);

	SAFE_RELEASE(pBackR);

	SAFE_RELEASE(pBack);

	pObj->SetWorldPos((float)tRS.iWidth / 2.f - 32.f, (float)tRS.iHeight / 3.f - 8.f, 0.f);
	pObj->Enable(false);

	SAFE_RELEASE(pObj);

	return true;
}

bool CTileMode::LoadXml(const char* pFileName, const std::string& strPathKey)
{
	if (m_bLock)
		return false;

	char pFullPath[MAX_PATH] = {};

	const char* pPath = GET_SINGLE(CPathManager)->FindMultibytePath(strPathKey);

	if (pPath)
		strcpy_s(pFullPath, pPath);

	strcat_s(pFullPath, pFileName);

	TiXmlDocument tDoc;

	tDoc.LoadFile(pFullPath);

	TiXmlElement* pElem = tDoc.FirstChildElement();

	m_eTexType = TILE_TEX_TYPE::TEX;
	memset(m_pTex, 0, sizeof(const char*) * (int)TILE_TEX_TYPE::END);

	searchXMLData(pElem);

	return true;
}

void CTileMode::searchXMLData(TiXmlElement* pElem)
{
	TiXmlHandle hRoot(0);
	TiXmlElement* pSubElem = pElem;
	TiXmlAttribute* pAttrib = NULL;

	static Vector2 m_vTileSize = {};
	static int m_pWidth[(int)TILE_TEX_TYPE::END] = {};
	static int m_pHeight[(int)TILE_TEX_TYPE::END] = {};
	static int m_iMapWidth = 0;
	static int m_iMapHeight = 0;
	static const char* m_pTag = nullptr;
	static int m_iZ = 0;
	static bool m_bAni = false;
	static bool m_bTileEnable = true;
	static float m_fTileOpacity = 1.f;
	static Vector3 m_vSpawn = {};
	static Vector3 m_vPos = {};
	static bool m_bGlobal = false;
	static float m_fMaxRange = 0.f;
	static float m_fMinRange = 0.f;
	static float m_fMaxVol = 0.f;
	static float m_fMinVol = 0.f;
	static std::vector<class CTileMap*>	m_vecMap;
	static int		m_iLayer = 0;

	hRoot = TiXmlHandle(pSubElem);
	pSubElem = hRoot.FirstChildElement().Element();

	if (!pSubElem)
		return;

	const char* pNode = NULL;
	const char* strAttrib = NULL;
	const char* pText = NULL;

	while (pSubElem)
	{
		pNode = pSubElem->Value();

		if (strcmp(pNode, "map") == 0)
		{
			pAttrib = pSubElem->FirstAttribute();

			while (pAttrib)
			{
				strAttrib = pAttrib->Name();
				pText = pAttrib->Value();

				if (strcmp(strAttrib, "width") == 0)
				{
					m_iMapWidth = atoi(pText);
				}

				else if (strcmp(strAttrib, "height") == 0)
				{
					m_iMapHeight = atoi(pText);
				}

				else if (strcmp(strAttrib, "tilewidth") == 0)
				{
					m_vTileSize.x = (float)atoi(pText);
				}

				else if (strcmp(strAttrib, "tilewidth") == 0)
				{
					m_vTileSize.y = (float)atoi(pText);
				}

				pAttrib = pAttrib->Next();
			}
		}

		else if (strcmp(pNode, "tileset") == 0)
		{
			pAttrib = pSubElem->FirstAttribute();
			while (pAttrib)
			{

				strAttrib = pAttrib->Name();
				pText = pAttrib->Value();

				if (strcmp(strAttrib, "name") == 0)
				{
					m_pTex[(int)m_eTexType] = pText;
				}

				else if (strcmp(strAttrib, "height") == 0)
				{
					m_pTex[(int)m_eTexType] = pText;
				}

				else if (strcmp(strAttrib, "tilewidth") == 0)
				{
					m_vTileSize.x = (float)atoi(pText);
				}

				else if (strcmp(strAttrib, "tileheight") == 0)
				{
					m_vTileSize.y = (float)atoi(pText);
				}

				pAttrib = pAttrib->Next();
			}
		}

		else if (strcmp(pNode, "image") == 0)
		{
			pAttrib = pSubElem->FirstAttribute();
			while (pAttrib)
			{
				strAttrib = pAttrib->Name();
				pText = pAttrib->Value();

				if (strcmp(strAttrib, "width") == 0)
				{
					m_pWidth[(int)m_eTexType] = atoi(pText);
				}

				else if (strcmp(strAttrib, "height") == 0)
				{
#ifdef _DEBUG
					char strDebug[32] = {};

					sprintf_s(strDebug, "Type: %d\n", m_eTexType);

					OutputDebugStringA(strDebug);
#endif

					if ((int)m_eTexType < 2 && (int)m_eTexType >= 0)
					{
						m_pHeight[(int)m_eTexType] = atoi(pText);
					}

					else 
					{
						break;
					}

					m_eTexType = TILE_TEX_TYPE((int)m_eTexType + 1);
				}

				pAttrib = pAttrib->Next();
			}
		}

		else if (strcmp(pNode, "layer") == 0)
		{
			pAttrib = pSubElem->FirstAttribute();

			while (pAttrib)
			{
				strAttrib = pAttrib->Name();
				pText = pAttrib->Value();

				if (strcmp(strAttrib, "name") == 0)
				{
					m_pTag = pText;
				}

				else if (strcmp(strAttrib, "width") == 0)
				{
					m_iMapWidth = atoi(pText);
				}

				else if (strcmp(strAttrib, "height") == 0)
				{
					m_iMapHeight = atoi(pText);
				}

				else if (strcmp(strAttrib, "visible") == 0)
				{
					m_bTileEnable = atoi(pText);
				}

				else if (strcmp(strAttrib, "opacity") == 0)
				{
					m_fTileOpacity = (float)atof(pText);
				}

				pAttrib = pAttrib->Next();
			}
		}

		else if (strcmp(pNode, "property") == 0)
		{
			pAttrib = pSubElem->FirstAttribute();

			while (pAttrib)
			{
				strAttrib = pAttrib->Name();
				pText = pAttrib->Value();

				if (strcmp(pText, "z") == 0)
				{
					pAttrib = pAttrib->Next();

					strAttrib = pAttrib->Name();
					pText = pAttrib->Value();

					m_iZ = atoi(pText);
				}

				else if (strcmp(pText, "animated") == 0)
				{
					m_bAni = atoi(pText);
				}

				else if (strcmp(pText, "layer") == 0)
				{
					m_eTexType = TILE_TEX_TYPE::TEX;

					pAttrib = pAttrib->Next();

					strAttrib = pAttrib->Name();
					pText = pAttrib->Value();

					m_iLayer = atoi(pText);
				}

				else if (strcmp(pText, "collide") == 0)
				{
					m_eTexType = TILE_TEX_TYPE::COL;
					m_bTileEnable = false;
				}

				else if (strcmp(pText, "special") == 0)
				{
					m_eTexType = TILE_TEX_TYPE::COL;
					m_bTileEnable = false;
				}

				pAttrib = pAttrib->Next();
			}
		}

		else if (strcmp(pNode, "data") == 0)
		{
			pAttrib = pSubElem->FirstAttribute();
			while (pAttrib)
			{

				strAttrib = pAttrib->Name();
				pText = pAttrib->Value();

				if (strcmp(strAttrib, "encoding") == 0 &&
					strcmp(pText, "csv") == 0)
				{
					const char* pTile = pSubElem->GetText();

					m_pScene->CreateLayer(m_pTag, m_iLayer);

					CLayer* pLayer = m_pScene->FindLayer(m_pTag);

					CObj* pMapObj = m_pScene->CreateObject<CObj>(m_pTag, pLayer);

					CTileMap* pMap = pMapObj->CreateComponent<CTileMap>(m_pTag, pLayer);

					pMap->SetWorldPos(0.f, 0.f, (float)m_iZ);

					pMap->CreateTile(m_iMapWidth, m_iMapHeight,
						Vector3(m_vTileSize.x, m_vTileSize.y, 0.f), Vector2(0.f, 0.f), Vector3(1.f, 1.f,0.f), TILE_TYPE::RECT);

					if (m_eTexType == TILE_TEX_TYPE::TEX)
					{
						GET_SINGLE(CResourceManager)->LoadTexture(m_pTex[(int)m_eTexType], TEXT("jam.png"));

						pMap->SetTileTexture(m_pTex[(int)m_eTexType]);

						pMap->LoadTileIdx(pTile, m_pWidth[(int)m_eTexType], m_pHeight[(int)m_eTexType]);
					}

					else if (m_eTexType == TILE_TEX_TYPE::COL)
					{
						GET_SINGLE(CResourceManager)->LoadTexture(m_pTex[(int)m_eTexType], TEXT("spectiles.png"));

						pMap->SetTileTexture(m_pTex[(int)m_eTexType]);

						pMap->LoadColIdx(pTile, m_pWidth[(int)m_eTexType], m_pHeight[(int)m_eTexType]);
					}

					pLayer->SetTileMap(pMap);

					pMap->Enable(m_bTileEnable);

					pMap->SetTileOpacity(m_fTileOpacity);

					pMapObj->SetRootComponent(pMap);

					if (strcmp(m_pTag, "1_FG1") == 0)
					{
						pMap->AddRenderState("ForeGround");
					}

					else if (strcmp(m_pTag, "1_FG2") == 0)
					{
						pMap->AddRenderState("ForeGround");
					}
					else
					{
						pMap->AddRenderState("DepthNoWrite");
					}

					m_vecMap.push_back(pMap);

					m_bTileEnable = true;
					m_fTileOpacity = 1.f;

					SAFE_RELEASE(pMap);

					SAFE_RELEASE(pMapObj);

					break;
				}

				pAttrib = pAttrib->Next();
			}
		}

		else if (strcmp(pNode, "objectgroup") == 0)
		{
			m_bGlobal = false;
			TiXmlElement* pSub = pSubElem->FirstChildElement();
			TiXmlElement* pSubChild = pSub;

			std::stack<TiXmlElement*>	st;
			std::stack<CObj*>			stackObj;

			while (true)
			{
				while (true)
				{
					if (!pSubChild)
						break;

					pNode = pSubChild->Value();

					if (strcmp(pNode, "object") == 0)
					{
						pAttrib = pSubChild->FirstAttribute();

						while (pAttrib)
						{
							strAttrib = pAttrib->Name();

							pText = pAttrib->Value();

							if (strcmp(strAttrib, "type") == 0)
							{
								m_pTag = pText;
							}

							else if (strcmp(strAttrib, "name") == 0)
							{
								m_pTag = pText;
							}

							else if (strcmp(strAttrib, "x") == 0)
							{
								m_vPos.x = (float)atoi(pText);
							}

							else if (strcmp(strAttrib, "y") == 0)
							{
								m_vPos.y = (float)atoi(pText);
							}

							else if (strcmp(strAttrib, "gid") == 0)
							{
								if (strcmp(m_pTag, "") == 0)
								{
									m_pTag = pText;
								}
							}

							pAttrib = pAttrib->Next();
						}

						CLayer* pLayer = m_pScene->FindLayer("Default");

						CObj* pObj = nullptr;

						if (strcmp(m_pTag, "boss_colossus") == 0)
						{
							pObj = m_pScene->CreateObject<CColossus>(m_pTag, pLayer, m_pScene->GetSceneType());
						}

						else if (strcmp(m_pTag, "boss_eyecube") == 0)
						{
							pObj = m_pScene->CreateObject<CEyecube>(m_pTag, pLayer, m_pScene->GetSceneType());
							pObj->SetWorldPos(0.f, 64.f, -100.f);
						}

						else if (strcmp(m_pTag, "boss_brainfreeze") == 0)
						{
							pObj = m_pScene->CreateObject<CBrainFreeze>(m_pTag, pLayer, m_pScene->GetSceneType());
						}

						else if (strcmp(m_pTag, "4292") == 0)
						{
							pObj = m_pScene->CreateCloneObj(m_pTag, m_pTag, pLayer, m_pScene->GetSceneType());
						}

						else
						{
							pObj = m_pScene->CreateObject<CObj>(m_pTag, pLayer);
						}

						pObj->AddWorldPos(Vector3(m_vPos.x, m_iMapHeight * m_vTileSize.y - m_vPos.y, m_vPos.z));

						stackObj.push(pObj);

						m_pTag = "";
					}

					else if (strcmp(pNode, "property") == 0)
					{
						pAttrib = pSubChild->FirstAttribute();

						while (pAttrib)
						{
							strAttrib = pAttrib->Name();

							pText = pAttrib->Value();

							if (strcmp(pText, "global") == 0)
							{
								pAttrib = pAttrib->Next();

								strAttrib = pAttrib->Name();
								pText = pAttrib->Value();

								m_bGlobal = atoi(pText);
							}

							else if (strcmp(pText, "MaxRange") == 0)
							{
								pAttrib = pAttrib->Next();

								strAttrib = pAttrib->Name();
								pText = pAttrib->Value();

								m_fMaxRange = (float)atof(pText);

								CObj* pObj = stackObj.top();

								if (pObj->GetName() == "camera_focus")
								{
									CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

									pCam->SetFocus(pObj);

									pCam->SetMax(m_fMaxRange);

									SAFE_RELEASE(pCam);
								}
							}

							else if (strcmp(pText, "MinRange") == 0)
							{
								pAttrib = pAttrib->Next();

								strAttrib = pAttrib->Name();
								pText = pAttrib->Value();

								m_fMinRange = (float)atof(pText);

								CObj* pObj = stackObj.top();

								if (pObj->GetName() == "camera_focus")
								{
									CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

									pCam->SetFocus(pObj);
									pCam->SetMin(m_fMinRange);

									SAFE_RELEASE(pCam);
								}
							}

							else if (strcmp(pText, "MaxVol") == 0)
							{
								pAttrib = pAttrib->Next();

								strAttrib = pAttrib->Name();
								pText = pAttrib->Value();

								m_fMaxVol = (float)atof(pText);
							}

							else if (strcmp(pText, "MinVol") == 0)
							{
								pAttrib = pAttrib->Next();

								strAttrib = pAttrib->Name();
								pText = pAttrib->Value();

								m_fMinVol = (float)atof(pText);
							}

							else if (strcmp(pText, "Sound") == 0)
							{
								pAttrib = pAttrib->Next();

								strAttrib = pAttrib->Name();
								pText = pAttrib->Value();

								char strName[_MAX_FNAME] = {};

								_splitpath_s(pText, nullptr, 0, nullptr, 0, strName, _MAX_FNAME, nullptr, 0);

								GET_SINGLE(CSoundManager)->LoadSound(strName, SOUND_TYPE::BGM, pText, DATA_PATH);

								CObj* pObj = stackObj.top();
								stackObj.pop();

								CLayer* pLayer = pObj->GetScene()->FindLayer("Default");

								CSound* pSnd = pObj->CreateComponent<CSound>(strName, pLayer);

								pSnd->SetSound(strName);
								pSnd->SetMaxVol(m_fMaxVol);
								pSnd->SetMinVol(m_fMinVol);
								pSnd->SetMax(m_fMaxRange);
								pSnd->SetMin(m_fMinRange);
								pSnd->Play(0.f);

								if (m_bGlobal)
								{
									pSnd->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);
								}

								else
								{
									pSnd->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_2D);
								}

								
								pObj->SetRootComponent(pSnd);
								pSnd->SetWorldPos(Vector3(m_vPos.x, m_iMapHeight * m_vTileSize.y - m_vPos.y, 0.f));

								SAFE_RELEASE(pSnd);

								SAFE_RELEASE(pObj);

								m_bGlobal = false;
							}

							else if (strcmp(pText, "ToMap") == 0)
							{
								pAttrib = pAttrib->Next();

								strAttrib = pAttrib->Name();
								pText = pAttrib->Value();

								CObj* pObj = stackObj.top();

								Vector3 vPos = pObj->GetWorldPos();

								CColliderRect* pRC = pObj->CreateComponent<CColliderRect>(pText, pObj->GetLayer());

								pObj->SetRootComponent(pRC);

								pRC->SetExtent(48.f, 48.f);

								pRC->SetCallBack(COLLISION_STATE::INIT, ColInit);

								if (pObj->GetName() == "internal")
								{
									CSpriteComponent* pGrad = pObj->CreateComponent<CSpriteComponent>("gradient", m_pScene->FindLayer("Fore"));

									pGrad->SetWorldScale(48.f, 48.f, 0.f);
									pGrad->CreateSprite("Idle", "DoorGradient", LOOP_OPTION::LOOP);
									pGrad->SetWorldPos(vPos);
									pGrad->SetPivot(0.5f, 0.5f, 0.f);
									pGrad->AddRenderState("DepthNoWrite");

									pObj->SetRootComponent(pGrad);

									pGrad->AddChild(pRC);

									SAFE_RELEASE(pGrad);
								}

								pRC->SetWorldPos(vPos);

								SAFE_RELEASE(pRC);
							}

							else if (strcmp(pText, "dir_y") == 0)
							{
								pAttrib = pAttrib->Next();

								strAttrib = pAttrib->Name();
								pText = pAttrib->Value();

								m_vDoorDir = Vector3(0.f, (float)atof(pText), 0.f);
							}

							else
							{
								char strExt[_MAX_EXT] = {};
								char strName[_MAX_FNAME] = {};

								_splitpath_s(pText, nullptr, 0, nullptr, 0, strName, _MAX_FNAME, strExt, _MAX_EXT);

								_strupr_s(strExt);

								if (strcmp(strExt, ".OGG") == 0)
								{
									GET_SINGLE(CSoundManager)->LoadSound(strName, SOUND_TYPE::BGM, pText, DATA_PATH);

									CObj* pObj = stackObj.top();
									stackObj.pop();

									CSound* pSnd = pObj->CreateComponent<CSound>(strName, pObj->GetLayer());

									pSnd->SetSound(strName);
									pSnd->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);

									pObj->SetRootComponent(pSnd);
									pSnd->SetWorldPos(m_vPos);

									if (m_bHub)
									{
										pSnd->SetVol(1.f);
										pSnd->Play(0.f);
									}

									SAFE_RELEASE(pSnd);

									SAFE_RELEASE(pObj);

									m_bGlobal = false;
								}
							}

							pAttrib = pAttrib->Next();
						}
					}

					st.push(pSubChild);

					pSubChild = pSubChild->FirstChildElement();
				}

				if (st.empty())
					break;

				pSubChild = st.top();

				st.pop();

				pSubChild = pSubChild->NextSiblingElement();
			}

			while (!stackObj.empty())
			{
				CObj* pObj = stackObj.top();

				stackObj.pop();

				SAFE_RELEASE(pObj);
			}
		}

		else if (strcmp(pNode, "properties") == 0)
		{
			TiXmlElement* pSubChild = pSubElem->FirstChildElement();

			pAttrib = pSubChild->FirstAttribute();

			while (pAttrib)
			{
				strAttrib = pAttrib->Name();
				pText = pAttrib->Value();

				if (strcmp(pText, "ambience") == 0)
				{
					pAttrib = pAttrib->Next();

					strAttrib = pAttrib->Name();
					pText = pAttrib->Value();

					char strName[_MAX_FNAME] = {};

					_splitpath_s(pText, nullptr, 0, nullptr, 0, strName, _MAX_FNAME, nullptr, 0);

					char pPath[MAX_PATH] = {};

					strcat_s(pPath, MAX_PATH, "SOUND\\SFX\\AMBIENCE\\");

					strcat_s(pPath, MAX_PATH, pText);

					GET_SINGLE(CSoundManager)->LoadSound(strName, SOUND_TYPE::BGM, pPath, DATA_PATH);

					CLayer* pLayer = m_pScene->FindLayer("Default");

					CObj* pObj = m_pScene->CreateObject<CObj>(pText, pLayer);

					CSound* pSnd = pObj->CreateComponent<CSound>(strName, pLayer);

					pSnd->SetSound(strName);
					pSnd->Play(0.f);
					pSnd->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);

					pObj->SetRootComponent(pSnd);

					SAFE_RELEASE(pSnd);

					SAFE_RELEASE(pObj);
				}

				pAttrib = pAttrib->Next();
			}
		}

		else if (strcmp(pNode, "anim") == 0)
		{
			float fSpeed = 0.f;

			pAttrib = pSubElem->FirstAttribute();

			while (pAttrib)
			{
				strAttrib = pAttrib->Name();
				pText = pAttrib->Value();

				if (strcmp(strAttrib, "speed") == 0)
				{
					fSpeed = (float)atof(pText);
				}

				else if (strcmp(strAttrib, "name") == 0)
				{
				}

				pAttrib = pAttrib->Next();
			}

			TiXmlElement* pSubChild = pSubElem->FirstChildElement();

			const char* pText = pSubChild->GetText();

			char* pContext = nullptr;

			std::vector<int>	vecFrame;

			char* pResult = strtok_s((char*)pText, ",", &pContext);

			while (pResult)
			{
				vecFrame.push_back(atoi(pResult));

				pResult = strtok_s(nullptr, ",", &pContext);
			}

			CTileMap::AddAnim(vecFrame, fSpeed);

			//size_t iSz = m_vecMap.size();

			//for (size_t i = 0; i < iSz; ++i)
			//{
			//	m_vecMap[i]->AddAnim(vecFrame, fSpeed);
			//}
		}

		searchXMLData(pSubElem);

		pSubElem = pSubElem->NextSiblingElement();
	}
}

void CTileMode::ShakeCam(float fSpeed, float fAmp, float fLimit, const Vector3& vDir)
{
	m_fShakeTime = 0.f;

	m_fAmplitude = fAmp;

	m_fSpeed = fSpeed;

	m_fShakeLimit = fLimit;

	m_vShakeDir = vDir;

	m_bShake = true;
}

void CTileMode::StopShaking()
{
	m_fShakeTime = m_fShakeLimit;
}

void CTileMode::Update(float fTime)
{
	if (m_bShake)
	{
		m_fShakeTime += fTime;

		if(m_fShakeTime >= m_fShakeLimit)
		{
			m_bShake = false;
			m_fShakeTime = 0.f;
		}

		if (!m_pCam)
		{
			m_pCam = GET_SINGLE(CCameraManager)->GetMainCam();
		}

		m_pCam->AddWorldPos(m_fSpeed * m_iShake * fTime * m_vShakeDir);

		m_fDist += fTime * m_fSpeed;

		if (m_fDist >= m_fAmplitude)
		{
			m_iShake *= -1;

			m_fDist -= m_fAmplitude;
		}
	}

	if (m_bFade)
	{
		m_fFade += m_fFadeScale * fTime;

		if(abs(m_fFade - m_fFadeStart) >= abs(m_fFadeEnd - m_fFadeStart) )
		{
			m_fFade = m_fFadeEnd;
			m_bFade = false;
		}

		CMaterial* pMtrl = m_pFade->GetMaterial();

		Vector4 vDif = pMtrl->GetDif();

		pMtrl->SetDiffuseColor(vDif.x, vDif.y, vDif.z, m_fFade);

		SAFE_RELEASE(pMtrl);
	}
}

void CTileMode::PreRender(float fTime)
{
	CGameMode::PreRender(fTime);

	if (GET_SINGLE(CEngine)->IsImgui())
	{
		SpawnFade();

		if (m_pPlayer)
		{
			((CPlayer*)m_pPlayer)->SpawnWindow();
		}
	}
}

void CTileMode::FadeOut(float fTime)
{
	CMaterial* pMtrl = m_pFade->GetMaterial();

	Vector4 vDif = pMtrl->GetDif();

	if (vDif.w + fTime >= 1.f)
	{
		pMtrl->SetDiffuseColor(vDif.x, vDif.y, vDif.z, 1.f);
	}

	else
	{
		pMtrl->SetDiffuseColor(vDif.x, vDif.y, vDif.z, vDif.w + fTime);
	}

	Vector3 vPos = m_pPlayer->GetWorldPos();
	m_pFade->SetWorldPos(vPos.x, vPos.y, -520.f);	

	SAFE_RELEASE(pMtrl);
}

void CTileMode::FadeIn(float fTime)
{
	CMaterial* pMtrl = m_pFade->GetMaterial();

	Vector4 vDif = pMtrl->GetDif();

	if (vDif.w - fTime < 0.f)
	{
		pMtrl->SetDiffuseColor(vDif.x, vDif.y, vDif.z, 0.f);
	}

	else
	{
		pMtrl->SetDiffuseColor(vDif.x, vDif.y, vDif.z, vDif.w - fTime);
	}

	Vector3 vPos = m_pPlayer->GetWorldPos();
	m_pFade->SetWorldPos(vPos.x, vPos.y, -520.f);

	SAFE_RELEASE(pMtrl);
}

void CTileMode::SetFade(float fStart, float fEnd, float fScale)
{
	m_fFadeStart = fStart;
	m_fFadeEnd = fEnd;
	m_fFadeScale = fScale;
	m_bFade = true;
	m_fFade = m_fFadeStart;

	CMaterial* pMtrl = m_pFade->GetMaterial();

	Vector4 vDif = pMtrl->GetDif();

	pMtrl->SetDiffuseColor(vDif.x, vDif.y, vDif.z, m_fFadeStart);

	SAFE_RELEASE(pMtrl);

	Vector3 vPos = {};

	if (m_pPlayer)
	{
		vPos = m_pPlayer->GetWorldPos();
	}

	else
	{
		CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

		Resolution tRS = pCam->GetResolution();

		vPos = pCam->GetWorldPos() + pCam->GetPivot() * Vector3(0.f, (float)tRS.iHeight, 0.f);

		SAFE_RELEASE(pCam);
	}

	m_pFade->SetWorldPos(vPos.x, vPos.y, -520.f);
}

void CTileMode::SetFadeColor(float r, float g, float b)
{
	CMaterial* pMtrl = m_pFade->GetMaterial();

	Vector4 vDif = pMtrl->GetDif();

	pMtrl->SetDiffuseColor(r, g, b, vDif.w);

	SAFE_RELEASE(pMtrl);
}

void CTileMode::SpawnFade()
{	
	if (ImGui::Begin("Fade"))
	{
		CMaterial* pMtrl = m_pFade->GetMaterial();

		Vector4 vColor = pMtrl->GetDif();

		ImGui::Text("Material");
		ImGui::ColorEdit4("Diffuse", &vColor.x);
		//ImGui::SliderFloat4("Dif", &vColor.x, 0.f, 1.f);

		pMtrl->SetDiffuseColor(vColor);

		SAFE_RELEASE(pMtrl);
	}

	ImGui::End();

}

void CTileMode::SetText(bool bOn)
{
	CObj* pObj = m_pScene->FindLayer("UI")->FindObj("font");
	pObj->Enable(bOn);
	SAFE_RELEASE(pObj);
	pObj = m_pScene->FindLayer("UI")->FindObj("desc");
	pObj->Enable(bOn);
	SAFE_RELEASE(pObj);
	pObj = m_pScene->FindLayer("Speacial")->FindObj("Back");
	pObj->Enable(bOn);
	SAFE_RELEASE(pObj);
}

void CTileMode::SetFontText(const std::string& str)
{
	CTitanFont* pObj = (CTitanFont*)m_pScene->FindLayer("UI")->FindObj("font");
	pObj->SetText(str, 16.f);
	SAFE_RELEASE(pObj);
}

void CTileMode::SetDescText(const std::string& str)
{
	CTitanFont* pObj = (CTitanFont*)m_pScene->FindLayer("UI")->FindObj("desc");
	pObj->SetText(str, 8.f, Vector4(0.5f, 0.5f, 0.5f, 1.f));
	SAFE_RELEASE(pObj);
}

void ColInit(CCollider* pSrc, CCollider* pDst, float fTime)
{
}

void ColStay(CCollider* pSrc, CCollider* pDst, float fTime)
{
}

void ColLast(CCollider* pSrc, CCollider* pDst, float fTime)
{
}