#include "Collision.h"
#include "../Component/ColliderRect.h"
#include "../Component/ColliderCircle.h"
#include "../Component/ColliderOBB2D.h"
#include "../Component/ColliderPixel.h"
#include "../Component/ColliderLine.h"
#include "../Component/ColliderPoint.h"

bool CCollision::CollisionRectToRect(CColliderRect* pSrc, CColliderRect* pDest)
{
	Vector3 vCross;

	if (CollisionRectToRect(vCross, ((CColliderRect*)pSrc)->GetInfo(), ((CColliderRect*)pDest)->GetInfo()))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);

		return true;
	}

	return false;
}

bool CCollision::CollisionRectToRect(Vector3& vCross, const RectInfo& tSrc, const RectInfo& tDest)
{
	if (tSrc.fL > tDest.fR)
		return false;

	if (tSrc.fR < tDest.fL)
		return false;

	if (tSrc.fB > tDest.fT)
		return false;

	if (tSrc.fT < tDest.fB)
		return false;

	vCross.x = ((tSrc.fR < tDest.fR ? tSrc.fR : tDest.fR)
		+ (tSrc.fL < tDest.fL ? tDest.fL : tSrc.fL)) / 2.f;
	vCross.y = ((tSrc.fT < tDest.fT ? tSrc.fT : tDest.fT)
		+ (tSrc.fB < tDest.fB ? tDest.fB : tSrc.fB)) / 2.f;

	return true;
}

bool CCollision::CollisionCircleToRect(CColliderCircle* pSrc, CColliderRect* pDest)
{
	Vector3 vCross;

	if (CollisionCircleToRect(vCross, ((CColliderCircle*)pSrc)->GetInfo(), ((CColliderRect*)pDest)->GetInfo()))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);

		return true;
	}

	return false;
}

bool CCollision::CollisionCircleToRect(Vector3& vCross, const CircleInfo& tSrc, const RectInfo& tDest)
{
	OBBInfo tOBB = {};

	tOBB.fLength[(int)WORLD_AXIS::AXIS_X] = (tDest.fR - tDest.fL) / 2.f;
	tOBB.fLength[(int)WORLD_AXIS::AXIS_Y] = (tDest.fT - tDest.fB) / 2.f;
	tOBB.vPos[(int)WORLD_AXIS::AXIS_X] = (tDest.fR + tDest.fL) / 2.f;
	tOBB.vPos[(int)WORLD_AXIS::AXIS_Y] = (tDest.fT + tDest.fB) / 2.f;

	return CollisionOBB2DToCircle(vCross, tOBB, tSrc);
}

bool CCollision::CollisionCircleToCircle(CColliderCircle* pSrc, CColliderCircle* pDest)
{
	Vector3 vCross;

	if (CollisionCircleToCircle(vCross, ((CColliderCircle*)pSrc)->GetInfo(), ((CColliderCircle*)pDest)->GetInfo()))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);

		return true;
	}

	return false;
}

bool CCollision::CollisionCircleToCircle(Vector3& vCross, const CircleInfo& tSrc, const CircleInfo& tDest)
{
	Vector3 vDst = tSrc.vCenter - tDest.vCenter;

	float fDst = vDst.Length();

	float fSum = tSrc.fRadius + tDest.fRadius;

	if (fDst < fSum)
	{
		vDst *= tSrc.fRadius / fSum;

		vCross = tSrc.vCenter - vDst;

		return true;
	}

	return false;
}

bool CCollision::CollisionCircleToPoint(CColliderCircle* pSrc, CColliderPoint* pDest)
{
	Vector3 vCross;

	Vector3 vPos = {};

	if (pSrc->IsUI())
		vPos = pDest->GetUIInfo();

	else
		vPos = pDest->GetInfo();

	if (CollisionCircleToPoint(vCross, ((CColliderCircle*)pSrc)->GetInfo(), vPos))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);

		return true;
	}

	return false;
}

bool CCollision::CollisionCircleToPoint(Vector3& vCross, const CircleInfo& tSrc, const Vector3& tDest)
{
	vCross = tDest;

	return (tSrc.vCenter - tDest).Length() < tSrc.fRadius;
}

bool CCollision::CollisionOBB2DToOBB2D(CColliderOBB2D* pSrc, CColliderOBB2D* pDest)
{
	Vector3 vCross;

	if (CollisionOBB2DToOBB2D(vCross, ((CColliderOBB2D*)pSrc)->GetInfo(), ((CColliderOBB2D*)pDest)->GetInfo()))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);

		return true;
	}

	return false;
}

