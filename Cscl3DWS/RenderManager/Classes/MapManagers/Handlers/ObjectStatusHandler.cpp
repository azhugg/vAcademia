
#include "StdAfx.h"
#include "RMContext.h"
#include "GetObjectStatusIn.h"
#include "ObjectStatusHandler.h"

using namespace MapManagerProtocol;

bool CObjectStatusHandler::Handle(CCommonPacketIn* apQueryIn, cm::cmIMapManCallbacks* mpCallbacks)
{
	ATLASSERT( FALSE);
	CGetObjectStatusIn packetIn((BYTE*)apQueryIn->GetData(), apQueryIn->GetDataSize());
	if (!packetIn.Analyse())
	{
		ATLASSERT( FALSE);
	}
	int iStatus = packetIn.GetStatus();
	// ���� ������ �������
	if(iStatus == 1){
		// ?? ���������� �� zoneID, ������ �� �� � ������ ���, ������� ����� ������
		// ���� �� ������, ������ � �� ���������� (-2 - ������ ��� ���� ��������� ��� � ������ ����������)
		unsigned int uiZoneID = packetIn.GetZoneID();
		if (gRM->mapMan->IsActiveZone(ZoneID(uiZoneID))){
			iStatus = 3; // 3 - ������ ���������� �� ������� � ��������� � ���� ���������, �� ��� �� ��������
			// ������� ����� ��� � ����������, �� ���� ������ ��� ����� 3, 
			// ������ ��� SceneManager �� ������ ��������, �� �������� �� ��� ������ � �����
		}else
			iStatus = -2; // -2 - ������ ��� ���� ��������� ��� � ������ ����������, ������� �� ����� �������� � ������
	}

	/*unsigned int uiUserData = 0;
	int iUserDataSize = 0;
	unsigned int* pUserData = (unsigned int*)apQueryIn->GetUserData(iUserDataSize);
	if(iUserDataSize >= 4){
		uiUserData = *pUserData;
	}
*/
	if (mpCallbacks != NULL)
	{
		ATLASSERT( FALSE);
		//mpCallbacks->onReceivedObjectStatus(packetIn.GetObjectID(), packetIn.GetBornRealityID(), iStatus, (void*)uiUserData);
	}
	return true;
}