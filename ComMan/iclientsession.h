
#if !defined(AFX_ICLIENTSESSION_H__375D114C_CE19_4DD9_A2C4_BF3F6D5B789B__INCLUDED_)
#define AFX_ICLIENTSESSION_H__375D114C_CE19_4DD9_A2C4_BF3F6D5B789B__INCLUDED_

namespace cs
{
	struct imessagehandler;

	struct isessionlistener
	{
		virtual void OnConnectLost() = 0;
		virtual void OnConnectRestored() = 0;
	};

	struct iclientsession
	{
		virtual int GetSendQueueSize() = 0;
		virtual int GetStatus() = 0;

		// �������� �� ������ ������, ������� �� ������� ������
		virtual void Send(unsigned char aType, const unsigned char FAR *aData, int aDataSize) = 0;
		// ���������� ��������� c ����������� ������������ ��� �������� ������ �� ���
		virtual void SendQuery(unsigned char aType, const unsigned char FAR *aData, int aDataSize) = 0;
		// �������� �� ������ ������, ������ ���� ��������� �������. 
		// ����� �������� ������� ��� ��������� � ��������� �����.
		virtual void SendSignalQuery(unsigned char aType, const unsigned char FAR *aData, int aDataSize) = 0;
		// ������� ������ �� ��������
		virtual void RemoveQueryByType(unsigned char aType) = 0;
		// ���������� ��������� ��� ��������� ���������
		virtual void SetMessageHandler(imessagehandler* pMesHandler) = 0;
		// ���������� ��������� ��� ��������� ������� ������� � �������������� �����
		virtual void SetSessionListener(isessionlistener* pListener) = 0;
		// ���������� ��������� ������� ��������
		virtual void ChangeConnection() = 0;
		// ������ ������ ������������ ������ �����
		virtual void EmulateBreak( /*int auConnectionMask*/) = 0;
		// ����������� ������������� ������� ������� ������� � ������
		virtual void SetMaxQueueSize( int aMaxQueueSize) = 0;
	};

	struct iclientsynchsession
	{		
		virtual int SendAndReceive(unsigned char aType, const unsigned char FAR *aData, int aDataSize, DWORD timeout = INFINITE, int* aQueryID = NULL) = 0;
	};
};

#endif