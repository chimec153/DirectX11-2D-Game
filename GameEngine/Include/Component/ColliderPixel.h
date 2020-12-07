#pragma once
#include "Collider.h"
class CColliderPixel :
	public CCollider
{
	friend class CObj;

private:
	CColliderPixel();
	CColliderPixel(const CColliderPixel& col);
	virtual ~CColliderPixel();

private:
	PixelInfo	m_tInfo;
#ifdef _DEBUG
	class CMesh2D* m_pPixel;
#endif

public:
	const PixelInfo& GetInfo()	const;
	void SetPixel(const TCHAR* pFileName, const std::string& strPathName = TEXTURE_PATH);
	void SetPixelFromFullPath(const TCHAR* pFullPath);
	void SetChkColor(unsigned char r, unsigned char g, unsigned char b);
	void SetAlpha(unsigned char a);
	void SetIgnColor(unsigned char r, unsigned char g, unsigned char b);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CColliderPixel* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	virtual bool Collision(class CCollider* pDest);
};

