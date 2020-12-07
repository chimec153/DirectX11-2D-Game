#pragma once

#include "../GameEngine.h"

class CRenderState
{
	friend class CRenderManager;

protected:
	CRenderState();
	virtual ~CRenderState();

protected:
	std::string			m_strTag;
	ID3D11DeviceChild*	m_pState;
	ID3D11DeviceChild*	m_pPrevState;

public:
	virtual void SetState();
	virtual void ResetState();
	const std::string& GetTag()	const;
	void SetTag(const std::string& strKey);
};

