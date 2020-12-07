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

CTileMode::CTileMode() :
	m_vTileSize(),
	m_iMapWidth(0),
	m_iMapHeight(0),
	m_eTexType(TILE_TEX_TYPE::TEX),
	m_pTag(nullptr),
	m_iZ(0),
	m_bAni(false),
	m_bTileEnable(true),
	m_fTileOpacity(1.f),
	m_vPos(),
	m_vSpawn(),
	m_bGlobal(false),
	m_fMaxRange(0.f),
	m_fMinRange(0.f),
	m_fMaxVol(0.f),
	m_fMinVol(0.f)
{
	memset(m_pWidth, 0, 4 * (int)TILE_TEX_TYPE::END);
	memset(m_pHeight, 0, 4 * (int)TILE_TEX_TYPE::END);
	memset(m_pTex, 0, sizeof(char*) * (int)TILE_TEX_TYPE::END);
}

CTileMode::~CTileMode()
{
}

bool CTileMode::Init()
{
	return true;
}

bool CTileMode::LoadXml(const char* pFileName, const std::string& strPathKey)
{
	char pFullPath[MAX_PATH] = {};

	const char* pPath = GET_SINGLE(CPathManager)->FindMultibytePath(strPathKey);

	if (pPath)
		strcpy_s(pFullPath, pPath);

	strcat_s(pFullPath, pFileName);

	TiXmlDocument tDoc;

	tDoc.LoadFile(pFullPath);

	TiXmlElement* pElem = tDoc.FirstChildElement();

	searchXMLData(pElem);

	//tDoc.Clear();

	return true;
}

void CTileMode::searchXMLData(TiXmlElement* pElem)
{
	TiXmlHandle hRoot(0);
	TiXmlElement* pSubElem = pElem;
	TiXmlAttribute* pAttrib = NULL;

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
					m_pHeight[(int)m_eTexType] = atoi(pText);
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

					m_pScene->CreateLayer(m_pTag, m_iZ);

					CLayer* pLayer = m_pScene->FindLayer(m_pTag);

					CObj* pMapObj = m_pScene->CreateObject<CObj>(m_pTag, pLayer);

					CTileMap* pMap = pMapObj->CreateComponent<CTileMap>(m_pTag);

					pMap->CreateTile(m_iMapWidth, m_iMapHeight,
						Vector3(m_vTileSize.x, m_vTileSize.y, 0.f), Vector2(0.f, 0.f), TILE_TYPE::RECT);

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

						CObj* pObj = m_pScene->CreateObject<CObj>(m_pTag, pLayer);

						pObj->SetWorldPos(Vector3(m_vPos.x, m_iMapHeight * m_vTileSize.y - m_vPos.y, m_vPos.z));

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

								CSound* pSnd = pObj->CreateComponent<CSound>(strName);

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

								Vector3 vPos = pObj->GetWorldPos() + Vector3(m_vTileSize.x /2.f, m_vTileSize.y /2.f, 0.f);

								CColliderRect* pRC = pObj->CreateComponent<CColliderRect>(pText);

								pObj->SetRootComponent(pRC);

								pRC->SetExtent(48.f, 48.f);

								pRC->SetWorldPos(vPos);

								pRC->SetCallBack(COLLISION_STATE::INIT, ColInit);

								SAFE_RELEASE(pRC);
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

									CSound* pSnd = pObj->CreateComponent<CSound>(strName);

									pSnd->SetSound(strName);
									pSnd->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);

									pObj->SetRootComponent(pSnd);
									pSnd->SetWorldPos(m_vPos);
									pSnd->Play(0.f);

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

		searchXMLData(pSubElem);

		pSubElem = pSubElem->NextSiblingElement();
	}
}

void ColInit(CCollider* pSrc, CCollider* pDst, float fTime)
{
	std::string strSrc = pSrc->GetName();

	std::string strDst = pDst->GetName();

	if (strDst == "PlayerBody")

	{
		if (strSrc == "JAM/colossus")
		{
			strSrc = strSrc + ".tmx";

			GET_SINGLE(CSceneManager)->CreateNextScene();

			GET_SINGLE(CSceneManager)->SetGameMode<CColossusMode>(false);

			CScene* pNext = GET_SINGLE(CSceneManager)->GetNextScene();

			((CColossusMode*)pNext->GetGameMode())->LoadXml(strSrc.c_str(), MAP_PATH);
		}
	}
}

void ColStay(CCollider* pSrc, CCollider* pDst, float fTime)
{
}

void ColLast(CCollider* pSrc, CCollider* pDst, float fTime)
{
}