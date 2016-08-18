#ifndef __SYNCMAN_H_
#define __SYNCMAN_H_

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SYNCMAN_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SYNCMAN_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SYNCMAN_EXPORTS
	#define SYNCMAN_API __declspec(dllexport)
#else
	#define SYNCMAN_API __declspec(dllimport)
#endif

#ifdef WIN32
	#define SYNCMAN_NO_VTABLE __declspec(novtable)
#else
	#define SYNCMAN_NO_VTABLE
#endif

#include "oms_context.h"

#define INCORRECT_RECORD_ID 0xFFFFFFFF
#define INCORRECT_REALITY_ID 0xFFFFFFFF
#define INCORRECT_EVENT_ID 0xFFFFFFFF

//#include "rmml.h"

namespace rmml {
struct syncObjectState;
struct syncObjectsForLoading;
}
struct ZONE_OBJECT_INFO;
class CComString;

using namespace oms;
using namespace rmml;

struct syncSeanceInfo
{
	unsigned int muSeanceID;
	// �������� ������
	MP_WSTRING mpwcName;
	// ������������ ������
	MP_WSTRING mpwcMembers;
	// ����� 
	MP_WSTRING mpwcAuthor;
	// URL 
	MP_WSTRING mpwcURL;

	unsigned long  mlDate;
	unsigned int  mlMode;
	unsigned int mlRecordID;

	syncSeanceInfo();
	syncSeanceInfo(unsigned int pSeanceID, unsigned int pRecordID, const wchar_t* pwcName, const wchar_t* pwcMembers, const wchar_t* mpwcAuthor, unsigned long  alDate, unsigned int alMode);

};

// ��������� ���������� � ������
// ��������� ������ ���� ������
// ��� ��������� ���������� - ����������� ������, ������� ������ ��� ��� �� ���������
// ������ ��� ������ ������ ���������� � ��������� �� ���
struct syncRecordInfo
{
	//#define SYNCHRI_NAME			0x1
	//#define SYNCHRI_DESCRIPTION	0x2
	//#define SYNCHRI_TAGS			0x4
	// �����, ������������ ����� ���������� ������������� 
	//int miFlags;
	// (���� ����� ��� ������. ���� == NULL, ������ �� ����������. ���� == "", ������ ������ ������)
	// ������������� ������ 
	unsigned int muRecordID;
	// �������� ������
	MP_WSTRING mpwcName;
	// �������� ������
	MP_WSTRING mpwcDescription;
	// ���������
	MP_WSTRING mpwcPreview;
	// �������
	MP_WSTRING mpwcSubject;
	// ����
	MP_WSTRING mpwcLesson;
	// ����
	MP_WSTRING mpwcTags;
	// �������
	MP_WSTRING mpwcLevel;
	// ����� (RO)
	MP_WSTRING mpwcAuthor;
	// ����� ������ ��� (RO)
	MP_WSTRING mpwcAuthorFullName;
	// ���� ��� ������
	MP_WSTRING mpwcLocation;
	// ����� �������� ������ (RO)
	MP_WSTRING mpwcCreationTime;
	time_t mlCreationTime;
	// ������������ � ms (RO)
	unsigned int mlDuration;
	// ���������� ������������ (RO)
	unsigned int muiCommentCount;
	// ������� ������ 
	unsigned int muDepth;
	unsigned int muAvatarInRecordCount;
	unsigned int muVisitorCount;
	unsigned int muReality;
	// URL 
	MP_WSTRING mpwcURL;
	bool mbPublishNow;
	unsigned int status;

public:

	syncRecordInfo() : 
	MP_WSTRING_INIT(mpwcName),
		MP_WSTRING_INIT(mpwcDescription),
		MP_WSTRING_INIT(mpwcPreview),
		MP_WSTRING_INIT(mpwcSubject),
		MP_WSTRING_INIT(mpwcLesson),
		MP_WSTRING_INIT(mpwcTags),
		MP_WSTRING_INIT(mpwcLevel),
		MP_WSTRING_INIT(mpwcAuthor),
		MP_WSTRING_INIT(mpwcAuthorFullName),
		MP_WSTRING_INIT(mpwcLocation),
		MP_WSTRING_INIT(mpwcCreationTime),
		MP_WSTRING_INIT(mpwcURL)
	{
		muRecordID = 0;

		mlCreationTime = 0;
		mlDuration = 0;
		muiCommentCount = 0;
		muDepth = 0;
		muVisitorCount = 0;
		muAvatarInRecordCount = 0;
		muReality = 0;
		status = 0;
		mbPublishNow = false;
	}

