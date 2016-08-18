#pragma once

#include "mlRMML.h"

namespace rmml {

class mlCachedSynchProp
{
public:
	std::string msName;
	mlSynchData moValue;
public:
	mlCachedSynchProp(const char* apcName, mlSynchData& oData);
	mlCachedSynchProp(const mlCachedSynchProp& src){
		*this = src;
	}
	void SetValue(mlSynchData& oData);
	const mlCachedSynchProp& operator=(const mlCachedSynchProp& src){
		msName = src.msName;
		moValue = src.moValue;
		return *this;
	}
};

// ������ ������������ ������� ��� ���������� �������
class mlObjectCachedSynchProps : public std::vector<mlCachedSynchProp>
{
public:
	unsigned int muID; // ������������� �������
	unsigned int muBornRealityID; // ������������� ���������� ��������
	mlSysSynchProps mSysProps; // ���������������� ��������� ��������
	std::string msSubobjPath; // ���� � ���������� (������ ��� � ��� ��� ������������ ��������������)
	typedef std::vector<mlObjectCachedSynchProps> VecSubobjProps;
	VecSubobjProps* mpvSOSPs; // ������ ���������������� ������� �����������
public:
	void AddProperty(const char* apcName, mlSynchData& oData);
	mlObjectCachedSynchProps():muID(0), muBornRealityID(0), mpvSOSPs(NULL){}
	~mlObjectCachedSynchProps(){
		if(mpvSOSPs != NULL)
			delete mpvSOSPs;
	}
	mlObjectCachedSynchProps(const mlObjectCachedSynchProps &src):mpvSOSPs(NULL){ *this = src;}
	mlObjectCachedSynchProps &operator=(const mlObjectCachedSynchProps &src)
	{
		clear();
		insert(begin(),src.begin(),src.end());
		muID = src.muID;
		muBornRealityID = src.muBornRealityID;
		mSysProps = src.mSysProps;
		msSubobjPath = src.msSubobjPath;
		if(mpvSOSPs != NULL)
			delete mpvSOSPs;
		if(src.mpvSOSPs == NULL){
			mpvSOSPs = NULL;
		}else{
			mpvSOSPs = new VecSubobjProps;
			mpvSOSPs->insert(mpvSOSPs->begin(), src.mpvSOSPs->begin(), src.mpvSOSPs->end());
		}
		return *this;
	}
	
};

enum EAddToCacheResult
{
	CR_ERROR, CR_NOT_FULL, CR_DISCONNECTED, CR_ADDED_NEW, CR_UPDATED
};

// ��� ��� ���������������� �������, 
// ��� ������� ��� ��� ��������
// (?? ���� ����������� ������� ���, ����� ����� ���� �� ������� �� ��� �� �������
//  pMLEl->SynchM3Props(*vi, oEventData))
class mlSynchPropCache : public std::vector<mlObjectCachedSynchProps>
{
	mlSynchedObjProps moSynchedObjProps; // ��� GetProperties
	VecSynchedObjProps moSubobjProps; // ��� GetProperties
	mlSynchData moData; // ��� GetProperties
	JSContext* mcx;
public:
	mlSynchPropCache():
	  MP_VECTOR_INIT(moSubobjProps)
	{ mcx = NULL; }
	void SetJSContext(JSContext* cx){ mcx = cx; }
	// �������� ���������������� �������� ������� � ���
	// (�������� �������� ��� �������������� �������)
	// (!!! �������� !!!)
	void AddProperties(mlSynchedObjProps& aSynchedObjProps, mlSynchData& oData);

	// �������� ���������������� �������� ������� � ��� (��� ������ ������� �� 2� �������)
	// (�������� �������� ��� �������������� �������)
	EAddToCacheResult AddProperties2(mlSynchData& oData);
	// �������� ���������������� �������� � ������� ��� ������������� ����
	bool GetProperties(unsigned int auObjectID, unsigned int auBornRealityID, mlSynchedObjProps* &aSynchedObjProps, mlSynchData* &oData, /*mlSysSynchProps* &apSysProps, */ VecSynchedObjProps* &aSubobjProps);
private:
	std::vector<mlObjectCachedSynchProps>::iterator FindSynchPropsObject(
		unsigned int auObjectID, unsigned int auBornRealityID,
		mlObjectCachedSynchProps*& aSynchProps);
};

}
