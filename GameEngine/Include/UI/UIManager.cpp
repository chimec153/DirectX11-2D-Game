#include "UIManager.h"
#include "../Device.h"

DEFINITION_SINGLE(CUIManager)

CUIManager::CUIManager()
{

}

CUIManager::~CUIManager()
{

}

const Matrix& CUIManager::GetProj() const
{
	return m_matProj;
}

void CUIManager::SetProj(const Matrix& mat)
{
	m_matProj = mat;
}

bool CUIManager::Init()
{
	Resolution tRS = RESOLUTION;

	m_matProj = DirectX::XMMatrixOrthographicOffCenterLH(0.f, (float)tRS.iWidth,
		0.f, (float)tRS.iHeight, 0.f, 3000.f);

	return true;
}
