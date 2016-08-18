#pragma once

namespace rmml {

/**
* JS-����� ��� ���������������� ������ �� ������
*/

// 0 - ������ �� ���������, 
#define SLS_UNDEFINED 0
// 1 - ������ ���������� � ����� � ���� �� ���� ������
#define SLS_EXISTS 1
// 2 - ��������� ������ �� ��������� ������� �������, �� ����� ��� �� ������
#define SLS_WAITING_STATUS 2
// 3 - ������ ���������� �� ������� � ��������� � ���� ���������, �� ��� �� ��������
#define SLS_NOT_LOADED_YET 3
// -1 - "����� ������" (������ �� ���������� ������)
#define SLS_BROKEN -1
// -2 - ������ ��� ���� ��������� ��� � ������ ����������, ������� �� ����� �������� � ������
#define SLS_NOT_VISIBLE -2
// -3 - �������� �����-�� ������ ��� �������� ��� �������� �������
#define SLS_LOAD_ERROR -3
// -4 - ������ ������� �� ����� ���� ���������
#define SLS_CANT_BE_DEFINED -4
// -5 - ������ �� ������ ����, �� ������ �� ���, ������ ��� ������ ������ �� �����
#define SLS_LOST -5

/*
  ���������������� ������ - ��� ������ �� ������, ������� ����� ���� ��������� �������� ���������� synch,
�.�. �������� ����� ������������� �������, �� �������� ����� ������������ ��� �����������, ���� �� ��� 
�� �������� ��� �������� �������� ������ �� ������ (getRef), ���� �� ��� ����������.

*/

class mlSynchLinkJSO:	
				public mlJSClass
{
friend class mlSynchData;
friend class mlSynch;
	unsigned int muObjectID;
	unsigned int muBornRealityID;
	mlRMMLElement* mpParentElem;
	mlSynch* mpParent;
	bool mbLinkIsTracking;	// ������ ������������� �� ������� ������ ��� �������� �������
public:
	mlSynchLinkJSO(void);
	void destroy() { MP_DELETE_THIS; }
	~mlSynchLinkJSO(void);
	static JSBool AlterJSConstructor(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
	MLJSCLASS_DECL2(mlSynchLinkJSO)
private:
	enum {
//		JSPROP_ref = 1, // �������� ������ �� �������� ������ (null, ���� ������� ��� ��� � �����)
		JSPROP_status = 1 	
					// 0 - ������ �� ���������, (SLS_UNDEFINED)
					// 1 - ������ ���������� � ����� � ���� �� ���� ������ (SLS_EXISTS)
					// 2 - ��������� ������ �� ��������� ������� �������, �� ����� ��� �� ������ (SLS_WAITING_STATUS)
					// 3 - ������ ���������� �� ������� � ��������� � ���� ���������, �� ��� �� �������� (SLS_NOT_LOADED_YET)
					// -1 - "����� ������" (������ �� ���������� ������) (SLS_BROKEN)
					// -2 - ������ ��� ���� ���������, ������� �� ����� �������� � ������ (SLS_NOT_VISIBLE)
					// -3 - �������� �����-�� ������ ��� �������� ��� �������� ������� (SLS_LOAD_ERROR)
					// -4 - ������ ������� �� ����� ���� ��������� (SLS_CANT_BE_DEFINED)
					// -5 - ������ �� ������ ����, �� ������ �� ���, ������ ��� ������ ������ �� ����� (SLS_LOST)
	};
	JSObject* ref;
	int status;
	JSFUNC_DECL(getRef)	// �������� �������� ������ �� �������� ������
private:
	void UpdateObjectID();
	void GetStatus();
	unsigned int CorrectBornRealityID();
	void CallEventJSFunc(const wchar_t* apwcName);
public:
	void Init(mlSynchLinkJSO* apSrc);
	void SetParents(mlRMMLElement* apParentElem, mlSynch* apParent = NULL){
		mpParentElem = apParentElem;
		mpParent = apParent;
	}
	JSObject* GetRef();
	// ���� ������ �� muObjectID � ������������� ref
	void UpdateRef(bool abElemDestroyed = false);
	int GetObjectID(unsigned int& aBornRealityID);
	// ���������� SceneManager-��, ����� ������� ����� � MapServer-�
	// auObjectID � auBornRealityID - ��� ��������, ��� � ���� Link-� ��� ��� ��������� ������ ������ ����� �������
	void onStatusReceived(unsigned int auObjectID, unsigned int auBornRealityID, int aiStatus);
};

}
