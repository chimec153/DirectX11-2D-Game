#include "RenderState.h"

CRenderState::CRenderState()	:
	m_pState(nullptr),
	m_pPrevState(nullptr)
{
}

CRenderState::~CRenderState()
{
	SAFE_RELEASE(m_pState);
	SAFE_RELEASE(m_pPrevState);
}

void CRenderState::SetState()
{
}

void CRenderState::ResetState()
{
}

const std::string& CRenderState::GetTag() const
{
	return m_strTag;
}

void CRenderState::SetTag(const std::string& strKey)
{
	m_strTag = strKey;
}
