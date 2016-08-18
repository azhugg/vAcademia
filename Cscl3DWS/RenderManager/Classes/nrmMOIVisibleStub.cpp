#include "stdafx.h"
#include "nrmMOIVisibleStub.h"

nrmMOIVisibleStub::nrmMOIVisibleStub() 
{
	m_pMaskImage = NULL;
}

nrmMOIVisibleStub::~nrmMOIVisibleStub() 
{
}

// ���������� moIVisible
// ��������
// ���������� ���������� ��������� 
void	nrmMOIVisibleStub::AbsXYChanged() 
{
	ATLASSERT( false);
}

// ���������� �������
void	nrmMOIVisibleStub::SizeChanged()
{
	ATLASSERT( false);
}

// ���������� ���������� �������
void	nrmMOIVisibleStub::AbsDepthChanged()
{
	ATLASSERT( false);
} 

// ���������� ���������� ���������
void	nrmMOIVisibleStub::AbsVisibleChanged()
{
	ATLASSERT( false);
} 

// ���������� ���������� ������������
void	nrmMOIVisibleStub::AbsOpacityChanged()
{
	ATLASSERT( false);
} 

// ������
mlSize	nrmMOIVisibleStub::GetMediaSize()
{
	ATLASSERT( false);
	mlSize size;
	ML_INIT_SIZE(size);
	return size;
}

void	nrmMOIVisibleStub::Crop(short awX, short awY, MLWORD awWidth, MLWORD awHeight)
{
	ATLASSERT( false);
}

mlRect	nrmMOIVisibleStub::GetCropRect()
{
	ATLASSERT( false);
	mlRect rect;
	ML_INIT_RECT(rect);
	return rect;
}

void	nrmMOIVisibleStub::SetMask(mlMedia* aMask, int aiX, int aiY)
{
	m_pMaskImage = aMask;

	nrmImage* pMask = NULL;

	if (m_pMaskImage && m_pMaskImage->GetType() == MLMT_IMAGE)
		pMask = (nrmImage *)m_pMaskImage->GetSafeMO();

	UpdateMask( pMask);
}

void	nrmMOIVisibleStub::SetMask(int aiX, int aiY, int aiW, int aiH)
{
	ATLASSERT( false);
}

mlMedia*	nrmMOIVisibleStub::GetMask()
{
	return m_pMaskImage;
}

// -1 - not handled, 0 - point is out, 1 - point is in
int		nrmMOIVisibleStub::IsPointIn(const mlPoint aPnt)
{
	ATLASSERT( false);
	return 0;
} 

void nrmMOIVisibleStub::UpdateMask( nrmImage* apMask)
{

}