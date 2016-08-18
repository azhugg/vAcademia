#pragma once
#include "SyncMan.h"
#include "imessagehandler.h"
#include "iclientsession.h"
#include "ObjectStateQueue.h"

#include "MapObjectQueue.h"
#include <json/json.h>

// Received Type - ���� ����������� ���������
#define RT_UpdatePacket				1  // ������ �����������
#define RT_BadUpdatePacket  		2  // ����������� ������ �� ������
#define RT_DeleteObject				3  // ������ ������
#define RT_SyncMessage				4  // ��������� ��������� ��������� �������
#define RT_ConnectRecord			5  // ������ �������������� � ������
#define RT_LogRecordsSend	        6  // ������ ���� ��������
#define RT_SeanceFinished			9	// ����� ��������
#define RT_LogSeanceCreated			10  // ����� ��������������� ������
#define RT_ReceivedSeances			11  // ������� ������ ������� ��������������� ��� ������ ����
#define RT_ReceivedFullSeanceInfo	12	// �������� ������ ���� � ������ ������� URL
#define RT_ReceivedObjectStatus		14  // ������� ������ �������
#define RT_SeancePaused				15  // ������� ������ �������
#define RT_SessionCheck				16  // ������ ������� �� ���������� ������
#define RT_SeanceLoaded				17  // ����� ��������������� �������� �� �������
#define RT_DisconnectRecord			18  // ������ ������������� �� ������ � ���������
#define RT_UpdatePlayingTime		19  // � ������� ������ ����� ����� �� ����� �������� �����
#define RT_SeancePlayed				20  // � ������� ������ ������� ��� ����� �������������
#define RT_SeanceRewinded			21  // � ������� ������ ������� ��� ����� ��������� �� ������
#define RT_SeanceFinishedPlaying	22  // � ������� ������ ������� ��� ����� ��������� �� ������
#define RT_ReceivedPrecizeURL		24  // � ������� ������ ���������� URL
#define RT_SeanceLeaved				25  // � ������� ������ ���������� URL
//#define RT_LessonCreated			26  // � ������� ������ ������������ ��� � ������� ������� �������.
#define RT_UserListReceived			27	// � ������� ������ ������ ������������� � ����������
#define RT_ServerError				28	// � ������� ������ ������ � �����
#define RT_EventEntered				29	// � ������� ������ ����������� � ������������� � ������
#define RT_EventLeaved				30	// � ������� ������ ����������� � ������������ �� ������
#define RT_EventUpdateInfo			34	// � ������� ������ ��������� � ��� ��� ����� ��� �������

#define RT_ObjectCreationResult		31	// � ������� ������ ����� � �������� �������
#define RT_StopRecord				32	// � ������� ������ ��������� � ��� ��� ������ ����� ������� ���� �����������
#define RT_SeekPos					33	// � ������� ������ ��������� � ��� ��� ������ ���� ���������� �� ������������ �������
#define RT_EventRecordsSend			35	// ������ �������� � �������
#define RT_ReceivedPrecizeURL		24  // � ������� ������ ���������� URL
#define RT_ReceivedSeancesInfo		39  // � ������� ������ ���������� � �������
#define RT_ReceivedEventUserRightsInfo		40  // � ������� ������ ���������� � ������ ������������ � �������
#define RT_ReceivedEventUserRightUpdate		42  // � ������� ������ ���������� � ������ ����� ������������ � �������
#define RT_ReceivedEventUserBanUpdate		43  // � ������� ������ � ���� ����� ������������ � �������
#define RT_ErrorCreateTempLocation			44	// C ������� ������ ��������� �� ������ �������� ��������� �������
#define RT_ReceivedEventsInfoIntoLocation	45  // � ������� ������ ���������� � ��������������� �������� � �������� � �� ������� ���������� 

