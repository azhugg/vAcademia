#include <winreg.h>
#pragma once

class CProtocol
{
public:
	CProtocol();
	~CProtocol();

	// ������ URL
	bool	parse( LPCTSTR alpUrl);

};
