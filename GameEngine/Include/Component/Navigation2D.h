#pragma once
#include "Component.h"

class CNavigation2D :
	public CComponent
{
	friend class CObj;

private:
	CNavigation2D();
	CNavigation2D(const CNavigation2D& nav);
	virtual ~CNavigation2D();

private:
	class CTileMap* m_pMap;
	std::queue<class CTile*> m_Path;
#ifdef _DEBUG
	class CTile* m_pPrevTile;
	class CTile* m_pPrevMouseTile;
#endif

public:
	void SetTileMap(class CTileMap* pMap);
	void FindPath(const Vector3& vStart, const Vector3& vEnd);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CNavigation2D* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

private:
	static bool Sort(class CTile* a, class CTile* b);
};

