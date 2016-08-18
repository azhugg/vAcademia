#pragma once

class DESKTOPMAN_API CCursorCapture
{
public:
	CCursorCapture(void);
	~CCursorCapture(void);

	void	DrawCursorIfNeeded(HDC hdcTo, RECT capturingRect);
	HCURSOR GetCurrentCursorHandle(POINT &pt);

public:
	//���������� ����� ����������� ������� (0-�����, 1-������������, 2-������������ �����������)
	void SetCursorState(int state);

private: 
	bool m_canShow;
	bool m_needScale;
};