	syncRecordInfo(unsigned int pRecordID, const wchar_t* pwcName, const wchar_t* pwcDescription, const wchar_t* pwcAuthor, const wchar_t* pwcAuthorFullName, const wchar_t* pwcLocation, const wchar_t* pwcSubject, const wchar_t* pwcLesson, const wchar_t* pwcLevel, unsigned int pDepth, const wchar_t* pwcPreview, unsigned int pAvatarCount, unsigned pVisitorCount, bool pIsReal, unsigned int pStatus, const wchar_t* pwcTags)	:
	MP_WSTRING_INIT(mpwcName),
		MP_WSTRING_INIT(mpwcDescription),
		MP_WSTRING_INIT(mpwcPreview),
		MP_WSTRING_INIT(mpwcSubject),
		MP_WSTRING_INIT(mpwcLesson),
		MP_WSTRING_INIT(mpwcTags),
		MP_WSTRING_INIT(mpwcLevel),
		MP_WSTRING_INIT(mpwcAuthor),
		MP_WSTRING_INIT(mpwcAuthorFullName),
		MP_WSTRING_INIT(mpwcLocation),
		MP_WSTRING_INIT(mpwcCreationTime),
		MP_WSTRING_INIT(mpwcURL)
	{
		muRecordID = pRecordID;
		if( pwcName)
			mpwcName = pwcName;
		if( pwcDescription)
			mpwcDescription = pwcDescription;
		if( pwcAuthor)
			mpwcAuthor = pwcAuthor;
		if( pwcAuthorFullName)
			mpwcAuthorFullName = pwcAuthor;
		if( pwcLocation)
			mpwcLocation = pwcLocation;
		if( pwcSubject)
			mpwcSubject = pwcSubject;
		if( pwcLesson)
			mpwcLesson = pwcLesson;
		if (pwcTags)
			mpwcTags = pwcTags;
		if( pwcLevel)
			mpwcLevel = pwcLevel;
		if( pwcPreview)
			mpwcPreview = pwcPreview;
		muDepth = pDepth;
		muVisitorCount = pVisitorCount;
		muAvatarInRecordCount = pAvatarCount;
		mbPublishNow = pIsReal;
		status = pStatus;
	}

	void SetID(unsigned int aRecID)
	{
		muRecordID = aRecID;
	}

	void SetName(std::wstring pwcName)
	{
		mpwcName = pwcName;
	}

	void SetAuthor(std::wstring pwcAuthor)
	{
		mpwcAuthor = pwcAuthor;
	}

	void SetDepth(unsigned int aDepth)
	{
		muDepth = aDepth;
	}
};