bool CCollision::CollisionOBB2DToOBB2D(Vector3& vCross, const OBBInfo& tSrc, const OBBInfo& tDest)
{
	Vector3 vDst = tSrc.vPos - tDest.vPos;

	for (int i = 0; i < (int)WORLD_AXIS::AXIS_Z; ++i)
	{
		float fX = tSrc.fLength[i];

		float fProjDToS = 0.f;

		for (int j = 0; j < (int)WORLD_AXIS::AXIS_Z; ++j)
			fProjDToS += abs(tSrc.vAxis[i].Dot(tDest.vAxis[j] * tDest.fLength[j]));

		float fDst = abs(vDst.Dot(tSrc.vAxis[i]));

		if (fDst > fX + fProjDToS)
			return false;
	}

	for (int i = 0; i < (int)WORLD_AXIS::AXIS_Z; ++i)
	{
		float fX = tDest.fLength[i];

		float fProjSToD = 0.f;

		for (int j = 0; j < (int)WORLD_AXIS::AXIS_Z; ++j)
			fProjSToD += abs(tDest.vAxis[i].Dot(tSrc.vAxis[j] * tSrc.fLength[j]));

		float fDst = abs(vDst.Dot(tDest.vAxis[i]));

		if (fDst > fX + fProjSToD)
			return false;
	}

	float fLength = sqrtf(tSrc.fLength[(int)WORLD_AXIS::AXIS_X] * tSrc.fLength[(int)WORLD_AXIS::AXIS_X]
		+ tSrc.fLength[(int)WORLD_AXIS::AXIS_Y] * tSrc.fLength[(int)WORLD_AXIS::AXIS_Y]);

	float fLength2 = sqrtf(tDest.fLength[(int)WORLD_AXIS::AXIS_X] * tDest.fLength[(int)WORLD_AXIS::AXIS_X]
		+ tDest.fLength[(int)WORLD_AXIS::AXIS_Y] * tDest.fLength[(int)WORLD_AXIS::AXIS_Y]);

	vCross = tSrc.vPos - vDst * fLength / (fLength + fLength2);

	return true;
}

bool CCollision::CollisionOBB2DToRect(CColliderOBB2D* pSrc, CColliderRect* pDest)
{
	Vector3 vCross;

	OBBInfo tInfo = {};

	RectInfo tRI = ((CColliderRect*)pDest)->GetInfo();

	tInfo.fLength[(int)WORLD_AXIS::AXIS_X] = (tRI.fR - tRI.fL)/2.f;
	tInfo.fLength[(int)WORLD_AXIS::AXIS_Y] = (tRI.fT - tRI.fB)/2.f;

	tInfo.vPos.x = (tRI.fL + tRI.fR) / 2.f;
	tInfo.vPos.y = (tRI.fT + tRI.fB) / 2.f;

	if (CollisionOBB2DToOBB2D(vCross, ((CColliderOBB2D*)pSrc)->GetInfo(), tInfo))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);

		return true;
	}

	return false;
}

bool CCollision::CollisionOBB2DToCircle(CColliderOBB2D* pSrc, CColliderCircle* pDest)
{
	Vector3 vCross;

	if (CollisionOBB2DToCircle(vCross, ((CColliderOBB2D*)pSrc)->GetInfo(), ((CColliderCircle*)pDest)->GetInfo()))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);

		return true;
	}

	return false;
}

bool CCollision::CollisionOBB2DToCircle(Vector3& vCross, const OBBInfo& tSrc, const CircleInfo& tDest)
{
	Vector3 vDst = tSrc.vPos - tDest.vCenter;

	for (int i = 0; i < (int)WORLD_AXIS::AXIS_Z; ++i)
	{
		float fDst = abs(vDst.Dot(tSrc.vAxis[i]));

		if (fDst > tDest.fRadius + tSrc.fLength[i])
			return false;
	}

	float fDst = vDst.Length();

	vDst /= fDst;

	float fDot = abs(vDst.Dot(tSrc.vAxis[(int)WORLD_AXIS::AXIS_X] * tSrc.fLength[(int)WORLD_AXIS::AXIS_X]))
		+ abs(vDst.Dot(tSrc.vAxis[(int)WORLD_AXIS::AXIS_Y] * tSrc.fLength[(int)WORLD_AXIS::AXIS_Y]));

	if (fDot + tDest.fRadius < fDst)
		return false;

	float fDiag = sqrtf(tSrc.fLength[(int)WORLD_AXIS::AXIS_X] * tSrc.fLength[(int)WORLD_AXIS::AXIS_X] 
		+ tSrc.fLength[(int)WORLD_AXIS::AXIS_Y] * tSrc.fLength[(int)WORLD_AXIS::AXIS_Y]);

	vCross = tSrc.vPos + (tDest.vCenter - tSrc.vPos) * fDiag / (fDiag + tDest.fRadius);

	return true;
}

bool CCollision::CollisionOBB2DToPoint(CColliderOBB2D* pSrc, CColliderPoint* pDest)
{
	Vector3 vCross;

	Vector3 vPos = {};

	if (pSrc->IsUI())
		vPos = pDest->GetUIInfo();

	else
		vPos = pDest->GetInfo();

	if (CollisionOBB2DToPoint(vCross, ((CColliderOBB2D*)pSrc)->GetInfo(), vPos))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);

		return true;
	}

	return false;
}

bool CCollision::CollisionOBB2DToPoint(Vector3& vCross, const OBBInfo& tSrc, const Vector3& tDest)
{
	Vector3 vDst = tDest - tSrc.vPos;

	if (abs(vDst.Dot(tSrc.vAxis[(int)WORLD_AXIS::AXIS_X])) > tSrc.fLength[(int)WORLD_AXIS::AXIS_X])
		return false;

	else if (abs(vDst.Dot(tSrc.vAxis[(int)WORLD_AXIS::AXIS_Y])) > tSrc.fLength[(int)WORLD_AXIS::AXIS_Y])
		return false;

	vCross = tDest;

	return true;
}

