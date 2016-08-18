#pragma once

#include "../CommonRenderManagerHeader.h"

#include "RealityInfo.h"
#include "3DObject.h"
#include "LoadingQueueInfo.h"
#include "OculusRift.h"
#include "Kinect.h"

/************************************************************************/
/* �������� ������ � �������� �����:
1. ��� ��������� ������������
*/
/************************************************************************/


/************************************************************************/
/* ����� �������� �� ������������� ���������� � ��������� ������ ����.
����������� ����������� ����������� � ������������ ��������, ��������� ��
������ � ����������� � ������ �������������.

������� ������ ���������:
1. ��� ��������� ������������ � ���� �� ��������� �����, ����� ����������� ���������� � 
����� ��������� � �������. � ����� ��������� �������, � ������� ��������� ������������ � 
��� �������� ��������, ������� �� ���� ����� �� ���������� �����.
���������� � �������� �������� � ����:
- ����� ��������
- �� ������� �������:
--��� �������
--��� ������
--���� � ��� ��������
--���������� � ������� �������

2. ������������ ����� ������ � �������, ���������� �� ��������� ���������� (��������� ��������� ��������, ���)

3. ��� ����������� ����� �������� ��� ��� ��������� ������ ���� ��� ����� ������� ��������� ����� �������, 
	������� ���������� �� ��������.

PS: ��������� �������� �������������� ��� ������ OctTree, ������� ��������� � �����
/************************************************************************/

/************************************************************************/
/* ��� ������ � �������� ������ �������������� ������
/************************************************************************/

/************************************************************************/
/* ����� ����� ��������� ��������� �������. ��� ����������� �������, ������� ����� 
���������, ������������ ������ �� ���, ����� ���� �� ������ ������� ��������������� 
��� ���������, � ����������� �� ���������� � ���������, �� ������ �������� �������.
/************************************************************************/


#include "ClassScriptsLoader.h"
#include "PrimObjectsLoader.h"
#include "MapManagerZone.h"
#include "CreateObjectZone.h"
#include "NatureSettings.h"
#include "nrmObject.h"
#include "oms_context.h"
#include "LogWriter.h"
#include "MapManagerPriority.h"
#include "Scene.h"
#include "TempPacketIn.h"
#include <VoipManClient.h>
#include "CrossingLine.h"
#include "IPriorityUpdater.h"


using namespace cm;
class	CMapManagerServer;

namespace Protocol
{
	class	CCommonPacketIn;
};
using namespace Protocol;

typedef	MP_VECTOR<CMapManagerZone*>			 CVectorZones;
typedef MP_MAP<ZONE_OBJECT_ID, ZoneIdentifier> CMapFixDBErrors;
typedef MP_MAP<unsigned int, CRealityInfo>	 CRealitiesMap;

typedef	struct _ZonesDescQueryResult
{
	MP_VECTOR<ZoneID>	zonesIDs;
} ZonesDescQueryResult;

typedef struct _SCRIPTED_OBJECT_DESC
	{
		C3DObject*	obj3d;
		mlMedia*	ismobject;
		_SCRIPTED_OBJECT_DESC()
		{
			obj3d = NULL;
			ismobject = NULL;
		}
	} SCRIPTED_OBJECT_DESC;

// ���������� �� ������� ��� ���������� ������ �� ������� �� �������
// ��� ��� �������� ������ �������
struct SObjectInfoForUpdate
{
	MP_STRING name;	// ��������� ������������� ������� (���� "", �� ����� �������� ��� ���� ��� ������������� �������������)
	MP_STRING res;		// src ������� ��� ������
	// ���������
	float x;
	float y;
	float z;
	// bounds-� (������������ ������)
	float bx;
	float by; 
	float bz; 
	// �������
	float rx;
	float ry;
	float rz;
	float rw;
	// �������
	float scalex;
	float scaley;
	float scalez; 
	// ������������� ����
	unsigned int zoneID;
	MP_STRING className;
	MP_WSTRING userProps;
	// ���: 0 - �� ���������, 1 - object, 2 - group
	int type;
	// ������������� �������
	unsigned int objectID;
	unsigned int bornRealityID;
	// ??
	int userData;
	// "�������": ?? 4 - ������
	int level;
	// ������������� �������
	MP_STRING location;

	SObjectInfoForUpdate();
};