// ����������� ��������� ��������� �������
#define RT_RecordEditorCutComplete			46  // �������� ���������� �������� Cut 
#define RT_RecordEditorCopyComplete			47  // �������� ���������� �������� Copy 
#define RT_RecordEditorPasteComplete		48  // �������� ���������� �������� Paste 
#define RT_RecordEditorDeleteComplete		49  // �������� ���������� �������� Delete
#define RT_RecordEditorSaveComplete			50  // �������� ���������� �������� Save
#define RT_RecordEditorSaveAsComplete		51  // �������� ���������� �������� SaveAs
#define RT_RecordEditorPlayComplete			52  // �������� ���������� �������� Play 
#define RT_RecordEditorPauseComplete		53  // �������� ���������� �������� Pause
#define RT_RecordEditorRewindComplete		54  // �������� ���������� �������� Rewind
#define RT_RecordEditorSeekComplete			55  // �������� ���������� �������� Seek
#define RT_RecordEditorUndoComplete			56  // �������� ���������� �������� Undo
#define RT_RecordEditorRedoComplete			57  // �������� ���������� �������� Redo
#define RT_RecordEditorOpenComplete			58  // �������� ���������� �������� Open

#define RT_LocationEntered					59  // ������������ ������ ������� �� �������

#define RT_InfoMessage						60  // �������������� ���������

// ��������� Map Server
#define MRT_FIRST							127
#define MRT_ZONES_DESC_QUERY_ID				128
#define MRT_ZONES_OBJECTS_DESC_QUERY_ID		129
#define MRT_PORTALS_STATES_QUERY_ID			130
#define MRT_PORTALS_STATES_CHANGE_QUERY_ID	131
#define MRT_OBJECTS_DESC_QUERY_ID			132
#define MRT_OBJECT_UPDATE_QUERY_ID			133
#define MRT_OBJECT_DELETE_BY_NAME_QUERY_ID	134
#define MRT_OBJECT_DESC_BY_NAME_QUERY_ID	135
#define MRT_AVATAR_DESC_QUERY_ID			136
// NatureSettingsQueryID					137
#define MRT_REALITY_SETTINGS_QUERY_ID		138
#define MRT_OBJECT_CREATE_QUERY_ID			139
#define MRT_OBJECT_DELETE_QUERY_ID			140
#define MRT_REALITY_DESC_QUERY_ID			141
#define MRT_OBJECT_STATUS_QUERY_ID			142
#define MRT_CREATE_LOCATION_QUERY_ID		143
#define MRT_DELETE_OBJECT_QUERY_ID			144
#define MRT_LAST							145



namespace rmml {
struct mlSysSynchProps;
struct syncObjectProp;
struct syncObjectState;
}

namespace SyncManProtocol {
class CTempPacketIn;
}

using namespace rmml;


