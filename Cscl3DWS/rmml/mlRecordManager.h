#pragma once

namespace rmml {

class mlWorldEventManager : public sync::syncIEventManagerCallback
{
public: 
	mlWorldEventManager(mlSceneManager* apSM);
	~mlWorldEventManager();

public:
	// �������� ������ ��� ������ ����
	virtual void onRecordError(int aiErrorCode);
	// ������ �� ����� ���� ������ ��� ��� ������ ������ ��� �� ������
	virtual void onRecordStartFailed(int iErrorCode, const wchar_t* aAdditionalInfo);
	// ������ ����������� � ������ � ���������
	virtual void onRecordConnected(unsigned int aiID, const wchar_t* aAuthorOrAssistant, unsigned char isAuthor, unsigned int aDuration, const wchar_t* aLocationName);
	// ������ ���������� �� ������ � ���������
	virtual void onRecordDisconnected(unsigned int auRecordID,  bool isAuthor, bool isNeedToAsk);
	// ������ ������� ������� � ��������� �����������
	virtual void onRecordStoped(unsigned int auRecordID, unsigned int auDuration, syncRecordInfo* appRecordInfo = NULL);
	// �������� ������� � ������������� � �������
	virtual void onEventEntered(unsigned int auiEventId, const wchar_t* awcLocationID, unsigned int auiEventRealityId, unsigned int auiPlayingRecordId, unsigned int auiBaseRecordId, unsigned int auiEventRecordId, bool abIsEventRecording, const wchar_t* aJSONDescription);
	// �������� ������� �� ������������ �� �������
	virtual void onEventLeaved(unsigned int auiPlayingSeanceID,unsigned int auiPlayingRecordID);
	// �������� ������� �� ��������� � �������
	virtual void onEventUpdateInfo(unsigned int auiEventId, const wchar_t* awcLocationID, unsigned int auiEventRealityId, unsigned int auiPlayingRecordId, const wchar_t* aJSONDescription);
	// �������� ������ ����� ������������� � ������ ������� (��������� ���� ����� ������� �� �������)
	virtual void onEventUserRightsInfo( unsigned int aEventID, const wchar_t *aEventUserRights);		
	// ��������  ����� ������������
	virtual void onEventUserRightUpdate( unsigned int aEventID, const wchar_t *aEventUserRights);
	// �������� � ���� (������������)
	virtual void onEventUserBanUpdate( const wchar_t *aEventName, const wchar_t *aEventUrl, char aBan);
	// �������� ������� � ����� � �������
	virtual void onLocationEntered( const wchar_t* awcLocationID, const wchar_t* aJSONDescription);

public:
	// ������� � �������
	void onRecordConnectedInternal(mlSynchData& aData);
	void onRecordStartFailedInternal(mlSynchData& aData);
	void onRecordDisconnectInternal(mlSynchData& aData);
	void onRecordStopedInternal(mlSynchData& aData);
	// ������� �� "WorldEvent"
	void onEventEnteredInternal(mlSynchData&  aData);
	void onEventLeavedInternal(mlSynchData& aData);
	void onLocationEnteredInternal(mlSynchData& aData);
	void onEventUpdateInfoInternal(mlSynchData&  aData);
	void onEventUserRightsInfoReceivedInternal(mlSynchData& aData);
	void onEventUserRightUpdateReceivedInternal(mlSynchData& aData);
	void onEventUserBanUpdateReceivedInternal(mlSynchData& aData);	

private:
	void LeaveCurrentRecording();
	void LeaveCurrentEvent();

private:
	bool			m_isInRecording;
	bool			m_isMyRecord;
	unsigned int	m_currentRecordingID;
	unsigned int	m_currentEventID;
	mlSceneManager* mpSM;
};


}