bool CCollision::CollisionPixelToRect(CColliderPixel* pSrc, CColliderRect* pDest)
{
	Vector3 vCross;

	if (CollisionPixelToRect(vCross, ((CColliderPixel*)pSrc)->GetInfo(), ((CColliderRect*)pDest)->GetInfo()))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);

		return true;
	}

	return false;
}

bool CCollision::CollisionPixelToRect(Vector3& vCross, const PixelInfo& tSrc, const RectInfo& tDest)
{
	if (tDest.fL > tSrc.tRI.fR)
		return false;

	else if (tDest.fR < tSrc.tRI.fL)
		return false;

	else if (tDest.fT < tSrc.tRI.fB)
		return false;

	else if (tDest.fB > tSrc.tRI.fT)
		return false;

	int iStartX, iStartY, iEndX, iEndY;

	iStartX = (int)(tDest.fL < tSrc.tRI.fL ? tSrc.tRI.fL : tDest.fL);
	iStartY = (int)(tDest.fT > tSrc.tRI.fT ? tSrc.tRI.fT : tDest.fT);
	iEndX = (int)(tDest.fR < tSrc.tRI.fR ? tDest.fR : tSrc.tRI.fR);
	iEndY = (int)(tDest.fB > tSrc.tRI.fB ? tDest.fB : tSrc.tRI.fB);

	iStartX -= (int)(tSrc.tRI.fL);
	iStartY -= (int)(tSrc.tRI.fB);
	iEndX -= (int)(tSrc.tRI.fL);
	iEndY -= (int)(tSrc.tRI.fB);

	for (int i = iStartY - 1; i >= iEndY; --i)
	{
		for (int j = iStartX; j <= iEndX; ++j)
		{
			int idx = (tSrc.iHeight - i - 1) * tSrc.iWidth + j;

			switch (tSrc.eType)
			{
			case PIXEL_COLLISION_TYPE::COLOR_IGN:
				if (tSrc.pPixel[idx].r == tSrc.tColor.r &&
					tSrc.pPixel[idx].g == tSrc.tColor.g &&
					tSrc.pPixel[idx].b == tSrc.tColor.b)
					continue;

				vCross.x = j + tSrc.tRI.fL;
				vCross.y = i + tSrc.tRI.fB;

				return true;
			case PIXEL_COLLISION_TYPE::LESS_EQUAL_ALPHA:
				if (tSrc.pPixel[idx].a <= tSrc.tColor.a)
					continue;

				vCross.x = j + tSrc.tRI.fL;
				vCross.y = i + tSrc.tRI.fB;

				return true;
			case PIXEL_COLLISION_TYPE::COLOR_CONFIRM:
				if (tSrc.pPixel[idx].r == tSrc.tChkColor.r &&
					tSrc.pPixel[idx].g == tSrc.tChkColor.g &&
					tSrc.pPixel[idx].b == tSrc.tChkColor.b)
					continue;

				vCross.x = j + tSrc.tRI.fL;
				vCross.y = i + tSrc.tRI.fB;

				return true;
			}
		}
	}

	return false;
}

bool CCollision::CollisionPixelToCircle(CColliderPixel* pSrc, CColliderCircle* pDest)
{
	Vector3 vCross;

	if (CollisionPixelToCircle(vCross, ((CColliderPixel*)pSrc)->GetInfo(), ((CColliderCircle*)pDest)->GetInfo()))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);

		return true;
	}

	return false;
}

bool CCollision::CollisionPixelToCircle(Vector3& vCross, const PixelInfo& tSrc, const CircleInfo& tDest)
{
	Vector3 vMin = tDest.vCenter - tDest.fRadius;
	Vector3 vMax = tDest.vCenter + tDest.fRadius;

	if (vMin.x > tSrc.tRI.fR)
		return false;

	else if (vMax.x < tSrc.tRI.fL)
		return false;

	else if (vMax.y < tSrc.tRI.fB)
		return false;

	else if (vMin.y > tSrc.tRI.fT)
		return false;

	int iStartX, iStartY, iEndX, iEndY;

	iStartX = (int)(vMin.x < tSrc.tRI.fL ? tSrc.tRI.fL : vMin.x);
	iStartY = (int)(vMax.y > tSrc.tRI.fT ? tSrc.tRI.fT : vMax.y);
	iEndX = (int)(vMax.x < tSrc.tRI.fR ? vMax.x : tSrc.tRI.fR);
	iEndY = (int)(vMin.y > tSrc.tRI.fB ? vMin.y : tSrc.tRI.fB);

	iStartX -= (int)(tSrc.tRI.fL);
	iStartY -= (int)(tSrc.tRI.fB);
	iEndX -= (int)(tSrc.tRI.fL);
	iEndY -= (int)(tSrc.tRI.fB);

	for (int i = iStartY - 1; i >= iEndY; --i)
	{
		for (int j = iStartX; j <= iEndX; ++j)
		{
			int idx = (tSrc.iHeight - i - 1) * tSrc.iWidth + j;

			bool bResult = false;

			switch (tSrc.eType)
			{
			case PIXEL_COLLISION_TYPE::COLOR_IGN:
				if (tSrc.pPixel[idx].r == tSrc.tColor.r &&
					tSrc.pPixel[idx].g == tSrc.tColor.g &&
					tSrc.pPixel[idx].b == tSrc.tColor.b)
					continue;

				vCross.x = j + tSrc.tRI.fL;
				vCross.y = i + tSrc.tRI.fB;

				bResult = true;
				break;
			case PIXEL_COLLISION_TYPE::LESS_EQUAL_ALPHA:
				if (tSrc.pPixel[idx].a <= tSrc.tColor.a)
					continue;

				vCross.x = j + tSrc.tRI.fL;
				vCross.y = i + tSrc.tRI.fB;

				bResult = true;
				break;
			case PIXEL_COLLISION_TYPE::COLOR_CONFIRM:
				if (tSrc.pPixel[idx].r == tSrc.tChkColor.r &&
					tSrc.pPixel[idx].g == tSrc.tChkColor.g &&
					tSrc.pPixel[idx].b == tSrc.tChkColor.b)
					continue;

				vCross.x = j + tSrc.tRI.fL;
				vCross.y = i + tSrc.tRI.fB;

				bResult = true;
				break;
			}

			if (!bResult)
				continue;

			if (CollisionCircleToPoint(vCross, tDest, Vector3(j + tSrc.tRI.fL, i + tSrc.tRI.fB, 0.f)))
				return true;
		}
	}

	return false;
}

