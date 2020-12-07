#pragma once

#include "../GameEngine.h"

class CCollision
{
public:
	static bool CollisionRectToRect(class CColliderRect* pSrc, 
		class CColliderRect* pDest);
	static bool CollisionRectToRect(Vector3& vCross, 
		const RectInfo& tSrc, const RectInfo& tDest);

public:
	static bool CollisionCircleToRect(class CColliderCircle* pSrc, 
		class CColliderRect* pDest);
	static bool CollisionCircleToRect(Vector3& vCross, 
		const CircleInfo& tSrc, const RectInfo& tDest);
	static bool CollisionCircleToCircle(class CColliderCircle* pSrc,
		class CColliderCircle* pDest);
	static bool CollisionCircleToCircle(Vector3& vCross,
		const CircleInfo& tSrc, const CircleInfo& tDest);
	static bool CollisionCircleToPoint(class CColliderCircle* pSrc,
		class CColliderPoint* pDest);
	static bool CollisionCircleToPoint(Vector3& vCross,
		const CircleInfo& tSrc, const Vector3& tDest);
	
public:
	static bool CollisionOBB2DToOBB2D(class CColliderOBB2D* pSrc,
		class CColliderOBB2D* pDest);
	static bool CollisionOBB2DToOBB2D(Vector3& vCross,
		const OBBInfo& tSrc, const OBBInfo& tDest);
	static bool CollisionOBB2DToRect(class CColliderOBB2D* pSrc,
		class CColliderRect* pDest);
	static bool CollisionOBB2DToCircle(class CColliderOBB2D* pSrc,
		class CColliderCircle* pDest);
	static bool CollisionOBB2DToCircle(Vector3& vCross,
		const OBBInfo& tSrc, const CircleInfo& tDest);
	static bool CollisionOBB2DToPoint(class CColliderOBB2D* pSrc,
		class CColliderPoint* pDest);
	static bool CollisionOBB2DToPoint(Vector3& vCross,
		const OBBInfo& tSrc, const Vector3& tDest);

public:
	static bool CollisionPixelToRect(class CColliderPixel* pSrc,
		class CColliderRect* pDest);
	static bool CollisionPixelToRect(Vector3& vCross,
		const PixelInfo& tSrc, const RectInfo& tDest);
	static bool CollisionPixelToCircle(class CColliderPixel* pSrc,
		class CColliderCircle* pDest);
	static bool CollisionPixelToCircle(Vector3& vCross,
		const PixelInfo& tSrc, const CircleInfo& tDest);
	static bool CollisionPixelToOBB2D(class CColliderPixel* pSrc,
		class CColliderOBB2D* pDest);
	static bool CollisionPixelToOBB2D(Vector3& vCross,
		const PixelInfo& tSrc, const OBBInfo& tDest);

public:
	static bool CollisionLineToRect(class CColliderLine* pSrc,
		class CColliderRect* pDest);
	static bool CollisionLineToRect(Vector3& vCross,
		const LineInfo& tSrc, const RectInfo& tDest);
	static bool CollisionLineToCircle(class CColliderLine* pSrc,
		class CColliderCircle* pDest);
	static bool CollisionLineToCircle(Vector3& vCross,
		const LineInfo& tSrc, const CircleInfo& tDest);
	static bool CollisionLineToOBB2D(class CColliderLine* pSrc,
		class CColliderOBB2D* pDest);
	static bool CollisionLineToOBB2D(Vector3& vCross,
		const LineInfo& tSrc, const OBBInfo& tDest);
	static bool CollisionLineToPixel(class CColliderLine* pSrc,
		class CColliderPixel* pDest);
	static bool CollisionLineToPixel(Vector3& vCross,
		const LineInfo& tSrc, const PixelInfo& tDest);
	static bool CollisionLineToLine(class CColliderLine* pSrc,
		class CColliderLine* pDest);
	static bool CollisionLineToLine(Vector3& vCross,
		const LineInfo& tSrc, const LineInfo& tDest);
	static bool CollisionLineToPoint(class CColliderLine* pSrc,
		class CColliderPoint* pDest);
	static bool CollisionLineToPoint(Vector3& vCross,
		const LineInfo& tSrc, const Vector3& tDest);

public:
	static bool CollisionPointToRect(class CColliderPoint* pSrc,
		class CColliderRect* pDst);
	static bool CollisionPointToRect(Vector3& vCrs,
		const Vector3& vSrc, const RectInfo& tDst);
	static bool CollisionPointToPixel(class CColliderPoint* pSrc,
		class CColliderPixel* pDst);
	static bool CollisionPointToPixel(Vector3& vCrs,
		const Vector3& vSrc, const PixelInfo& tDst);
	static bool CollisionPointToPoint(class CColliderPoint* pSrc,
		class CColliderPoint* pDst);
	static bool CollisionPointToPoint(Vector3& vCrs,
		const Vector3& vSrc, const Vector3& tDst);
};

