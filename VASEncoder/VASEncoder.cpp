// VASEncoder.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
#ifdef DESKTOP_DETECT_MEMORY_LEAKS
	_CrtSetDbgFlag(
		_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) |
		/*_CRTDBG_CHECK_ALWAYS_DF |*/
		_CRTDBG_DELAY_FREE_MEM_DF |
		_CRTDBG_LEAK_CHECK_DF);
#endif
    return TRUE;
}
