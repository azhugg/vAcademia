#pragma once

class CProcessWindowFinder
{
public:
	CProcessWindowFinder();
	~CProcessWindowFinder();

	// ������ URL
	HWND	get_process_hwnd( const CComString& asExePath);
};
