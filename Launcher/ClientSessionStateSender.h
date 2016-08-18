#pragma once

#include "..\nengine\ncommon\Thread.h"
#include <map>
#include "Tlhelp32.h"
#include <string>

typedef unsigned int socklen_t;
#include "../curl/include/curl/curl.h"


#define INCORRECT_EXIT_STATUS			11 // ������������ �������� ��������� � ���������� �� (����� Windows)
#define PLAYER_NOT_RUNNING_STATUS		13 // �������� ���������� �� ������ ��������� player
#define LOGIN_WND_NOT_CREATED_STATUS	14 // ���� ��������� �� ���������
#define LOGIN_PROCESS_FAILED_STATUS		15 // ��������� �� ������
#define NOT_NORMAL_CLOSED_STATUS		16 // ��������� ����� ��������
#define MAKE_DUMP_STATUS				17 // ������� ����
#define PLAYER_NOT_RUNNING_LONG_TIME	18 // Player ����������� ������ 3-� ����� �� ��������� ���� ������
#define APP_HANG_SYSTEM					19 // ��������� ��� ��� ����� �������� ��������� �������� (������ ��������, ����� ����� � �.�)
#define UPDATER_NOT_RUNNING_STATUS		44 // �������� �� ����������
#define UPDATER_NOT_RUNNING_PROCESS_STATUS	45 // �������� �� ����������


class CClientSessionStateSender;

//������� ��������� ������
static int writer(char *data, size_t size, size_t nmemb, std::string *buffer)
{
	//���������� - ���������, �� ��������� �������
	int result = 0;
	//��������� �����
	if (buffer != NULL)
	{
		//��������� � ������ ������ �� data, � ���������� nmemb
		buffer->append(data, size * nmemb);
		//��������� ����� �������� ������
		result = size * nmemb;
	}
	//���������� ���������
	return result;
}


struct ThreadInfo
{
	int sessionID;
	unsigned char status;
	std::string serverIP;
	std::string login;
	std::wstring appPath;
	std::wstring fileName;
	std::string allProcess;
	unsigned int socketError;
	unsigned int socketStatus;
	std::string proxySetting;
	std::string computerName;
	std::wstring logFileName;
	std::string comment;
	std::string version;

	CClientSessionStateSender* sender;

	ThreadInfo( int _sessionID, unsigned char _status, std::string _serverIP, std::string _login, std::wstring _appPath, std::wstring _fileName, std::string _allProcess, unsigned int _socketError, unsigned int _socketStatus, std::string _proxySetting,	std::string _computerName, std::wstring _logFileName, CClientSessionStateSender* _sender, std::wstring _comment, std::string _version);	
};

struct UploadThreadInfo
{
	std::wstring logFileName;
	std::string serverIP;
	std::wstring appPath;
	CClientSessionStateSender* sender;

	UploadThreadInfo(std::wstring _logFileName, std::string _serverIP, std::wstring _appPath, CClientSessionStateSender* _sender)
	{
		logFileName = _logFileName;
		serverIP = _serverIP;
		appPath =_appPath;
		sender = _sender;
	}
};

class CClientSessionStateSender
{
public:
	CClientSessionStateSender();
	~CClientSessionStateSender();

	void SendStatus(int auSessionID, unsigned char aStatus, std::string login, std::wstring logPath, unsigned int socketError,unsigned int socketStatus, std::string proxySetting,  std::string allProcess, std::wstring comment, std::string version, bool isLoginSended = false);
	void SetPropertyFromPlayerINI(std::string section, std::string key, std::string value);

	static std::string GetPropertyFromPlayerINI(std::string section, std::string key);

	void SetFileName(std::wstring _fileName);
	void CreateUploadThread(std::wstring logPath);

	void SetLogDirectory();
	std::wstring GetLogDirectory();
	std::wstring GetSettingsDirectory();
	void SetErrorCode(int errorCode);
	int GetErrorCode();

	void SetLoginSuccess(bool isLoginSuccess);
	void SetWriteLogSuccess(bool isWriteLogSuccess);

	bool IsLogWriteSuccess()
	{
		return m_isWriteLogSuccess;
	}

	std::string GetWorldServer();
	static bool LoadProxyInfo();
	static CURLcode CheckCurlProxys( CURL *curl, CURLcode result);
	static bool IsExistProxy();

private:	

private:
	CThread m_threadDumpServer;
	CThread m_threadWorldServer;
	CThread m_uploadThread;
	CThread m_threadWatch;

	std::map<std::string, unsigned short> m_processName;

	int m_errorCode;

	int lastSendedStatus;
	std::wstring appPath;
	std::wstring settingPath;
	std::wstring fileName;
	bool m_isLoginSuccess;
	bool m_isWriteLogSuccess;
};