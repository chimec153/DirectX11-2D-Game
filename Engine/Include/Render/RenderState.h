#pragma once

#include "../Ref.h"

class CRenderState	:
	public CRef
{
	friend class CRenderManager;

protected:
	CRenderState();
	virtual ~CRenderState();

protected:
	ID3D11DeviceChild*	m_pState;
	ID3D11DeviceChild*	m_pPrevState;

public:
	virtual void SetState();
	virtual void ResetState();
};