struct syncRecordExtendedInfo //: syncRecordInfo
{
	unsigned int seanceID;
	unsigned int realityID;
	unsigned int recordID;
	unsigned int recRealityID;
	unsigned int brRealityID;
	MP_WSTRING brRecordName;
	MP_WSTRING brRecordDescription;
	MP_WSTRING recordDuration;
	MP_WSTRING brDuration;
	__int64 fromStartDuration;
	__int64 eventDate;
	MP_STRING locationURL;
	MP_WSTRING language;
	unsigned int eventRecordID;
	MP_WSTRING name;
	MP_WSTRING description;
	MP_WSTRING recordName;
	MP_WSTRING recordDescription;
	unsigned int expected_length;
	unsigned int recID;
	unsigned int brRecID;
	MP_WSTRING login;
	MP_WSTRING firstname;
	MP_WSTRING rFirstname;
	MP_WSTRING lastname;
	MP_WSTRING rLastname;
	MP_WSTRING post;
	MP_WSTRING courseName;
	unsigned int open;
	unsigned int paid;
	unsigned int brDepth;
	unsigned int depth;
	unsigned int recordStatus;
	MP_STRING previewImage;
	MP_STRING brPreviewImage;
	MP_WSTRING locationDescription;
	MP_WSTRING loc_preview;
	unsigned int courseEventsCount;
	unsigned int courseEventNum;
	MP_WSTRING strTags;

public:
	syncRecordExtendedInfo() :
	MP_WSTRING_INIT(brRecordName),
		MP_WSTRING_INIT(brRecordDescription),
		MP_WSTRING_INIT(recordDuration),
		MP_WSTRING_INIT(brDuration),
		MP_WSTRING_INIT(language),
		MP_WSTRING_INIT(name),
		MP_WSTRING_INIT(description),
		MP_WSTRING_INIT(recordName),
		MP_WSTRING_INIT(recordDescription),
		MP_WSTRING_INIT(login),
		MP_WSTRING_INIT(firstname),
		MP_WSTRING_INIT(rFirstname),
		MP_WSTRING_INIT(lastname),
		MP_WSTRING_INIT(rLastname),
		MP_WSTRING_INIT(post),
		MP_WSTRING_INIT(courseName),
		MP_WSTRING_INIT(locationDescription),
		MP_WSTRING_INIT(loc_preview),
		MP_WSTRING_INIT(strTags),
		MP_STRING_INIT(locationURL),
		MP_STRING_INIT(previewImage),
		MP_STRING_INIT(brPreviewImage)
	{

	}
};

namespace sync
{

	struct syncStartRecordInfo
	{
		const wchar_t* mpwcAuthor;
		// ���� ��� ������
		unsigned long mlDate;

		syncStartRecordInfo(){
			// miFlags = 0;
			mpwcAuthor = NULL;
			mlDate = 0;
		}
		syncStartRecordInfo(const wchar_t* pwcAuthor, unsigned long pDate)
		{
			mpwcAuthor = pwcAuthor;
			mlDate = pDate;
		}
	};

	struct syncCountRecordInfo
	{
		unsigned int totalCountRecord;
		unsigned int totalCountRecordNow;
		unsigned int totalCountRecordWithoutStatus;
		unsigned int totalCountRecordSoon;
	};

	struct syncObjectsForLoading
	{
		unsigned int objectID;
		unsigned int bornRealityID;
	};

	struct SYNCMAN_NO_VTABLE syncISyncManCallbacks
	{
		// � ������� ��� ����� ������ �����
		virtual void onUpdatePlayingTime(unsigned int auTime) = 0;
		virtual void onSeanceSeekPos(unsigned int auTime) = 0;
		// � ������� �������� ���������� � ������� �� ������ ����
		virtual void onReceivedSeancesInfo(unsigned int auSeancesCount, syncSeanceInfo** appSeancesInfo, unsigned int auID, int aiErrorCode) = 0;
		// � ������� �������� ���������� � ������� �� ������ ����
		virtual void onReceivedFullSeanceInfo(unsigned int aiSeanceObjectID,  syncSeanceInfo* appSeancesInfo, int aiErrorCode) = 0;
		// � ������� �������� ���� ��� ������� ������� �����
		virtual void onSeanceLeaved() = 0;
		// ����� ��������������� �� ������� ������������� ������
		virtual void onSeanceCreated(unsigned int aiSeanceID, unsigned int aiSeanceObjectID, unsigned int aSeanceMode, const wchar_t* aAuthor, const wchar_t* SeanceURL, int aiErrorCode) = 0;
		// ����� ��������
		virtual void onSeanceClosed(unsigned int aiSeanceID) = 0;
		// ������� ������ �������
		virtual void onReceivedObjectStatus(unsigned int auObjectID, unsigned int auBornRealityID, int aiStatus) = 0;
		// �������� ��������� � ����� � ������������ ������
		virtual void onSeancePaused(unsigned int auBornRealityID) = 0;
		// �������� ��������� � ����� � �����
		virtual void onJoinedToSeance(unsigned int aiSeanceRecord, unsigned int aiSeanceID, unsigned char isAuthor, unsigned char isSuspended, int aMinutes) = 0;
		// �������� ��������� � �� ��� ������ ������������ ������
		virtual void onSeancePlayed() = 0;
		// �������� ��������� � �� ��� ��������� ���������
		virtual void onSeanceRewinded() = 0;
		// �������� ��������� � �� ��� ����� �������� �� �����
		virtual void onSeanceFinishedPlaying() = 0;
		// ������� ������ ������������� � ����������
		virtual void onUserListReceived(unsigned int realityID, unsigned short userCount, void* userList) = 0;
		// �������� � ������� ������
		virtual void onServerErrorReceived(unsigned int errorCode) = 0;
		// ������� ��������� �������� �������
		virtual void onObjectCreationResult(unsigned int auiErrorCode, unsigned int auiObjectID, unsigned int auiBornRealityID, unsigned int auiRmmlHandlerID) = 0;		
		// �������� ������ �������� ��������� �������
		virtual void onErrorCreateTempLocation( unsigned int aErrorCode) = 0;
		virtual void onInfoMessage( unsigned int aMsgCode, const wchar_t *apMessage) = 0;
	};