bool CCollision::CollisionPixelToOBB2D(CColliderPixel* pSrc, CColliderOBB2D* pDest)
{
	Vector3 vCross;

	if (CollisionPixelToOBB2D(vCross, ((CColliderPixel*)pSrc)->GetInfo(), ((CColliderOBB2D*)pDest)->GetInfo()))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);

		return true;
	}

	return false;
}

bool CCollision::CollisionPixelToOBB2D(Vector3& vCross, const PixelInfo& tSrc, const OBBInfo& tDest)
{
	RectInfo tRC = {};

	Vector3 vRT = tDest.vAxis[(int)WORLD_AXIS::AXIS_X] * tDest.fLength[(int)WORLD_AXIS::AXIS_X] 
		+ tDest.vAxis[(int)WORLD_AXIS::AXIS_Y] * tDest.fLength[(int)WORLD_AXIS::AXIS_Y];
	Vector3 vRB = tDest.vAxis[(int)WORLD_AXIS::AXIS_X] * tDest.fLength[(int)WORLD_AXIS::AXIS_X] 
		- tDest.vAxis[(int)WORLD_AXIS::AXIS_Y] * tDest.fLength[(int)WORLD_AXIS::AXIS_Y];

	Vector3 vMin = {};
	Vector3 vMax = {};

	if (vRT.x < 0)
		vRT.x *= -1.f;

	if (vRT.y < 0)
		vRT.y *= -1.f;

	if (vRB.x < 0)
		vRB.x *= -1.f;

	if (vRB.y < 0)
		vRB.y *= -1.f;

	vMax.x = vRT.x > vRB.x ? vRT.x : vRB.x;
	vMax.y = vRT.y > vRB.y ? vRT.y : vRB.y;

	vMin = -vMax;

	vMax += tDest.vPos;
	vMin += tDest.vPos;

	if (tSrc.tRI.fL > vMax.x)
		return false;

	else if (tSrc.tRI.fR < vMin.x)
		return false;

	else if (tSrc.tRI.fB > vMax.y)
		return false;

	else if (tSrc.tRI.fT < vMin.y)
		return false;

	int iStartX, iStartY, iEndX, iEndY;

	iStartX = (int)(tSrc.tRI.fL < vMin.x ? vMin.x : tSrc.tRI.fL);
	iStartY = (int)(tSrc.tRI.fT < vMax.y ? tSrc.tRI.fT : vMax.y);
	iEndX = (int)(tSrc.tRI.fR > vMax.x ? vMax.x : tSrc.tRI.fR);
	iEndY = (int)(tSrc.tRI.fB < vMin.y ? vMin.y : tSrc.tRI.fB);

	iStartX -= (int)tSrc.tRI.fL;
	iEndX -= (int)tSrc.tRI.fL;
	iStartY -= (int)tSrc.tRI.fB;
	iEndY -= (int)tSrc.tRI.fB;

	for (int i = iStartY - 1; i >= iEndY; --i)
	{
		for (int j = iStartX; j <= iEndX; ++j)
		{
			int idx = (tSrc.iHeight - i - 1) * tSrc.iWidth + j;

			bool bResult = false;

			switch (tSrc.eType)
			{
			case PIXEL_COLLISION_TYPE::COLOR_IGN:
				if (tSrc.pPixel[idx].r == tSrc.tColor.r &&
					tSrc.pPixel[idx].g == tSrc.tColor.g &&
					tSrc.pPixel[idx].b == tSrc.tColor.b)
					continue;

				vCross.x = j + tSrc.tRI.fL;
				vCross.y = i + tSrc.tRI.fB;

				bResult = true;
				break;
			case PIXEL_COLLISION_TYPE::LESS_EQUAL_ALPHA:
				if (tSrc.pPixel[idx].a <= tSrc.tColor.a)
					continue;

				vCross.x = j + tSrc.tRI.fL;
				vCross.y = i + tSrc.tRI.fB;

				bResult =  true;
				break;
			case PIXEL_COLLISION_TYPE::COLOR_CONFIRM:
				if (tSrc.pPixel[idx].r != tSrc.tChkColor.r ||
					tSrc.pPixel[idx].g != tSrc.tChkColor.g ||
					tSrc.pPixel[idx].b != tSrc.tChkColor.b)
					continue;

				vCross.x = j + tSrc.tRI.fL;
				vCross.y = i + tSrc.tRI.fB;

				bResult = true;
				break;
			}

			if (bResult)
			{
				if (CollisionOBB2DToPoint(vCross, tDest, Vector3(j + tSrc.tRI.fL, i + tSrc.tRI.fB, 0.f)))
					return true;					
			}
			
		}
	}

	return false;
}

