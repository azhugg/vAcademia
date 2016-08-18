#pragma once

#include "../CommonRenderManagerHeader.h"

#include "MapManager.h"
#include "MapManagerZone.h"
#include "MapManagerStructures2.h"
#include "MapServerSession.h"
#include "MapServerDataBlock.h"
#include "NatureSettings.h"
#include "iclientsession.h"
#include "oms_context.h"
#include "LogWriter.h"
#include "GetObjectsEndTask.h"

//////////////////////////////////////////////////////////////////////////
class CMapManagerServer
{
public:
	CMapManagerServer(oms::omsContext* context);
	~CMapManagerServer(void);

public:
	// ������ ������� ��������� ������ �� �������
	//void get_object_info_start(CComString& name, unsigned int aRealityID);
	// ������ ������� ��������� ������ �� �������� � ����
	void get_zones_objects_start(unsigned int aPrevRealityID, unsigned int aRealityID
								, ZoneID aCurrentZone, const std::vector<ZoneID>& aFullQueryZones);
	// ��������� ������� ��������� ������ �� �������� � ����� (���������� �� ������� ������ � �������)
	void	get_zones_objects_end(bool isStaticObjectsOnly, CCommonPacketIn* pMapDataAsynchObject, unsigned int aRealityID, std::vector<ZONE_OBJECT_INFO *>& zonesInfo, std::vector<ZONE_OBJECT_INFO *>& syncZonesInfo, bool abFilterPacketsByReality = true, IObjectReceivedHandler* objectHandler = NULL);
	bool	getObjectsEnd(bool isStaticObjectsOnly, CCommonPacketIn* pMapDataAsynchObject, unsigned int aRealityID, std::vector<ZONE_OBJECT_INFO *>& zonesInfo, bool abFilterPacketsByReality = true, IObjectReceivedHandler* objectHandler = NULL);
	// ����������  ������ �������� ����� getObjectsEnd  � ��� ������: zonesInfo � syncZonesInfo
	void	sort_zones_objects(std::vector<ZONE_OBJECT_INFO *> _zonesInfo, std::vector<ZONE_OBJECT_INFO *>& zonesInfo, std::vector<ZONE_OBJECT_INFO *>& syncZonesInfo, bool isFirstTimeSort = true);

	// ������ ������� ��������� ������ �� �������
	void get_avatar_start(const wchar_t* apwcURL);

	void create_location(unsigned int auiPointCount, CVector3D points[], const wchar_t* apwcLocationName);
	void delete_location();
	void create_communication_area(unsigned int auiPointCount, CVector3D points[], const wchar_t* apwcLocationName);
	void delete_communication_area( const wchar_t* apwcLocationID);

	// ������ ������� ��������� ������ � �������� �������� 
	void get_objects_start(cm::cmObjectIDs* anObjects, unsigned int anObjectsCount, unsigned int aRealityID);

	void	lock_data_handlers();
	void	unlock_data_handlers();

	// ������� ������
	void create_object(SObjectInfoForUpdate& aObjectInfo, unsigned int auRmmlQueryID);

	// ���������� ����������
	void set_reality( unsigned int aReality);

	// ������� ������ �� ��������� ���������� ����������
	//void query_reality_info(unsigned int realityID);

	void update();

private:
	void Unpack(CCommonPacketIn* apQueryIn, char** unpackBuffer, unsigned long* unpackedSize);
	void FreeUnpackBuffer(char* unpackBuffer);
	void get_actual_zones_info(CGetObjectsEndTask* task, std::vector<ZONE_OBJECT_INFO *>& _zonesInfo, bool isStaticObjectsOnly);

	oms::omsContext*    m_context;

	int								packageID;
	cm::cmICommunicationManager*	pComManager;

	CRITICAL_SECTION				m_cs;

	MP_VECTOR<CGetObjectsEndTask*> m_getObjectsEndTasks;
	bool bOldServer;
};
