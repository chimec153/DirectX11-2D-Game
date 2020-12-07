#pragma once
#include "Component.h"
class CSceneComponent :
	public CComponent
{
	friend class CObj;

protected:
	CSceneComponent();
	CSceneComponent(const CSceneComponent& com);
	virtual ~CSceneComponent() = 0;

protected:
	class CTransform*				m_pTransform;
	CSceneComponent*				m_pParent;
	std::vector<CSceneComponent*>	m_vecChild;
	SCENE_COMPONENT_TYPE			m_eSceneComponentType;
	SCENECOMPONENT_CLASS_TYPE		m_eSceneComponentClassType;

protected:
	class CMaterial*				m_pMaterial;
	class CMesh*					m_pMesh;
	bool							m_bInst;
	std::vector<class CRenderState*>	m_vecRenderState;

public:
	class CMesh* GetMesh()	const;
	void SetMesh(const std::string& strName);
	void SetMesh(class CMesh* pMesh);
	class CMaterial* GetMaterial()	const;
	void SetMaterial(class CMaterial* pMaterial);
	bool IsInstanced()	const;
	void SetInstance(bool bInst = true);
	void SetLayer(class CLayer* pLayer);

public:
	void SetTexture(REGISTER_TYPE eType, const std::string& strTag, int iRegister = 0, int iCount = 1,
		unsigned int iType = (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL | (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX);
	void SetTexture(REGISTER_TYPE eType, class CTexture* pTex, int iRegister = 0, int iCount = 1,
		unsigned int iType = (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL | (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX);
	void SetTexture(REGISTER_TYPE eType, const std::string& strTag, const TCHAR* pFileName, const std::string& strPathName = TEXTURE_PATH, int iRegister = 0, int iCount = 1,
		unsigned int iType = (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL | (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX);
	void SetTextureFromFullPath(REGISTER_TYPE eType, const std::string& strTag, const TCHAR* pFullPath, int iRegister = 0, int iCount = 1,
		unsigned int iType = (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL | (int)SHADER_CBUFFER_TYPE::CBUFFER_VERTEX);

public:
	bool AddChild(CSceneComponent* pChild);
	bool DeleteChild(CSceneComponent* pChild);
	bool Detach(CSceneComponent* pChild);
	SCENE_COMPONENT_TYPE GetSceneComponentType()	const;
	CSceneComponent* FindComponent(const std::string& strTag);
	template <typename T>
	T* FindComByType()
	{
		if (typeid(T) == typeid(*this))
		{
			AddRef();

			return (T*)this;
		}

		else
		{
			size_t iSz = m_vecChild.size();

			for (size_t i = 0; i < iSz; ++i)
			{
				T* pCom = m_vecChild[i]->FindComByType<T>();

				if (pCom)
					return pCom;
			}
		}

		return nullptr;
	}
	SCENECOMPONENT_CLASS_TYPE GetSceneComponentClassType()	const;
	void SetScene(class CScene* pScene);
	void SetSceneComType(SCENE_COMPONENT_TYPE eType);
	CSceneComponent* GetParent()	const;
	void SetObj(class CObj* pObj);
	void AddRenderState(const std::string& strKey);
	class CRenderState* FindRenderState(const std::string& strKey);
	void DeleteRenderState(const std::string& strKey);
	void SetState();
	void ResetState();

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CSceneComponent* Clone() = 0;

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	void SetInheritScale(bool bInherit);
	void SetInheritRotX(bool bInherit);
	void SetInheritRotY(bool bInherit);
	void SetInheritRotZ(bool bInherit);
	void InheritScale();
	void InheritRot();
	void InheritPos();

public:
	void SetRelativeScale(const Vector3& v);
	void SetRelativeScale(float x, float y, float z);
	void SetRelativePos(const Vector3& v);
	void SetRelativePos(float x, float y, float z);
	void SetRelativeRot(const Vector3& v);
	void SetRelativeRot(float x, float y, float z);
	void SetRelativeRotX(float x);
	void SetRelativeRotY(float y);
	void SetRelativeRotZ(float z);
	void AddRelativeScale(const Vector3& v);
	void AddRelativeScale(float x, float y, float z);
	void AddRelativePos(const Vector3& v);
	void AddRelativePos(float x, float y, float z);
	void AddRelativeRot(const Vector3& v);
	void AddRelativeRot(float x, float y, float z);
	void AddRelativeRotX(float x);
	void AddRelativeRotY(float y);
	void AddRelativeRotZ(float z);

public:
	Vector3 GetVelocityScale()			const;
	Vector3 GetVelocityRot()			const;
	Vector3 GetVelocity()				const;
	float GetVelocityAmt()				const;
	Vector3 GetRelativeScale()			const;
	Vector3 GetRelativeRot()			const;
	Vector3 GetRelativePos()			const;
	Vector3 GetRelativeAxis(WORLD_AXIS axis)	const;

public:
	Vector3 GetWorldScale()				const;
	Vector3 GetWorldRot()				const;
	Vector3 GetWorldPos()				const;
	Vector3 GetWorldAxis(WORLD_AXIS axis)		const;
	Vector3 GetPivot()					const;
	Vector3 GetMeshSize()				const;

public:
	Matrix GetMatScale()				const;
	Matrix GetMatRot()					const;
	Matrix GetMatPos()					const;
	Matrix GetMatWorld()				const;

public:
	void SetWorldScale(const Vector3& v);
	void SetWorldScale(float x, float y, float z);
	void SetWorldPos(const Vector3& v);
	void SetWorldPos(float x, float y, float z);
	void SetWorldRot(const Vector3& v);
	void SetWorldRot(float x, float y, float z);
	void SetWorldRotX(float x);
	void SetWorldRotY(float y);
	void SetWorldRotZ(float z);
	void AddWorldScale(const Vector3& v);
	void AddWorldScale(float x, float y, float z);
	void AddWorldPos(const Vector3& v);
	void AddWorldPos(float x, float y, float z);
	void AddWorldRot(const Vector3& v);
	void AddWorldRot(float x, float y, float z);
	void AddWorldRotX(float x);
	void AddWorldRotY(float y);
	void AddWorldRotZ(float z);
	void SetPivot(const Vector3& v);
	void SetPivot(float x, float y, float z);
	void SetMeshSize(const Vector3& v);

public:
	void GetAllComponentName(std::vector<Hierarchy>& vecHierarchy);
	void GetAllComponent(std::vector<CSceneComponent*>& vecCom);
};