bool CCollision::CollisionLineToRect(CColliderLine* pSrc, CColliderRect* pDest)
{
	Vector3 vCross;

	if (CollisionLineToRect(vCross, ((CColliderLine*)pSrc)->GetInfo(), ((CColliderRect*)pDest)->GetInfo()))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);

		return true;
	}

	return false;
}

bool CCollision::CollisionLineToRect(Vector3& vCross, const LineInfo& tSrc, const RectInfo& tDest)
{
	OBBInfo tOC = {};

	tOC.vPos.x = (tDest.fR + tDest.fL) / 2.f;
	tOC.vPos.y = (tDest.fT + tDest.fB) / 2.f;
	tOC.fLength[(int)WORLD_AXIS::AXIS_X] = (tDest.fR - tDest.fL) / 2.f;
	tOC.fLength[(int)WORLD_AXIS::AXIS_Y] = (tDest.fT - tDest.fB) / 2.f;

	return CollisionLineToOBB2D(vCross, tSrc, tOC);
}

bool CCollision::CollisionLineToCircle(CColliderLine* pSrc, CColliderCircle* pDest)
{
	Vector3 vCross;

	if (CollisionLineToCircle(vCross, ((CColliderLine*)pSrc)->GetInfo(), ((CColliderCircle*)pDest)->GetInfo()))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);

		return true;
	}

	return false;
}

bool CCollision::CollisionLineToCircle(Vector3& vCross, const LineInfo& tSrc, const CircleInfo& tDest)
{
	Vector2 vCenter = Vector2(tDest.vCenter.x, tDest.vCenter.y);

	Vector2 vStart = tSrc.vStart - vCenter;
	Vector2 vEnd = tSrc.vEnd - vCenter;

	float fDst1 = vEnd.Length();

	if (fDst1 < tDest.fRadius)
	{
		vCross = Vector3(tSrc.vEnd.x, tSrc.vEnd.y, 0.f);
		return true;
	}

	float fDst2 = vStart.Length();

	if (fDst2 < tDest.fRadius)
	{
		vCross = Vector3(tSrc.vStart.x, tSrc.vStart.y, 0.f);
		return true;
	}

	if (vEnd.x == vStart.x)
	{
		if (tDest.fRadius * tDest.fRadius - (tSrc.vStart.x - tDest.vCenter.x) * (tSrc.vStart.x - tDest.vCenter.x) < 0.f)
			return false;
	}

	else
	{
		float fK = (vEnd.y - vStart.y) / (vEnd.x - vStart.x);

		float fX = (fK * vStart.x - vStart.y) / (fK + 1 / fK);

		float fY = -1 / fK * fX;

		float fDst = sqrtf(fX * fX + fY * fY);

		if (fDst > tDest.fRadius)
			return false;
	}

	if (vStart.Dot(vStart - vEnd) < 0.f)
		return false;

	if (vEnd.Dot(vEnd - vStart) < 0.f)
		return false;

	vCross = Vector3((tSrc.vStart.x + tSrc.vEnd.x)/2.f, (tSrc.vStart.y + tSrc.vEnd.y)/2.f, 0.f);
	
	return true;
}

bool CCollision::CollisionLineToOBB2D(CColliderLine* pSrc, CColliderOBB2D* pDest)
{
	Vector3 vCross;

	if (CollisionLineToOBB2D(vCross, ((CColliderLine*)pSrc)->GetInfo(), ((CColliderOBB2D*)pDest)->GetInfo()))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);

		return true;
	}

	return false;
}

