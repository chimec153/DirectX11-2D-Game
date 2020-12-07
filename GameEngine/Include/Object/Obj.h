#pragma once

#include "../Ref.h"
#include "../Component/SceneComponent.h"

class CObj :
	public CRef
{
	friend class CScene;
	friend class CEngine;
	friend class CLayer;

protected:
	CObj();
	CObj(const CObj& obj);
	virtual ~CObj();

protected:
	bool								m_bStart;
	class CScene*						m_pScene;
	class CInputObj*					m_pInput;
	class CLayer*						m_pLayer;

public:
	bool IsStart()	const;
	void SetScene(class CScene* pScene);
	class CScene* GetScene()	const;
	class CLayer* GetLayer()	const;
	void SetLayer(class CLayer* pLayer);

protected:
	class CSceneComponent*				m_pRootComponent;
	std::vector<class CObjComponent*>	m_vecObjComponent;
	int									m_iObjClassType;

public:
	int GetClassType()	const;

protected:
	std::function<void(const std::string&)>		m_EditorDelete;

public:
	template <typename T>
	void SetEditorDelete(T* pObj, void(T::*pFunc)(const std::string&))
	{
		m_EditorDelete = bind(pFunc, pObj, std::placeholders::_1);
	}

public:
	void SetRootComponent(class CSceneComponent*);
	class CSceneComponent* FindSceneComponent(const std::string& strTag);
	template <typename T>
	T* FindComByType()
	{
		return m_pRootComponent->FindComByType<T>();
	}
	virtual void Notify(const std::string& strTag);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Input(float fTime);
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CObj* Clone();
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
	template <typename T>
	T* CreateComponent(const std::string& strName)
	{
		T* pComponent = new T;

		pComponent->SetName(strName);
		pComponent->m_pScene = m_pScene;
		pComponent->m_pObj = this;
		pComponent->m_pLayer = m_pLayer;

		if (!pComponent->Init())
		{
			SAFE_RELEASE(pComponent);
			return nullptr;
		}

		if (pComponent->GetType() == COMPONENT_TYPE::CT_OBJECT)
		{
			pComponent->AddRef();
			m_vecObjComponent.push_back((CObjComponent*)pComponent);
		}

		if (m_bStart)
			pComponent->Start();

		return pComponent;		
	}

public:
	void GetAllComponentName(std::vector<Hierarchy>& vecHierarchy);
	void GetAllComponent(std::vector<CSceneComponent*>& vecCom);
};

