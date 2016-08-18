#include "stdafx.h"
#include "LoadUpdater.h"
#include <wininet.h> 
#include "shlobj.h"
#include <time.h>
#include "ProcessFinder.h"

#include "..\nengine\ncommon\IniFile.h"
#include "ClientSessionStateSender.h"
#include "ProxyFinder.h"
#include <stdio.h>

//��������� �����, ��� �������� ��������� ������, ������ ������������ � ����� ����������
static char errorBuffer[CURL_ERROR_SIZE];
//��������� ����� ����������� ������
static string buffer;

ThreadInfoForVersion::ThreadInfoForVersion(std::string _serverIP, std::string _version, std::string _buildType)
{
	serverIP = _serverIP;
	curVersion = _version;
	buildType = _buildType;
}


DWORD WINAPI VersionThreadProc(LPVOID lpParam)
{
	ThreadInfoForVersion* info = (ThreadInfoForVersion*)lpParam;

	// ����������, ����� ��������� ����������� ������ 
	// (����������� ���������� user_agent � ��������� HTTP), ������ ��� ��� �������
	// ���������� ��� ���������� ����������.

	CURL *curl; 
	CURLcode result;  
	curl = curl_easy_init(); 
	std::string buffer;

	if(curl) 
	{  
		std::string url = "http://" + info->serverIP +"/vu/php_0_1/getVersion.php?version=" + info->curVersion + "&buildtype=" + info->buildType;
		//������ ��� ����������� �����
		//����������, ���� �������� ������
		curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);
		//������ ����� - �������� �������� �� ������ http://...
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		//��������� ������ ������, ���� ����
		/*if ((info->proxySetting!="unknown") && (info->proxySetting!=""))
			curl_easy_setopt(curl, CURLOPT_PROXY, "192.168.0.62:80");*/
		//������ ����� ����������� ��������� ��������
		//curl_easy_setopt(curl, CURLOPT_HEADER, 1);
		//��������� ������� ��������� ������ ��� ������ ���������� ������
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
		//��������� ���� ���������� ����������� ������
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
		//��������� ���������� ������
		result = curl_easy_perform(curl);
		DWORD statusCode;
		CURLcode result1 = curl_easy_getinfo(curl,CURLINFO_RESPONSE_CODE,&statusCode);
		//��������� ���������� ���������� ��������
		if(result != CURLE_OK || result1 == CURLE_OK && CClientSessionStateSender::IsExistProxy() && (statusCode < 200 || statusCode > 299))
		{
			result = CClientSessionStateSender::CheckCurlProxys( curl, result);
		}
		if (result == CURLE_OK)     
		{
			bool hwndFound = false;
			WCHAR szPlayerDir[MAX_PATH] = { 0 };
			GetModuleFileNameW(NULL, szPlayerDir, MAX_PATH);
			PathRemoveFileSpecW(szPlayerDir);

			if (buffer.size() < 100)
			while (!hwndFound)
			{
				int i = 0;
				CProcessWindowFinder finder;		
				std::vector<SFindedProcess> processes = finder.GetPlayerWindow( "player.exe");
				for (int m = 0; m < processes.size(); m++)
				{
					HWND hWnd = processes[m].window;
					unsigned int parentProcessID = processes[m].parentProcessID;
					WINDOWPLACEMENT wndpl; wndpl.length = sizeof(WINDOWPLACEMENT);

					if ( IsWindowVisible(hWnd) && !IsHungAppWindow(hWnd) && GetWindowPlacement(hWnd, &wndpl) && wndpl.showCmd != SW_HIDE)
					{
						hwndFound = true;
						COPYDATASTRUCT ds;
						ds.dwData = 2;
						ds.cbData = (DWORD)buffer.length() + 1;
						ds.lpData = (char*)buffer.c_str();
						SendMessage( hWnd, WM_COPYDATA, 6, (LPARAM)(LPVOID) &ds);
						return 0;
					}
					else
					{
						if (i > 120)
						{
							return 0;
						}
						Sleep(1000);
						i++;
					}
				}
			}
		}  
	}


	//��������� ������
	curl_easy_cleanup(curl);

	return 0;
}

std::string CLoadUpdater::GetPropertyFromPlayerINI(std::string section, std::string key)
{
	// get folder of the current process
	WCHAR szPlayerDir[MAX_PATH] = { 0 };
	GetModuleFileNameW(NULL, szPlayerDir, MAX_PATH);
	PathRemoveFileSpecW(szPlayerDir);

	std::wstring fileName = szPlayerDir;
	fileName += L"\\player.ini";

	CIniFile iniFile(fileName);
	return iniFile.GetString( section.data(), key.data());
}

void CLoadUpdater::SendVersion()
{	
	ThreadInfoForVersion* infoWorldServer = NULL;
	CClientSessionStateSender::LoadProxyInfo();
	infoWorldServer = new ThreadInfoForVersion( GetPropertyFromPlayerINI( "paths", "server"), GetPropertyFromPlayerINI( "settings", "version"), GetPropertyFromPlayerINI( "settings", "buildtype"));
	m_threadWorldServer.SetStackSize(256 * 1024);
	m_threadWorldServer.SetParameter((void*)infoWorldServer);
	m_threadWorldServer.SetThreadProcedure(VersionThreadProc);
	m_threadWorldServer.Start();
}

void CLoadUpdater::SetErrorCode(int errorCode)
{
	m_errorCode = errorCode;
}
   