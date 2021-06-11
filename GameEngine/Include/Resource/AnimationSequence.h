#pragma once
#include "../Ref.h"
typedef struct _tagKeyFrame
{
	double	dTime;
	Vector3	vPos;
	Vector3	vScale;
	Vector4	vRot;
}KEYFRAME, * PKEYFRAME;

typedef struct _tagBoneKeyFrame
{
	std::vector<PKEYFRAME>	vecKeyFrame;
	int		iBoneIndex;
	int		iRef;

	_tagBoneKeyFrame() :
		iBoneIndex(-1)
		, iRef(1)
	{
	}

	~_tagBoneKeyFrame()
	{
		SAFE_DELETE_VECLIST(vecKeyFrame);
	}
}BONEKEYFRAME, *PBONEKEYFRAME;

class CAnimationSequence :
	public CRef
{
	friend class CAnimation;
	friend class CResourceManager;

private:
	CAnimationSequence();
	CAnimationSequence(const CAnimationSequence& seq);
	virtual ~CAnimationSequence();

private:
	class CAnimation*			m_pAni;
	bool						m_bLoop;
	float						m_fPlayTime;
	float						m_fPlayScale;
	float						m_fFrameTime;
	float						m_fStartTime;
	float						m_fEndTime;
	float						m_fTimeLength;
	int							m_iStartFrame;
	int							m_iEndFrame;
	int							m_iFrameLength;
	int							m_iFrameMode;
	int							m_iChangeFrame;
	std::vector<PBONEKEYFRAME>	m_vecKeyFrame;
	bool						m_bEnd;
	char						m_pFullPath[MAX_PATH];
	std::string					m_strKey;

	ID3D11Buffer*				m_pBuffer;
	ID3D11ShaderResourceView*	m_pSRV;

public:
	int GetFrameCount()	const;
	bool IsEnd()	const;

public:
	bool Init();
	CAnimationSequence* Clone();
	void GetKeyFrame(std::vector<PBONEKEYFRAME>& vecFrame);
	bool LoadFbxAnimation(const char* pFullPath, bool bLoop);
	bool Load(const TCHAR* pFileName, const std::string& strPathKey = MESH_PATH);
	bool LoadMultiByte(const char* pFileName, const std::string& strPathKey = MESH_PATH);
	bool LoadFullPath(const TCHAR* pFullPath);
	bool LoadFullPathMultiByte(const char* pFullPath);
	bool Save(const TCHAR* pFileName, const std::string& strPathKey = MESH_PATH);
	bool SaveMultiByte(const char* pFileName, const std::string& strPathKey = MESH_PATH);
	bool SaveFullPath(const TCHAR* pFullPath);
	bool SaveFullPathMultiByte(const char* pFullPath);
	bool CreateBuffer(int iCount,int iSize, void* pData);

public:
	void SetPlayScale(float fScale);
	void SetPlayTime(float fTime);

public:
	bool CreateSequence(struct _tagFbxAnimationClip* pClip, bool bLoop);
	bool CreateSequence(const std::string& strKey, bool bLoop, int iStartFrame, int iEndFrame, float fPlayTime, const std::vector<_tagBoneKeyFrame*>& vecFrame);
	bool CreateSequence(bool bLoop, const TCHAR* pFilePath, const std::string& strPathKey = MESH_PATH);
	bool CreateSequence(const TCHAR* pFullPath);
	bool CreateSequenceMultiByte(const char* pFullPath);
	void SetShader(int iRegister);
	void ResetShader(int iRegister);

public:
	void SpawnWindow() override;
};