class CSyncManager :
	public sync::syncISyncManager,
	public cs::imessagehandler,
	public cs::isessionlistener,
	protected cm::cmIComManCallbacks
{
public:
	static int iWrongSynchPacket;
	CSyncManager( omsContext* aContext);
	virtual ~CSyncManager();

	// ���������� � ������� �������������
	virtual void Connect();
	// ��������� �� ������� �������������
	virtual void Disconnect();
	// ������� �������� �������������
	virtual void Release();

	// �������� ��� ������ ���������
	virtual unsigned int GetHashObj( const syncObjectState& object_all_props, bool abLogPacket);
	// �������� �� ������ ������ �� ��������� ���������������� ������� �������
	virtual void SendObjectState(const syncObjectState& object_props);
	// �������� ��������� ��������� ��������� ����������������� �������
	virtual bool NextObjectState(syncObjectState* &object_props);
	// �������� ��������� ��������� ��������� ��� ����������� ����������������� �������
	virtual bool NextObjectState(ZONE_OBJECT_INFO* aZoneObjectInfo, syncObjectState* &object_props);
	// ���������� ������ �� ���������� ���������������� �������
	virtual void FreeObjectState(syncObjectState* &object_props);
	// ���������� ������ �� ���������� ���������������� �������
	virtual void FreeCachedObjectState(syncObjectState* &object_props);
	// �������� �������� ������� � ������� �� ��������
	bool PutMapObject( const ZONE_OBJECT_INFO& aZoneObjectInfo);
	// ���������� ������� ������� ��� ���������� ������� �������� �� �����������
	void SetAvatarPosition(const float x, const float y, char* locationName);

	// ����� �������� ������� �� �������
	bool NextMapObject( ZONE_OBJECT_INFO*& aZoneObjectInfo);
	// ������� �������, � ������� currentRealityID == auRealityID
	void DeleteObjectsExceptReality( unsigned int auRealityID);
	// ���������� ������ �������� �������
	void FreeMapObject( ZONE_OBJECT_INFO*& aZoneObjectInfo);
	bool IsNewObjectWasAdded();
	bool IsObjectQueueEmpty();
	// ���������� ���������� �������� �������� �� ������� �� ��������
	int GetNearestMapObjectCount() const;

	// ���� �� ����������� ������ ������� ������� �������� � ������� ��������,
	// ����� ProcessSyncMessages() �� ����������
	void SetStateToAnalysisObjectsList(bool state);
	bool GetStateToAnalysisObjectsList();

	// ������� ��������� �������
	virtual void SendObject( unsigned char* aData, unsigned int aDataSize);
	// ������� ��������� �������
	virtual void QueryObject( unsigned int anObjectID, unsigned int aBornRealityID,unsigned int aRealityID, long long aCreatingTime, bool checkFrame=true);
	// �������� ������ ��������� �������, ������� ����� �� ������� ���������� ���������
	virtual void AddQueryLatentedObject( unsigned int anObjectID, unsigned int aBornRealityID, unsigned int aCurrentRealityID, unsigned int auiHash);
	// ������� ��������� ��������, ������� ����� �� ������� ���������� ���������
	virtual void QueryLatentedObjects();
	// ������� ���������������� ���������
	virtual void SendMessage( unsigned int anObjectID, unsigned int aBornRealityID, unsigned char* aData, unsigned short aDataSize);
	// ������� ����� ��������������� ���� �� URL
	virtual omsresult createSeanceByURL(syncSeanceInfo& aSeanceInfo, unsigned int auIDSeanceRmmlObject);
	// �������� URL ��� ������������ �� ������ ��� ���������
	virtual omsresult PreciseURL(const wchar_t* apwcURL);
	// ����� ������
	virtual omsresult StartRecord(int aiRecordMode, const wchar_t* apwcRecordName, int& aiErrorCode);
	// �������� ������
	virtual omsresult StopRecord( unsigned int& aRecordID, int& aiErrorCode, syncRecordInfo** aRecordInfo);
	// ���������� ���������, ����������� �������� ������ �� SyncManager-�
	virtual bool SetDefaultCallbacks(sync::syncISyncManCallbacks* apCallbacks, sync::syncIRecordManagerCallback* apRMCallback, sync::syncIEventManagerCallback* apEventManagerCallbacks);
	// �������� �� ������� ���������� � ������ 
	virtual omsresult UpdateRecordInfo(syncRecordInfo& aRecordInfo);
	// �������� ����� ������� (auID - ����� ������������� ������ �������, ������� ���� �������� � callback)
	virtual omsresult GetRecords(unsigned int auID, int aiBegin, int aiCount, const wchar_t* apwcFilter, const wchar_t* apwcOrderBy, sync::syncIRecordManagerCallback * pCallback);
	// �������� ����� ������� ��������������� �� ���������� ������ ����
	virtual omsresult GetSeances(unsigned int auID, unsigned int auSeancesObjectID);
	// ������ ��������������� ������ (�������� ������ �� ������)
	virtual omsresult Play(unsigned int auID);
	// ����������� ��������������� ������ (�������� ������ �� ������)
	virtual omsresult Seek( unsigned long seekPos);
	// ���������� ����� ������������, ������������ ���������������� ������
	virtual omsresult SetPilotLogin( const wchar_t* alpcPilotLogin, unsigned int auCurrentEventID);
	// �������� ���������� � ������
	virtual omsresult QueryFullSeanceInfo(unsigned int auIDSeanceRmmlObject, unsigned int auID);
	// ���������/��������� ������������ �������� ������������ ������� � ������������ ����������
	virtual bool SetLocationEnteringPermissionForUser( const wchar_t* apwcUserLogin, const wchar_t* apwcLocationID, unsigned int auRealityID, bool abAllow);
	
	// ! jedi ��������������� ��� ��� �� ������� ��������� ������� ���� ����� �������� ������������� �������
	// ������ ������ �������
	//virtual omsresult AvatarChangeLocation(const wchar_t* apwcLocationName,  unsigned int auiObjectID, unsigned int auiBornRealityID);
	
	// ��������� ��������� ���� ����������� �� ���� ��������� �� ������� �������������, ������� ����� ���� � ���������� ��������
	virtual void ProcessSyncMessages();
	// ������� ������ � ����� ��������������� (�������� ������ �� ������)
	virtual omsresult DeleteRecord(unsigned int auID);
	// ������� ������ � ������� (�������� ������ �� ������)
	virtual omsresult DeleteRecordIntoRecycleBin(unsigned int auID);
	// ������� ������ (�������� ������ �� ������)
	virtual omsresult DeleteRecords( unsigned int *pRecIDs, unsigned int count);
	// �������� ������ ������������� �� �������
	virtual omsresult GetUserList( unsigned int realityID);
	// ���������� ��������������� ������
	virtual omsresult Stop();
	// ������������� ��������������� ������
	virtual omsresult Pause();
	// ���������� ������ �� ������
	virtual omsresult Rewind();
	// ������� ������
	virtual omsresult DeleteObject(unsigned int auID, unsigned int auRealityID);
	// �������� ������ �������
	virtual omsresult GetObjectStatus(unsigned int auObjectID, unsigned int auBornRealityID);
	// ������ �����������
	virtual omsresult CloseClient();
	virtual omsresult GetEventRecords(unsigned int auID, int aiBegin, int aiCount, const wchar_t* apwcFilter, const wchar_t* apwcOrderBy, sync::syncIRecordManagerCallback2 *pCallback);

	virtual omsresult Save( const wchar_t* apwcAuthor, const wchar_t* apwcCreationTime, const wchar_t* apwcName, const wchar_t* apwcDescription);

	// �������� �� ������ � ���������� �������
	// ��������� �������� ������
	virtual void RecordEditorCut(unsigned int startPos, unsigned int endPos);
	// ������� �������� ������
	virtual void RecordEditorPaste(unsigned int pastePos);
	// ����������� �������� ������
	virtual void RecordEditorCopy(unsigned int startPos, unsigned int endPos);
	// �������� �������� ������
	virtual void RecordEditorDelete(unsigned int startPos, unsigned int endPos);
	// ������ �������� ��������� ������� (�������� ������ �� ������)	
	virtual void RecordEditorUndo();
	// ������ �������� ��������� ������� (�������� ������ �� ������)	
	virtual void RecordEditorRedo();
	// �������� ������ ��� �������������� (�������� ������ �� ������)
	virtual void RecordEditorOpen(unsigned int aRecordID);
	// ���������� ������ �� ��������� ������� (�������� ������ �� ������)
	virtual void RecordEditorSave();
	// ���������� ������ ��� ����� ������ �� ��������� �������  (�������� ������ �� ������)
	virtual void RecordEditorSaveAs(const wchar_t* apwcName, const wchar_t* apwcDescription, bool abOverwrite, bool abPublicate);
	// ������ ��������������� ������
	virtual void RecordEditorPlay();
	// �������������� ��������������� ������
	virtual void RecordEditorPause();
	// ���������� �������������� ������ �� ������
	virtual void RecordEditorRewind();
	// ���������� ��������������� ������
	virtual void RecordEditorSeek(unsigned int aSeekPos);


	virtual void SetLogSyncMessage(bool bVal);

	// ������� ������ ��� �������� ���������. ��������� �������
	virtual cs::iclientsession* GetServerSession();

	// ��� ������ ��� ������ �����������
	virtual void OnLogout( int iCode);

	virtual omsresult GetEventUserRight( unsigned int eventID);
	virtual omsresult SetEventUserRights( unsigned int eventID, wchar_t *apwcDescr);
	virtual omsresult SetEventUserDefaultRights( unsigned int eventID, unsigned int  aRight);
	virtual omsresult SetEventUserBan( unsigned int eventID, wchar_t *apwcDescr);

	// ���������� �� ������ ����� �������������� ������������� ����� ������� �����
	// � ������ ������������ ��������� ��������� ������ � �������� URL ��� ������������
	virtual void ConnectToWorld( const wchar_t *apwcRequestedUrl, const syncObjectState* mpcAvatarState);

	// �������� ��� ������ ���������
	void LogBinSyncState( const syncObjectState& aProps);

	BEGIN_IN_MESSAGE_MAP()
		//IN_MESSAGE2_CHECKSUM( RT_UpdatePacket, OnUpdatePacket)
		IN_MESSAGE_ONE_OF_RANGE(RT_UpdatePacket, OnAnySyncMessageReceived)
		//IN_MESSAGE2( RT_BadUpdatePacket, OnBadUpdatePacket)
		IN_MESSAGE_ONE_OF_RANGE(RT_BadUpdatePacket, OnAnySyncMessageReceived)
		//IN_MESSAGE2( RT_DeleteObject, OnDeleteObject)
		IN_MESSAGE_ONE_OF_RANGE(RT_DeleteObject, OnAnySyncMessageReceived)
		//
		IN_MESSAGE2( RT_SyncMessage, OnMessage)
		IN_MESSAGE2( RT_ConnectRecord, OnRecordConnected)
		IN_MESSAGE2( RT_DisconnectRecord, OnRecordDisconnected)
		IN_MESSAGE2( RT_StopRecord, OnRecordStoped)
		IN_MESSAGE2( RT_LogRecordsSend, OnGettingLogRecords)
		IN_MESSAGE2( RT_SeanceFinished, OnSeanceFinished)
		IN_MESSAGE2( RT_LogSeanceCreated, OnLogSeanceCreated)
		IN_MESSAGE2( RT_ReceivedSeances, OnReceivedSeances)
		IN_MESSAGE2( RT_ReceivedFullSeanceInfo, OnReceivedFullSeanceInfo)
		IN_MESSAGE2( RT_ReceivedObjectStatus, OnReceivedObjectStatus)
		IN_MESSAGE2( RT_SeancePaused, OnSeancePaused)
		IN_MESSAGE2( RT_SessionCheck, OnSessionCheck)
		IN_MESSAGE2( RT_SeanceLoaded, OnSeanceLoaded)
		IN_MESSAGE2( RT_UpdatePlayingTime, OnUpdatePlayingTime)
		IN_MESSAGE2( RT_SeancePlayed, OnSeancePlayed)
		IN_MESSAGE2( RT_SeanceRewinded, OnSeanceRewinded)
		IN_MESSAGE2( RT_SeekPos, OnSeanceSeekPos)
		IN_MESSAGE2( RT_SeanceFinishedPlaying, OnSeanceFinishedPlaying)
		IN_MESSAGE2( RT_ReceivedPrecizeURL, OnReceivedPrecizeURL)
		IN_MESSAGE2( RT_SeanceLeaved, OnSeanceLeaved)
		IN_MESSAGE2( RT_UserListReceived, OnUserListReceived)
		IN_MESSAGE2( RT_ServerError, OnServerErrorReceived)
		IN_MESSAGE2( RT_EventEntered, OnEventEntered)
		IN_MESSAGE2( RT_EventLeaved, OnEventLeaved)
		IN_MESSAGE2( RT_EventUpdateInfo, OnEventUpdateInfo)
		IN_MESSAGE2( RT_ObjectCreationResult, OnObjectCreationResult)
		IN_MESSAGE2( RT_EventRecordsSend, OnGettingEventRecords)
		IN_MESSAGE2( RT_ReceivedSeancesInfo, OnReceivedSeancesInfo)
		IN_MESSAGE2( RT_ReceivedEventUserRightsInfo, OnReceivedEventUserRightsInfo)
		//IN_MESSAGE2( RT_ReceivedEventUserRolesUpdate, OnReceivedEventUserRolesUpdate)	
		IN_MESSAGE2( RT_ReceivedEventUserRightUpdate, OnReceivedEventUserRightUpdate)	
		IN_MESSAGE2( RT_ReceivedEventUserBanUpdate, OnReceivedEventUserBanUpdate)
		IN_MESSAGE2( RT_ErrorCreateTempLocation, OnErrorCreateTempLocation)
		IN_MESSAGE2( RT_ReceivedEventsInfoIntoLocation, OnReceivedEventsInfoIntoLocation)
		IN_MESSAGE2( RT_LocationEntered, OnLocationEntered)
		IN_MESSAGE2( RT_InfoMessage, OnInfoMessage)
		////////////////////////////////////////////////////////////////////////////////////////
		// ��������� ��� RecordEditor
		////////////////////////////////////////////////////////////////////////////////////////
		IN_MESSAGE2( RT_RecordEditorCutComplete, OnRecordEditorCutComplete)
		IN_MESSAGE2( RT_RecordEditorCopyComplete, OnRecordEditorCopyComplete)
		IN_MESSAGE2( RT_RecordEditorPasteComplete, OnRecordEditorPasteComplete)
		IN_MESSAGE2( RT_RecordEditorDeleteComplete, OnRecordEditorDeleteComplete)
		IN_MESSAGE2( RT_RecordEditorSaveComplete, OnRecordEditorSaveComplete)
		IN_MESSAGE2( RT_RecordEditorSaveAsComplete, OnRecordEditorSaveAsComplete)
		IN_MESSAGE2( RT_RecordEditorPlayComplete, OnRecordEditorPlayComplete)
		IN_MESSAGE2( RT_RecordEditorPauseComplete, OnRecordEditorPauseComplete)
		IN_MESSAGE2( RT_RecordEditorRewindComplete, OnRecordEditorRewindComplete)
		IN_MESSAGE2( RT_RecordEditorSeekComplete, OnRecordEditorSeekComplete)
		IN_MESSAGE2( RT_RecordEditorUndoComplete, OnRecordEditorUndoComplete)
		IN_MESSAGE2( RT_RecordEditorRedoComplete, OnRecordEditorRedoComplete)
		IN_MESSAGE2( RT_RecordEditorOpenComplete, OnRecordEditorOpenComplete)
		////////////////////////////////////////////////////////////////////////////////////////
		// ��������� ��� MapSessionWrapper
		////////////////////////////////////////////////////////////////////////////////////////
		IN_MESSAGE_ONE_OF_RANGE(MRT_AVATAR_DESC_QUERY_ID, OnAnySyncMessageReceived)
		IN_MESSAGE_ONE_OF_RANGE(MRT_ZONES_DESC_QUERY_ID, OnAnySyncMessageReceived)
		IN_MESSAGE_ONE_OF_RANGE(MRT_ZONES_OBJECTS_DESC_QUERY_ID, OnAnySyncMessageReceived)
		IN_MESSAGE_ONE_OF_RANGE(MRT_OBJECTS_DESC_QUERY_ID, OnAnySyncMessageReceived)
		IN_MESSAGE_ONE_OF_RANGE(MRT_PORTALS_STATES_QUERY_ID, OnAnySyncMessageReceived)
		IN_MESSAGE_ONE_OF_RANGE(MRT_OBJECT_DESC_BY_NAME_QUERY_ID, OnAnySyncMessageReceived)
		IN_MESSAGE_ONE_OF_RANGE(MRT_OBJECT_UPDATE_QUERY_ID, OnAnySyncMessageReceived)
		IN_MESSAGE_ONE_OF_RANGE(MRT_REALITY_DESC_QUERY_ID, OnAnySyncMessageReceived)
		IN_MESSAGE_ONE_OF_RANGE(MRT_OBJECT_STATUS_QUERY_ID, OnAnySyncMessageReceived)
		IN_MESSAGE_ONE_OF_RANGE(MRT_CREATE_LOCATION_QUERY_ID, OnAnySyncMessageReceived)
		IN_MESSAGE_ONE_OF_RANGE(MRT_DELETE_OBJECT_QUERY_ID, OnAnySyncMessageReceived)
	END_IN_MESSAGE_MAP()

	void OnConnectLost();
	void OnConnectRestored();

	void OnLocationsListChanged();

	void CheckHashAvatarsThread();
	void CheckHashObjectsThread();

protected:
	omsContext* pContext;
	cs::iclientsession* pClientSession;
	unsigned int iRecordID;
	sync::syncISyncManCallbacks* pCallbacks;
	sync::syncIRecordManagerCallback * pRMCallBack;
	sync::syncIEventManagerCallback* pEventManagerCallbacks;
	unsigned int IDLogObject;
	syncRecordInfo oRecordInfo;
	//bool bCheckHash;
	//HANDLE eventCheckHash;
	// ������ ���� � ������

protected:
	void onQueryComplete(int aiID, const wchar_t* apwcResponse=NULL, int* aiErrorCode=NULL);

	void OnUpdatePacket( BYTE* aData, int aDataSize, DWORD aCheckSum);
	void OnBadUpdatePacket( BYTE* aData, int aDataSize);
	void OnDeleteObject( BYTE* aData, int aDataSize);
	void OnMessage( BYTE* aData, int aDataSize);
	void OnRecordConnected( BYTE* aData, int aDataSize);
	void OnRecordDisconnected( BYTE* aData, int aDataSize);
	void OnRecordStoped( BYTE* aData, int aDataSize);
	void OnUpdatePlayingTime( BYTE* aData, int aDataSize);
	void OnGettingLogRecords( BYTE* aData, int aDataSize);
	void OnSeanceFinished( BYTE* aData, int aDataSize);
	void OnLogSeanceCreated( BYTE* aData, int aDataSize);
	void OnReceivedSeances( BYTE* aData, int aDataSize);
	void OnReceivedFullSeanceInfo( BYTE* aData, int aDataSize);
	void OnReceivedObjectStatus( BYTE* aData, int aDataSize);
	void OnSeancePaused( BYTE* aData, int aDataSize);
	void OnSessionCheck( BYTE* aData, int aDataSize);
	void OnSeanceLoaded( BYTE* aData, int aDataSize);
	void OnSeancePlayed( BYTE* aData, int aDataSize);
	void OnSeanceRewinded( BYTE* aData, int aDataSize);
	void OnSeanceSeekPos( BYTE* aData, int aDataSize);
	void OnSeanceFinishedPlaying( BYTE* aData, int aDataSize);
	void OnReceivedPrecizeURL( BYTE* aData, int aDataSize);
	void OnSeanceLeaved( BYTE* aData, int aDataSize);
	void OnUserListReceived( BYTE* aData, int aDataSize);
	void OnServerErrorReceived( BYTE* aData, int aDataSize);
	void OnEventEntered( BYTE* aData, int aDataSize);
	void OnEventUpdateInfo( BYTE* aData, int aDataSize);
	void OnEventLeaved( BYTE* aData, int aDataSize);
	void OnObjectCreationResult( BYTE* aData, int aDataSize);
	void OnGettingEventRecords( BYTE* aData, int aDataSize);
	void OnReceivedSeancesInfo( BYTE* aData, int aDataSize);
	void OnReceivedEventsInfoIntoLocation( BYTE* aData, int aDataSize);
	void OnLocationEntered( BYTE* aData, int aDataSize);
	void OnInfoMessage( BYTE* aData, int aDataSize);
	void OnReceivedEventUserRightsInfo( BYTE* aData, int aDataSize);	
	void OnReceivedEventUserRolesUpdate( BYTE* aData, int aDataSize);
	void OnReceivedEventUserRightUpdate( BYTE* aData, int aDataSize);	
	void OnReceivedEventUserBanUpdate( BYTE* aData, int aDataSize);
	void OnErrorCreateTempLocation( BYTE* aData, int aDataSize);

	////////////////////////////////////////////////////////////////////////////////////////
	// ��������� ��� recordEditor
	////////////////////////////////////////////////////////////////////////////////////////
	void OnRecordEditorCutComplete( BYTE* aData, int aDataSize);
	void OnRecordEditorCopyComplete( BYTE* aData, int aDataSize);
	void OnRecordEditorPasteComplete( BYTE* aData, int aDataSize);
	void OnRecordEditorDeleteComplete( BYTE* aData, int aDataSize);
	void OnRecordEditorSaveComplete( BYTE* aData, int aDataSize);
	void OnRecordEditorSaveAsComplete( BYTE* aData, int aDataSize);
	void OnRecordEditorPlayComplete( BYTE* aData, int aDataSize);
	void OnRecordEditorPauseComplete( BYTE* aData, int aDataSize);
	void OnRecordEditorRewindComplete( BYTE* aData, int aDataSize);
	void OnRecordEditorSeekComplete( BYTE* aData, int aDataSize);
	void OnRecordEditorUndoComplete( BYTE* aData, int aDataSize);
	void OnRecordEditorRedoComplete( BYTE* aData, int aDataSize);
	void OnRecordEditorOpenComplete( BYTE* aData, int aDataSize);
	
	////////////////////////////////////////////////////////////////////////////////////////
	// ��������� ��� MapSessionWrapper
	////////////////////////////////////////////////////////////////////////////////////////
	void OnAnySyncMessageReceived( BYTE aMessageID, BYTE* aData, int aDataSize, int aCheckSum);

private:
	syncObjectState* CreateSyncObjectState( BYTE* aData, int aDataSize, DWORD aCheckSum, std::string& anErrorText);
	bool PutSyncData( BYTE* aData, int aDataSize, DWORD aCheckSum, std::string& anErrorText);
	void PushObjectStateToQueue( syncObjectState* aObjectState);
	syncObjectState* CreateNotFoundSyncObject( unsigned int anObjectID, unsigned int aBornRealityID, unsigned int aRealityID);
	void PutNotFoundSyncObject( unsigned int anObjectID, unsigned int aBornRealityID, unsigned int aRealityID);
	bool NextMapObject( ZONE_OBJECT_INFO*& aZoneObjectInfo, ZONE_OBJECT_INFO* aFirst);

private:
	void syncObjectSysPropsToJsonValue( Json::Value &root, mlSysSynchProps *sys_props);
	void syncObjectPropsToJsonValue( Json::Value &root, std::vector<syncObjectProp>::iterator &iter, int index);
	void syncObjectStateToJsonValue( Json::Value &root, syncObjectState *object_props);
	void writeLog( const char* alpcLog);

	int syncDataLogWriteThread();
	static DWORD WINAPI syncDataLogWriteThread_(LPVOID param);

	void GetLanguage( CWComString &sLang);


private:
	bool IsReady();
	bool IsConnected();

private:
	CRITICAL_SECTION mutex;
	CRITICAL_SECTION syncMessageCS;

	// ������� ���������� ��������� ��������
	CObjectStateQueue objectStateQueue;

	// ������� �������� ��������
	CMapObjectQueue mapObjectQueue;

	// 
	FILE *fpSynchLog;
	HANDLE hSendEvent;
	HANDLE hSyncDataLogThread;
	MP_LIST< std::string> oSyncSendQueue;
	// ����������� ������ �� ��������� oSyncSendQueue
	CRITICAL_SECTION csSyncSendQueue;
	bool bSyncLog;
	bool m_isNewObjectAdded;

	bool m_isAsynchAnalysisObjectsList;

	MP_MAP<std::string, int> m_classLoaded;

	// ������� ������� �������������. ����� ��� ���������� ����������� ��������� ������� � rmml � RM
	typedef struct _ReceivedMessage
	{
		unsigned int		uiMessageID;
		unsigned int		uiCheckSum;
		SyncManProtocol::CTempPacketIn*	pCommonPacketIn;
	} ReceivedMessage;
	typedef MP_VECTOR<ReceivedMessage>	VecReceivedMessage;
	typedef VecReceivedMessage::iterator	VecReceivedMessageIt;
	VecReceivedMessage		m_receivedMessages;
};