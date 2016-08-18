
#include "mlRMML.h"

namespace rmml {

void mlCachedSynchProp::SetValue(mlSynchData& oData)
{
	moValue.clear();
	unsigned short usLength;
	oData >> usLength;
	moValue.put(oData.getDataInPos(), usLength);
}

mlCachedSynchProp::mlCachedSynchProp(const char* apcName, mlSynchData& oData)
{
	msName = apcName;
	SetValue(oData);
}

void mlObjectCachedSynchProps::AddProperty(const char* apcName, mlSynchData& oData)
{
	mlCachedSynchProp* pCSP = NULL;
	std::vector<mlCachedSynchProp>::iterator vi = begin();
	for(; vi != end(); vi++){
		mlCachedSynchProp& oCSP = *vi;
		if(isEqual(oCSP.msName.c_str(), apcName)){
			pCSP = &oCSP;
			break;
		}
	}
	// ���� �������� � ����� ������ ��� ����, 
	if(pCSP != NULL){
		// �� �������� ��� ��������
		pCSP->SetValue(oData);
	}else{
		// ����� �������� ����� ��������
		push_back(mlCachedSynchProp(apcName, oData));
	}
}

// �������� ���������������� �������� ������� � ���
// (�������� �������� ��� �������������� �������)
// (!!! �������� !!!)
void mlSynchPropCache::AddProperties(mlSynchedObjProps& aSynchedObjProps, mlSynchData& oData)
{
	// ���� ������ ��� ������� ��� ����
	mlObjectCachedSynchProps* pOCSPs = NULL;
	std::vector<mlObjectCachedSynchProps>::iterator vi2 = begin();
	for(; vi2 != end(); vi2++){
		mlObjectCachedSynchProps& oOCSPsTemp = *vi2;
		if (oOCSPsTemp.muID == aSynchedObjProps.uObjID &&
			oOCSPsTemp.muBornRealityID == aSynchedObjProps.uBornRealityID){
			pOCSPs = &oOCSPsTemp;
			break;
		}
	}
	// ���� ������ ���
	if(pOCSPs == NULL){
		// �� ������� 
		push_back(mlObjectCachedSynchProps());
		pOCSPs = &(*rbegin());
	}

	std::vector<mlSynchedProp>::iterator vi = aSynchedObjProps.vProps.begin();
	for(; vi != aSynchedObjProps.vProps.end(); vi++){
		mlSynchedProp& oSP = (*vi);
		jsval jsv = JSVAL_TRUE;
		char* pchNameEnd = &(oSP.pcName[oSP.usNameLen]);
		char ch = *pchNameEnd;
		*pchNameEnd = '\0';
		// ������������� �������� ��-�����������
		oData.setPos(oSP.iValuePos);
		pOCSPs->AddProperty(oSP.pcName, oData);
		*pchNameEnd = ch;
	}
}


/*
������	| �������		| ������
4		| objId 		������������� �������
2		| version		������ �������
1		| sysPropCount 	���������� ������������ ��������� �������
sysPropCount ��� { 	
1		| sysPropID 	������������� ���������� ��������
n		| sysPropValue 	�������� ���������� �������� (��.����)
} 	
2		| propCount 	���������� ���������� � ������� �������
propCount ��� { 	
2		| propNameLength 		����� ����� ��������
propNameLength | nameLength 	�������� ��������
2		| propValueLength 		����� �������� ��������
propValueLength | propValue 	�������� ��������
}
*/

#define CHECK_INPUT_DATA \
	if(oData.inTheEnd()){ \
		if(bNewObject) pop_back(); \
		return CR_ERROR; \
	}

bool CheckInputData( mlSynchPropCache* synchPropCache, mlSynchData& synchData, bool& abNewObject)
{
	if(synchData.inTheEnd())
	{
		if(abNewObject) 
			synchPropCache->pop_back();
		return false;
	}
	return true;
}


EAddToCacheResult mlSynchPropCache::AddProperties2(mlSynchData& oData)
{
	unsigned int uiID = 0;
	unsigned int uiBornRealityID = 0;
	unsigned int uiRealityID = 0;
	unsigned short usVersion = 0;
	oData >> uiID; // 4		| objId 		������������� �������
	oData >> uiBornRealityID; // 4		| bornRealityID 		������������� ���������� ��������
	oData >> uiRealityID; // 4		| bornRealityID 		������������� ���������� ��������
	oData >> usVersion;	// 2		| version		������ �������

	// ���� ������ ��� ������� ��� ����
	mlObjectCachedSynchProps* pOCSPs = NULL;
	FindSynchPropsObject( uiID, uiBornRealityID, pOCSPs);
	
	bool bNewObject = false;
	// ���� ������ ���
	if(pOCSPs == NULL){
		// �� ������� 
		bNewObject = true;
		push_back(mlObjectCachedSynchProps());
		pOCSPs = &(*rbegin());
	}

	pOCSPs->muID = uiID;
	pOCSPs->muBornRealityID = uiBornRealityID;
	mlTrace(mcx, "Adding synch props to cache for %d:%d {\n", uiBornRealityID, uiID);
	if(!mlSynch::ParseSysProps(oData, pOCSPs->mSysProps)){
		if(bNewObject)
			pop_back();
		return CR_ERROR;
	}

	bool bFlagsIsRead = ((pOCSPs->mSysProps.miSet & MLSYNCH_FLAGS_SET_MASK) != 0);
	if(bNewObject && !bFlagsIsRead){
		pop_back();
		return CR_NOT_FULL;
	}

	bool bConnectedIsRead = ((pOCSPs->mSysProps.miSet & MLSYNCH_FLAGS_SET_MASK) != 0);
	if(bNewObject && bConnectedIsRead && !pOCSPs->mSysProps.mbConnected){
		pop_back();
		return CR_DISCONNECTED;
	}

	if( !CheckInputData( this, oData, bNewObject))
		return CR_ERROR;
	MLWORD wPropsCnt;
	oData >> wPropsCnt;
	for(int ij=0; ij < wPropsCnt; ij++){

		if( !CheckInputData( this, oData, bNewObject))
			return CR_ERROR;
		char* pszPropName = NULL;

		oData >> pszPropName;
		if(pszPropName == NULL){
			if(bNewObject)
				pop_back();
			return CR_ERROR;
		}
		int iValuePos = oData.getPos();
		pOCSPs->AddProperty(pszPropName, oData);
		// ���������� value
		oData.setPos(iValuePos);
		unsigned short usLength;
		oData >> usLength;
		oData.skip(usLength);
	}
//return true;
	//
	if(!oData.inTheEnd()){
		// ������ ���� ���� ��� ������������� �������� ���������
		unsigned short iSChildCount = 0;
		oData >> iSChildCount;
		if(iSChildCount > 0 && pOCSPs->mpvSOSPs == NULL)
			pOCSPs->mpvSOSPs = new mlObjectCachedSynchProps::VecSubobjProps;
		mlObjectCachedSynchProps::VecSubobjProps* pvSOSPs = pOCSPs->mpvSOSPs;
		for (int i = 0; i < iSChildCount; i++)
		{
			if( !CheckInputData( this, oData, bNewObject))
				return CR_ERROR;
			char* pcName = NULL;
			oData >> pcName;
			if(pcName == NULL){
				break;	// �����-�� ���� � ������, ��������� � �������
			}
			// ���� ��� ���������� ��� ���-�� ���������, �� �������� ��� ���� ������
			mlObjectCachedSynchProps* pSubobjOCSPs = NULL;
			mlObjectCachedSynchProps::VecSubobjProps::iterator vi = pvSOSPs->begin();
			for(; vi != pvSOSPs->end(); vi++){
				if(isEqual(pcName, (*vi).msSubobjPath.c_str())){
					pSubobjOCSPs = &(*vi);
					break;
				}
			}
			if(pSubobjOCSPs == NULL){
				// ����� ������� ��� ���� ����� ������
				pvSOSPs->push_back(mlObjectCachedSynchProps());
				pSubobjOCSPs = &(*pvSOSPs->rbegin());
				pSubobjOCSPs->msSubobjPath = pcName;
			}
			if( !CheckInputData( this, oData, bNewObject))
				return CR_ERROR;
			unsigned int uiChildSize;
			oData >> uiChildSize;
			if( !CheckInputData( this, oData, bNewObject))
				return CR_ERROR;
			if(!mlSynch::ParseSysProps(oData, pSubobjOCSPs->mSysProps)){
				return CR_ERROR;
			}
			if( !CheckInputData( this, oData, bNewObject))
				return CR_ERROR;;

			MLWORD wPropsCnt;
			oData >> wPropsCnt;
			for(int ij=0; ij < wPropsCnt; ij++){

				if( !CheckInputData( this, oData, bNewObject))
					return CR_ERROR;;

				char* pszPropName = NULL;
				oData >> pszPropName;
				if(pszPropName == NULL){
					return CR_ERROR;
				}
				int iValuePos = oData.getPos();
				pSubobjOCSPs->AddProperty(pszPropName, oData);
				// ���������� value
				oData.setPos(iValuePos);
				unsigned short usLength;
				oData >> usLength;
				oData.skip(usLength);
			}
		}
	}
	mlTrace(mcx, "}");
	if (bNewObject)
		return CR_ADDED_NEW;
	else
		return CR_UPDATED;
}

// ��������� ��������� mlSynchedObjProps �� ������ mlCachedSynchProp-�� �� ����
void FillSynchedObjProps(mlSynchedObjProps& aSynchedObjProps, mlObjectCachedSynchProps* apOCSPs, mlSynchData& oData){
	aSynchedObjProps.oSysProps = apOCSPs->mSysProps;
	std::vector<mlCachedSynchProp>::iterator vi = apOCSPs->begin();
	for(; vi != apOCSPs->end(); vi++){
		mlCachedSynchProp& oCSP = *vi;
		aSynchedObjProps.vProps.push_back(mlSynchedProp());
		mlSynchedProp& oSP = aSynchedObjProps.vProps.back();
		oSP.usNameLen = oCSP.msName.length();
		int iNamePos = oData.getPos();
		// oSP.pcName = (char*)moData.getDataInPos(iNamePos + 2);
		oSP.pcName = (char*)(iNamePos + 2);
		oData << oCSP.msName.c_str();
		unsigned short usValueLen = oCSP.moValue.size();
		oSP.usValueLength = usValueLen;
		oData << usValueLen;
		oSP.iValuePos = oData.getPos();
		oData.put(oCSP.moValue.data(), usValueLen);
	}
}

void SetNamePointers(mlSynchedObjProps& aSynchedObjProps, mlSynchData& oData){
	// �������� ������� ���� �� �������� ������ � ������
	std::vector<mlSynchedProp>::iterator vi3 = aSynchedObjProps.vProps.begin();
	for(; vi3 != aSynchedObjProps.vProps.end(); vi3++){
		mlSynchedProp& oSP = *vi3;
		oSP.pcName = (char*)oData.getDataInPos((int)(oSP.pcName));
int jj=0;
	}
}

// �������� ���������������� �������� � ������� ��� ������������� ����
bool mlSynchPropCache::GetProperties(unsigned int auObjectID, unsigned int auBornRealityID, mlSynchedObjProps* &aSynchedObjProps, mlSynchData* &oData, /*mlSysSynchProps* &apSysProps,*/ VecSynchedObjProps* &aSubobjProps)
{
	mlObjectCachedSynchProps* pOCSPs = NULL;
	std::vector<mlObjectCachedSynchProps>::iterator vi2 = FindSynchPropsObject(
		auObjectID, auBornRealityID, pOCSPs);
	if(pOCSPs == NULL)
		return false;

	// ������������ mlSynchData � mlSynchedObjProps ��� ������� � aiObjectID
	moSynchedObjProps.uObjID = auObjectID;
	moSynchedObjProps.uBornRealityID = auBornRealityID;
	moSynchedObjProps.vProps.clear(); // mlSynchedProp
	moData.clear();
	//
	FillSynchedObjProps(moSynchedObjProps, pOCSPs, moData);

	aSynchedObjProps = &moSynchedObjProps;
	//apSysProps = &(pOCSPs->mSysProps);

	aSubobjProps = NULL;
	if(pOCSPs->mpvSOSPs != NULL && pOCSPs->mpvSOSPs->size() > 0){
		moSubobjProps.clear();
		mlObjectCachedSynchProps::VecSubobjProps::iterator vi4 = pOCSPs->mpvSOSPs->begin();
		for(; vi4 != pOCSPs->mpvSOSPs->end(); vi4++){
			mlObjectCachedSynchProps& oOCSP = *vi4;
			moSubobjProps.push_back(mlSynchedObjProps());
			mlSynchedObjProps& oSOP = moSubobjProps.back();
			oSOP.sSubobjPath = oOCSP.msSubobjPath;
			FillSynchedObjProps(oSOP, &oOCSP, moData);
		}
		aSubobjProps = &moSubobjProps;
	}
	SetNamePointers(moSynchedObjProps, moData);
	if(aSubobjProps != NULL){
		VecSynchedObjProps::iterator vi5 = moSubobjProps.begin();
		for(; vi5 != moSubobjProps.end(); vi5++){
			mlSynchedObjProps& oSOP = *vi5;
			SetNamePointers(oSOP, moData);
		}
	}

	oData = &moData;
	// ������� ��� �� �������
	this->erase(vi2);
	return true;
}

std::vector<mlObjectCachedSynchProps>::iterator mlSynchPropCache::FindSynchPropsObject(
	unsigned int auObjectID, unsigned int auBornRealityID,
	mlObjectCachedSynchProps*& aSynchProps)
{
	mlObjectCachedSynchProps* pOCSPs = NULL;
	std::vector<mlObjectCachedSynchProps>::iterator vi2 = begin();
	for(; vi2 != end(); vi2++){
		mlObjectCachedSynchProps& oOCSPsTemp = *vi2;
		if (oOCSPsTemp.muID == auObjectID &&
			oOCSPsTemp.muBornRealityID == auBornRealityID){
			aSynchProps = &oOCSPsTemp;
			break;
		}
	}
	return vi2;
}

}