	struct SYNCMAN_NO_VTABLE syncIRecordManagerCallback
	{
		// � ������� �������� ���������� � ������� (auID - ������������� �������, ���������� � GetRecords())
		virtual void onReceivedRecordsInfo( sync::syncCountRecordInfo& totalCountRec, unsigned int auRecordCount, syncRecordInfo** appRecordsInfo, unsigned int auID, int aiErrorCode) = 0;
	};


	struct SYNCMAN_NO_VTABLE syncIEventManagerCallback
	{
		// �������� ������ ��� ������ ����
		virtual void onRecordError(int aiErrorCode) = 0;
		// ������ �� ����� ���� ������ ��� ��� ������ ������ ��� �� ������
		virtual void onRecordStartFailed(int iErrorCode, const wchar_t* aAdditionalInfo) = 0;
		// ������ ����������� � ������ � ���������
		virtual void onRecordConnected(unsigned int aiID, const wchar_t* aAuthorOrAssistant, unsigned char isAuthor, unsigned int aDuration, const wchar_t* aLocationName) = 0;
		// ������ ���������� �� ������ � ���������
		virtual void onRecordDisconnected(unsigned int auRecordID,  bool isAuthor, bool isNeedToAsk) =0;
		// ������ ������� ������� � ��������� �����������
		virtual void onRecordStoped(unsigned int auRecordID, unsigned int auDuration = 0, syncRecordInfo* appRecordInfo = NULL) =0;
		// �������� ������� � ������������� � �������
		virtual void onEventEntered(unsigned int auiEventId, const wchar_t* awcLocationID, unsigned int auiEventRealityId, unsigned int auiPlayingRecordId, unsigned int auiBaseRecordId, unsigned int auiEventRecordId, bool abIsEventRecording, const wchar_t* aJSONDescription) = 0;
		// �������� ������� �� ������������ �� �������
		virtual void onEventLeaved(unsigned int auiPlayingSeanceID,unsigned int auiPlayingRecordID) = 0;
		// �������� ������� �� ��������� � �������
		virtual void onEventUpdateInfo(unsigned int auiEventId, const wchar_t* awcLocationID, unsigned int auiEventRealityId, unsigned int auiPlayingRecordId, const wchar_t* aJSONDescription) = 0;
		// �������� ������ ����� ������������� � ������ ������� (��������� ���� ����� ������� �� �������)
		virtual void onEventUserRightsInfo( unsigned int aEventID, const wchar_t *aEventUserRights) = 0;		
		// ��������  ����� ������������
		virtual void onEventUserRightUpdate( unsigned int aEventID, const wchar_t *aEventUserRights) = 0;
		// �������� � ���� (������������)
		virtual void onEventUserBanUpdate( const wchar_t *aEventName, const wchar_t *aEventUrl, char aBan) = 0;
		// �������� ������� � ����� � �������
		virtual void onLocationEntered( const wchar_t* awcLocationID, const wchar_t* aJSONDescription) = 0;
	};