bool CCollision::CollisionLineToOBB2D(Vector3& vCross, const LineInfo& tSrc, const OBBInfo& tDest)
{
	Vector3 vS = Vector3(tSrc.vStart.x, tSrc.vStart.y, 0.f);
	Vector3 vE = Vector3(tSrc.vEnd.x, tSrc.vEnd.y, 0.f);

	float fS = tDest.vAxis[(int)WORLD_AXIS::AXIS_X].Dot(vS);
	float fE = tDest.vAxis[(int)WORLD_AXIS::AXIS_X].Dot(vE);

	if (fS > fE)
	{
		float f = fS;
		fS = fE;
		fE = f;
	}

	float fF = tDest.vAxis[(int)WORLD_AXIS::AXIS_X].Dot(tDest.vPos) + tDest.fLength[(int)WORLD_AXIS::AXIS_X];
	float fB = tDest.vAxis[(int)WORLD_AXIS::AXIS_X].Dot(tDest.vPos) - tDest.fLength[(int)WORLD_AXIS::AXIS_X];

	if (fB > fE)
		return false;

	if (fF < fS)
		return false;

	fS = tDest.vAxis[(int)WORLD_AXIS::AXIS_Y].Dot(vS);
	fE = tDest.vAxis[(int)WORLD_AXIS::AXIS_Y].Dot(vE);

	if (fS > fE)
	{
		float f = fS;
		fS = fE;
		fE = f;
	}

	fF = tDest.vAxis[(int)WORLD_AXIS::AXIS_Y].Dot(tDest.vPos) + tDest.fLength[(int)WORLD_AXIS::AXIS_Y];
	fB = tDest.vAxis[(int)WORLD_AXIS::AXIS_Y].Dot(tDest.vPos) - tDest.fLength[(int)WORLD_AXIS::AXIS_Y];

	if (fB > fE)
		return false;

	if (fF < fS)
		return false;

	Vector3 vN = (vS - vE).Cross(Vector3::Axis[(int)WORLD_AXIS::AXIS_Z]);

	vN.Normalize();
	
	float fDot_ = (tDest.vPos - vS).Dot(vN);

	float fDot = abs(fDot_);

	float fDot2 = abs(vN.Dot(tDest.vAxis[(int)WORLD_AXIS::AXIS_X] * tDest.fLength[(int)WORLD_AXIS::AXIS_X])) 
		+ abs(vN.Dot(tDest.vAxis[(int)WORLD_AXIS::AXIS_Y] * tDest.fLength[(int)WORLD_AXIS::AXIS_Y]));

	if (fDot > fDot2)
		return false;

	if (CollisionOBB2DToPoint(vCross, tDest, vS))
		return true;

	else if (CollisionOBB2DToPoint(vCross, tDest, vE))
		return true;

	else
	{
		vCross = tDest.vPos - vN * fDot_;
	}

	return true;
}

bool CCollision::CollisionLineToPixel(CColliderLine* pSrc, CColliderPixel* pDest)
{
	Vector3 vCross;

	if (CollisionLineToPixel(vCross, ((CColliderLine*)pSrc)->GetInfo(), ((CColliderPixel*)pDest)->GetInfo()))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);

		return true;
	}

	return false;
}

bool CCollision::CollisionLineToPixel(Vector3& vCross, const LineInfo& tSrc, const PixelInfo& tDest)
{
	RectInfo tRect = {};

	if (tSrc.vStart.x < tSrc.vEnd.x)
	{
		tRect.fL = tSrc.vStart.x;
		tRect.fR = tSrc.vEnd.x;
	}

	else
	{
		tRect.fR = tSrc.vStart.x;
		tRect.fL = tSrc.vEnd.x;
	}

	if (tSrc.vStart.y < tSrc.vEnd.y)
	{
		tRect.fB = tSrc.vStart.y;
		tRect.fT = tSrc.vEnd.y;
	}

	else
	{
		tRect.fT = tSrc.vStart.y;
		tRect.fB = tSrc.vEnd.y;
	}

	if (tRect.fL > tDest.tRI.fR)
		return false;

	else if (tRect.fR < tDest.tRI.fL)
		return false;

	else if (tRect.fT < tDest.tRI.fB)
		return false;

	else if (tRect.fB > tDest.tRI.fT)
		return false;

	int iStartX, iStartY, iEndX, iEndY;

	iStartX = (int)(tRect.fL < tDest.tRI.fL ? tDest.tRI.fL : tRect.fL);
	iStartY = (int)(tRect.fT > tDest.tRI.fT ? tDest.tRI.fT : tRect.fT);
	iEndX = (int)(tRect.fR < tDest.tRI.fR ? tRect.fR : tDest.tRI.fR);
	iEndY = (int)(tRect.fB > tDest.tRI.fB ? tRect.fB : tDest.tRI.fB);

	iStartX -= (int)(tDest.tRI.fL);
	iStartY -= (int)(tDest.tRI.fB);
	iEndX -= (int)(tDest.tRI.fL);
	iEndY -= (int)(tDest.tRI.fB);

	for (int i = iStartY - 1; i >= iEndY; --i)
	{
		for (int j = iStartX; j <= iEndX; ++j)
		{
			int idx = (tDest.iHeight - i - 1) * tDest.iWidth + j;

			bool bResult = false;

			switch (tDest.eType)
			{
			case PIXEL_COLLISION_TYPE::COLOR_IGN:
				if (tDest.pPixel[idx].r == tDest.tColor.r &&
					tDest.pPixel[idx].g == tDest.tColor.g &&
					tDest.pPixel[idx].b == tDest.tColor.b)
					continue;

				vCross.x = j + tDest.tRI.fL;
				vCross.y = i + tDest.tRI.fB;

				bResult = true;
				break;
			case PIXEL_COLLISION_TYPE::LESS_EQUAL_ALPHA:
				if (tDest.pPixel[idx].a <= tDest.tColor.a)
					continue;

				vCross.x = j + tDest.tRI.fL;
				vCross.y = i + tDest.tRI.fB;

				bResult = true;
				break;
			case PIXEL_COLLISION_TYPE::COLOR_CONFIRM:
				if (tDest.pPixel[idx].r == tDest.tChkColor.r &&
					tDest.pPixel[idx].g == tDest.tChkColor.g &&
					tDest.pPixel[idx].b == tDest.tChkColor.b)
					continue;

				vCross.x = j + tDest.tRI.fL;
				vCross.y = i + tDest.tRI.fB;

				bResult = true;
				break;
			}

			if (bResult)
			{
				Vector3 v;

				if (CollisionLineToPoint(v, tSrc, vCross))
					return true;
			}
		}
	}

	return false;
}

