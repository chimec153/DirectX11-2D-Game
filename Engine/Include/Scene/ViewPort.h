#pragma once
#include "../Ref.h"
class CViewPort :
	public CRef
{
	friend class CScene;

private:
	CViewPort();
	virtual ~CViewPort();

private:
	int							m_iZOrder;
	std::list<class CUIObject*>	m_UIList;

public:
	int GetZOrder()	const;
	void AddUI(class CUIObject* pUI);

public:
	bool Init();
	void Start();
	void Input(float fTime);
	void Update(float fTime);
	void PostUpdate(float fTime);
	void Collision(float fTime);
	void PreRender(float fTime);
	void Render(float fTime);
	void PostRender(float fTime);

public:
	void Save(FILE* pFile);
	void Load(FILE* pFile);

private:
	static bool SortY(class CUIObject* pObj1, class CUIObject* pObj2);
};

