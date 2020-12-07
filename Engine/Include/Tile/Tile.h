#pragma once
#include "../Component/SceneComponent.h"
class CTile :
	public CSceneComponent
{
	friend class CTileMap;
	friend class CObj;

private:
	CTile();
	CTile(const CTile& tile);
	virtual ~CTile();

private:
	TILE_OPTION		m_eTileOption;
	TILE_TYPE		m_eTileType;
	int				m_iIdx;
	int				m_iX;
	int				m_iY;
	int				m_iXTexIdx;
	int				m_iYTexIdx;
	Vector2			m_vSize;
	Matrix			m_matWV;
	bool			m_bAnim;
	std::vector<Vector2>	m_vecTexIdx;
	float			m_fFrameTime;
	float			m_fMaxTime;
	int				m_iFrame;

public:
	const Vector2 GetTexIdx()	const;
	const Vector2 GetSize()	const;
	const Matrix& GetWV()	const;
	const Vector2 GetTextureSize()	const;
	void SetAnim(const Vector2& iStart, int iCount, float fMaxTime);
		
public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CTile* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

};