#define PMO_IS_NULL(PMO) ((((unsigned)PMO)&~3)==0)

//////////////////////////////////////////////////////////////////////////
class CMapManager : public nrmObject,
					public cm::cmIMapManager,
					public CMapManagerPriority,
					public IChangesListener,
					public IPriorityUpdater
{
public:
	CMapManager(oms::omsContext* context, CNRManager* manager);
	~CMapManager(void);

	// ���������� iscenemanager
public:
	bool	IsPointInActiveZones(const float x, const float y);
	
	void	SetMainObjectCoords(float x, float y, float z);
	CVector3D GetMainObjectCoords();

	void StartTrackVisibleScreensInCurrentLocation();
	std::map<std::wstring, int> GetVisibleScreensInCurrentLocation();
	void EndTrackVisibleScreensInCurrentLocation();

	bool	GetKinectPoints(float* points);

	void	StartRender3D();
	void	StartPhysics();
	bool	IsRender3DNeeded()const;
	bool	IsPhysicsNeeded()const;
	void	SetIsRender3DNeeded( bool abVal);

	int		InitOculusRift(bool isNeedToEnable);
	void	GetOculusRiftDesktopParams(int& wResolution, int& hResolution, int& displayX, int& displayY);

	bool	InitKinect(bool isNeedToEnable);
	bool	InitGamepad(bool isNeedToEnable) override;

	void	CheckForLoadedMapInfo();
	void	CheckForLostedLods();

	void	UpdateObjectsInfo(std::vector<ZONE_OBJECT_INFO *>& info);
	void	UpdateSyncObjects(std::vector<ZONE_OBJECT_INFO *>& info);

	void	OnChanged(int eventID);

	void	OnZoneEntered(ZoneID zoneID);

	bool	IsInZone()const;	

	virtual void OnLogout( int iCode);

	mlMedia* GetScene3DPtr();
	bool IsScene3DReady();

public:
	void OnRealityDesc(CCommonPacketIn* apQueryIn);
	void OnDeleteObject(CCommonPacketIn* apQueryIn);
	void OnLocationCreated(CCommonPacketIn* apQueryIn);

private:
	void NotifyRealityLoaded( CRealityInfo& realityInfo);

	void TrackVisibleScreensInCurrentLocation(int firstObject);

	CRect2D GetObjectRect(C3DObject* obj3d);
	CRect2D GetZonesRect();

public:
	// �������� ������ �����
	void	Update(DWORD time);							
	// ������������� �����, � ������� ����� ��������� �������
	void	SetScene(CScene* scene);								
	C3DObject*	CreateStaticObject(ZONE_OBJECT_INFO* info);
	void	ReloadZonesInfo();
	
	SCRIPTED_OBJECT_DESC CreateScriptedObject(ZONE_OBJECT_INFO* info);
	nrmPlugin* CreatePlugin();
	void ObjectLeaveZone(const ZONE_OBJECT_INFO& info, CMapManagerZone* zone);
	CScene* GetScene();
	C3DObject* GetAvatar();

	void	ExtractIncludes( IAsynchResource* asynch, std::vector<std::string>&	includeFilesVector);

	CVector3D *GetLocationVector3D(rmml::ml3DPosition* leftTopPoint,  rmml::ml3DPosition* rightBottomPoint, rmml::ml3DPosition* outsidePoint);
// ���������� cmIMapManager
public:
	// ���������� cmIMapManager
	// ��������� � ������� ������ ��� �������� ������� � ������� ���
	virtual oms::omsresult CreateMyAvatar(const wchar_t* apwcURL);
	// �������� ������ �������
	virtual  std::vector<rmml::ml3DBounds>  GetVisibleLocations();
	// �������� ������ ������� ������� (��� �������)
	virtual  std::vector<rmml::ml3DBounds>  GetVisibleCommunicationAreas();
	
	// ���������, ����� �� ���� ������� ������� �� ��������� �����������
	virtual int IsLocationCanBeCreated(rmml::ml3DPosition* aLeftPoint, rmml::ml3DPosition* aRightPoint );
	// ������������ ��������� �� �������
	virtual int HandleServerMessage( unsigned char aMessageID, unsigned char* aData, int aDataSize);
	// ��������� � ��� ������� ������� ��������
	virtual void DumpLoadingQueue();
	virtual oms::omsresult CreateObjects( cmObjectIDs* objects, unsigned int objectsCount);
	// ������� ��������� �������
	virtual oms::omsresult CreateLocation(rmml::ml3DPosition* leftTopPoint,  rmml::ml3DPosition* rightBottomPoint, rmml::ml3DPosition* outsidePoint, const wchar_t* apwcLocationName);	
	// ������� ��������� �������
	virtual oms::omsresult DeleteLocation();
	// 
	bool SetCallbacks(cm::cmIMapManCallbacks* callbacks);
	void HandleTeleportError( unsigned int auErrorCode, const wchar_t* apwcDescription, const wchar_t* apwcEventInfo);
	virtual int GetCurrentRealityID();
	cmRealityType GetCurrentRealityType();
	virtual int GetPrevRealityID();
	// ���������� URL �����, ��� ��������� ������
	virtual int GetAvatarLocationURL(wchar_t* url, int aiBufferSize);
	// ���������� ���������� �������
	virtual void GetAvatarPosition( float& aX, float& aY, float& aZ, char** aLocation = NULL);
	virtual void GetAvatarQuaternion( float& aX, float& aY, float& aZ, float& aW);

	// ���������� ������
	virtual void MoveAvatar( const cmTeleportInfo& teleportInfo);
	// ��������� �������� ������ ��������
	virtual bool IsAvatar(rmml::mlMedia* apObject);
	// ���������� ������, ������� ������� �������� �������
	void SetAvatar(rmml::mlMedia* object, const ZONE_OBJECT_INFO& objectInfo);
	//�������� ������ �� ������� �� �����
	virtual rmml::mlMedia* GetAvatarByName(std::string& name);
	virtual voip::IVoipCharacter* GetIVoipAvatarByName(const char* name);
	// ��������� MapServer-� ������ �� �������� ������� �� ���� ������
	bool Remove(rmml::mlMedia* object);
	// ��������� MapServer-� ������ �� �������� �������
	bool CreateObject(const rmml::mlObjectInfo4OnServerCreation* objectDesc, unsigned int auRmmlQueryID);
	// �������� �������� ����������
	bool GetRealityInfo( unsigned int realityID, cmIRealityInfo*& realityInfo);
	// 
	ZoneID GetCurrentZoneID();
	virtual unsigned long GetCurrentZoneId();
	//
	void ActivateZones();
	void ActivateZones(ZoneIdentifier aCurrentZone);
	// ���������� ���������� � �������� ��������� �����.
	// � �������� ��������� ����� �� ��� ���������� ��� ������.
	bool UpdateVoipConnection();

	// ������ ���� ������� ��������, ������� ����� ���������
	void ReadFilterFile();
	// ��������� � ������������ � ��������, ����� �� ��������� ������?
	bool AllowCreateByFilter(const char* objName, CMapObjectVector &translation);
	// ��������� � ������������ � Nap-��������, ����� �� ��������� ������?
	bool AllowCreateByNapFilter(const std::string& objName, const std::string& className,
										 const int level, const int type, const CMapObjectVector &translation);
	// ���������� �������� ���� �������
	const char* GetObjectLocationName(mlMedia* mlObj);
	std::string GetObjectCommunicationAreaName(mlMedia* mlObj);	
	void DeleteObject(mlMedia* rmmlObject);
	void RemoveLocation();
	void RemoveCommunicationArea();
	void DeleteTempLocations(const std::vector<unsigned int>& aExcludeObjects);
	void RemoveLocation(const char* objName);
	void RemoveCommunicationArea(const char* objName);
	bool IsObjectCreating(unsigned int objectID);
	// ��������� ���� � ������� �������
	int LoadClassFile( const char* className);
	// ��������� �������� ������� ��� ��� ���������
	EFillingObjectInfoResult FillObjectInfo( const ZONE_OBJECT_INFO& aZoneObjectInfo, rmml::mlObjectInfo4Creation& anObjectInfo);
	// ���������� ����� ����� � ��������
	void OnConnectLost();
	// ���������� �������������� ����� � ��������
	void OnConnectRestored();
	// �������� �������� ������� � ����� � ��������� ������������
	bool GetLocationByXYZ(float afX, float afY, float aFZ, rmml::ml3DLocation &aLocation);
	// �������� �������� ������� � ����� � ��������� ������������
	char* GetLocationNameByXYZ(float afX, float afY, float afZ);
	// �������� ��������� ������� � ���� ������ (��� �������� �� objects_map.params)
	bool GetLocationParams( const wchar_t* apwcLocationID, wchar_t* paramsBuffer, unsigned int auParamsBufferSize);
	// ��������� ��� �������, ������� ������ �������������� ��� �������� ����� � ����������.
	MapManagerError GetRealityGuardName(unsigned int auRealityID, wchar_t* apwcBuffer, unsigned int auBufferLength);

public:
	int		LoadClassFile(std::string& className);
	bool	LoadPrimObject(std::string& fileName, std::string& objName, IAsynchResource** asynchResource);
	void	ExecJS(const wchar_t* js, wchar_t** result = NULL);
	void	ExecJS(const char* js, wchar_t** result = NULL);
	bool	IsJSObjClassDefined(LPCTSTR className);

public:
	void	UpdateResourceLoadingPriority(IAsynchResource* asynchRes);

private:
	void	Clear();
	ZoneID	GetObjectZone(C3DObject* obj);
	bool	QueryZonesObjects( const std::vector<ZoneID>& aZonesIDs);
	void	GetNeighbourZonesIDs(ZoneID	zoneID, std::vector<ZoneID>& zones);
	void	ReloadWorld();
	void	MoveAvatarOnUpdate();
//	void	QueryRealityInfo( unsigned int realityID);
	void	UpdateLoadingPriority(CCamera3D* camera);
	bool	TeleportReality( cmTeleportInfo& aTeleportInfo);
	bool	TraceGroundLevelAtPoint( float ax, float ay, float* az);

	void	DeleteNonZoneObjects();	

	cmRealityType GetPrevRealityType();

public:
	ZoneID	GetObjectGlobalZone(C3DObject* obj);
	CMapManagerZone*	GetZone(ZoneID zoneID);
	bool	AddZoneIfNotExists(ZoneID zoneID);
	void	SetConnectedRealityID( unsigned int aConnectedRealityID);
	void	OnObjectsRecieved(unsigned int aPrevRealityID, unsigned int aCurrentRealityID, const std::vector<ZONE_OBJECT_INFO *>& aNewObjects);
	void	OnGroundLoaded(ZONE_OBJECT_INFO* info);
	void	UpdateActiveZones();
	void	ReloadWorldImpl();
	void	CollectSynchObjects( std::vector< mlMedia*>& aSynchObjects);

public:
	SLocationInfo *CreateLocationInfo( ZONE_OBJECT_INFO *info, std::string &nam);
	void AddLocationBounds( ZONE_OBJECT_INFO *info);
	void AddCommunicationAreaScales( ZONE_OBJECT_INFO *info);
	void CheckLocation();
	void CheckCoordinateXY();
	void OnLocationChanged();
	void OnCommunicationAreaChanged();
	void ClearLocationBounds();
	// �������� ���� �������� ������ �������, ��������� �� � ������� ����������
	void ClearLocationBoundsRNZ();
	void GetObjectsInLocation(std::vector<mlObjectInLocation >& objects);
	int SelectObjectsInLocation();
	unsigned int GetCurrentLocationName(wchar_t* aLocationName, unsigned int aNumberOfElements);

	void GetCommunicationAreaName( std::vector<SLocationInfo *> vCommunicationArea, std::string &asName);
	std::string GetCurrentCommunicationAreaName();
	const void GetSynchID(unsigned int &objID, unsigned int &bornRealityID);

	SLocationInfo* GetObjectLocation(C3DObject* obj);
	void GetObjectCommunicationArea(C3DObject* obj, std::vector<SLocationInfo *> &av_CommunicationArea);
	SLocationInfo* GetLocationByXYZ(float x, float y, float z);

	int IsCurrentCommunicationArea( SLocationInfo *apArea);
	void GetCommunicationAreaByXYZ(float x, float y, float z, std::vector<SLocationInfo *> &avCommunicationArea);
	SLocationInfo* GetLocationByID(const wchar_t* apwcLocationID);

private:
	bool GetCrossingLocationBounds( SLocationInfo *pLocInfo, CPoint &ptObj, CPoint &ptMyAvatar, CPoint &pt1, CPoint &ptRes);
	// �������� ����� ����������� ������� (ptP1, ptP2) � ��������� �������
	// ���������� ���������� ����������� � ���� ����������� � ptCP1 � ptCP2
	int GetCrossingsWithLocationBounds(const SLocationInfo *pLocInfo, const CPoint &ptP1, const CPoint &ptP2, CPoint &ptCP1, CPoint &ptCP2);

	void UpdateVisibilityDistance();

public:
	bool GetNewPositionObjectOnBoundLocation( const wchar_t *apwcLocationID, rmml::ml3DPosition &myAvatarPos, rmml::ml3DPosition &objPos, rmml::ml3DPosition &ptObjNew);
	void OnMemoryOverflow();

	void StartKinect();
	void StopKinect();
	void SetKinectMode(int iMode);
	// ---
	void SetMarkerType(int iMarker);
	void EnableFaceTracking();
	void DisableFaceTracking();
	// ---
	void GetKinectState(bool& abHeadFound, bool& abLeftHandFound,  bool& abLeftLegFound,  bool& abRightHandFound,  bool& abRightLegFound,  bool& abOneSkeletFound, bool& abMarkersNotFound);

private:
	void	DeletePendingObjects(bool isTimeLimited = true);
	void	WriteLog(LPCSTR alpcLog);

	bool repeatQueries;

private:
	oms::omsContext*    m_context;
	C3DObject*			m_mainObject;
	ZONE_OBJECT_INFO	m_mainObjectInfo;
	ZoneID				m_currentZoneID;
	CVectorZones		m_zones;
	CVectorZones		m_activeZones;
	CCreateObjectZone	m_objectZone;
	CMapManagerServer*	m_server;
	CScene*				m_scene3d;
	bool				m_acceptRegisterQueries;
	bool				m_groundLoadedMessageWasSend;
	cmTeleportInfo		m_teleportInfo;
	bool				m_needTeleport;
	MP_VECTOR<mlObjectInLocation> m_selectedObjects;
	CMapManagerZone*	m_commonZone;
	CLoadingQueueInfo	m_loadingQueueInfo;
	bool				m_bModuleLoaded;

	CVector3D			m_mainObjectCoords;

	MP_VECTOR<int>	m_objectsForRemove;
	MP_VECTOR<moMedia*> m_childrenForRemove;

	MP_VECTOR<IAsynchResource *> m_asynchResources;	

	DWORD m_updateProgressZonesCounter;	

	MP_VECTOR<SLocationInfo *> m_locationsInfo;
	MP_VECTOR<SLocationInfo *> m_communicationAreasInfo;
	SLocationInfo* m_currentLocation;
	MP_VECTOR<SLocationInfo *> m_vCurrentCommunicationArea;

	CRITICAL_SECTION cs;

	unsigned int m_createdAvatarID;

	cm::cmIMapManCallbacks* m_callbacks;

	MP_VECTOR<MP_STRING> m_filter;

	CClassScriptsLoader m_classScriptsLoader;
	CPrimObjectsLoader m_primObjectsLoader;
	CRealitiesMap       m_realitiesCache;
	unsigned int        m_currentRealityID;
	unsigned int        m_prevRealityID;
	unsigned int        m_teleportingRealityID;

	__int64		m_lastKinectHandleTime;
	__int64		m_lastUpdateTime;

	bool m_isInZone;
	bool m_isRender3DNeeded;
	bool m_isPhysicsNeeded;
	__int64 m_startRender3DTime;
	__int64 m_lastLogTimeOnUpdate;

	int m_sceneChangedCounter;
	bool bIsNotFly;

	int m_savedDistanceKoef;

	MP_VECTOR<C3DObject *> m_trackedScreens;
	MP_MAP<MP_WSTRING, int> m_lastVisibleScreensInCurrentLocation;
	unsigned int m_lastCheckedForTrackObjectCount;

public:
	typedef struct _ReceivedMessage
	{
		unsigned int		iMessageID;
		MapManagerProtocol::CTempPacketIn*	pCommonPacketIn;
	} ReceivedMessage;
	MP_VECTOR<ReceivedMessage>	m_receivedMessages;
	
	CVectorZones* GetActiveZones();
	bool IsActiveZone(ZoneID zoneID);

	ILogWriter *GetILogWriter()
	{
		return m_context->mpLogWriter;
	}

	COculusRift m_oculusRift;
	CKinect m_kinect;
};