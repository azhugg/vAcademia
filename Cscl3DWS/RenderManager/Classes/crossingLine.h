
#if !defined(AFX_CrossingLine__INCLUDED_)
#define AFX_CrossingLine__INCLUDED_

#include "Sprite.h"
#include "nrmObject.h"
#include "nrmMOIVisibleStub.h"
#include "NRManager.h"
#include "IChangesListener.h"

//**************************************************************
//  CPoint
//  ������������� ����� �� ���������
//**************************************************************
class CPoint
{
public:
	CPoint(){};
	CPoint( double ax, double ay)
	{
		x = ax;
		y = ay;
	}
	double x;
	double y;
};

//**************************************************************
//  CrossType
//  ���� ����������� ������
//**************************************************************
enum enumCrossType
{
	ctParallel,    // ������� ����� �� ������������ ������
	ctSameLine,    // ������� ����� �� ����� ������
	ctOnBounds,    // ������ ������������ � �������� ������ ��������
	ctInBounds,    // ������ ������������ �   �������� ��������
	ctOutBounds    // ������ ������������ ��� �������� ��������
};

//**************************************************************
//  CrossResultRec
//  ��������� �������� ����������� ������
//**************************************************************
struct CrossResultRec
{
	enumCrossType  type;  // ��� �����������
	CPoint      pt;    // ����� �����������
};


// ������� ����������� ���� ��������, ������ ���������� ����� ���������
class CCrossingLine 
{
public:
	CCrossingLine();
	virtual ~CCrossingLine();

	CrossResultRec *GetResult( const CPoint &p11, const CPoint &p12, const CPoint &p21, const CPoint &p22);
// ���������� moMedia
private:
	CrossResultRec result;
};

#endif // !defined(AFX_CrossingLine__INCLUDED_)
