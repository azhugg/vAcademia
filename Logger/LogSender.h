
#pragma once

#define MAX_INT_LENGTH		20
#define MAX_DOUBLE_LENGTH	50

#include <string>
#include "logger.h"

#define ST_ClientLog					5 // ������� ������ log-�
#define MAX_STRING_IN_PACKET_SIZE	16384
#define MAX_SESSION_QUEUE_SIZE	500000

using namespace lgs;

class CLogSender : public ILogger
{
//friend class CGlobalSingletonStorage;

public:
	//void SetApplicationName(std::string appName);
	CLogSender(oms::omsContext* aContext);
	void Destroy(){ MP_DELETE_THIS;};

	void SetClientSession(cs::iclientsession* apClientSession);

	void SetMode(bool isWorking);

	// ������ ���� � �������� (�������� � ����� �������) ��� ������
	void WriteLPCSTR(const char* str);
	void WriteLnLPCSTR(const char* str);
	void WriteLPCSTR(const char* time, const char* str);
	void WriteLnLPCSTR(const char* time, const char* str);
	// ������ ���� ��� �������� (�������� � ����� �������) ��� ������
	void PrintLPCSTR(const char* str);
	void PrintLnLPCSTR(const char* str);

protected:
	// ������ ���� c ������� ��� ������
	void WriteLPCSTR(const char* time, const char* str, const char* delimiter);
	// ������ ���� ��� ������� ��� ������
	void PrintLPCSTR(const char* str, const char* delimiter);

private:
	CRITICAL_SECTION m_cs;

	CLogSender(const CLogSender&);
	CLogSender& operator=(const CLogSender&);
	~CLogSender();

	bool m_isWorking;
	MP_STRING m_logName;
	cs::iclientsession *pClientSession;
	MP_STRING sMessages;
};
