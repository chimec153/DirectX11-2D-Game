#include "pch.h"
#include "SoulMonster.h"
#include "../../IdleState.h"

CSoulMonster::CSoulMonster()    :
    m_pState(new CIdleState)
	, m_eBossType(BOSS_TYPE::NONE)
{
}

CSoulMonster::CSoulMonster(const CSoulMonster& obj) :
    CObj(obj)
	, m_eBossType(obj.m_eBossType)
{
    if (obj.m_pState)
    {
        m_pState = obj.m_pState->Clone();
    }
    else
    {
        m_pState = nullptr;
    }
}

CSoulMonster::~CSoulMonster()
{
    SAFE_RELEASE(m_pState);
}

State CSoulMonster::GetState() const
{
    return m_pState->GetState();
}

void CSoulMonster::SetState(State eStat)
{
    m_pState->SetState(eStat);
}

bool CSoulMonster::Init()
{
    if (!CObj::Init())
        return false;

    return true;
}

void CSoulMonster::Start()
{
    CObj::Start();
}

void CSoulMonster::Update(float fTime)
{
    CObj::Update(fTime);
}

void CSoulMonster::PostUpdate(float fTime)
{
    CObj::PostUpdate(fTime);
}

void CSoulMonster::Collision(float fTime)
{
    CObj::Collision(fTime);
}

void CSoulMonster::PreRender(float fTime)
{
    CObj::PreRender(fTime);
}

void CSoulMonster::Render(float fTime)
{
    CObj::Render(fTime);
}

void CSoulMonster::PostRender(float fTime)
{
    CObj::PostRender(fTime);
}

CSoulMonster* CSoulMonster::Clone()
{
    return new CSoulMonster(*this);
}

void CSoulMonster::Save(FILE* pFile)
{
    CObj::Save(pFile);

    State eStat = GetState();

    fwrite(&eStat, 4, 1, pFile);

    m_pState->Save(pFile);

    fwrite(&m_eBossType, 4, 1, pFile);
}

void CSoulMonster::Load(FILE* pFile)
{
    CObj::Load(pFile);

    State eStat = State::SLEEP;

    fread(&eStat, 4, 1, pFile);

    m_pState->Load(pFile);
    m_pState->SetObj(this);

    fread(&m_eBossType, 4, 1, pFile);
}
