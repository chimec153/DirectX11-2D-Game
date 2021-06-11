#pragma once
#include "Object\Obj.h"
class CLight :
    public CObj
{
    friend class CScene;

private:
    CLight();
    CLight(const CLight& obj);
    virtual ~CLight();

public:
    virtual bool Init();
    virtual void Start();
    virtual void Update(float fTime);
    virtual void PostUpdate(float fTime);
    virtual void Collision(float fTime);
    virtual void PreRender(float fTime);
    virtual void Render(float fTime);
    virtual void PostRender(float fTime);
    virtual CLight* Clone();

public:
    virtual void Save(FILE* pFile);
    virtual void Load(FILE* pFile);
};