	struct SYNCMAN_NO_VTABLE syncIRecordManagerCallback2
	{
		// � ������� �������� ���������� � ������� (auID - ������������� �������, ���������� � GetRecords())
		virtual void onReceivedRecordsExtendedInfo( unsigned int totalCount, unsigned int auRecordCount, syncRecordExtendedInfo** appRecordsInfo, unsigned int auID, int aiErrorCode) = 0;
	};

	struct SYNCMAN_NO_VTABLE syncISyncManager
	{
		// ���������� � ������� �������������
		virtual void Connect() = 0;
		// ��������� �� ������� �������������
		virtual void Disconnect() = 0;
		// ������� �������� �������������
		virtual void Release() = 0;

		// �������� ��� ������ ���������
		virtual unsigned int GetHashObj( const syncObjectState& object_all_props, bool abLogPacket) = 0;
		// �������� �� ������ ������ �� ��������� ���������������� ������� �������
		virtual void SendObjectState(const syncObjectState& object_props)=0;
		// �������� ��������� ��������� ��������� ����������������� �������
		virtual bool NextObjectState(syncObjectState* &object_props)=0;
		// �������� ��������� ��������� ��������� ��� ����������� ����������������� �������
		virtual bool NextObjectState(ZONE_OBJECT_INFO* aZoneObjectInfo, syncObjectState* &object_props)=0;
		// ���������� ������ �� ���������� ���������������� �������
		virtual void FreeObjectState(syncObjectState* &object_props)=0;
		// ���������� ������ �� ���������� ���������������� �������
		virtual void FreeCachedObjectState(syncObjectState* &object_props)=0;
		// �������� �������� ������� � ������� �� ��������
		virtual bool PutMapObject( const ZONE_OBJECT_INFO& aZoneObjectInfo)=0;
		// ���������� ������� ������� ��� ���������� ������� �������� �� �����������
		virtual void SetAvatarPosition(const float x, const float y, char* locationName) = 0;
		// ������� �������, � ������� currentRealityID == auRealityID
		virtual void DeleteObjectsExceptReality( unsigned int auRealityID)=0;
		// ����� �������� ������� �� �������
		virtual bool NextMapObject( ZONE_OBJECT_INFO*& aZoneObjectInfo)=0;
		// ���������� ������ �������� �������
		virtual void FreeMapObject( ZONE_OBJECT_INFO*& aZoneObjectInfo)=0;
		// ��� �� �������� ������ � ������� ���������� �������
		virtual bool IsNewObjectWasAdded() = 0;
		// ����� ������� �������� ��� ���
		virtual bool IsObjectQueueEmpty() = 0;
		// ���������� ���������� �������� �������� �� ������� �� ��������
		virtual int GetNearestMapObjectCount() const = 0;

		// ������� ������ ����
		//virtual void ListenZones( unsigned int aRealityID, unsigned int aZoneCount, unsigned long* aZones) = 0;
		// ������� ��������� �������
		virtual void SendObject( unsigned char* aData, unsigned int aDataSize) = 0;
		// ������� ��������� �������
		virtual void QueryObject( unsigned int anObjectID, unsigned int aBornRealityID,unsigned int aRealityID, long long aCreatingTime, bool checkFrame=true) = 0;
		// �������� ������ ��������� �������, ������� ����� �� ������� ���������� ���������
		virtual void AddQueryLatentedObject( unsigned int anObjectID, unsigned int aBornRealityID, unsigned int aCurrentRealityID, unsigned int auiHash) = 0;
		// ������� ��������� ��������, ������� ����� �� ������� ���������� ���������
		virtual void QueryLatentedObjects() = 0;
		// ������� ���������������� ���������
		virtual void SendMessage( unsigned int anObjectID, unsigned int aBornRealityID, unsigned char* aData, unsigned short aDataSize) = 0;