bool CCollision::CollisionLineToLine(CColliderLine* pSrc, CColliderLine* pDest)
{
	Vector3 vCross;

	if (CollisionLineToLine(vCross, ((CColliderLine*)pSrc)->GetInfo(), ((CColliderLine*)pDest)->GetInfo()))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);

		return true;
	}

	return false;
}

bool CCollision::CollisionLineToLine(Vector3& vCross, const LineInfo& tSrc, const LineInfo& tDest)
{
	Vector3 vA = Vector3(tSrc.vStart.x, tSrc.vStart.y, 0.f);
	Vector3 vE = Vector3(tSrc.vEnd.x, tSrc.vEnd.y, 0.f);
	Vector3 vB = vE - vA;

	Vector3 vC = Vector3(tDest.vStart.x, tDest.vStart.y, 0.f);
	Vector3 vF = Vector3(tDest.vEnd.x, tDest.vEnd.y, 0.f);
	Vector3 vD = vF - vC;

	Vector3 vN1 = Vector3::Axis[(int)WORLD_AXIS::AXIS_Z].Cross(vB);
	Vector3 vN2 = Vector3::Axis[(int)WORLD_AXIS::AXIS_Z].Cross(vD);

	Vector3 vG = vC - vA;
	Vector3 vH = vF - vA;
	Vector3 vI = vE - vC;

	float E = 0.f;

	if (vN1.Dot(vG) * vN1.Dot(vH) <= E &&
		vN2.Dot(-vG) * vN2.Dot(vI) <= E)
	{
		float Z = vB.Cross(vD).z;

		if (Z == 0.f)
		{
			if (vA.x != vE.x)
			{
				float X1, X2;
				float Y1, Y2;

				if (vA.x > vE.x)
				{
					X1 = vA.x;
					X2 = vE.x;
					Y1 = vA.y;
					Y2 = vE.y;
				}					

				else
				{
					X1 = vE.x;
					X2 = vA.x;
					Y1 = vE.y;
					Y2 = vA.y;
				}

				float X3, X4;
				float Y3, Y4;

				if (vC.x > vF.x)
				{
					X3 = vC.x;
					X4 = vF.x;
					Y3 = vC.y;
					Y4 = vF.y;
				}

				else
				{
					X3 = vF.x;
					X4 = vC.x;
					Y3 = vF.y;
					Y4 = vC.y;
				}

				if (X2 > X3 ||
					X4 > X1)
					return false;

				else
				{
					if (X1 > X3)
					{
						if (X2 > X4)
						{
							vCross.x = (X2 + X3) / 2.f;
							vCross.y = (Y2 + Y3) / 2.f;
						}

						else
						{
							vCross.x = (X4 + X3) / 2.f;
							vCross.y = (Y4 + Y3) / 2.f;
						}
					}

					else
					{
						if (X2 > X4)
						{
							vCross.x = (X2 + X1) / 2.f;
							vCross.y = (Y2 + Y1) / 2.f;
						}

						else
						{
							vCross.x = (X4 + X1) / 2.f;
							vCross.y = (Y4 + Y1) / 2.f;
						}
					}

					return true;
				}
			}

			else
			{
				float X1, X2;
				float Y1, Y2;

				if (vA.y > vE.y)
				{
					X1 = vA.x;
					X2 = vE.x;
					Y1 = vA.y;
					Y2 = vE.y;
				}

				else
				{
					X1 = vE.x;
					X2 = vA.x;
					Y1 = vE.y;
					Y2 = vA.y;
				}

				float X3, X4;
				float Y3, Y4;

				if (vC.y > vF.y)
				{
					X3 = vC.x;
					X4 = vF.x;
					Y3 = vC.y;
					Y4 = vF.y;
				}

				else
				{
					X3 = vF.x;
					X4 = vC.x;
					Y3 = vF.y;
					Y4 = vC.y;
				}

				if (Y2 > Y3 ||
					Y4 > Y1)
					return false;

				else
				{
					if (Y1 > Y3)
					{
						if (Y2 > Y4)
						{
							vCross.x = (X2 + X3) / 2.f;
							vCross.y = (Y2 + Y3) / 2.f;
						}

						else
						{
							vCross.x = (X4 + X3) / 2.f;
							vCross.y = (Y4 + Y3) / 2.f;
						}
					}

					else
					{
						if (Y2 > Y4)
						{
							vCross.x = (X2 + X1) / 2.f;
							vCross.y = (Y2 + Y1) / 2.f;
						}

						else
						{
							vCross.x = (X4 + X1) / 2.f;
							vCross.y = (Y4 + Y1) / 2.f;
						}
					}

					return true;
				}
			}
		}

		else
		{
			float fP = (vD.Cross(vA - vC)).z / Z;

			vCross = vA + vB * fP;

			return true;
		}
	}

	return false;
}

