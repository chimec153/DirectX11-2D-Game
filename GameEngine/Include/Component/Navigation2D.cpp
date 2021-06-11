#include "Navigation2D.h"
#include "../Tile/TileMap.h"
#include "../Tile/Tile.h"
#include "../Object/Obj.h"
#ifdef _DEBUG
#include "../Input.h"
#endif

CNavigation2D::CNavigation2D()	:
	m_pMap(nullptr)
#ifdef _DEBUG
	, m_pPrevTile(nullptr)
	, m_pPrevMouseTile(nullptr)
#endif
{
	m_eType = COMPONENT_TYPE::CT_OBJECT;
}

CNavigation2D::CNavigation2D(const CNavigation2D& nav)	:
	CComponent(nav)
	, m_pMap(nav.m_pMap)
#ifdef _DEBUG
	, m_pPrevTile(nav.m_pPrevTile)
	, m_pPrevMouseTile(nav.m_pPrevMouseTile)
#endif
{
	if (m_pMap)
		m_pMap->AddRef();
}

CNavigation2D::~CNavigation2D()
{
	SAFE_RELEASE(m_pMap);
}

void CNavigation2D::SetTileMap(CTileMap* pMap)
{
	SAFE_RELEASE(m_pMap);

	m_pMap = pMap;

	if (m_pMap)
		m_pMap->AddRef();
}

void CNavigation2D::FindPath(const Vector3& vStart, const Vector3& vEnd)
{
	while (!m_Path.empty())
	{
#ifdef _DEBUG
		m_Path.front()->SetColor(Vector4::White);
#endif
		m_Path.pop();
	}

	std::list<CTile*> OpenNode;
	std::queue<CTile*> CloseNode;
	std::stack<CTile*>	PathStack;

	if (m_pMap)
	{
		CTile* pEndTile = m_pMap->GetTile(Vector2(vEnd.x, vEnd.y));

		if (!pEndTile)
			return;

		CTile* pStartTile = m_pMap->GetTile(Vector2(vStart.x, vStart.y));

		if (!pStartTile)
			return;

		if (pStartTile == pEndTile)
			return;

		int iCountX = m_pMap->GetCountX();

		OpenNode.push_back(pStartTile);

		while (!OpenNode.empty())
		{
			CTile* _pTile = OpenNode.front();

			OpenNode.pop_front();

			_pTile->SetClose(true);
			CloseNode.push(_pTile);

			int iIndexX = _pTile->GetIndexX();
			int iIndexY = _pTile->GetIndexY();
			int iIndex = iIndexY * iCountX + iIndexX;

			bool bFind = false;

			for (int i = -1; i <= 1; i++)
			{
				for (int j = -1; j <= 1; j++)
				{
					if (iIndexX + j < 0)
						continue;

					else if (iIndexY + i < 0)
						continue;

					else if (i == 0 && j == 0)
						continue;

					CTile* __pTile = m_pMap->GetTile(iIndex + j + i * iCountX);

					if (__pTile->IsClose())
						continue;

					if (__pTile == pEndTile)
					{
						bFind = true;

						PathStack.push(__pTile);

						CTile* pParent = _pTile;

						while (pParent)
						{
							PathStack.push(pParent);

							pParent = pParent->GetParent();
						}

						CTile* pNext = nullptr;

						while (!PathStack.empty())
						{
							CTile* pAdd = PathStack.top();
#ifdef _DEBUG

							pAdd->SetColor(Vector4::Blue);
#endif

							m_Path.push(pAdd);

							if (PathStack.size() == 2)
							{
								pNext = PathStack.top();
							}

							PathStack.pop();
						}

						Vector3 vDir = pNext->GetWorldPos() - m_pObj->GetWorldPos();

						vDir.Normalize();

						Vector3 vAxisY = Vector3::Axis[static_cast<int>(WORLD_AXIS::AXIS_Y)];

						float fAngle = RadToDeg(acosf(vDir.Dot(vAxisY)));

						if (vDir.x < 0.f)
							fAngle *= -1.f;

						m_pObj->SetWorldRotZ(-fAngle);
					}

					if (bFind)
					{
						break;
					}
						

					TILE_OPTION eOption = __pTile->GetTileOption();

					if (eOption == TILE_OPTION::NOMOVE)
						continue;

					float fLocalG = (i + j + 3) % 2 * 0.4f + 1.f;
					float fG = fLocalG;

					if (_pTile)
					{
						fG += _pTile->GetG();
					}

					if (__pTile->IsOpen())
					{
						float fPrevG = __pTile->GetG();

						float fCurG = fG;

						if (fPrevG > fCurG)
						{
							__pTile->SetParent(_pTile);
							__pTile->SetG(fCurG);
							continue;
						}

						else
							continue;
					}

					__pTile->SetParent(_pTile);

					__pTile->SetG(fG);

					int iCurX = iIndex % iCountX + j;
					int iCurY = iIndex / iCountX + i;

					int iIdxX = pEndTile->GetIndexX();
					int iIdxY = pEndTile->GetIndexY();

					int iDifX = abs(iIdxX - iCurX);
					int iDifY = abs(iIdxY - iCurY);

					if (iDifX > iDifY)
					{
						__pTile->SetH(iDifY * 1.4f + (iDifX - iDifY));
					}

					else
					{
						__pTile->SetH(iDifX * 1.4f + (iDifY - iDifX));
					}

					__pTile->SetOpen(true);
					OpenNode.push_back(__pTile);
				}

				if (bFind)
					break;
			}

			if (bFind)
				break;

			OpenNode.sort(Sort);
		}
	}

	while (!OpenNode.empty())
	{
		CTile* pTile = OpenNode.front();

		pTile->SetG(0.f);
		pTile->SetH(0.f);
		pTile->SetOpen(false);
		pTile->SetClose(false);
		pTile->SetParent(nullptr);

		OpenNode.pop_front();
	}
	while (!CloseNode.empty())
	{
		CTile* pTile = CloseNode.front();

		pTile->SetG(0.f);
		pTile->SetH(0.f);
		pTile->SetOpen(false);
		pTile->SetClose(false);
		pTile->SetParent(nullptr);

		CloseNode.pop();
	}
}

