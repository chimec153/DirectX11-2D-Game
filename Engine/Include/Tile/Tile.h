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
	float				m_fXTexIdx;
	float				m_fYTexIdx;
	Vector2			m_vSize;
	Matrix			m_matWV;
	bool			m_bAnim;
	std::vector<Vector2>	m_vecTexIdx;
	float			m_fFrameTime;
	float			m_fMaxTime;
	int				m_iFrame;
	class CTileMap* m_pMap;
	bool			m_bOpen;
	bool			m_bClose;
	float			m_fH;
	float			m_fG;
	class CTile*	m_pParent;
	Vector4			m_vColor;

public:
	const Vector2 GetTexIdx()	const;
	const Vector2 GetSize()	const;
	const Matrix& GetWV()	const;
	const Vector2 GetTextureSize()	const;
	void SetAnim(const std::vector<Vector2>& iStart, float vecFrame);
	class CTileMap* GetMap()	const;
	TILE_OPTION GetTileOption()	const;
	void SetTileOption(TILE_OPTION eOp);
	int GetIndexX()	const;
	int GetIndexY()	const;
	void SetOpen(bool bOpen);
	void SetClose(bool bClose);
	bool IsOpen()	const;
	bool IsClose()	const;
	void SetH(float fH);
	void SetG(float fG);
	float GetH()	const;
	float GetG()	const;
	class CTile* GetParent()	const;
	void SetParent(class CTile* pParent);
	void SetTexIdx(const Vector2& vIdx);
	void SetColor(const Vector4& vColor);
	const Vector4& GetColor()	const;
		
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

