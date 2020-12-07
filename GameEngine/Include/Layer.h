#pragma once
#include "Ref.h"
class CLayer :
	public CRef
{
	friend class CScene;

private:
	CLayer();
	CLayer(const CLayer& layer);
	virtual ~CLayer();

private:
	int									m_iZOrder;
	std::list<class CObj*>				m_ObjList;
	bool								m_bStart;
	std::function<class CObj* (int)>	m_EditorObjCreate;
	class CScene*						m_pScene;
	std::vector<class CInstancing*>		m_vecInst;
	std::vector<class CSceneComponent*>	m_vecCom;
	bool								m_b2D;
	bool								m_bUI;
	class CInstancing*					m_pTileInst;
	class CTileMap*						m_pTileMap;

public:
	void SetZOrder(int iZ);
	int GetZOrder()	const;
	class CObj* FindObj(const std::string& strTag)	const;
	bool IsStart()	const;
	void AddObj(class CObj* pObj);
	void AddInst(class CInstancing* pInst);
	void AddCom(class CSceneComponent* pCom);
	void Clear();
	void Set2D();
	void SetUI();
	class CInstancing* GetTileInst()	const;
	void SetTileInst(class CInstancing* pInst);
	void SetTileMap(class CTileMap* pMap);
	class CTileMap* GetTileMap()	const;
	void SetFontPos(const Vector3& vPos);
	void AddTileInstState(const std::string& strKey);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Input(float fTime);
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

private:
	static bool SortY(class CObj* pSrc, class CObj* pDst);
	static bool SortYCom(class CSceneComponent* pSrc, class CSceneComponent* pDst);
	static bool SortZ(class CSceneComponent* pSrc, class CSceneComponent* pDst);
};

