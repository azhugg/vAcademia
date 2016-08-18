#pragma once

#include "../CommonRenderManagerHeader.h"

class CBSpline;

typedef MP_VECTOR<CVector3D> splinePointsVector;

typedef struct _BoundaryPoints
{
	splinePointsVector boundarySplinePoints;
	bool bSmallRegion;
	CVector3D minPoint;

	_BoundaryPoints();
} BoundaryPoints;

class CSplinePoints
{
public:
	CSplinePoints();
	virtual ~CSplinePoints();
	
	enum
	{
		LEFT_UP,		// ����-������� �������
		RIGHT_UP,		// �����-������� �������
		LEFT_DOWN,		// ����-������ �������
		RIGHT_DOWN,		// �����-������ �������
		CENTER,			// �����
		SMALL,			// ������� ������� ������ ������� ���������� ����� � �������
	};

public:	
	typedef MP_VECTOR<CVector3D>::iterator m_splinePointsIter;
	splinePointsVector m_splinePoints;

private:
	CVector3D minX;
	CVector3D minY;
	CVector3D maxX;
	CVector3D maxY;
	CVector3D center;
	float width;
	float height;
	std::vector<CVector3D>::iterator minXIter;
	std::vector<CVector3D>::iterator minYIter;
	std::vector<CVector3D>::iterator maxXIter;
	std::vector<CVector3D>::iterator maxYIter;

public:
	BoundaryPoints m_boundaryPoints;

	void initialize( CBSpline *pSpline, float aExact = 0.01f);
	void add( CVector3D point);	
	BoundaryPoints *getBoundaryPoints( CVector3D avPos, float radius);
};