#pragma once
class CGameMode
{
	friend class CScene;

protected:
	CGameMode();
	~CGameMode();

protected:
	class CScene*		m_pScene;
	class CObj*			m_pPlayer;

public:
	void SetPlayer(class CObj* pPlayer);
	class CObj* GetPlayer()	const;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);

};