		// ������� ����� ��������������� ���� �� URL
		virtual omsresult createSeanceByURL(syncSeanceInfo& aSeanceInfo, unsigned int auIDSeanceRmmlObject);
		// �������� URL ��� ������������ �� ������ ��� ���������
		virtual omsresult PreciseURL(const wchar_t* apwcURL) = 0;
		// ����� ������
		virtual omsresult StartRecord(int aiRecordMode,  const wchar_t* apwcRecordName, int& aiErrorCode) = 0;
		// �������� ������
		virtual omsresult StopRecord( unsigned int& aRecordID, int& aiErrorCode, syncRecordInfo** aRecordInfo) = 0;
		// ���������� ���������, ����������� �������� ������ �� SyncManager-�
		virtual bool SetDefaultCallbacks(syncISyncManCallbacks* apCallbacks, syncIRecordManagerCallback* apRMCallback, sync::syncIEventManagerCallback* apEventManagerCallbacks)=0;
		// ������ �������� ������
		// virtual omsresult SetRecordName( unsigned int aRecordID, const wchar_t* aRecName) = 0;
		// ������ �������� ������
		// virtual omsresult SetRecordDesc( unsigned int aRecordID, const wchar_t* aRecDesc) = 0;
		// �������� �� ������� ���������� � ������ 
		virtual omsresult UpdateRecordInfo(syncRecordInfo& aRecordInfo) = 0;
		// �������� ����� ������� (auID - ����� ������������� ������ �������, ������� ���� �������� � callback)
		virtual omsresult GetRecords(unsigned int auID, int aiBegin, int aiCount, const wchar_t* apwcFilter, const wchar_t* apwcOrderBy, sync::syncIRecordManagerCallback * pCallback) = 0;
		// �������� ����� ������� ��������������� �� ���������� ������ ����
		virtual omsresult GetSeances(unsigned int auID, unsigned int auSeancesObjectID) = 0;
		// ������ ��������������� ������ (�������� ������ �� ������)
		virtual omsresult Play(unsigned int auID) = 0;
		// ����������� ��������������� ������ (�������� ������ �� ������)
		virtual omsresult Seek( unsigned long seekPos) = 0;
		// ���������� ����� ������������, ������������ ���������������� ������
		virtual omsresult SetPilotLogin( const wchar_t* alpcPilotLogin, unsigned int auCurrentEventID) = 0;
		// ���������� ������ (�������� ������ �� ������)
		virtual omsresult Save( const wchar_t* apwcAuthor, const wchar_t* apwcCreationTime, const wchar_t* apwcName, const wchar_t* apwcDescription) = 0;
		// �������� ���������� � ������
		virtual omsresult QueryFullSeanceInfo(unsigned int auIDSeanceRmmlObject, unsigned int auID) = 0;
		// ������� ������ �������� (�������� ������ �� ������)
		virtual omsresult DeleteRecord(unsigned int auID) = 0;
		// ������� ������ � ������� (�������� ������ �� ������)
		virtual omsresult DeleteRecordIntoRecycleBin(unsigned int auID) = 0;
		// ������� ������ (�������� ������ �� ������)
		virtual omsresult DeleteRecords( unsigned int *pRecIDs, unsigned int count) = 0;
		// �������� ������ ������������� �� �������
		virtual omsresult GetUserList( unsigned int realityID) = 0;
		// ���������� ��������������� ������
		virtual omsresult Stop() = 0;
		// ������������� ��������������� ������
		virtual omsresult Pause() = 0;
		// ���������� ������ �� ������
		virtual omsresult Rewind() = 0;
		// ������� ������
		virtual omsresult DeleteObject(unsigned int auID, unsigned int auRealityID) = 0;
		// �������� ������ �������
		virtual omsresult GetObjectStatus(unsigned int auObjectID, unsigned int auBornRealityID) = 0;
		// ������ �����������
		virtual omsresult CloseClient() = 0;
		
		// ! jedi ��������������� ��� ��� �� ������� ��������� ������� ���� ����� �������� ������������� �������
		// ������ ������� �������
		//virtual omsresult AvatarChangeLocation(const wchar_t* apwcLocationName,  unsigned int auiObjectID, unsigned int auiBornRealityID) = 0;
		
