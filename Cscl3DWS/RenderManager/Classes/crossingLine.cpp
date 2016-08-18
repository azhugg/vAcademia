
#include "StdAfx.h"
#include "CrossingLine.h"

//#include "GlobalSingletonStorage.h"
//#include "RMContext.h"

CCrossingLine::CCrossingLine()
{
}

CrossResultRec *CCrossingLine::GetResult( const CPoint &p11, const CPoint &p12, const CPoint &p21, const CPoint &p22)
{
	// �����������
	double Z  = (p12.y-p11.y)*(p21.x-p22.x)-(p21.y-p22.y)*(p12.x-p11.x);
	// ��������� 1
	double Ca = (p12.y-p11.y)*(p21.x-p11.x)-(p21.y-p11.y)*(p12.x-p11.x);
	// ��������� 2
	double Cb = (p21.y-p11.y)*(p21.x-p22.x)-(p21.y-p22.y)*(p21.x-p11.x);

	// ���� ��������� � ����������� = 0, ������ ���������
	if( (Z == 0)&&(Ca == 0)&&(Cb == 0) )
	{
		result.type = ctSameLine;
		return &result;
	}

	// ���� ����������� = 0, ������ �����������
	if( Z == 0 )
	{
		result.type = ctParallel;
		return &result;
	}

	double Ua = Ca/Z;
	double Ub = Cb/Z;

	result.pt.x = p11.x + (p12.x - p11.x) * Ub;
	result.pt.y = p11.y + (p12.y - p11.y) * Ub;

	// ���� 0<=Ua<=1 � 0<=Ub<=1, ����� ����������� � �������� ��������
	if( (0 <= Ua)&&(Ua <= 1)&&(0 <= Ub)&&(Ub <= 1) )
	{
		( (Ua == 0)||(Ua == 1)||(Ub == 0)||(Ub == 1) ) ? result.type = ctOnBounds : result.type = ctInBounds;
	}
	// ����� ����� ����������� �� ��������� ��������
	else
	{
		result.type = ctOutBounds;
	}
	return &result;
}

CCrossingLine::~CCrossingLine()
{
}