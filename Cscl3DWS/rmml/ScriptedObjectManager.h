#pragma once

#include <map>
#include <vector>

namespace rmml {

class mlRMMLElement;
typedef std::vector<mlRMMLElement*> ObjectsVector;

/*
CScriptedObjectManager
  - ������ ������ �� ��������� �������,
  - ������������� �������� ������ �������� �� �� ���������������,
  - ������������� �������� �������� ��������.

���������� ������ ���� - ��� 3D ������ ����, � �������� ����� ���������� �����. 
�������������, ��� ���� ��������� rmml-������.
�� �������� �������� ��������� ������� ���������� ������� ����������� �� ��� ���������:
  - ������������� ������ ����� ���������� -- ��� �������, ����� ��������;
  - �������������� �� ����������� -- ���� ��� ������ ������.
��� �������� ������ ��������� ����� ���� ��������, ������������� � ������.
� ������ � ��������������� �� ����������� ��������� (���������) ��� �������������� ������� � ����� 
���������� ����� ��������� ��� ����� ������ ������� - ���������� � ��������������� � 
���������� ��������������� (���������� ������ � ����� ���������������� ������ ������������).
��� �������� ������� �� ������ ��������� ������ ������ ����� ������ ��������������� ��
����������� ������� ����� ���� ����� ����.

����� �������, ��� ������� ����� ����������, ����� �������������� �� ����������� �������� 
(��������), ����� ���� ���������������� ����� ��������������� - objectID.
��� ������������� �������������� �� ����������� �������� (��������) ���������� ���
�������������� - objectID � bornRealityID

������ �� �����, �������� �������� � ���� ����� (map):
  - �� ����� objectID,
  - �� �������� ����� - objectID, bornRealityID
*/

class CScriptedObjectManager
{
public:
	CScriptedObjectManager(void);
	virtual ~CScriptedObjectManager(void);

	void RegisterObject( mlRMMLElement* apMLEl);
	void ChangeObjectRealityAndBornReality( mlRMMLElement* apMLEl, unsigned int auRealityID, unsigned int auBornRealityID);
	void UnregisterObject( mlRMMLElement* apMLEl);
	mlRMMLElement* GetObject(unsigned int auObjID, unsigned int auRealityID, bool aAccurately = true);
	mlRMMLElement* GetOneRealityObject(unsigned int auObjID);
	void GetObjects( ObjectsVector& anObjects);
	// ���������� ����� ��������. ����������� #84
	void GetRealAvatars( ObjectsVector& anObjects);
	// ���������� ���������� ��������, ������� ����� �������� ��� ������ �� ������
	void GetRecordedAvatars( ObjectsVector& anObjects);
	// ���������� �������, �� ���������� �������
	void GetNoAvatarSyncObjects( ObjectsVector& anObjects);

private:
	void JoinScriptedObject( std::string& aJoinedObject);

	// ����� ���������� ��������
	typedef MP_MAP<unsigned __int64, mlRMMLElement*> CMovableRealityObjectMap;
	CMovableRealityObjectMap movableRealityObjects;

	// ����� ���������������� ��������, �������� �� ������� �����������
	typedef MP_MAP<unsigned int, mlRMMLElement*> COneRealityObjectMap;
	COneRealityObjectMap oneRealityObjects;

	// ��������� ��������������� �������
	struct CObjectIdentifiers 
	{
		unsigned int objectID;
		unsigned int bornRealityID;

		CObjectIdentifiers() {}
		CObjectIdentifiers( unsigned int anObjectID, unsigned int aBornRealityID) :
			objectID( anObjectID), bornRealityID( aBornRealityID) 
		{
		}
		bool operator==( const CObjectIdentifiers& aRight)
		{
			if (objectID != aRight.objectID)
				return false;
			if (bornRealityID != aRight.bornRealityID)
				return false;
			return true;
		}
		bool operator!=( const CObjectIdentifiers& aRight)
		{
			return !(*this == aRight);
		}
	};

	// ����� ���� ������������������ �������� - ��������� ������� �������������� ��� �������������,
	// ������� ����, ����� ������ ��� ���������������
	typedef MP_MAP< mlRMMLElement*, CObjectIdentifiers> CAllObjectMap;
	CAllObjectMap allObjectMap;
};

}