bool CCollision::CollisionLineToPoint(CColliderLine* pSrc, CColliderPoint* pDest)
{
	Vector3 vCross;

	Vector3 vPos = {};

	if (pSrc->IsUI())
		vPos = pDest->GetUIInfo();

	else
		vPos = pDest->GetInfo();

	if (CollisionLineToPoint(vCross, ((CColliderLine*)pSrc)->GetInfo(), vPos))
	{
		pSrc->SetCross(vCross);
		pDest->SetCross(vCross);

		return true;
	}

	return false;
}

bool CCollision::CollisionLineToPoint(Vector3& vCross, const LineInfo& tSrc, const Vector3& tDest)
{
	Vector3 vLine = Vector3(tSrc.vEnd.x - tSrc.vStart.x, tSrc.vEnd.y - tSrc.vStart.y, 0.f);
	Vector3 vNrm = vLine.Cross(Vector3::Axis[(int)WORLD_AXIS::AXIS_Z]);

	Vector3 vStart = Vector3(tDest.x, tDest.y, 0.f) - Vector3(tSrc.vStart.x, tSrc.vStart.y, 0.f);

	if (vNrm.Dot(vStart) > 0.f)
	{
		vNrm.Normalize();

		float fDist = vStart.Dot(vNrm);

		vCross = tDest - vNrm * fDist;

		return true;
	}

	return false;
}

bool CCollision::CollisionPointToRect(CColliderPoint* pSrc, CColliderRect* pDst)
{
	Vector3 vCrs;

	Vector3 vPos = {};

	if (pDst->IsUI())
		vPos = pSrc->GetUIInfo();

	else
		vPos = pSrc->GetInfo();

	if (CollisionPointToRect(vCrs, vPos, pDst->GetInfo()))
	{
		pSrc->SetCross(vCrs);
		pDst->SetCross(vCrs);

		return true;
	}

	return false;
}

bool CCollision::CollisionPointToRect(Vector3& vCrs, const Vector3& vSrc, const RectInfo& tDst)
{
	if (tDst.fL > vSrc.x)
		return false;

	if (tDst.fR < vSrc.x)
		return false;

	if (tDst.fB > vSrc.y)
		return false;

	if (tDst.fT < vSrc.y)
		return false;

	vCrs = vSrc;

	return true;
}

bool CCollision::CollisionPointToPixel(CColliderPoint* pSrc, CColliderPixel* pDst)
{
	Vector3 vCrs;

	Vector3 vPos = {};

	if (pDst->IsUI())
		vPos = pSrc->GetUIInfo();

	else
		vPos = pSrc->GetInfo();

	if (CollisionPointToPixel(vCrs, vPos, pDst->GetInfo()))
	{
		pSrc->SetCross(vCrs);
		pDst->SetCross(vCrs);

		return true;
	}

	return false;
}

bool CCollision::CollisionPointToPixel(Vector3& vCrs, const Vector3& vSrc, const PixelInfo& tDst)
{
	Vector3 vPt = vSrc;

	vPt.x -= tDst.tRI.fL;
	vPt.y -= tDst.tRI.fB;

	int x = (int)vPt.x;
	int y = (int)vPt.y;

	y = tDst.iHeight - y;

	if (x < 0.f || x >= tDst.iWidth)
		return false;

	if (y < 0.f || y >= tDst.iHeight)
		return false;

	int idx = y * tDst.iWidth + x;

	switch (tDst.eType)
	{
	case PIXEL_COLLISION_TYPE::COLOR_IGN:
		if (tDst.pPixel[idx].r == tDst.tColor.r &&
			tDst.pPixel[idx].g == tDst.tColor.g &&
			tDst.pPixel[idx].b == tDst.tColor.b)
			return false;
		break;
	case PIXEL_COLLISION_TYPE::LESS_EQUAL_ALPHA:
		if (tDst.pPixel[idx].a <= tDst.tColor.a)
			return false;
		break;
	case PIXEL_COLLISION_TYPE::COLOR_CONFIRM:
		if (tDst.pPixel[idx].r != tDst.tChkColor.r ||
			tDst.pPixel[idx].g != tDst.tChkColor.g ||
			tDst.pPixel[idx].b != tDst.tChkColor.b)
			return false;
		break;
	}

	vCrs = vSrc;

	return true;
}

bool CCollision::CollisionPointToPoint(CColliderPoint* pSrc, CColliderPoint* pDst)
{
	Vector3 vCrs = {};

	Vector3 vDst = {};
	Vector3 vSrc = {};

	if (pSrc->IsUI())
		vDst = pDst->GetUIInfo();

	if (pDst->IsUI())
		vSrc = pSrc->GetUIInfo();

	if (CollisionPointToPoint(vCrs, vSrc, vDst))
	{
		pSrc->SetCross(vCrs);
		pDst->SetCross(vCrs);

		return true;
	}

	return false;
}

bool CCollision::CollisionPointToPoint(Vector3& vCrs, const Vector3& vSrc, const Vector3& tDst)
{
	vCrs = vSrc;

	return vSrc == tDst;
}
