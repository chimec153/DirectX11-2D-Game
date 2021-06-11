#pragma once
#include "../Ref.h"

typedef struct _tagBone
{
	std::string	strName;
	int			iDepth;
	int			iParentIndex;
	Matrix		 matOffset;
	Matrix		 matBone;
	int			iRef;
	_tagBone() :
		strName()
		, iDepth(0)
		, iParentIndex(-1)
		, matOffset()
		, matBone()
		, iRef(1)
	{
	}
}BONE, *PBONE;

class CSkeleton :
	public CRef
{
	friend class CMesh;
	friend class CResourceManager;

private:
	CSkeleton();
	CSkeleton(const CSkeleton& skel);
	virtual ~CSkeleton();

private:
	std::vector<PBONE>			m_vecBone;
	ID3D11Texture2D*			m_pBoneTex;
	ID3D11ShaderResourceView*	m_pSRV;
	ID3D11Buffer*				m_pBuffer;
#ifdef _DEBUG
	ID3D11Buffer* m_pDebugBuffer;
#endif

public:
	size_t GetBoneCount()	const;
	int GetBoneIndex(const std::string& strKey)	const;
	PBONE GetBone(int iIdx = 0)	const;
	PBONE GetBone(const std::string& strKey)	const;
	const Matrix& GetBoneMatrix(int iIdx)	const;
	const Matrix GetBoneMatrix(const std::string& strKey)	const;

public:
	void AddBone(const PBONE bone);
	bool CreateBoneTexture(void* pData);
	bool UpdateBoneTexture(const std::vector<Matrix>& vecMat);
	bool Save(const char* pFilePath, const std::string& strPathKey = MESH_PATH);
	bool SaveFullPath(const char* pFullPath);
	bool Load(const std::string& strKey, const char* pFilePath, const std::string& strPathKey = MESH_PATH);
	bool LoadFullPath(const std::string& strKey, const char* pFullPath);
	void SetShader();
	void ResetShader();
	void Update(float fTime, const Matrix& matWorld);
	virtual CSkeleton* Clone();

public:
	template <typename T>
	static T* CreateSkeleton(const std::string& strKey)
	{
		T* pSkel = new T;

		pSkel->SetName(strKey);

		return pSkel;
	}

public:
	void SpawnWindow() override;
};