		// ��������� ��������� ���� ����������� �� ���� ��������� �� ������� �������������, ������� ����� ���� � ���������� ��������
		virtual void ProcessSyncMessages() = 0;
		virtual omsresult GetEventRecords(unsigned int auID, int aiBegin, int aiCount, const wchar_t* apwcFilter, const wchar_t* apwcOrderBy, sync::syncIRecordManagerCallback2 *pCallback) = 0;

		// ���� �� ����������� ������ ������� ������� �������� � ������� ��������,
		// ����� ProcessSyncMessages() �� ����������
		virtual void SetStateToAnalysisObjectsList(bool state) = 0;
		virtual bool GetStateToAnalysisObjectsList() = 0;

		// �������� �� ������ � ���������� �������
		// ��������� �������� ������
		virtual void RecordEditorCut(unsigned int startPos, unsigned int endPos) = 0;
		// ������� �������� ������
		virtual void RecordEditorPaste(unsigned int pastePos) = 0;
		// ����������� �������� ������
		virtual void RecordEditorCopy(unsigned int startPos, unsigned int endPos) = 0;
		// �������� �������� ������
		virtual void RecordEditorDelete(unsigned int startPos, unsigned int endPos) = 0;
		// ������ �������� ��������� ������� (�������� ������ �� ������)	
		virtual void RecordEditorUndo() = 0;
		// ������ �������� ��������� ������� (�������� ������ �� ������)	
		virtual void RecordEditorRedo() = 0;
		// �������� ������ ��� �������������� (�������� ������ �� ������)
		virtual void RecordEditorOpen(unsigned int aRecordID) = 0;
		// ���������� ������ �� ��������� ������� (�������� ������ �� ������)
		virtual void RecordEditorSave() = 0;
		// ���������� ������ ��� ����� ������ �� ��������� �������  (�������� ������ �� ������)
		virtual void RecordEditorSaveAs(const wchar_t* apwcName, const wchar_t* apwcDescription, bool abOverwrite, bool abPublicate) = 0;
		// ������ ��������������� ������
		virtual void RecordEditorPlay() = 0;
		// �������������� ��������������� ������
		virtual void RecordEditorPause() = 0;
		// ���������� �������������� ������ �� ������
		virtual void RecordEditorRewind() = 0;
		// ���������� ��������������� ������
		virtual void RecordEditorSeek(unsigned int aSeekPos) = 0;

		// ������ �������� ��������� ������� (�������� ������ �� ������)	
		virtual void SetLogSyncMessage(bool bVal) = 0;

		// ������� ������ ��� �������� ���������. ��������� �������
		virtual cs::iclientsession* GetServerSession() = 0;
		// ��� ������ ��� ������ �����������
		virtual void OnLogout( int iCode) = 0;
		virtual void OnLocationsListChanged() = 0;
		
		virtual omsresult GetEventUserRight( unsigned int eventID) = 0;
		virtual omsresult SetEventUserRights( unsigned int eventID, wchar_t *apwcDescr) = 0;
		virtual omsresult SetEventUserDefaultRights( unsigned int eventID, unsigned int aRight) = 0;
		virtual omsresult SetEventUserBan( unsigned int eventID, wchar_t *apwcDescr) = 0;

		// ���������� �� ������ ����� �������������� ������������� ����� ������� �����
		// � ������ ������������ ��������� ��������� ������ � �������� URL ��� ������������
		virtual void ConnectToWorld( const wchar_t *apwcRequestedUrl, const syncObjectState* mpcAvatarState)=0;
		// ���������/��������� ������������ �������� ������������ ������� � ������������ ����������
		virtual bool SetLocationEnteringPermissionForUser( const wchar_t* apwcUserLogin, const wchar_t* apwcLocationID, unsigned int auRealityID, bool abAllow) = 0;
	};

	typedef syncISyncManager ISyncManager;

	SYNCMAN_API omsresult CreateSyncManager( oms::omsContext* aContext);
	SYNCMAN_API void DestroySyncManager( oms::omsContext* aContext);
}
#endif