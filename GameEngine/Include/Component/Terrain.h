#pragma once
#include "SceneComponent.h"
class CTerrain :
	public CSceneComponent
{
	friend class CObj;

private:
	CTerrain();
	CTerrain(const CTerrain& terrain);
	virtual ~CTerrain() override;

private:
	int	m_iCountX;
	int m_iCountY;
	std::vector<Vector3>	m_vecPos;
	std::vector<VERTEX3D>	m_vecVtx;
	std::vector<Vector3>	m_vecNormal;
	ID3D11Texture2D*	m_pHeightMapTexture;
	TERRAINCBUFFER		m_tCBuffer;
	std::vector<Vector3>	m_vecFaceNormal;
	bool					m_bEditMode;
	class CDecal*			m_pDecal;
	class CTexture*			m_pBrushTexture;
	ScratchImage*			m_pTerrainImage;

public:
	bool CreateHeightMap(int iWidth, int iHeight, const std::string& strName, 
		const TCHAR *pFileName, const std::string& strPathKey = TEXTURE_PATH);
	void SetTiling(float fTileX, float fTileY);
	void ComputeNormal();
	void ComputeTangent();
	void SetSplatCount(int iCount);
	int GetIndex(const Vector3& vPos);
	float GetHeight(int iIndex);
	float GetHeight(const Vector3& vPos);
	const std::vector<Vector3>& GetVecPos()	const;
	void SetEditMode(bool bEdit);

public:
	virtual bool Init() override;
	virtual void Start() override;
	virtual void Update(float fTime) override;
	virtual void PostUpdate(float fTime) override;
	virtual void Collision(float fTime) override;
	virtual void PreRender(float fTime) override;
	virtual void Render(float fTime) override;
	virtual void PostRender(float fTime) override;
	virtual CTerrain* Clone();
public:
	virtual void Save(FILE* pFile) override;
	virtual void Load(FILE* pFile) override;

public:
	void CollisionInit(class CCollider* pSrc, class CCollider* pDest, float fTime);
	void CollisionStay(class CCollider* pSrc, class CCollider* pDest, float fTime);
	void CollisionEnd(class CCollider* pSrc, class CCollider* pDest, float fTime);

public:
	virtual void SpawnWindow();

private:
	void AddHeight(const Vector3& vCross, float fTime);

private:
	static std::unordered_map<std::string, class CTerrain*>	m_mapTerrain;
	static CTerrain* m_pMainTerrain;

public:
	static void SetMainTerrain(const std::string& strKey);
	static CTerrain* GetMainTerrain();
	static void AddTerrain(class CTerrain* pTerrain);
	static class CTerrain* FindTerrain(const std::string& strKey);
	static void Destroy();

};

