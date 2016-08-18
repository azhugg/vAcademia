#pragma once

#include "FileNameStorage.h"

namespace rmml {

/**
 * ������� RMML-������� (Java-�������)
 * ������������ mlRMMLBuilder-�� ��� ������������� RMML-��������. 
 * � ����� ��� ������� ��������, ����������� ��� ������������� ������������� �������
 */
class mlRMMLScript: public mlRMMLElement
{
friend class mlRMMLBuilder;
	//bool mbRef; // mjss �������� ������� �� ������ ������
	// mjss �������� ������� �� ������ ������
	#define SCRF_REF (1 << 0)
	// ������ ��� �� �������������
	#define SCRF_NOT_COMPILED (1 << 1)
	unsigned char mucFlags; // �����: mbRef
	void* mpData; // ������� �� ���� ������� (event ��� watch ��� event expression)

	void Compile(const wchar_t* apwcData, const char* apszSrcName, int aiLine);
	void Compile(const char* apszData, const char* apszSrcName, int aiLine);
	mlresult Load(const wchar_t* apwcSrc);
	// ��������� ������ � ���� ������, ����� �������������� ��� �����
	void SaveToCompileLater(const wchar_t* apwcData, const char* apszSrcName, int aiLine);
//	mlresult SetEventListener(wchar_t* apwcValue);
	// �������������� ������, ���� �� ��� �� �������������
	void Compile();
public:
	mlRMMLScript(void);
	void destroy() { MP_DELETE_THIS }
	~mlRMMLScript(void);

	// ���������� mlRMMLElement
	mlRMMLElement* Duplicate(mlRMMLElement* apNewParent);
		// ������� ���������� �������
	mlresult SetProperty(const wchar_t* apwcName,const wchar_t* apwcValue);
	mlresult SetValue(wchar_t* apwcValue);
//	bool NeedToBeDeleted(){ return true; }
	mlresult SetMeAsEventListener();
	void YouAreSetAsEventListener(char aidEvent, mlRMMLElement* apSender);
	mlresult RemoveMeFromEventListeners();
	bool EventNotify(char aidEvent, mlRMMLElement* apSender);
	void EventSenderDestroyed(mlRMMLElement* apSender);
	void Activate();
	mlString GetEventOrWatchName();
	mlString GetScriptName();
	mlString GetTagName(){ return L"script"; }
//	bool NeedToSave(int aiPropGroupType,const wchar_t* apwcName,
//		const wchar_t* apwcVal,mlRMMLElement* apTempElem);
	mlString GetValue(int aiIndent);
	SynchEventSender* GetSynchEventSenderStruct(){
		if(mType==MLST_SYNCHMS) return (SynchEventSender*)mpData;
		return NULL;
	}
	void SynchEventNotify(){
		Execute();
	}
	//
//	void Execute(JSContext* cx, JSObject* obj);
	mlresult Execute(jsval* apjsvResult=NULL);
	void GetWaitingEvents();
	struct EventExpressionStruct;
	bool CheckEventExpr(EventExpressionStruct* apEvExprStruct);

	std::string GetName();
	void SetName(std::string fileName);

	bool IsCompiled(){ return !(mucFlags & SCRF_NOT_COMPILED); }

	// �������� ������ ������ � ������, � ������� �������� ������������������ ������
	int GetNonCompiledScriptSize();

	// ����� ������ ����������� �� �������� ����� (parent=null), �� ����, ����� �� �������� ������������ � �������
	virtual void UnlinkFromEventSenders(bool abWithChildren = true);

private:
	unsigned short m_srcID;	
	unsigned short m_line;
};

}