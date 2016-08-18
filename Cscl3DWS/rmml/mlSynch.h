#pragma once

#include "mlSynchState.h"

namespace rmml {


struct mlPropToSynch
{
	// ���� ����� ��������� ������ ����� �������
	MP_WSTRING msName; // ��� ��������
public:
	mlPropToSynch(const wchar_t* apwcName):
		MP_WSTRING_INIT(msName)
	{
		msName = apwcName;
	}

	mlPropToSynch(const mlPropToSynch& other):
		MP_WSTRING_INIT(msName)
	{
		msName = other.msName;
	}
};

class mlPropsToSynch : public MP_VECTOR_DC<mlPropToSynch>
{
	typedef std::vector<mlPropToSynch> baseClass;
public: 
	mlPropsToSynch()
	{
		MP_VECTOR_INIT_THIS;
	}

	void AddProp(const wchar_t* apwcName);
	void Clear(){
		baseClass::clear();
	}
	typedef baseClass::iterator iterator;
	typedef baseClass::reference reference;
#if _MSC_VER >= 1500
	iterator begin(){ return baseClass::begin(); }
	iterator end(){ return baseClass::end(); }
#elif _MSC_VER > 1200
	iterator begin(){ return ((iterator)baseClass::begin()._Myptr); }
	iterator end(){ return ((iterator)baseClass::end()._Myptr); }
#else
	iterator begin(){ return ((iterator)baseClass::begin()); }
	iterator end(){ return ((iterator)baseClass::end()); }
#endif
	reference back(){ return (reference)baseClass::back(); }
	void pop_back(){
		baseClass::pop_back();
	}
};

class mlRMMLElement;

class mlSynchPropListeners{
	typedef MP_VECTOR<mlRMMLElement*> v_elems;
	typedef MP_VECTOR<mlRMMLElement*>::iterator v_elems_iter;
	typedef v_elems* pv_elems;
	struct mlSynchPropListener{
		MP_WSTRING sProp;
		v_elems vListeners;
		mlSynchPropListener():
			MP_VECTOR_INIT(vListeners),
			MP_WSTRING_INIT(sProp)
		{
		}
		mlSynchPropListener(const wchar_t* apwcSynchProp, mlRMMLElement* apMLElListener):
			MP_VECTOR_INIT(vListeners),
			MP_WSTRING_INIT(sProp)
		{
			sProp = apwcSynchProp;
			vListeners.push_back(apMLElListener);
		}
		mlSynchPropListener(const mlSynchPropListener& oSrc):
			MP_VECTOR_INIT(vListeners),
			MP_WSTRING_INIT(sProp)
		{
			sProp = oSrc.sProp;
			vListeners = oSrc.vListeners;
		}
	};
	MP_VECTOR<mlSynchPropListener> mvListeners;
	typedef MP_VECTOR<mlSynchPropListener>::iterator iterListeners;
	bool mbChanged;
	//
public:
	mlSynchPropListeners():
		MP_VECTOR_INIT(mvListeners)
	{
		mbChanged=true;
	}
	~mlSynchPropListeners(){
	}
	void NotifySenderDestroyed(mlRMMLElement* apMLElSender);
	pv_elems Get(const wchar_t* apwcSynchProp);
	void Set(const wchar_t* apwcSynchProp, mlRMMLElement* apMLElListener);
	void Remove(const wchar_t* apwcSynchProp, mlRMMLElement* apMLElListener);
	bool IsChanged(){ return mbChanged; }
	void ClearChanged(){ mbChanged=false; }
};


/*
�������� ��������� (�������) ���������������� ��������.
�� ����� ���������� �� �__".
��������� �������� ������������ � �������������� ��� ����������, ��� � ��������� �������.
  __zone � (�����) ������������� ����, � ������� ��������� ������
    ��������������� �� ����������� �� MapManager-�
  __flags � (�����) ����������� �����
    0-� ���� � ���� ����������, �� ��� ������ � ����������� �������� �������������
      ��������������� ����� �������� __type=2 � �������� synch
    1-� ���� � ���� ����������, �� �������� ����� ������ ������ �����������, ���� �� ���������� (���� ���, �� ����� �� ����� ������)
      ��������������� ����� �������� __type=1 � �������� synch
  __owner � (������) ����������� ������� (������ �� ����� �������� ��������)
    ���� ����� ������������.
    ����� ����������� ��� �������� ����� __owner=1 � �������� synch (���� � ������� ����������� ��� �� ����������, �� ������������� ������������� ����� �������� ������, ��������� ������)
    ������:
      bool synch.capture() � ���������� ��������� ������ � ���� �������������
      bool synch.free() � ���������� ����������� � ������������� ������� ������
      bool synch.isMyOwn() � �������� �� ������ �������������� �������? (���� ��, �� ����� ������ �������� � ��� ����� ����������������)
      bool synch.isFree() � �������� �� ������? (����������� ��� �� ����������?)
	  bool synch.mayChange() - ����� �� ������ �������� �������? (���� ��, �� ��������� � ������� ������������ ����� ������� �������� � �������� � ������ ��������)
    �������:
	  synch.onOwnerChange � �������� ����������� ������� (��� ������ �����������)
	  synch.onMessage - ������ ��������� �� ������� �������
	  synch.onInitialized - ��������� ������� ������������� � ����������� ��������� �� ����
	                        (������� �������, ����� ����� ���� ���������� �������������� �������������, ��������, ����������� ������ � ���� �����, ������ ��, � ������� �� ���)
	  synch.onCommunicationAreaChange � ��������� ���� �������
	  synch.onLocationChange � ��������� �������

  ���� ������ ���� ����� �������� ��������� ������� �������. ��� ��������� ����� �������� ������ �������-������������ �������.
  � ������� ����, ��� ���� ��������� ��� ����� ��������� ���:
	  otherOwnerObjec.synch.sendMessage(...);
  � ������� ������������ � ����� ������� ����� ������������� �������
	  <script event="synch.onMessage">
		// Event.sender = ���������_�������������_�������-�����������
		// Event.args � ������ ����������, ���������� ��� ������ sendMessage (������ �� ������� ���� ������ ������������)
		...
	  </script>

���� ������� ���:
1) �������� ��������� (�� ��������)
2) onInitialized
*/


#define EVNM_onOwnerChange L"onOwnerChange"
#define EVNM_onMessage L"onMessage"
#define EVNM_onInitialized L"onInitialized"
#define EVNM_onReset L"onReset"
#define EVNM_onCommunicationAreaChange L"onCommunicationAreaChange"
#define EVNM_onLocationChange L"onLocationChange"

typedef MP_VECTOR<mlRMMLElement*> v_elems;

bool isPropertyOrObjectNameValid(unsigned char* str);


// MayChange error codes
#define MCERROR_NOERROR					0		// ����� �������
#define MCERROR_OBJECTISCAPTURED		1		// �� ����� �������, ������ ��� �����;
#define MCERROR_OBJECTISRECORDED		2		// �� ����� �������, ������ ��� �������
#define MCERROR_OBJECTISNONCHANGABLE	3		// �� ����� �������, ������ ��� ��� ������ �� �������� � ������ (�������� ����� ��� �������)
#define MCERROR_WRONGSYNCOBJECT			4		// �� ����� �������, ������ ��� ��� ������������ ������. ��� ASSERT

/**
 * JS-����� �������� synch RMML-��������� ��� 3-� ������ �������������
 */
class mlSynch:
					public mlJSClass,
					public mlIBinSynch
{
private:
	bool isPaused;	// ��� �������
	// ��������� ������ ��������� ������� 
	unsigned short mwServerVersion;
	// ����������� ������ ��������� ������� 
	unsigned short mwClientVersion;

	mlRMMLElement* mpParentElem;
	mlSynch* mpParent;
	MP_WSTRING msName; // ��� �������� ���������� (����� mpParent != NULL)
	mlPropsToSynch mPropsToSynch; // ������ ������� �� ����������, ������� ���������� � �� ���� ����� ����������������
	mlSynchPropListeners mListeners;
	bool mbSynchronizing; // ==true, ���� ���� ������� ��������� �������� ������� � ���������� � �������
	bool mbSetting; // ==true, ���� ���� ������� ��������� �������� ��������
	wchar_t* mpwcPropName; // ��� ��������, ������� ���������������
	jsval mjsvProp; // �������� ��������, ������� ���������������
	//// ����� ��������� ������� 
	// 0� - ���������� ����
	int miChanged;

	// ������������� ���������
	bool isSynchInitialized;
	// ����� ������������� ����� �������� �������� synch
	bool mbInitialization;
	//unsigned short mwVersion;
	mlSysSynchProps mSSPs;
	// ���� ��������� ���������� �������� ���������� ��� ��������� ��������-������� �������
	// (���� ==true, �� ��� 3� ��������� �� ���� ����������, ��� ���������� ��� ��������.
	//  ���������� ����� ������ �� ��������� ����� ����������)
	bool mbSubPropSetting;

	// ��������� ��������� ��������� ������� ��� ������� �� ������
	syncObjectBinState binStateForSending;

	// ������ ���������������� ����������� �������� (� mlSynch ��� �������� ������)
	v_elems* mpSynchedChildren; // �������� 1 - ��� ���������������� �����������
	// ������� ������������� ������� (����� ���� ������ ��� ����� � ��� �� �������)
	int mOutLocationID;
	int mOutCommunicationAreaID;
	// ���� ������� �� ���������������, �� ����� ���������� ������� ������� ���������� � �������
	__int64 fullStateQueryTime;

	// ���� =true, �� �� ����������������, �������� ���������, �� ���������� ���������������� �����
	bool mbDontSynch;
	// ����� ��������� ��������� ������� ������������� 
	unsigned __int64 mSetPropLocalTime;
	// ��� ���������� ����������� �� ������ ���������
	syncObjectState::StateType nextSendingStateType;
	bool mbWaitForLoading; // watch, Synched, Reset
public:
	mlSynch(void);
	void destroy() { MP_DELETE_THIS; }
	~mlSynch(void);
	static JSBool AlterJSConstructor(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
MLJSCLASS_DECL2(mlSynch)
	bool IsMyOwn();
	bool IsFree();
	bool IsRecorded();
	int  MayChange();
	void BeginHashEvaluating();
	void EndHashEvaluating();
	void ForceVersions();
	bool GetBinState( BYTE*& aBinState, int& aBinStateSize);
	void SetWaitForLoadingStatus();
	bool IsWaitForLoading();
	void ResetWaitForLoadingStatus();
private:
	enum { 
		JSPROP_serverTime=1
	};
	static JSBool array_length_getter(JSContext *cx, JSObject *obj, jsval id, jsval *vp);

	static JSBool array_length_setter(JSContext *cx, JSObject *obj, jsval id, jsval *vp);

	static JSBool serverTime_getter(JSContext *cx, JSObject *obj, jsval id, jsval *vp);

	// bool synch.capture() � ���������� ��������� ������ � ���� �������������
	JSFUNC_DECL(capture)
	// bool synch.free() � ���������� ����������� � ������������� ������� ������
	JSFUNC_DECL(free)
	// bool synch.isMyOwn() � �������� �� ������ �������������� �������? (���� ��, �� ����� ������ �������� � ��� ����� ����������������)
	JSFUNC_DECL(isMyOwn)
	// bool synch.isFree() � �������� �� ������? (����������� ��� �� ����������?)
	JSFUNC_DECL(isFree)
	// ���������� ��������� ��������� �������
	JSFUNC_DECL(settingStart)
	// ��������� ��������� ������� ���������
	JSFUNC_DECL(settingEnd)
	// bool synch.mayChange() - ����� ������ �������� �������. 
	// (��������� � ������� ������������ ����� ������� �������� � �������� � ������ ��������)
	JSFUNC_DECL(mayChange)
	// ��������� ���������� ���������� ������� � ���� ������ MapServer-�
	JSFUNC_DECL(fixPos)
	// ������� ������ �� ���� ������ MapServer-�
	JSFUNC_DECL(remove)
	// �������� ��������� �������-������������ �������
	JSFUNC_DECL(sendMessage)
	// ������� ������ �� ������ �� ������� �������� ������� �� ������ (Link)
	// JSFUNC_DECL(restoreObject)
	// ���������� ������������� ������������-������������
	JSFUNC_DECL(getOwner)
	// ���������� ������������� �������
	JSFUNC_DECL(getLocation)
	// ���������� �����, ����� ������ ����� ������ ��������
	JSFUNC_DECL(getDeleteTime)
	// ������ �����, ����� ������� ���� ����� ������� ������
	JSFUNC_DECL(setDeleteTime)
	JSFUNC_DECL(pause)
	JSFUNC_DECL(resume)
	// ���������� ������������� �������
	JSFUNC_DECL(getCommunicationArea)
	// ���������� ��������� binState ������ ��� ���
	JSFUNC_DECL(isEmptyBinState)


	//
	void CallListeners2(const wchar_t* apwcSynchProp);
	jsval GetEventJSVal(char aidEvent);
	void CallListeners(char aidEvent);
	//
	bool Free();
	bool IsSOAObject(){ return (mSSPs.miFlags & MLSYNCH_SERVER_OWNER_ASSIGNATION_MASK)?true:false; }
	bool IsOOSObject(){ return (mSSPs.miFlags & MLSYNCH_OWNER_ONLY_SYNCHR_MASK)?true:false; }
	mlSynch* GetTopParent();
	void CallEventFunction(const wchar_t* apwcEventName);
	void FireEvent(const wchar_t* apwcEventName);
	bool GetParentElementWithID(mlRMMLElement*& apParentElement);
	bool ParentElementIsWithID();
	bool GetSynchOfParentElementWithID(mlSynch*& apSynch);
	moIBinSynch* GetIBinSynch();

	// ������ ��������� �������� ������� ������������ ������� 
	// aulDeleteTime - � ������������� (������������ �������� �������)
	bool SetDeleteTime(unsigned __int64 aulDeleteTime);

	// �������� (���������) ��������� ����� � �������������
	unsigned __int64 GetServerTime();

	//// �������� �������� ���������� �������� ������� ������������ ������� 

	// �������� ������� ��������� ��� ������� � ������� 
	__int64 GetTimeZoneDelta();

	// ������� �������� �� ���������� ������� � UTC � �������������
	unsigned __int64 ServerTimeToLocalUTC(unsigned __int64 aulServerTime);

	// �������������� ���������� ������� � ����������� ���
	std::string FormatServerTime(unsigned __int64 aulServerTime);

// ���������� mlIBinSynch
public:
	bool StateChanged(unsigned int auStateVersion, BSSCCode aCode, void* apData, unsigned long aulSize, 
		unsigned long aulOffset=0, unsigned long aulBlockSize=0);

public:
	bool IsObsoleteFullState( syncObjectState& aState);
	inline unsigned short GetServerVersion() { return mwServerVersion;}
	inline unsigned short GetClientVersion() { return mwClientVersion;}
	bool Capture();
	void SetSynchFlags(unsigned int auiFlags);
	unsigned int GetSynchFlags();
	mlSynchData oPropsToSynch;
	void SetParents(mlRMMLElement* apParentElem, mlSynch* apParent = NULL){
		mpParentElem = apParentElem;
		mpParent = apParent;
	}
	// ���������� ��� �������� � ���������, � �������. ���������� ��� ��������� �������������. ����������� #477
	void Reset();
	// ���������� ������� �������� � �������� ���������
	void ResetState();
	// ���������� ������������� mlSynch, ��� ���� �������� ���������
	void SetChanged(const wchar_t* apwcPropName);
	// ���������� ���� ��������� ���������� ��������
	__forceinline void SetChanged(int aiMask);

	void SetEventListener(const wchar_t* apwcSynchProp, mlRMMLElement* apMLElListener){
		mListeners.Set(apwcSynchProp, apMLElListener);
	}

	void RemoveEventListener(const wchar_t* apwcSynchProp, mlRMMLElement* apMLElListener){
		mListeners.Remove(apwcSynchProp, apMLElListener);
	}
	
	// �������� ������������ �������� ������� � ����� �������
	bool GetSynchProps( syncObjectState& aoState, JSObject* &jsoSynched);
	// �������� ������������ �������� ������� �����������
	bool GetSynchSubObjProps(syncObjectState& aoState, JSObject* &jsoSynched, const std::wstring& awsPrefix);
	// ���� ������������� ���������
	inline bool IsSynchInitialized() { return isSynchInitialized;};
	inline void ComleteInitialization() { isSynchInitialized = true;};
	inline bool ResetInitialized() { isSynchInitialized = false;};
	// ���������� ����� ���������� ������� ������� ���������� � �������
	inline __int64 GetFullStateQueryTime() { return fullStateQueryTime;}
	// ������������� ����� ���������� ������� ������� ���������� � �������
	inline void SetFullStateQueryTime( const __int64 aTime) { fullStateQueryTime = aTime;}
	// ���� ���� ��������� � ���������������� ���������
	bool IsSynchChanged();
	// ����������� �������� �� ������ ������� ���������
	void ForceWholeStateSending();
	// ������������, ��� ������ ������ ��������� �������
	void SimulateFullStateCome(JSObject* jsoSynched);
	// ���������� �������� ������� � ��������, ���������� � ������� (� ����� �������)
	ESynchStateError SetSynchProps( const syncObjectState& aState, JSObject* &jsoSynched, bool& aSynchedEmpty);
private:
	// ���������� �������� �������� � ��������, ���������� � �������
	bool SetSynchProp( const syncObjectProp& aProp);
	// ���������� ��������� �������� � ��������, ���������� �� ����
	ESynchStateError SetSysSynchProps(const mlSysSynchProps& apSysProps, bool bDontTrace);
	// ���������� �������� � Event.synched
	void SetPropToSynchedJSO(JSObject* ajsoSynched, const char* apcPropName);
public:
	// ���������� ����� ������������� (��� ������������� ����� ���������� �������� �������� synch)
	void SetInitializationMode(bool abMode){ mbInitialization = abMode;	}
	// ������ ������������ �� ����� ���� � ������ (���������� MapManager-��)
	void ZoneChanged(int aiZone);
	// ������ ������� � ������ ���������� (���������� MapManager-��)
	void RealityChanged(unsigned int auReality);
	// z
	float GetZ();
	// x
	float GetX();
	// y
	float GetY();

	void SetXY(float aX, float aY);
	void SetZ(float aZ);
	// ������ �������� z ������� ������� ������������ � ���� ��� ����������
	bool NeedToSetZ();
	// � ������� ������ ��������� �� ������� �������
	void HandleSynchMessage(const wchar_t* apwcSender, mlSynchData& aData);
	// ��������� mPropsToSynch (��������, ��� ����, ����� �� �������� 0-� ��������, ������ ��� ���� ���������, ���������� � �������)
	void DontSynchLastChanges();
	// ������������� onInitialized
	void FireOnInitialized();
	// ������������� onReset
	void FireOnReset();
	// ��������� ���������� ��������� ���������
	void ApplyBinStateUpdating(const syncObjectBinState& aBinStateUpdating);
	// ��������� ������ �������� ���������
	void ApplyFullBinState();
	// ������� ������ �� ������ �� ������� �������� ������� �� ������
	// void RestoreObject(mlSynchLinkJSO* apLink);
	// ������������� ����, ��� ���������� ���������������� ��������� ������ �� �����������
	__forceinline void SetSubobjChanged() {
		miChanged |= MLSYNCH_SUB_OBJ_CHANGED_MASK;
	}
	__forceinline bool IsSubObjSynchChanged(){ return (miChanged & MLSYNCH_SUB_OBJ_CHANGED_MASK)?true:false; }
	__forceinline v_elems* GetSynchedChildren(){ return mpSynchedChildren; }
	v_elems* CreateSynchedChildren();
	#define ML_SYNCH_CHILDREN_EMPTY_VAL ((v_elems*)1)
	__forceinline void SetSynchedChildrenEmpty(){ mpSynchedChildren = ML_SYNCH_CHILDREN_EMPTY_VAL; }
	// 3D-������ ��������� � �������� ��� ������� (�������� ��� � �� ����������)
	void SetLocationSID(const wchar_t* pszLocationID);
	void SetCommunicationAreaSID(const wchar_t* pszCommunicationAreaID);	
	// ������������� ������������� ������� (�� ��������� ��� ��������� �� ���������� ���������������� ������)
	void DontSynchAwhile(){ mbDontSynch = true; }
	// ����������� ������������� 
	void RecommenceSynchronization(){ mbDontSynch = false; }
	// ������������� ������� ��������������
	bool DontSynch(){ return mbDontSynch; }
	// ���������� ����� ���������� ���������. ����������� #414
	inline unsigned __int64 GetLastChangingTime() { return mSetPropLocalTime;};
	// ���������� ��������� �������� � json-�������
	mlString GetSysPropertiesJson( int aIndent, const wchar_t* aIdentStr);
	// 
	bool IsApplyableFullState( const syncObjectState& aState);

	bool IsHashEvaluating(){ return mbHashEvaluating;}

private:
	bool			mbHashEvaluating;
	// push state variables
	mlPropsToSynch	mPushPropsToSynch;
	int				mPushMiChanged;
	syncObjectState::StateType mPushNextSendingStateType;
};


}
