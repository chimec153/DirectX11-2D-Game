#pragma once

#include "../Ref.h"

class CObj :
	public CRef
{
	friend class CScene;

protected:
	CObj();
	CObj(const CObj& obj);
	virtual ~CObj() = 0;

protected:
	bool				m_bStart;
	class CScene*		m_pScene;

public:
	bool IsStart()	const
	{
		return m_bStart;
	}

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
};