bool CNavigation2D::Init()
{
	if (!CComponent::Init())
		return false;

	return true;
}

void CNavigation2D::Start()
{
	CComponent::Start();
}

void CNavigation2D::Update(float fTime)
{
	CComponent::Update(fTime);

	Vector3 vObjPos = m_pObj->GetWorldPos();
#ifdef _DEBUG
	if (m_pMap)
	{
		if (m_pPrevTile)
		{
			m_pPrevTile->SetColor(Vector4::White);
		}
		if (m_pPrevMouseTile)
		{
			m_pPrevMouseTile->SetColor(Vector4::White);
		}

		CTile* pTile = m_pMap->GetTile(Vector2(vObjPos.x, vObjPos.y));

		if(pTile)
		pTile->SetColor(Vector4::Green);

		m_pPrevTile = pTile;

		Vector3 vMousePos = GET_SINGLE(CInput)->GetWorldMousePos();

		CTile* _pTile = m_pMap->GetTile(Vector2(vMousePos.x, vMousePos.y));

		if(_pTile)
		_pTile->SetColor(Vector4::Red);

		m_pPrevMouseTile = _pTile;

		if (GET_SINGLE(CInput)->IsMouseUp(MOUSE_BUTTON_TYPE::LEFT))
		{
			if (_pTile)
			{
				if (_pTile->GetTileOption() == TILE_OPTION::NONE)
				{
					_pTile->SetTileOption(TILE_OPTION::NOMOVE);
					_pTile->SetTexIdx(Vector2(0.5f, 0.5f));
				}

				else
				{
					_pTile->SetTileOption(TILE_OPTION::NONE);
					_pTile->SetTexIdx(Vector2(1.f, 1.f));
				}
			}
		}
	}
#endif

	if (!m_Path.empty())
	{
		CTile* pTile = m_Path.front();

		Vector3 vTilePos = pTile->GetWorldPos();

		Vector3 vPos = m_pObj->GetWorldPos();

		Vector3 vDir = vTilePos - vPos;

		float fLength = vDir.Length();

		if (fLength < 10.f)
		{
#ifdef _DEBUG
			m_Path.front()->SetColor(Vector4::White);
			CTile* pTile = m_Path.front();
			TCHAR strDebug[MAX_PATH] = {};
			swprintf_s(strDebug, TEXT("Tile Index X: %d,Tile Index Y: %d\n"), pTile->GetIndexX(), pTile->GetIndexY());
			OutputDebugString(strDebug);
#endif
			m_Path.pop(); 

			if (!m_Path.empty())
			{
				Vector3 vDir = m_Path.front()->GetWorldPos() - vPos;
				vDir.Normalize();

				Vector3 vAxis = Vector3::Axis[static_cast<int>(WORLD_AXIS::AXIS_Y)];

				float fNextAngle = RadToDeg(acosf(vAxis.Dot(vDir)));

				if (vDir.x < 0.f)
					fNextAngle *= -1.f;

				m_pObj->SetWorldRotZ(-fNextAngle);
#ifdef _DEBUG
				TCHAR strDebug[MAX_PATH] = {};
				swprintf_s(strDebug, TEXT("Turn Angle1: %.2f\n"), -fNextAngle);
				OutputDebugString(strDebug);
#endif
			}

			return;
		}

		vDir /= fLength;

		Vector3 vAxisY = m_pObj->GetWorldAxis(WORLD_AXIS::AXIS_Y);

		float fCurAngle = RadToDeg(acosf(vAxisY.Dot(vDir)));

		if (fCurAngle > 90.f ||
			fCurAngle < -90.f)
		{
#ifdef _DEBUG
			m_Path.front()->SetColor(Vector4::White);
			CTile* pTile = m_Path.front();
			TCHAR strDebug[MAX_PATH] = {};
			swprintf_s(strDebug, TEXT("Tile Index X: %d,Tile Index Y: %d\n"), pTile->GetIndexX(), pTile->GetIndexY());
			OutputDebugString(strDebug);
#endif
			m_Path.pop();

			if (!m_Path.empty())
			{
				Vector3 vDir = m_Path.front()->GetWorldPos() - vPos;
				vDir.Normalize();

				Vector3 vAxis = Vector3::Axis[static_cast<int>(WORLD_AXIS::AXIS_Y)];

				float fNextAngle = RadToDeg(acosf(vAxis.Dot(vDir)));

				if (vDir.x < 0.f)
					fNextAngle *= -1.f;

				m_pObj->SetWorldRotZ(-fNextAngle);
#ifdef _DEBUG
				TCHAR strDebug[MAX_PATH] = {};
				swprintf_s(strDebug, TEXT("Turn Angle2: %.2f\n"), -fNextAngle);
				OutputDebugString(strDebug);
#endif
			}

			return;
		}
		else
		{
			Vector3 vAxis = Vector3::Axis[static_cast<int>(WORLD_AXIS::AXIS_Y)];

			float fNextAngle = RadToDeg(acosf(vAxis.Dot(vDir)));

			if (vDir.x < 0.f)
				fNextAngle *= -1.f;

			m_pObj->SetWorldRotZ(-fNextAngle);
			m_pObj->AddWorldPos(vDir * 200.f * fTime);
#ifdef _DEBUG
			TCHAR strDebug[MAX_PATH] = {};
			swprintf_s(strDebug, TEXT("Turn Angle3: %.2f\n"), -fNextAngle);
			OutputDebugString(strDebug);
#endif
		}
	}
}

void CNavigation2D::PostUpdate(float fTime)
{
	CComponent::PostUpdate(fTime);
}

void CNavigation2D::Collision(float fTime)
{
	CComponent::Collision(fTime);
}

void CNavigation2D::PreRender(float fTime)
{
	CComponent::PreRender(fTime);
}

void CNavigation2D::Render(float fTime)
{
	CComponent::Render(fTime);
}

void CNavigation2D::PostRender(float fTime)
{
	CComponent::PostRender(fTime);
}

CNavigation2D* CNavigation2D::Clone()
{
	return new CNavigation2D(*this);
}

void CNavigation2D::Save(FILE* pFile)
{
	CComponent::Save(pFile);
}

void CNavigation2D::Load(FILE* pFile)
{
	CComponent::Load(pFile);
}

bool CNavigation2D::Sort(CTile* a, CTile* b)
{
	if (a->GetG() + a->GetH() == b->GetG() + b->GetH())
	{
		return a->GetH() < b->GetH();
	}

	return a->GetG() + a->GetH() < b->GetG() + b->GetH();
}
