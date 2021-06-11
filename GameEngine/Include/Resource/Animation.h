#pragma once

#include "../Ref.h"
#include "Skeleton.h"
#include "ResourceManager.h"

typedef struct _tagNotify
{
	class CAnimationSequence* pSequence;
	std::vector<std::function<void(float)>>	vecFunc;
	int							iFrame;
	float						fTime;
	std::string					strKey;
	bool						bCall;
}NOTIFY, *PNOTIFY;

class CAnimation :
	public CRef
{
private:
	CAnimation();
	CAnimation(class CAnimation& ani);
	virtual ~CAnimation();

private:
	class CSkeleton*				m_pSkeleton;
	class CScene*					m_pScene;
	float							m_fTime;
	class CAnimationSequence*		m_pDefault;
	class CAnimationSequence*		m_pCurrent;
	class CAnimationSequence*		m_pChange;
	std::unordered_map<size_t, class CAnimationSequence*>	m_mapClip;
	class CComputeShader*			m_pShader;
	ANIMATIONCBUFFER				m_tCBuffer;
	class CSceneComponent*			m_pOwner;
	float							m_fGlobalTime;
	std::list<class CBoneSocket*>	m_SocketList;
	std::vector<PNOTIFY>			m_vecNotify;
	std::vector<Matrix>				m_vecFrames;

	PSTRUCTUREDBUFFER				m_pBuffer;
	ID3D11Buffer*					m_pBoneBuffer;
	ID3D11UnorderedAccessView*		m_pBoneUAV;
	ID3D11Buffer*					m_pBoneSrcBuffer;
#ifdef _DEBUG
	ID3D11Buffer* m_pDebugBuffer;
#endif

public:
	void SetOwner(class CSceneComponent* pCom);
	class CSkeleton* GetSkeleton()	const;
	void SetInstIndex(int iIndex);
	void SetFrame(int iFrame);
	template <typename T>
	static T* CreateAnimation(const std::string& strKey,
		class CScene* pScene, const std::string& strBone = "",
		const char* pFilePath = nullptr, const std::string& strPathKey = MESH_PATH)
	{
		T* pAni = new T;

		pAni->SetName(strKey);
		pAni->m_pScene = pScene;

		if (!pAni->Init())
		{
			SAFE_RELEASE(pAni);
			return nullptr;
		}

		if (pFilePath)
		{
			pAni->LoadSkeleton(strBone, pFilePath, strPathKey);
		}

		return pAni;
	}

public:
	void AddSequence(const std::string& strKey, const std::string& strSeq);
	void ChangeSequence(const std::string& strKey);
	void SetSkeleton(class CSkeleton* pSkel);
	void SetSkeleton(const std::string& strKey);
	void LoadSkeleton(const std::string& strKey, const char* pFilePath, const std::string& strPathKey = MESH_PATH);

private:
	class CAnimationSequence* FindSequence(const std::string& strKey)	const;

public:
	void CreateNotify(const std::string& strKey, const std::string& strSeq, int iFrame);
	void AddCallBack(const std::string& strKey, void(*pFunc)(float));
	template <typename T>
	void AddCallBack(const std::string& strKey, T* pObj, void(T::* pFunc)(float))
	{
		PNOTIFY pNot = FindNotify(strKey);

		if (!pNot)
			return;

		pNot->vecFunc.push_back(std::bind(pFunc, pObj, std::placeholders::_1));
	}

	PNOTIFY FindNotify(const std::string& strKey);

public:
	void CreateSocket(const std::string& strKey, const std::string& strBone);
	void SetSocketComponent(const std::string& strKey, class CSceneComponent* pCom);
	class CBoneSocket* FindSocket(const std::string& strKey);
	void DeleteSocket(const std::string& strKey);
	void SetSocketOffset(const std::string& strKey, const Vector3& vOffset);
	void SetSocketRotation(const std::string& strKey, const Vector4& vRot);

public:
	bool Init();
	void Start();
	void Update(float fTime);
	CAnimation* Clone();

public:
	void SetShader();
	void ResetShader();

public:
	void SpawnWindow();

};