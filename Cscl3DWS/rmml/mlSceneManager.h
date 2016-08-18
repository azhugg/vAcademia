#pragma once

#include <string>
#include <map>
//#include <fstream>
#include "jsdbgapi.h"
#include "rmmld.h"
#include "mlBuildEvalCtxt.h"
#include "mlNextUpdateDTimeList.h"
//#ifdef JS_THREADSAFE
//#include "jslock.h"
//#endif
#ifdef CSCL
#include "mlRMMLJSPlServer.h"
#include "mlRMMLJSPlVoice.h"
#include "mlRMMLJSPlVideoCapture.h"
#include "mlRMMLJSPlCodeEditor.h"
#include "mlRMMLJSPlDesktop.h"
#include "mlRMMLJSPlMap.h"
#include "mlLogSeance.h"
#include "mlLogRecord.h"
#include "mlSynch.h"
#include "mlSynchPropCache.h"
#include "mlUtilityThread.h"
#include "mlScriptTerminationThread.h"
#include "mlScriptPreloader.h"
#include "ScriptedObjectManager.h"
#endif
#include "rmmlsafe.h"
#include "mlObjectsStat.h"
#include "mlRecordManager.h"
#include "IXBOXController.h"

#define TRACE_LOST_ELEMS 

namespace rmml {

/**
 * �������� ����
 * �������������� JS-Engine, 
 * ��������� ��������� ���������/����������/����, 
 * ��������� � �������������� �����
 */

using namespace std;

struct mlXMLBuffer;
class cstr_streambuf;
extern const char gpszUIDefault[];
typedef gd_xml_stream::CXMLArchive<mlXMLBuffer> CXMLArchiveWithXmlBuffer;

struct mlXMLArchive: public res::resIResource{
	MP_WSTRING msSrc;
	int miSrcIdx;
	res::resIResource* mpRes;
	res::resIWrite* mpWrite;
	cstr_streambuf* mpStrStreamBuf;
	istream* mpIS;
	iostream* mpOS;
	mlXMLBuffer* mpBuf;
	gd_xml_stream::CXMLArchive<mlXMLBuffer>* mpAr;
	mlRMMLBuilder* mpBuilder;
	mlXMLArchive():
		MP_WSTRING_INIT(msSrc)
	{
		miSrcIdx=-1;
		mpRes=NULL;
		mpWrite=NULL;
		mpStrStreamBuf=NULL;
		mpIS=NULL;
		mpOS=NULL;
		mpBuf=NULL;
		mpAr=NULL;
		mpBuilder=NULL;
	}
// res::resIResource
public:
	omsresult GetIStream(istream* &apistrm){
		apistrm = mpIS;
		return OMS_OK;
	}
	omsresult GetStream(oms::Stream* &apOMSStream){
		return OMS_ERROR_NOT_IMPLEMENTED;
	}
#ifndef CSCL
	shared_ptr<oms::Stream> GetStream(){
		return shared_ptr<oms::Stream>();
	}
#endif
	unsigned int Read( unsigned char* aBuffer, unsigned int aBufferSize, unsigned int *aErrorRead = NULL);
	unsigned int Write( unsigned char* aBuffer, unsigned int aBufferSize){
		return 0;
	}
	bool Seek( long aPos, unsigned char aFrom){
		return false;
	}
	unsigned long GetPos(){
		return 0; //mpStrStreamBuf->
	}
	unsigned long GetSize();
	void Close(){
		MP_DELETE_THIS;
	}
	const wchar_t* GetURL(){
		return L"";
	}
	bool GetFileNameRefineLoaded(wchar_t **bstrName, const wchar_t *pCurName)
	{
		return false;
	}
	void Download(res::resIAsynchResource* aAsynch, const wchar_t* apwcLocalPath){}
	void DownloadToCache(res::resIAsynchResource* aAsynch){}
	bool Unpack(res::resIAsynchResource* aAsynch=NULL){ return false; }
	bool UnpackToCache(res::resIAsynchResource* aAsynch=NULL){ return false; }
	unsigned long GetDownloadedSize(){ return 0; }
	unsigned int GetState(){ return 0; }
	unsigned int GetUnpackingProgress(){ return 0; }
	bool Upload(resIResource* apSrcRes, res::resIAsynchResource* aAsynch=NULL){ return false; }
	unsigned long GetUploadedSize(){ return 0; }
	__int64 GetTime() {return 0;}
	unsigned long GetCachedSize() {return 0;}
	bool RegisterAtResServer( res::resIAsynchResource* aAsynch){ return false;}
};

class mlStringCollection{
	MP_VECTOR<wchar_t*> mvColl;
public:
	mlStringCollection():
		MP_VECTOR_INIT(mvColl)
	{
	}
	~mlStringCollection(){
		std::vector<wchar_t*>::iterator vi;
		for(vi=mvColl.begin(); vi!=mvColl.end(); vi++){
			MP_DELETE_ARR( *vi);
		}
		mvColl.clear();
	}
	int Find(const wchar_t* apwcStr){
		int iIdx = 0;
		std::vector<wchar_t*>::iterator vi;
		for(vi = mvColl.begin(); vi != mvColl.end(); vi++,iIdx++){
			if(isEqual(apwcStr,(const wchar_t*)*vi)) return iIdx;
		}
		return -1;
	}
	int FindMatch(const wchar_t* apwcStr){
		mlString sFilePath(apwcStr);
		mlString::size_type pos = 0;
		// sFilePath.replace('/', '\\');
		while((pos = sFilePath.find(L"/",pos)) != mlString::npos){
			sFilePath.replace(pos, 1, L"\\");
		}
		pos = 0;
		while((pos = sFilePath.find(L"\\\\",pos)) != mlString::npos){
			sFilePath.replace(pos, 1, L"\\");
		}
		int iIdx = 0;
		std::vector<wchar_t*>::iterator vi;
		for(vi = mvColl.begin(); vi != mvColl.end(); vi++,iIdx++){
			mlString sSrcFile((const wchar_t*)*vi);
			// ������� ��� ��������
			pos = sSrcFile.rfind(L':');
			if(pos != mlString::npos)
				sSrcFile.erase(0, pos);
			else
				pos = 0;
			// sSrcFile.replace('/', '\\');
			while((pos = sSrcFile.find(L"/",pos)) != mlString::npos){
				sSrcFile.replace(pos, 1, L"\\");
			}
			//
			if(sFilePath.find(sSrcFile) != mlString::npos)
				return iIdx;
			if(sSrcFile.find(sFilePath) != mlString::npos)
				return iIdx;
			if(isEqual(apwcStr,(const wchar_t*)*vi)) return iIdx;
		}
		return -1;
	}
	int Add(const wchar_t* apwcStr){
		int iIdx = Find(apwcStr);
		if(iIdx >= 0) return iIdx;
		int iLen=wcslen(apwcStr);
		wchar_t* pwcAddStr = MP_NEW_ARR( wchar_t, iLen+1);
		//wcscpy(pwcAddStr,apwcStr);
		rmmlsafe_wcscpy(pwcAddStr, iLen+1, 0, apwcStr);
		mvColl.push_back(pwcAddStr);
		return mvColl.size()-1;
	}
	const wchar_t* operator[](int aIdx) const{
		if(aIdx < 0 || aIdx >= ((int)mvColl.size())) return NULL;
		return mvColl[aIdx];
	}
};

class mlSceneDatas{
public:
	class mlSceneData{
	public:
		mlRMMLElement* mpScene;
		MP_WSTRING msPath;
	public:
		mlSceneData(mlRMMLElement* apScene):
			MP_WSTRING_INIT(msPath)
		{ 
			mpScene=apScene; 
		}
	};
private:
	MP_VECTOR<mlSceneData*> mvDatas;
	typedef std::vector<mlSceneData*>::iterator mlSceneDataIter;
public:
	mlSceneDatas():
		MP_VECTOR_INIT(mvDatas)
	{		
	}
	~mlSceneDatas(){
		mlSceneDataIter vi=mvDatas.begin();
		for(; vi!=mvDatas.end(); vi++)
		{
			MP_DELETE( *vi);
		}
		mvDatas.clear();
	}
	void AddSceneData(mlSceneData* apData){
		if(apData==NULL) return;
		mvDatas.push_back(apData);
	}
	bool RemoveSceneData(mlRMMLElement* apScene){
		for(mlSceneDataIter vi=mvDatas.begin(); vi!=mvDatas.end(); vi++){
			mlSceneData* pData=*vi;
			if(pData->mpScene==apScene){
				mvDatas.erase(vi);
				MP_DELETE( pData);
				return true;
			}
		}
		return false;
	}
	mlSceneData* GetSceneData(mlRMMLElement* apScene){
		for(mlSceneDataIter vi=mvDatas.begin(); vi!=mvDatas.end(); vi++){
			mlSceneData* pData=*vi;
			if(pData->mpScene==apScene) return pData;
		}
		return NULL;
	}
};

#define GOID_Player		1
#define GOID_Module		2
#define GOID_Mouse		3
#define GOID_Map		4
#define GOID_Url		5
#define GOID_Voice		6
#define GOID_Recoder	7
#define GOID_Server		8
#define GOID_RecordEditor		9
#define GOID_VideoCapture		10
#define GOID_CodeEditor		11

typedef MP_VECTOR_DC<mlRMMLElement*> v_elems_dc;

class mlSceneManager:
		public mlISceneManager
{
friend class mlRMMLBuilder;
#ifdef RMML_DEBUG
friend class rmmld::mldRMMLDebugger;
#endif
	JSContext *cx;

	// RMML-������ ���������� � ��������� (�������� JS-�����������)
	JSObject* mjsoClasses;
//	map<wchar_t*, mlRMMLElement*> mMapProtos;
//	typedef pair<wchar_t*,mlRMMLElement*> Pair4ProtoMap;
//	typedef map<wchar_t*,mlRMMLElement*>::const_iterator Iter4ProtoMap;

	// �������� �����
	mlRMMLComposition* mpScene;
	JSObject* mpPreviousScene;
	// ������ �� XML-�� �� ������
	MP_WSTRING msSceneXML;

	// ���������� ����������������� ����������
	mlRMMLComposition* mpUI;

	// ����� ������ ��������������
	mlRMMLComposition* mpAuthScene;

	mlRMML mRMML;

	omsContext* mpContext;

	int miUpdateNumber; 

	void SetScene(mlRMMLComposition* apNewScene=NULL);
	void SetUIScene(mlRMMLComposition* apNewUIScene=NULL);
	typedef std::vector<JSObject*> v_jso;
	void RemoveSceneElemsRefs(JSObject* obj, v_jso* apLookedup=NULL);

	mlEModes meMode;

	mlETestModes meTestMode;
	std::wstring msTestResDir;

	mlSceneDatas mSceneDatas;

	// =true, ���� ����� �������� � ������ Update(...)
	bool mbUpdating;
	// �������: ����� ������� ������� Update(...)-� IsSceneStructChanged() ����� ����� false
	unsigned char muStructChangedUpdCntr;

	mlBuildEvalCtxt mBuildEvalCtxt;

	bool bRestoreSynchronization;

	// ������ ����������� �� Update ���������
	v_elems mvUpdatableElements;

	// ����� ���������� ���������� ������ ����������� �������� �����
	__int64 mlLastUpdatableListBuildingTime;
	
	// ������ ����������� �� Update ��������� ����������������� ���������
	v_elems mvUpdatableUIElements;

	// ����� ���������� ���������� ������ ����������� �������� UI
	__int64 mlLastUIUpdatableListBuildingTime;

	// ��������� ������ ����������� ���������
	bool mbUpdatableElementsChanged;

	MP_VECTOR<unsigned int> m_synchObjectsInReality;
	unsigned int m_synchObjectsInRealityCreated;
	unsigned int m_synchObjectsInRealityCount;

	// ������ ����� ��������� ���������: 
	// ����� ��� ������� � �������� �����, 
	// �� ���� ����� �������� ��� ������������� � mvUpdatableElements/mvUpdatableUIElements � � mvObjectsToSynch
	v_elems mvNewElements; 

	v_elems mvObjectsToSynch;

	MP_VECTOR<unsigned int> mvDeletedInUpdate;	// ������ ��������, ������� ���� ������� �� ���������� Update
								// (������������ � LoadAndCheckInternalPath)

	v_mos mvMOToDeleteOut;

	bool mbDestroying; // ���� ���������� ����������� SceneManager-�
	v_mos mvPluginsMOs; // ������ �����-�������� �������� ��� �������� �� ����� ��������� ��������� � ����������� SceneManager-�

	mlScriptPreloader* mScriptPreloader;

	bool mbDontUpdateOneTime;

#ifdef JS_THREADSAFE
	// ������ ���������, ������� ���� ����� ������� �� ��������� Update-�
	v_elems mvElemsToDelete;
	MP_VECTOR<mlJSClass*> mvObjectsToDelete;
#endif

	MP_WSTRING msStartURL;

public:
	unsigned int mCountIds;
	mlLogSeanceJSO* pSeance;
	mlLogRecordJSO* pRecord;
	mlSceneManager(omsContext*);
	~mlSceneManager(void);

	void OnVisibilityDistanceKoefChanged(int koef);

	void OnWindowMoved();
	void OnKinectFound();
	void OnOculusRiftFound() override;
	void OnWindowSizeChanged( const int& auClientWidth, const int& auClientHeight);

	bool mbObjetcsUnfreezenAfterFreeze;
	bool mbObjetcsFreezeUnfreezenObjects;
	bool mbObjectsFrozen;

	bool mbPreloadingMode;
	bool mbLoadingMode;

	bool mbNewEventsToNextUpdateMode;	// ���� ==true, �� ��������� ���� ��������������� 
										// �� ������� Update-� ������� ����� ������������ �� ��������� Update
	oms::omsUserEvent::UpdateID mEventQueueUpdateID;

	bool mbTraceEvents;	// ���� ==true, �� � ��� ��������� ��� ���� �� �������������� �� update-� ��������

	bool InitJSE(void);
	void DestroyJSE(void);
	bool IsDestroying(){ return mbDestroying; }

	bool mbObjectCreating; // Tandy: ��� ��������� ����� �������������� ��������...

	void SetCurrentExecutingScriptLine(int lineIndex)
	{
		m_currentExecutingScriptLine = lineIndex;
	}

	int GetCurrentExecutingScriptLine()
	{
		return m_currentExecutingScriptLine;
	}

//#ifdef ENABLE_OBJECT_STAT
	void SetCurrentExecutingScriptParams( const std::string& srcName, int lineIndex)
	{
		m_currentExecutingScriptSrc = srcName;
		m_currentExecutingScriptLine = lineIndex;
	}

	void GetCurrentExecutingScriptParams( std::string& srcName, int& lineIndex)
	{
		srcName = m_currentExecutingScriptSrc;
		lineIndex = m_currentExecutingScriptLine;
	}
//#endif

	mlSyncCallbackManager * GetSyncCallbackManager();
	mlWorldEventManager*	GetWorldEventCallbackManager();

	omsContext* GetContext(){ return mpContext; }
	JSContext* GetJSContext(){ return cx; }
//	mlEModes GetMode(){ return meMode; }
	bool IsGlobalObjectEvent(const wchar_t* apwcName,const wchar_t* apwcEvent,char &aidObj,char &aidEvent);
	static mlString RefineResPath(mlString aXPath,const wchar_t* apwcResPath);
	mlString RefineResPathEl(mlRMMLElement* apElem,const wchar_t* apwcResPath);
	mlString GetFullResPath(const wchar_t* apwcResPath);
	// �������� ��� ���������� ������� �� �������������� �������
	mlString ReplaceAliasesToModuleIDs(const mlString awsPath);
	void RemoveScene(mlRMMLElement* apScene){
		mSceneDatas.RemoveSceneData(apScene);
	}
	void CallGC(mlRMMLElement* apMLEl = NULL){ mpElemRefToClear = apMLEl; mbNeedToCallGC = true; }
#ifndef JS_THREADSAFE
	bool Updating(){ return mbUpdating; }
#else
	bool Updating(){ return mbUpdating && (JS_GetContextThread(cx) == JS_GetCurrentThreadId()); }
#endif
	// =true, ���� ��������� Update-�� ����� ���������� ����� ��� ��������� �������� �����
	bool IsSceneStructChanged(){ return muStructChangedUpdCntr!=0; }
	void SceneStructChanged(){ /*ResetEvent();*/ muStructChangedUpdCntr=2; }
	//	bool IsSceneLoaded(){ return mbSceneLoaded; }
	void SaveInternalPath(mlSynchData &Data,mlRMMLElement* apMLEl);
	mlRMMLElement* LoadAndCheckInternalPath(mlSynchData &Data);
	void GetRMMLVersion(unsigned &auMajor, unsigned &auMinor){
		auMajor=RMML_VERSION_MAJOR;
		auMinor=RMML_VERSION_MINOR;
	}
	mlRMMLElement* FindElemByStringID(const wchar_t* apwcStringID);
	mlBuildEvalCtxt* GetBuildEvalCtxt(){ return &mBuildEvalCtxt; }
	mlRMMLComposition* GetScene(){ return mpScene; }

	mlRMMLElement* GetMyAvatar();

	// � ����������� SM-� ���������� ������� ����������
	void DestroyPluginLater(moMedia* apMO);

	// JS-������ ��������� � ��������� ������, 
	// ������� ��������� ������� � ��������� ������ ��� ������������ �������� � Update-�
	void AddObjToDeleteLater(void* apObj, int aiType);

	bool IsAlreadyIncluded(const wchar_t* apwcSrc){ return mScriptPreloader->IsAlreadyIncluded(apwcSrc); }

	// �������� JavaScript call stack � ���� ������
	mlString GetJSCallStack();

	void OptimizeLists();
	
	mlString mwsAllocComment;
	void SetAllocComment(const wchar_t* apwcAllocComment);
	const wchar_t* GetAllocComment(){ return mwsAllocComment.c_str(); }

	// �������������� ������ ������
	void InitClassObject(mlRMMLElement* apClass);

	// ������� �� Classes ������ �� ��������� �����
	jsval FreeClass(const mlString wsClassName);

	// �������� ���������� ������-�� JS-�������
	void JSScriptExecutionStarted(void* aCurrentScript){ if(mpScriptTerminationThread.get() != NULL) mpScriptTerminationThread->ScriptExecutionStarted(aCurrentScript); }

	// JS-������ ����������
	void JSScriptExecuted(void* aCurrentScript){ if(mpScriptTerminationThread.get() != NULL) mpScriptTerminationThread->ScriptExecuted(aCurrentScript); }

	// ���������� "���������" �������� JS-������
	void JSScriptCancelTerimation(){ JSScriptExecuted(NULL); }

	// �������� timeout ���������� JS-������� (���������� �� mlScriptTerminationThread)
	void JSScriptExecutionTimeout(bool abInterrupted, int aiTimeout);

	// �������� ������ ����� ���������� �������� (��� callback-������� mlScriptTerminationThread)
	mlScriptTerminationThread* GetScriptTerminationThread(){ return mpScriptTerminationThread.get(); }

	void DontUpdateOneTime(){ mbDontUpdateOneTime = true; }

protected:
	static JSClass JSRMMLClassesClass;
	virtual mlresult InitGlobalJSObjects(JSContext* cx , JSObject* obj);

	void OptimizeList(v_elems& avElements);
	void DumpList(v_elems& avElements, const wchar_t* apwcListName);
	void DumpLists();	

// �������� �������� ��������� ������� �� ��������� update-��
	struct mlNextUpdatesEvent{
		mlSynchData mData;
		int miEventID;
		int muUpdateCounter;	// ������� update-�� ��� �������� ����������
		bool mbForceBUMCheck; // ���� �� ���� ��������� ����� ������ ��� ������?
		mlNextUpdatesEvent(const mlSynchData& aData, int aiEventID, int auUpdateCounter, bool abForceBUMCheck){
			mData.put(aData.data(), aData.size());
			miEventID = aiEventID;
			muUpdateCounter = auUpdateCounter;
			mbForceBUMCheck = abForceBUMCheck;
		}
	};
	typedef MP_VECTOR<mlNextUpdatesEvent*> mlNextUpdatesEvents;
	mlNextUpdatesEvents mvlNUEvents;

// ��������� ��������� ��������� ������� (�������� && � || � ���������)
protected:
	struct EventToRestore{
		mlRMMLElement* pElem;
		char evID;
		jsval val;
		EventToRestore()
		{
			pElem = NULL;
			evID = 0;			
		}
		EventToRestore(mlRMMLElement* apElem, char aEvID, jsval aVal){
			pElem = apElem;
			evID = aEvID;
			val = aVal;
		}
	};
public:
	// ���� ������ �������� ������� ��� ��������� ���������� 
	bool mbCheckingEvents;
	bool IsCheckingEvents(){ return mbCheckingEvents; }
	mlRMMLElement* mpSELMLEl; // �������, ������� ���� ������������� ���������� � ������ ECM_Check
	// ������ �������, �������� ������� ���� ��������������� ����� mpvFiredEvents = NULL
	MP_VECTOR<EventToRestore> mvEventsToRestore;
	void RestoreEventJSVals();
	// ������ �������, ������� �� ���� sequencer � ������� ��� ���������, 
	// � GetProperties �� ��� ������������ true, ���� mpvFiredEvents != NULL
	std::vector<EventSender>* mpvFiredEvents; // ���� != NULL, ������ checking events mode
//	std::vector<EventSender>* mpvWaitingEvents; // ���� != NULL, ������ get waiting events mode
	int CheckEvent(char aIdEvent, mlRMMLElement* apSender, jsval *vp);

// ���������� ����������� JS-������� "Mouse"
public:
	int miCursorType;
	int miHandCursorType;
	// �������� ��� ������ �� �������, ��������� c �������� ��������� 
	// (��������, ���� ������� ���������)
	// ���� apMLEl==NULL, ������������ ������ �� ����� ��������
	void ResetMouse(mlRMMLElement* apMLEl);
protected:
	JSObject* mjsoMouse;
	#define ML_MOUSE_EVENTS_COUNT 9
	v_elems_dc maMouseListeners[ML_MOUSE_EVENTS_COUNT];
	void HandleMouseEvent(oms::omsUserEvent &ev);
	void CallVisibleListeners(const v_elems* apvElems, int aiEventIdx);
	mlPoint mMouseXY;
	mlRMMLElement* mpButUnderMouse;
	mlButtonUnderMouse mBUM; // �������������� ���� �� ������� ��� ������
	v_elems* mpBUMParents;	// ������������ ����������-������ ������ ��� ������
	// mlRMMLVisible* mpVisibleUnderMouse;
	// mlRMMLElement* mp3DObjectUnderMouse; // 3D-������, ������� ��� ������
	// mlRMMLElement* mpTextureObjectUnderMouse; // RMML-������-��������, ������� ��� ������
	// mlPoint mpntTextureObjectMouseXY;	// ���������� ���� � ��������� �������-��������
	bool mbUseHandCursorChanged;
	bool m_realityLoadedEventWasGeneratedAlready;
	mlRMMLElement* mpButPressed;
	v_elems* mpBPParents;	// ������������ ����������-������ ������� ������
	mlRMMLElement* mpButRPressed;
	v_elems* mpBRPParents;	// ������������ ����������-������ ������� ������ �������� ���� ������
	__int64 alLastInpEvTime;
	__int64 mlTime;
	bool mbBigCursor;
	bool mbDragging;
	struct mlModalButton{
		mlRMMLElement* mpButton;
		mlRMMLVisible* mpVisible;	// ���� mpButton - 3D-������, �� mpVisible - ������ �� viewport
		mlModalButton(mlRMMLElement* apButton, mlRMMLVisible* apVisible = NULL){
			mpButton = apButton;
			mpVisible = apVisible;
		}
	};
	// ������, ������� �������� �����.
	// ��� ���������� �� ���� ����� ������������ ���, ���� �� �� �������� ����� ��� �� �������� ���
	mlModalButton mCaptureMouseObject; 

	bool mbForceSetCursor;

//	bool mbMouseEnabled;
//	void DisableMouse();
//	void EnableMouse();
	void GetMouseState(mlSynchData &aData);
	void SetMouseState(mlSynchData &aData);
	inline bool GetAbsEnabled(mlRMMLVisible* apVisible);
	inline void SetCursorForUnderMouse(bool abForce = false);	

	struct mlMouseEventParams
	{
		int doubleClick;
		int leftButton; 
		int middleButton; 
		int rightButton;
		int ctrl;
		int shift;

		mlMouseEventParams();
	//	void Update(const mlMouseEventParams &aMEP);
	};

	mlMouseEventParams mMouseEventParams;

	void SetMouseEventBoolProperty(const char* apcName, int aiValue);
	void SetMouseEventParams(/*const mlMouseEventParams aMEP, bool abSetEventObjProps = true*/);
	void DeleteMouseEventParams();

public:
	// � ���� ������ ����� ������������ ���������� ����������� ������� MouseMove
	MP_VECTOR<unsigned short> mvSkipedMouseMoves;
	bool addMouseListener(char aidEvent,mlRMMLElement* apMLEl);
	bool removeMouseListener(char aidEvent,mlRMMLElement* apMLEl);
	void NotifyMouseListeners(char chEvID);
	void hideMouse();	// { mpContext->mpWindow->SetCursor(0); }
	void showMouse();   // { mpContext->mpWindow->SetCursor(1); }
	mlPoint GetMouseXY(){ return mMouseXY; }
	void SetMouseXY(mlPoint &aPoint){ mMouseXY=aPoint; }
	mlPoint GetTextureObjectMouseXY(){ return mBUM.mLocalXY; }
	_int64 GetTime(){ return mlTime; }
	void SetNormalCursor(int aiCursor=-1, bool abForce = false);	//{ mpContext->mpWindow->SetCursor(1); }
	void SetHandCursor(int auCursor=0, bool abForce = false);	//{ mpContext->mpWindow->SetCursor(2); }
	void HideCursor();		//{ mpContext->mpWindow->SetCursor(0); }
	void ShowCursor();		// {}
	void UseHandCursorChanged(mlRMMLElement* apMLEL){
		if(apMLEL == mpButUnderMouse)
			mbUseHandCursorChanged = true;
	}
	void SetBigCursor(bool abBig);
	bool IsCursorBig(){ return mbBigCursor; }
	void ForceSetCursor(){ mbForceSetCursor = true; }
	void NotifyMouseModalButtons();
	mlButtonUnderMouse* GetButtonUnderMouse(){ return &mBUM; }
	mlRMMLVisible* GetVisibleUnderMouse(){ return mBUM.mpVisible; }
	mlRMMLElement* Get3DObjectUnderMouse(){ return mBUM.mp3DObject; }
	mlRMMLElement* GetTextureObjectUnderMouse(){ return mBUM.mpTextureObject; }
	void ResetMouseTarget();
	// ��������� ���� (��� ����������� ���� ����� ���������� ������ � ����� �������)
	bool CaptureMouse(mlRMMLElement* apMLEl);
	bool ReleaseMouse(mlRMMLElement* apMLEl);
	bool SendMouseEvent(unsigned int eventId, unsigned int wparam, uintN argc, jsval *argv, jsval *rval);
	// ���������� � ���������� ������ Event ���������� ���������� �� mBUM
	void SetTextureXYToEventGO(mlRMMLElement* apMLEl = NULL);

// ���������� ����������� JS-������� "Key"
protected:
	JSObject* mjsoKey;
	#define ML_KEY_EVENTS_COUNT 3
	v_elems_dc maKeyListeners[ML_KEY_EVENTS_COUNT];
	void HandleKeyEvent(oms::omsUserEvent &aEvent);
	unsigned short muKeyCode;
	unsigned short muKeyStates;
	unsigned short muScanCode;
	unsigned short muAscii;
	unsigned short muUnicode;
	mlRMMLElement* mpButFocused;
	mlSynchData mFocusedButInternalPath;
	void ResetKey(bool bUI = true);
	void GetKeyState(mlSynchData &aData);
	void SetKeyState(mlSynchData &aData);
	struct mlPressedKey{ 
		unsigned short muKeyCode;
		unsigned short muKeyStates;
		unsigned short muScanCode;
		unsigned short muAscii;
		unsigned short muUnicode;
		__int64 miTime;	
		mlPressedKey(){}
		mlPressedKey(unsigned short auKeyCode, unsigned short auKeyStates, unsigned short auScanCode, 
			unsigned short auAscii, unsigned short auUnicode, __int64 aiTime)
		{
			muKeyCode = auKeyCode;
			muKeyStates = auKeyStates;
			muScanCode = auScanCode;
			muAscii = auAscii;
			muUnicode = auUnicode;
			miTime = aiTime;
		}
	};
	typedef MP_VECTOR<mlPressedKey> PressedKeyVector;
	PressedKeyVector mvPressedKeys;
	bool IsEqual(const mlPressedKey& aKey1, const mlPressedKey& aKey2);
	void AddPressedKey(const mlPressedKey& aKey);
	void RemovePressedKey(const mlPressedKey& aKey);
		// ��������� ������ ������� ������ �� ������� "������������" � ���� ��� �� ����� ���� ��� ������, �� ���������� ��� ��� onKeyUp
	void CheckPressedKeys();
	
public:
	bool addKeyListener(char aidEvent,mlRMMLElement* apMLEl);
	bool addGamepadListener(char aidEvent,mlRMMLElement* apMLEl);
	bool removeKeyListener(char aidEvent,mlRMMLElement* apMLEl);
	bool removeGamepadListener(char aidEvent,mlRMMLElement* apMLEl);
	bool OnControllerFound() override;
	void OnControllerLost() override;
	short GetGamepadKey();
	short GetLeftThumbState();
	short GetRightThumbState();
	const char* GetLeftThumbValue();
	const char* GetRightThumbValue();
	short GetXThumbValue(JSString* value);
	short GetYThumbValue(JSString*value);
	unsigned short GetLeftTrigger();
	unsigned short GetRightTrigger();
	unsigned short GetKeyCode(){ return muKeyCode; }
	unsigned short GetKeyStates(){ return muKeyStates; }
	unsigned short GetKeyScanCode(){ return muScanCode; }
	void SetKeyCode(unsigned short auKeyCode){ muKeyCode=auKeyCode; }
	unsigned short GetAsciiKeyCode(){ return muAscii; }
	unsigned short GetUnicodeKeyCode(){ return muUnicode; }
	void SetFocus(mlRMMLElement* apMLEl=NULL, bool abDontCheckActive = false);
	mlRMMLElement* GetFocus(){ return mpButFocused; }
private:
	void handleGamepadButton(WORD button, WORD newButtons);
	void HandleThumb(short &curState, short &thumb, short newState);
	short GetThumbState(SHORT xPower, SHORT yPower, SHORT deadzone);
	short InternalGetLeftThumbState();
	short InternalGetRightThumbState();
	const char* GetThumbValue(SHORT xPower, SHORT yPower, SHORT deadzone);
	unsigned short GetTrigger(BYTE power, SHORT threshold);
	std::vector<WORD> m_XInputButtons;
	IXBOXController *m_ctrl;
	WORD m_prevButtons;
	WORD m_pressedButton;
	short m_leftThumbState, m_pressedLeftThumb;
	short m_rightThumbState, m_pressedRightThumb;

	//Typedef for extern function for XBOXController
	HINSTANCE hGamepadDll;
	
	typedef IXBOXController* (*newControllerType)();
	newControllerType NewController;

// ���������� ��������� ������� ���������� ��������
public:
	// v_elems mvUIGOListeners;
	// ���������� ��������� ������� ����������� �������
	void SetEventListener(char aidSender,char aidEvent,mlRMMLElement* apListener);
	// ������� ��������� �� ������ ������� ����������� ������� 
	void RemoveEventListener(char aidSender,mlRMMLElement* apMLEl);
	// ������ ��� ����������� �������
	mlString GetEventName(char aidSender,char aidEvent);
	// ������� ��������� �� ���� �������'��
	void RemoveEventListener(mlRMMLElement* apMLEl);

private:
	void CreateSyncObjectsIfNeeded(const bool isNormalMode);

// ���������� ����������� ������� "Player"
private:
	JSObject* mjsoPlayer;
	#define ML_PLAYER_EVENTS_COUNT 30
	v_elems_dc maPlayerListeners[ML_PLAYER_EVENTS_COUNT];
	bool addPlayerListener(char aidEvent,mlRMMLElement* apMLEl);
	bool removePlayerListener(char aidEvent,mlRMMLElement* apMLEl);
	void ResetPlayer();
	void GetPlayerState(mlSynchData &aData);
	void SetPlayerState(mlSynchData &aData);
	bool SetPlayerEventData(char aidEvent, mlSynchData &Data);
	void SavePlayerInternalPath(mlSynchData &Data);
	void OnShowInfoStateInternal(mlSynchData& aData);
	// "module"
	#define ML_MODULE_EVENTS_COUNT 8
	v_elems_dc maModuleListeners[ML_MODULE_EVENTS_COUNT];
	bool addModuleListener(char aidEvent,mlRMMLElement* apMLEl);
	bool removeModuleListener(char aidEvent,mlRMMLElement* apMLEl);
	void ResetModule();
	mlresult LoadEduMeta(mlXMLArchive* pXMLArchive, JSObject* ajsoModule=NULL, bool abGetInfo=false);
	mlresult LoadTechMeta(mlXMLArchive* pXMLArchive, JSObject* ajsoModule=NULL, bool abGetInfo=false);
	mlresult LoadTechMetaDefault();
	void LoadTechMetaScene( JSObject* jsaScenes, const wchar_t *aTitle, const wchar_t *aPath);
	mlresult AddResourceModuleAliases(const mlString asResourceModuleID, const mlString asAlias);
	void TranslateKeyCode();

	void OnWindowFileDropInternal(mlSynchData& aData);
	void OnWindowMovedInternal();
	void OnKinectFoundInternal();
	void OnOculusRiftFoundInternal();
	void OnVisibilityDistanceKoefChangedInternal(mlSynchData& aData);
	void OnWindowSizeChangedInternal(mlSynchData& aData);

	MP_WSTRING mwsModuleID;
public:
	int miCurrentSceneIndex;
	JSObject* mjsoModule;
	void NotifyPlayerListeners(char chEvID);
	void NotifyPlayerOnRestoreUnsynchedListeners();
	void NotifyPlayerOnDisconnectListeners();
	void NotifyPlayerOnRestoreStateUnsynchedListeners();
	void NotifyPlayerOnWindowSizeChanged( const int& auClientWidth, const int& auClientHeight);
	void NotifyPlayerOnErrorReportListeners();
	void NotifyModuleListeners(char chEvID);
	void NotifyCurrentSceneIndex();
	void NotifyUnloadScene();
	std::wstring GetModuleID(){ return mwsModuleID; }
	void SetModuleID(const wchar_t* apwcModuleID){ mwsModuleID = apwcModuleID==NULL?L"":apwcModuleID; }
	bool UIButPressed(){ 
		if(mpScene==NULL) return true;
		return (mpButPressed!=NULL && mpButPressed->GetScene()==mpUI); 
	}
	mlresult LoadModuleMeta(const wchar_t* apwcModuleID, JSObject* ajsoModule, bool abGetInfo=false);
	mlresult Load(const wchar_t* asSrc, bool abAsynch){ return LoadXML(asSrc, false, abAsynch);	}
	mlresult LoadInto(const wchar_t* asSrc, mlRMMLElement* apParent);

// ��������� ����������� ������� Player.server
#ifdef CSCL
friend void CreateServerJSObject(JSContext *cx, JSObject *ajsoPlayer);
friend void CreateRecordEditorJSObject(JSContext *cx, JSObject *ajsoPlayer);
friend void CreateUrlJSObject(JSContext *cx, JSObject *ajsoPlayer);
friend void CreateVoiceJSObject(JSContext *cx, JSObject *ajsoPlayer);
friend void CreateVideoCaptureJSObject(JSContext *cx, JSObject *ajsoPlayer);
friend void CreateCodeEditorJSObject(JSContext *cx, JSObject *ajsoPlayer);
private:
	JSObject* mjsoServer;
	JSObject* mjsoUrl;
	JSObject* mjsoVoice;
	JSObject* mjsoVideoCapture;
	JSObject* mjsoCodeEditor;
	mlRequestList mRequestList;
	mlServerCallbackManager* mServerCallbackManager;
	mlServiceCallbackManager* mServiceCallbackManager;
	
private:
	#define ML_SERVER_EVENTS_COUNT 14
	v_elems_dc maServerListeners[ML_SERVER_EVENTS_COUNT];
	bool addServerListener(char aidEvent,mlRMMLElement* apMLEl);
	bool removeServerListener(char aidEvent,mlRMMLElement* apMLEl);
	void NotifyServerListeners(char chEvID);
	void OnServerConnectedInternal(mlSynchData& aData);
	void OnServerConnectionErrorInternal(mlSynchData& aData);
	void OnServerDisconnectedInternal(mlSynchData& aData);
	void OnObjectInServerNotFound(mlSynchData& aData);
	void onSyncServerErrorInternal(mlSynchData& aData);

	omsresult DeleteObjectMyAvatar( mlMedia* apObject);
	omsresult DeleteObjectsOnLogout();
	void OnLogoutInternal(mlSynchData& aData);
	void OnNotifyServerTOInfoInternal(mlSynchData& aData);
	void OnSeancesInfoReceivedInternal(mlSynchData& aData);
	void OnEventsInfoIntoLocationReceivedInternal(mlSynchData& aData);
	void OnProxyAuthErrorInternal(mlSynchData& aData);
	void OnConnectToServerErrorInternal(mlSynchData& aData);
	void OnErrorCreateTempLocationInternal(mlSynchData& aData);

	void OnIPadSessionChangeSlideInternal(mlSynchData& aData);

	void OnInfoMessageInternal(mlSynchData& aData);

public:
	void ResetPlayerServer();
	mlRequestList* GetRequestList(){ return &mRequestList; }
	void SetServerCallbacks(){ mServerCallbackManager->SetCallbacks(cx); }
	void SetServiceCallbacks(){ mServiceCallbackManager->SetCallbacks(cx); }
	JSObject* GetPlServerGO(){ return mjsoServer; }
	JSObject* GetPlayerGO(){ return mjsoPlayer; }
	JSObject* GetPlUrlGO(){ return mjsoUrl; }
	JSObject* GetPlVoiceGO(){ return mjsoVoice; }
	JSObject* GetPlVideoCaptureGO(){ return mjsoVideoCapture; }
	JSObject* GetPlCodeEditorGO(){ return mjsoCodeEditor; }
	int SendRequestForObjectStatus(unsigned int auObjectID, unsigned int auBornRealityID, mlSynchLinkJSO* apObjectLink);
	void onReceivedMapServerObjectStatus(unsigned int auObjectID, unsigned int auBornRealityID, int aiStatus, unsigned int uiUserData);
#endif


	// ��������� ����������� ������� Player.recordEditor
#ifdef CSCL
	friend void CreateServerJSObject(JSContext *cx, JSObject *ajsoPlayer);
	friend void CreateRecordEditorJSObject(JSContext *cx, JSObject *ajsoPlayer);
	friend void CreateUrlJSObject(JSContext *cx, JSObject *ajsoPlayer);
	friend void CreateVoiceJSObject(JSContext *cx, JSObject *ajsoPlayer);
private:
	JSObject* mjsoRecordEditor;

private:
#define ML_RECORD_EDITOR_EVENTS_COUNT 14
	v_elems_dc maRecordEditorListeners[ML_RECORD_EDITOR_EVENTS_COUNT];
	bool addRecordEditorListener(char aidEvent,mlRMMLElement* apMLEl);
	bool removeRecordEditorListener(char aidEvent,mlRMMLElement* apMLEl);
	void NotifyRecordEditorListeners(char chEvID);

	void OnCutOperationCompleteInternal(mlSynchData& aData);
	void OnCopyOperationCompleteInternal(mlSynchData& aData);
	void OnPasteOperationCompleteInternal(mlSynchData& aData);
	void OnDeleteOperationCompleteInternal(mlSynchData& aData);
	void OnUndoOperationCompleteInternal(mlSynchData& aData);
	void OnRedoOperationCompleteInternal(mlSynchData& aData);
	void OnSaveOperationCompleteInternal(mlSynchData& aData);
	void OnSaveAsOperationCompleteInternal(mlSynchData& aData);
	void OnPlayOperationCompleteInternal(mlSynchData& aData);
	void OnPauseOperationCompleteInternal(mlSynchData& aData);
	void OnRewindOperationCompleteInternal(mlSynchData& aData);
	void OnSeekOperationCompleteInternal(mlSynchData& aData);
	void OnOpenOperationCompleteInternal(mlSynchData& aData);

public:
	JSObject* GetPlRecordEditorGO(){ return mjsoServer; }
#endif


// ��������� ����������� ������� Player.server.recorder
#ifdef CSCL
friend void CreateSRecorderJSObject(JSContext *cx, JSObject *ajsoPlServer);
private:
	mlWorldEventManager*		mWorldEventManager;
	mlSyncCallbackManager*	mSyncCallbackManager;
	JSObject* mjsoSRecorder;
	unsigned int muiPlayingEventID;
	unsigned int muiPlayingRecordID;
public:
	#define ML_RECORDER_EVENTS_COUNT 38
	v_elems_dc maRecorderListeners[ML_RECORDER_EVENTS_COUNT];
	bool addRecorderListener(char aidEvent,mlRMMLElement* apMLEl);
	bool removeRecorderListener(char aidEvent,mlRMMLElement* apMLEl);
	void ResetRecorderEvents();
	void NotifyRecorderListeners(char chEvID);
	void NotifyOnDesktopFileSaved( wchar_t* fileName, char* target);
	void NotifyOnSharingSessionStarted(const char* alpcTarget, const char* alpcSessionKey, const char* alpcFileName);
	void NotifyOnSharingPreviewCancel(const char* alpcTarget);
	void NotifyOnSharingPreviewOK(const char* alpcTarget);
	void NotifyOnSharingFileSaved(const char* alpcTarget, const char* alpcFileName);
	void NotifyOnSharingViewSessionStarted(const char* alpcTarget, const char* alpcSessionKey, bool canRemoteAccess);
	void NotifyOnSharingSetBarPosition(const char* alpcTarget, double sound, double duration);
	void NotifyOnSharingVideoEnd(const char* alpcTarget);	
	void NotifyOnTextureSizeChanged(const char* alpcTarget, unsigned int width, unsigned int height);
	void NotifyOnSessionClosed(const char* alpcTarget);
	void NotifyOnViewSessionClosed(const char* alpcTarget);
	void NotifyOnRemoteAccessModeChanged(const char* alpcTarget, bool canRemoteAccess);
	void NotifyWebCameraState(const char* alpcTarget, int aCode);
	void NotifySharingEventByCode(const char* alpcTarget, int stateCode);
	void NotifyGlobalMouseEvent(unsigned int wParam, float aX, float aY);
	void NotifyAvatarImageFileSaved( wchar_t* tmpFileName, const char* fileName);
	void NotifyAfterCreatingOfObjects(int aiObjectsLoaded, int aiToLoadObjectCount);
	void NotifyBeforeCreatingOfObjects();
	void SetSyncCallbacks(){ mSyncCallbackManager->SetCallbacks(cx); }
	JSObject* GetSRecorderGO(){ return mjsoSRecorder; }
	void onSeancePaused();
	void onSeancePlayed();
	void onSeanceRewinded();
	void onSeanceFinishedPlaying();
	void onSeanceLeaved();
#endif
	unsigned int GetCurrentEventID() { return muiPlayingEventID; }
	unsigned int GetPlayingRecordID() { return muiPlayingRecordID; }
	void SetCurrentEventID(unsigned int aPlayingEventID) { muiPlayingEventID = aPlayingEventID; }
	void SetPlayingRecordID(unsigned int aPlayingRecordID) { muiPlayingRecordID = aPlayingRecordID; }

// ��������� ����������� ������� Player.map
private:
	mlMapCallbackManager* mMapCallbackManager;
public:
	JSObject* mjsoMap;
	#define ML_MAP_EVENTS_COUNT 11
	v_elems_dc maMapListeners[ML_MAP_EVENTS_COUNT];
	bool addMapListener(char aidEvent,mlRMMLElement* apMLEl);
	bool removeMapListener(char aidEvent,mlRMMLElement* apMLEl);
	void ResetMap();
	void NotifyMapListeners(char chEvID);
	void NotifyMapOnMyAvatarCreatedListeners();
	void SetMapCallbacks(){ mMapCallbackManager->SetCallbacks(cx); }
	JSObject* GetPlMapGO(){ return mjsoMap; }
	JSObject* LocationToJSObject(const ml3DLocation& aLocation, bool abWithParams);

	// ��������� ����������� ������� Player.reality
#ifdef CSCL
public:
	unsigned int uCurrentRealityID;
	__forceinline void SetCurrentRealityID( unsigned int auRealityID) {
		uCurrentRealityID = auRealityID;
	}
#endif

	// ��������� ����������� ������� Player.url
#ifdef CSCL
public:
#define ML_URL_EVENTS_COUNT 1
	v_elems_dc maUrlListeners[ML_URL_EVENTS_COUNT];
	bool addUrlListener(char aidEvent,mlRMMLElement* apMLEl);
	bool removeUrlListener(char aidEvent,mlRMMLElement* apMLEl);
	void ResetUrl();
	void NotifyUrlListeners(char chEvID);
	void NotifyUrlOnSet();
#endif

	// ��������� ����������� ������� Player.voice
#ifdef CSCL
public:
	mlVoipCallbackManager* mVoipCallbackManager;
	mlDesktopCallbackManager* mDesktopCallbackManager;
	void SetDesktopCallbacks(){ mDesktopCallbackManager->SetCallbacks(cx); }
#define ML_VOICE_EVENTS_COUNT 27
#define ML_VIDEOCAPTURE_EVENTS_COUNT 2
	v_elems_dc maVoiceListeners[ML_VOICE_EVENTS_COUNT];
	v_elems_dc maVideoCaptureListeners[ML_VIDEOCAPTURE_EVENTS_COUNT];
#define ML_CODEEDITOR_EVENTS_COUNT 2
	v_elems_dc maCodeEditorListeners[ML_CODEEDITOR_EVENTS_COUNT];
	bool addVoiceListener(char aidEvent,mlRMMLElement* apMLEl);
	bool removeVoiceListener(char aidEvent,mlRMMLElement* apMLEl);
	void SetVoipCallbacks(){ mVoipCallbackManager->SetCallbacks(cx); }
	void ResetVoice();
	void NotifyVoiceListeners(char chEvID);
	void NotifyVoiceOnConnectSuccess();
	void NotifyVoiceOnConnectStart();
	void NotifyVoiceOnConnectFailed( char* aVoipServerIP);
	void NotifyVoiceOnConnectLost();
	void NotifyVoiceOnDisconnected();
	void NotifyVoiceOnKicked();
	void NotifyVoiceOnAddRoom(const char* roomName);
	void NotifyVoiceOnRemoveRoom(const char* roomName);
	void NotifyVoiceOnUserTalking(const char* userName);
	void NotifyVoiceOnUserStoppedTalking(const char* userName);
	void NotifyVoiceOnUserFileSaved(const char* userName, const char* fileName);
	void NotifyVoiceOnAudioFilePlayed(const char* userName, const char* fileName);
	void NotifyVoiceOnRecordStarted();
	void NotifyVoiceOnRecordStopped(unsigned int auErrorCode);
	void NotifyVoiceOnEvoClientWorkStateChanged(unsigned int aSessionType, unsigned int aWorkState, unsigned int auErrorCode);
	void NotifyVoiceOnSRModeStatusChanged(unsigned int auStatus);
	void NotifyVoiceOnNotified(unsigned int auCode, unsigned int auData);
	// ��� ������������ ������
	void NotifySetInputDevice(unsigned int auCode, int aiToIncludeSoundDevice);
	void NotifySetOutputDevice(unsigned int auCode, int aiToIncludeSoundDevice);
	void NotifyGetInputDevices( const wchar_t *apMicDeviceNames, const wchar_t *apMixDeviceNames);
	void NotifyGetOutputDevices(wchar_t *apDeviceNames);
	void NotifyAutoFindedMicName(const wchar_t *apDeviceGuid, const wchar_t *apDeviceName, const wchar_t *apDeviceLineName);
	void NotifyDeviceErrorCode(int anErrorCode);
	void NotifyReInitializeVoipSystem();
#endif

// ��������� ����������� ������� Player.videoCapture
	bool addVideoCaptureListener(char aidEvent, mlRMMLElement* apMLEl);
	void NotifyVideoCaptureListeners(char chEvID);
	bool removeVideoCaptureListener(char aidEvent,mlRMMLElement* apMLEl);
	void NotifyVideoCaptureError();
	void ResetVideoCapture();
	void OnVideoCaptureError(const unsigned int errorCode);
	void OnVideoCaptureErrorInternal(mlSynchData& aData);

	// ��������� ����������� ������� Player.videoCapture
	bool addCodeEditorListener(char aidEvent, mlRMMLElement* apMLEl);
	void NotifyCodeEditorListeners(char chEvID);
	bool removeCodeEditorListener(char aidEvent,mlRMMLElement* apMLEl);
	void OnCodeEditorClosedInternal(mlSynchData& aData);
	void NotifyCodeEditorClosed();
	void ResetCodeEditor();
	void OnCodeEditorClose(const wstring& text);

// ���������� ����������� ������� "Event"
private:
	JSObject* mjsoEvent;
public:
	void ResetEvent();
	JSObject* GetEventGO(){ return mjsoEvent; }

// ���������� ������������� 
private:
	typedef std::vector<cm::cmObjectIDs> CVectorID;

	omsresult HandleObjectNotFound( unsigned int aObjectID, unsigned int aBornReality, unsigned int aRealityID);
	bool NeedToUpdateRMNow(const __int64 alTime, int &aiDeltaTime, const bool isNormalMode);
	void UpdateAllElements(const __int64 alTime, const bool isNormalMode);
	void GlobalTimeShifted(const __int64 alTime, const __int64 alTimeShift);
	
	// ����������� ������ ����������� ���������
	bool RebuildUpdatableElements(const __int64 alTime);
	// ��������� ����������� ��������
	void UpdateUpdatableElements(const __int64 alTime);

	// ������� ���������������� �������
	void CreateSyncObjects( unsigned int aTimeQuota);
	// ��������� ��������� ��������� ����������������� �������
	void ApplySyncState( const syncObjectState* const pState, CVectorID* aNotFoundSynchObjectIDs = NULL);
	// ��������� ���������������� �������
	void UpdateSyncElements();
	// ��������� �������, ��������� � IPad
	void UpdateIPadElements();
	// ���������� �� ������ ������� ���������������� ��������
	void SendSyncElements();


	// ���������� ������ �� ��������� ���������������� ��������, ������� ����� �� ����������� � �������.
	void SendQueryOldSyncElements( ObjectsVector &objects, unsigned int aCurrentTime, unsigned int aWaitTime, mlRMMLElement* pNonSendElem = NULL);
	// ����������� ��������� ���������������� ��������, ������� ����� �� ����������� � �������. ����������� #414
	void QueryOldSyncElements();

	// ������� ����������� �������������� ������� �������� ���� ������ ������.
	void DeleteUnicalID( unsigned int auiID, unsigned int auiBornRealityID);

	mlNextUpdateDTimeList mlNUDTime;
	unsigned long mulSynchEventsGlobalTime;
	__int64 mlLastUpdateTime;
	int miLastUpdateDeltaTime;
	mlEModes mePrevMode;
	bool mbNeedToShiftGlobalTime;
	mlRMMLElement* mbUIScriptActivatingStartElem;
	MP_WSTRING msClientStringID;
	typedef MP_VECTOR<mlSynchLinkJSO*> VecSynchLinks;
	VecSynchLinks mvTrackingLinks;

	// ����� ���������� ������� ������ ���������. ����������� #414
	__int64 mlLastOldSyncElementsTime;
	__int64 mlLastOldAvatarsTime;

	// ������ ���������� ��������
	CScriptedObjectManager scriptedObjectManager;

public:
	mlRMMLElement* GetScriptedObjectByID(unsigned int auObjID, unsigned int auRealityID, bool aAccurately = true);
//	void ChangeObjectBornReality( mlMedia* apMedia, unsigned int auBornRealityID);
	omsresult DeletePreviousRealityObjects(const std::vector<unsigned int>& aExcludeObjects);

public:
	enum SynchModes{
		synchMode1,
		synchMode2,
		synchMode3
	} mSynchMode;
	bool mbDontSendSynchEvents;
	bool mbSceneMayHaveSynchEls;
	unsigned short muExternEventID;
	void SetSynchJSHook();
	static void* SynchJSHook(JSContext *cx, JSStackFrame *fp, JSBool before,
						JSBool *ok, void *closure);
	JSObject* CreateElementByClassName(const char* apszClassName){
		return mRMML.CreateElement(cx,apszClassName);
	}
	void GetSceneData(mlRMMLElement* apScene, mlSynchData &aData);
	void SetSceneData(mlRMMLElement* apScene, mlSynchData &aData);
	void UpdateNextUpdateDeltaTime(int aiDeltaTime);
	unsigned long GetSynchEventsGlobalTime(){ return mulSynchEventsGlobalTime; }
	void SetActivationUIScriptsFlag(mlRMMLElement* apStartElem);
	void ResetActivationUIScriptsFlag(mlRMMLElement* apElem);
	bool IsUIScriptIsActivating(){ return mbUIScriptActivatingStartElem != NULL; }
	void SetSynchMode(SynchModes aSynchMode){ mSynchMode = aSynchMode; }
	SynchModes GetSynchMode(){ return mSynchMode; }
	const wchar_t* GetClientStringID(){ return msClientStringID.c_str(); }
	void SetClientStringID(const wchar_t* apwcStringID);
	// ��� ����������
	void ElementCreating(mlRMMLElement* apMLEl);
	void ElementDestroying(mlRMMLElement* apMLEl);

	void MayBeAddedToScene(mlRMMLElement* apMLEl); // � rmml-�������� parent ��� null, � ���� �� null: ������� ��� � ������ ����� ��������
	void MayBeRemovedFromScene(mlRMMLElement* apMLEl); // � rmml-�������� parent ��� �� null, � ���� null: ������ ��� �� �������

		// abCheck - ���������, ����� ������ ��� ���� � ������
	void AddUpdatableElement(mlRMMLElement* apElem, bool abCheck);
	void AddSynchedElement(mlRMMLElement* apElem, bool abCheck);

	static void AddElemToList(v_elems& avElements, mlRMMLElement* apElem, bool abCheck);
	static bool RemoveFromList(v_elems& avElements, mlRMMLElement* apMLEl);

	void TrackLink(mlSynchLinkJSO* apLink);
	void LinkDestroying(mlSynchLinkJSO* apLink);
	void NotifyLinksElemDestroying(mlRMMLElement* apElem);

	// ������������ ������� ���������������� ��������� ��� ������������
	bool IsSynchUpdatingSuspended();
	void SuspendSynchStatesSending();
	void ResumeSynchStatesSending();

	void DeleteObjectInternal( unsigned int auObjectID, unsigned int auBornRealityID);

	// ��������� ������������ � ���������� include-�� ��� �������
	// ����� ����� �� ������� ���� ���������, �� �������� �� �������
	mlresult Preload(const wchar_t* asSrc);

	// ������� � ��� ������ RMML-���������, ������� �� � �������� �����, �� JS-������ �� ��� �������
	void TraceLeakedElements();
	// ������� � ��� ������ ����������� RMML-��������� (� ������� ���������� Update() �� ������ update sceneManager-�)
	void TraceUpdatableElements();

private:
	mlSynchData msdLastState;
	void GetClassesState(mlSynchData &aData);
	void SetClassesState(mlSynchData &aData);
	void GetGOState(mlSynchData &aData);
	void SetGOState(mlSynchData &aData);
	void HandleSyncMessageInternal(mlSynchData& aData);
	void RestoreSynchronizationInternal( const wchar_t* apwcRequestedUrl);

//
public:
	mlStringCollection mSrcFileColl;
	bool mbWasExitFromScript;
	jsval mjsvalExit;
	void FreezeElementIfNeed(mlRMMLElement* pElem);
	void UnfreezeElement(mlRMMLElement* pElem);
	bool mbParentIsSetFromJS; // =true, ���� parent �������� ��������������� ������-�� �� JS-�������: ����� ���� ��������� � mvNewElements �� ������ ������

	enum enCustomEventIDs{
		CEID_serverQueryComplete = 1,
		CEID_mapTeleported,
		CEID_mapTeleportError,
		CEID_mapShowZone,
		CEID_syncDeleteObject,
		CEID_syncHandleMessage,
		CEID_syncOnReceivedRecordsInfo,
		CEID_syncOnReceivedCreatedSeanceInfo,
		CEID_syncOnReceivedSeancesInfo,
		CEID_mapObjectCreated,
		CEID_syncOnObjectStatusReceived,
		CEID_mapObjectStatusReceived,
		CEID_syncOnReceivedFullSeanceInfo,
		CEID_serviceOnGetService,
		CEID_serviceOnReceivedMethodResult,
		CEID_serviceOnNotify,
		CEID_serviceOnConnectRestored,
		CEID_syncOnFreezeSeance,
		CEID_syncOnJoinedToSeance,
		CEID_voipOnConnectSuccess,
		CEID_voipOnConnectStart,
		CEID_voipOnConnectFailed,
		CEID_voipOnConnectLost,
		CEID_voipOnDisconnected,
		CEID_voipOnKicked,
		CEID_voipOnAddRoom,
		CEID_voipOnRemoveRoom,
		CEID_voipOnUserTalking,
		CEID_voipOnUserStoppedTalking,
		CEID_voipOnUserFileSaved,
		CEID_voipOnAudioFilePlayed,
		CEID_voipOnRecordingChanged,
		CEID_voipOnEvoClientWorkStateChanged,
		CEID_voipOnSRModeStatusChanged,
		CEID_voipOnNotified,
		CEID_voipOnSetInputDevice,
		CEID_voipOnSetOutputDevice,
		CEID_voipOnGetInputDevices,
		CEID_voipOnGetOutputDevices,
		CEID_voipOnDeviceErrorCode,
		CEID_voipOnReInitializeVoipSystem,
		CEID_voipOnRecievedFindedMicName,
		CEID_syncOnUnfreezeSeance,
		CEID_syncRestoreSynchronization,
		CEID_syncOnUnfreezeWithFreezeSeance,
		CEID_ConnectRecordInLocation,
		CEID_desktopOnApplyFrame,
		CEID_desktopOnDestinationRequest,
		CEID_desktopOnRaiseSessionStartedEvent,
		CEID_desktopOnRaisePreviewCancelEvent,
		CEID_desktopOnRaisePreviewOKEvent,
		CEID_desktopOnRaiseViewSessionStartedEvent,
		CEID_desktopOnTextureSizeChangedEvent,
		CEID_desktopOnSessionClosedEvent,
		CEID_desktopOnViewSessionClosedEvent,
		CEID_desktopOnWaitOwnerEvent,
		CEID_desktopOnRemoteAccessModeChangedEvent,
		CEID_desktopOnVideoInputDeviceInitializedEvent,
		CEID_desktopOnSharingEventByCode,
		CEID_desktopOnSetBarPositionEvent,
		CEID_desktopOnVideoEndEvent,		
		CEID_desktopOnGlobalMouseEvent,		
		CEID_RecordStartFailed,
		CEID_DisconnectRecordInLocation,
		CEID_StopRecordInLocation,
		CEID_UpdatePlayingTime,
		CEID_SeanceSeekPos,
		CEID_syncOnSeancePlayed,
		CEID_syncOnSeanceRewinded,
		CEID_syncOnSeanceFinishedPlaying,
		CEID_serverOnConnected,
		CEID_serverOnConnectionFailed,
		CEID_serverOnDisconnected,
		CEID_onPreciseURL,
		CEID_syncObjectInServerNotFound,
		CEID_worldServiceOnGetService,
		CEID_worldServiceOnReceivedMethodResult,
		CEID_worldServiceOnNotify,
		CEID_syncOnSeanceLeaved,
		CEID_moduleOnOpened,
		CEID_moduleOnCreated,
		CEID_moduleOnError,
		CEID_desktopOnSharingFileSaved,
		CEID_setModuleToLoadAndLoadScene,
		CEID_syncOnServerError,
		CEID_eventEntered,
		CEID_eventLeaved, 
		CEID_locationEntered,
		CEID_onShowInfoState,
		CEID_onLogout,
		CEID_eventUpdateInfo,
		CEID_playerFileDroped,
		CEID_ResourceLoadedAsynchronously,
		CEID_mapURLChanged,
		CEID_onNotifyServerTOInfo,
		CEID_onSeancesInfo,		
		CEID_onProxyAuthError,		
		CEID_onEventUserRightsInfo,
		CEID_onEventUserRightUpdate,
		CEID_onEventUserBanUpdate,
		CEID_onConnectToServerError,
		CEID_onErrorCreateTempLocation,
		CEID_onUserSleep,
		CEID_onUserWakeUp,
		CEID_onEventsInfoIntoLocation,
		CEID_onWindowMoved,
		CEID_onCutOperationComplete,
		CEID_onCopyOperationComplete,
		CEID_onPasteOperationComplete,
		CEID_onDeleteOperationComplete,
		CEID_onSaveOperationComplete,
		CEID_onSaveAsOperationComplete,
		CEID_onUndoOperationComplete,
		CEID_onRedoOperationComplete,
		CEID_onPlayOperationComplete,
		CEID_onPauseOperationComplete,
		CEID_onRewindOperationComplete,
		CEID_onSeekOperationComplete,
		CEID_onOpenOperationComplete,
		CEID_onKinectFound,
		CEID_serverIPadSessionChangeSlide,
		CEID_onVisibilityDistanceKoefChanged,
		CEID_onWindowSizeChangedExternally,
		CEID_onLoadStateChanged,
		CEID_syncOnInfoMessage,
		CEID_updaterKey,
		CEID_onVideoCaptureError,
		CEID_onVersionReceived,
		CEID_onCodeEditorClosed,
		CEID_onOculusRiftFound
	};
private:
	mlRMMLElement* mpElemRefToClear;
	bool mbNeedToCallGC;
	__int64 mlLastCallGCTime;
	__int64 mlLastForciblyCallGCTime;
	__int64 mlLastMemSizeCheckTime;
	// ��������� RMML-������
	// abUI = true, ���� ����������� UI
	// abAsynch = true, ���� ����� ��������� ���������� (�.�. ��������� �������� �� ��������� update-��)
	mlresult LoadXML(const char* asSrc, bool abUI, bool abAsynch, mlRMMLElement* apParent = NULL);
	mlresult LoadXML(const wchar_t* asSrc, bool abUI, bool abAsynch, mlRMMLElement* apParent = NULL);
	//	void FreezeElementIfNeed(mlRMMLElement* pElem);
	void FreezeAllObjects();
	void UnFreezeAllObjects();
	void HandleScriptEvent(oms::omsUserEvent &aEvent);
	void HandleInternalEvent(oms::omsUserEvent &aEvent);
	void HandleCustomEvent(oms::omsUserEvent &aEvent);
	unsigned int GetUpdateEventHandlingTimeQuota();
	void CallGCIfNeeded(const __int64 alTime);

	// customEvent
	void OnLoadStateChanged(mlSynchData& aData);

// ��������� XML
public:
	struct mlLibXml2Funcs{
#ifdef RMML_USE_LIBXML2
		typedef xmlParserInputPtr (*Func_xmlNewIOInputStream)
			(xmlParserCtxtPtr ctxt,
			 xmlParserInputBufferPtr input,
			 xmlCharEncoding enc);
		/*
		Create a new input stream structure encapsulating the @input into a stream suitable for the parser.

		ctxt: an XML parser context 
		input: an I/O Input 
		enc: the charset encoding if known 
		Returns: the new input stream or NULL 
		*/
		Func_xmlNewIOInputStream xmlNewIOInputStream;
		typedef xmlParserInputBufferPtr (*Func_xmlAllocParserInputBuffer)(xmlCharEncoding enc);
		Func_xmlAllocParserInputBuffer xmlAllocParserInputBuffer;
		typedef xmlParserCtxtPtr (*Func_xmlNewParserCtxt)(void);
		Func_xmlNewParserCtxt xmlNewParserCtxt;
		typedef void (*Func_xmlFreeParserCtxt)(xmlParserCtxtPtr ctxt);
		Func_xmlFreeParserCtxt xmlFreeParserCtxt;
		typedef int (*Func_xmlParseDocument)(xmlParserCtxtPtr ctxt);
		Func_xmlParseDocument xmlParseDocument;
		typedef void (*Func_xmlFreeDoc)(xmlDocPtr cur);
		Func_xmlFreeDoc xmlFreeDoc;
		typedef xmlErrorPtr (*Func_xmlGetLastError)(void);
		Func_xmlGetLastError xmlGetLastError;
		typedef int (*Func_xmlAddEncodingAlias)(const char *name, const char *alias);
		Func_xmlAddEncodingAlias xmlAddEncodingAlias;
		typedef xmlCharEncoding (*Func_xmlParseCharEncoding)(const char *name);
		Func_xmlParseCharEncoding xmlParseCharEncoding;
		typedef xmlCharEncodingHandlerPtr (*Func_xmlFindCharEncodingHandler)(const char *name);
		Func_xmlFindCharEncodingHandler xmlFindCharEncodingHandler;
		typedef int (*Func_xmlSwitchInputEncoding)
			(xmlParserCtxtPtr ctxt,
			 xmlParserInputPtr input,
			 xmlCharEncodingHandlerPtr handler);
		Func_xmlSwitchInputEncoding xmlSwitchInputEncoding;
		typedef void (*Func_xmlSetInputStreamFilePath)
			(xmlParserInputPtr input,
			 const char *filename,
			 const char *directory);
		Func_xmlSetInputStreamFilePath xmlSetInputStreamFilePath;
		
		typedef xmlTextReaderPtr (*Func_xmlNewTextReader)(xmlParserInputBufferPtr input, const char *asURI);
		Func_xmlNewTextReader xmlNewTextReader;
		
		typedef xmlTextReaderPtr (*Func_xmlReaderForMemory)(const char * buffer,int size,const char * URL,const char * encoding,int options);
		Func_xmlReaderForMemory xmlReaderForMemory;

		typedef void (*Func_xmlFreeTextReader)(xmlTextReaderPtr reader);
		Func_xmlFreeTextReader xmlFreeTextReader;
		typedef int (*Func_xmlTextReaderRead)(xmlTextReaderPtr reader);
		Func_xmlTextReaderRead xmlTextReaderRead;
		typedef xmlChar* (*Func_xmlTextReaderName)(xmlTextReaderPtr reader);
		Func_xmlTextReaderName xmlTextReaderName;
		typedef int (*Func_xmlTextReaderNodeType)(xmlTextReaderPtr reader);
		Func_xmlTextReaderNodeType xmlTextReaderNodeType;
		typedef int (*Func_xmlTextReaderDepth)(xmlTextReaderPtr reader);
		Func_xmlTextReaderDepth xmlTextReaderDepth;
		typedef int (*Func_xmlTextReaderIsEmptyElement)(xmlTextReaderPtr reader);
		Func_xmlTextReaderIsEmptyElement xmlTextReaderIsEmptyElement;
		typedef int (*Func_xmlTextReaderHasAttributes)(xmlTextReaderPtr reader);
		Func_xmlTextReaderHasAttributes xmlTextReaderHasAttributes;
		typedef int (*Func_xmlTextReaderMoveToNextAttribute)(xmlTextReaderPtr reader);
		Func_xmlTextReaderMoveToNextAttribute xmlTextReaderMoveToNextAttribute;
		typedef int (*Func_xmlTextReaderMoveToElement)(xmlTextReaderPtr reader);
		Func_xmlTextReaderMoveToElement xmlTextReaderMoveToElement;
		typedef int (*Func_xmlTextReaderHasValue)(xmlTextReaderPtr reader);
		Func_xmlTextReaderHasValue xmlTextReaderHasValue;
		typedef xmlChar* (*Func_xmlTextReaderValue)(xmlTextReaderPtr reader);
		Func_xmlTextReaderValue xmlTextReaderValue;
		typedef void (*Func_xmlSetGenericErrorFunc)(void *ctx, xmlGenericErrorFunc handler);
		Func_xmlSetGenericErrorFunc xmlSetGenericErrorFunc;

#endif
	};
private:
	unsigned mhmLibXml2;
	mlLibXml2Funcs mLibXml2Funcs;
	void DetectLibXml2();
public:
	mlresult OpenXMLArchive(const wchar_t* apwcSrc, mlXMLArchive* &apXMLArch, bool abCreateGDArchive = true);
//	mlresult OpenXMLArchive(const wchar_t* apwcSrc, res::resIResource* &apXMLArch, int &aiSrcIdx);
	mlresult OpenXMLArchive2Write(const wchar_t* apwcSrc, mlXMLArchive* &apXMLArch);
	mlresult CloseXMLArchive(mlXMLArchive* &apXMLArch);
	mlLibXml2Funcs* GetLibXml2Funcs(){ 
#ifdef RMML_USE_LIBXML2
		return (mhmLibXml2!=NULL)?&mLibXml2Funcs:NULL;
#else
		return NULL;
#endif
	}

// ��������� �������
private:
	mlSynchData oDebugInfoData;
	void HandleGetDebugInfoEvent(oms::omsUserEvent &aEvent);
	// ������ RMML-���������, ������� �� ��������� �� � ����� �� ���� (parent==null)
	typedef std::map<mlRMMLElement*, int> MapNotInScenesElems;
	MapNotInScenesElems mmapNotInScenesElems;
	// ������ ���� ���������-�������, ������� ������ ���������� � mmapNotInScenesElems
	// � �� ��� ��� ����
	MapNotInScenesElems mmapClasses;
	void OutRMMLObjectTreeByIter(mlRMMLIterator& iter, std::ostream* aOStream, mlRMMLElement* apElem = NULL);
public:
	void CheckBug();
	unsigned long mStartLoadSceneTime;
	void UpdateNotInScenesElems(mlRMMLElement* apElem, bool abRemove = false, bool abClass = false);
	void RecheckNotInScenesElems();

// ��������� ������ �������������� (Authoring mode)
private:
	omsresult AuthoringUpdate();
	mlRMMLElement* CreateElementByType(int aiType);
	mlresult SaveXML(const wchar_t* asSrc);
	void LoadAuthScene();
	void SetAuthScene(mlRMMLComposition* apAuthScene);
	mlRMMLElement* GetSceneVisibleAt(const mlPoint aPnt, mlIVisible** appVisible=NULL);
	mlRMMLElement* GetUIVisibleAt(const mlPoint aPnt, mlIVisible** appVisible=NULL);

public:
	mlRMMLElement* GetVisibleAt(const mlPoint aPnt);

// ��������� ������ ��������
private:
	unsigned int muElToLoadCnt; // ����� ���������� ���������, ������� ���� ���������
	unsigned int muElLoadedCnt; // ���������� ��� ����������� ���������
	mlresult SetLoadingProgress(double adProgress); // 0.0-1.0
	bool mbWasAuthoringMode;
	bool bMainModuleLoaded;
	bool IsMainModuleLoaded();

// ��������� ��������� ������
friend JSBool JSMouseGetProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp);
private:
	std::list<mlModalButton> mlModalButs;
	bool mbNeedToNotifyMouseModalButtons;
public:
	void UpdateModalButList(mlRMMLElement* apMLEl);
	void RemoveFromModalButList(mlRMMLElement* apMLEl, bool abNotifyLater = false);
	mlRMMLElement* GetModalButtonUnderMouse(mlPoint &aMouseXY, bool &bAllModalsInvisible);

// ��������� ���������������� ������
#ifdef JS_THREADSAFE
private:
	std::auto_ptr<mlUtilityThread> mpUtilityThread;
	std::auto_ptr<mlScriptTerminationThread> mpScriptTerminationThread;
#endif

// ���������� ���������� mlISceneManager
public:
	// ��������� ���������� ����������������� ����������
	omsresult LoadUI(const char* asSrc);
	omsresult LoadUI(const wchar_t* asSrc);
	// ��������� ���������� ������
	omsresult LoadModuleMeta();
	// ��������� ���������� ������ � �����
	mlresult LoadModuleMetaAndLoadScene(unsigned int aID, wchar_t *asModuleID, wchar_t* asScene);
	omsresult LoadModuleMetaAndLoadSceneInternal(mlSynchData& aData);
	// ��������� ����� ������ �� ������
	omsresult LoadScene(int aiNum);
	// ��������� �����
	omsresult LoadScene(const char* asSrc);
	omsresult LoadScene(const wchar_t* asSrc);
	// �������� �����
	omsresult Update(const _int64 alTime, int &aiDeltaTime, const bool isNormalMode);
	// ������ ��������� �� ���������� ����������������� ����������
	mlMedia* GetUI(){ return mpUI; }
	// ������ ��������� �� �������� �����
	mlMedia* GetActiveScene(){ if(meMode==smmLoading) return NULL;  return mpScene; }
	// ��������� JavaScript
	omsresult ExecScript(const wchar_t* apwcScript, int aiLength=-1, wchar_t* *appwcRetVal=NULL);
	// ���������� ����� ������ ��������� ����
	omsresult SetMode(mlEModes aeMode);
	// �������� ����� ������ ��������� ����
	mlEModes GetMode(){ return meMode; }
	// ���������� �������� ����� ������ ��������� ����
	void SetTestMode(mlETestModes aeTesmMode);
	// �������� ����� ������ (0 ��� ������, ������ ����� 1)
	mlETestModes GetTestMode(){ return meTestMode; }
	// ���������� ���������� �������� ��������
	void SetTestResDir(const wchar_t* apw�TestResDir);
	// �������� ���������� �������� ��������
	bool GetTestResDir( wchar_t* apw�TestResDir, int aSizeTestResDir);
	// ����� ��������� �����
	omsresult GetState(void* &apData, unsigned long &aSize);
	// ������������ ��������� �����
	omsresult SetState(void* aData, unsigned long aDataSize);
	// ������ ��������� �� ���������� ���������
	mlMedia* GetAuthScene(){ return mpAuthScene; }
	// ������ ��������� �� ������� �� ��� ������� �����
	mlMedia* GetObjectByTarget(const wchar_t* apwcTarget);
	//������ RealityID �������a �� ��� ������� �����
	unsigned int GetObjectRealityIDByTarget(const wchar_t* apwcTarget);
	// ������ ��������� �� ����� ����
	mlMedia* GetWorldScene() const { return mpWorld3DScene;}

	//// ��������� �������������
	// ������ ������ �������
	virtual void LocationChanged(const wchar_t* apwcLocationName,  unsigned int auiObjectID, unsigned int auiBornRealityID);
	virtual void CommunicationAreaChanged(const wchar_t* apCommunicationAreaName,  unsigned int auiObjectID, unsigned int auiBornRealityID);
	// ������� ����� �������
	virtual mlMedia* CreateAvatar(mlObjectInfo4Creation &aAvatarInfo);
	// ������� ������
	virtual mlMedia* CreateObject(mlObjectInfo4Creation &aObjectInfo);
	// ����������� ������������ �� ������ � ��������� ���������
	// ���������� ���������� ����� ����������� � ������� �� ���� ������
	// !!!! ��������� ���������� ������� ���� ������ NULL
	//  !!! reportError(L"message", L"���������", 0);
	//  !!! reportError(L"no_flash_decoder", 0);
	// ���������������� ����
	// no_flash_decoder - �� ������ ������� Flash
	// no_stream_decoder - �� ������ ������� ��� ������������� ���������� ��������
	virtual void reportError(const wchar_t *code, ...);
	// ���������������� ����
	// no_found_resource - �� ������� ����� ������
	//  param0 - ������
	// message - ������ ���������
	//  param0 - ������ c ����������
	virtual void reportWarning(const wchar_t *code, ...);
	// ��������������� ������������� ����� ������ ����� � ��������
	virtual omsresult RestoreSynchronization( const wchar_t* apwcRequestedUrl);
	// ���������� ������������� ������� � ����
	virtual bool IsObjectExist( unsigned int auObjectID, unsigned int auBornRealityID);
	// ������� ������
	virtual omsresult DeleteObject( unsigned int auObjectID, unsigned int auBornRealityID);
	virtual omsresult DeleteObject( mlMedia* apObject, moMedia** *appaObjects = NULL);
	// ������������ ��������� �� ������� �������
	virtual omsresult HandleSyncMessage( unsigned int aiObjectID, unsigned int auBornRealityID, const wchar_t* apwcSender, unsigned char* apData, unsigned short aiDataSize);
	// ���������� ������ ��������
	virtual int ExtractIncludes( const wchar_t* apcFileSrc, const wchar_t** appIncludes, int count);

	/*
	// Vladimir 2011/04/14
	*/
	// ���������� ����� ��� ��������������� �����
	virtual void SetLogin(const wchar_t* apwcLogin);
	// ���������� ������ ��� ��������������� �����
	virtual void SetPassword(const wchar_t* apwcPassword);
	// ���������� ��� ������: �������������� - 1 ��� �� �������������� - 0 - ��� ��������������� �����
	virtual void SetPasswordEncoding(int iPasswordEncodingMode);
	// ���������� ���� ��� ��������������� �����
	virtual void SetLoginKey(const wchar_t* apwcLoginKey);
	// ���������� ���� ��� ��������������� ����� (����� ���������� � loginKey)
	virtual void SetUpdaterKey(const wchar_t* apwcUpdaterKey);
	/*
	// end
	*/

	// ���������� ������� �� URL
	virtual void SetLocationUrl(const wchar_t* apwcURL);
	virtual void SetLocationUrlAndTeleport(const wchar_t* apwcURL);
	virtual void SetLocationUrl(const wchar_t* apwcLocation, const wchar_t* apwcPlace
		, float x, float y, float z, unsigned long zoneID
		, const wchar_t* apwcAction, const wchar_t* apwcActionParam);
	virtual const wchar_t* GetLocationURL();
	// ���������� �� ����������� ��� ������ rmml-��������
	virtual bool IsJSObjClassDefined(const wchar_t* apwcClassName);
	// ������� ���� � ������� �� �������� ������� � trace
	// abLost - �������� ���� ������ �� ���������� RMML-��������� (parent==null)
	// aiType - 0: ���, 1: 3D-�������, 2: 3D-���������, 3: 3D-������, 4: ����������
	// apwcObjName - ��� �������
	virtual bool TraceObjectsRefs(bool abLost=true, int aiType=0, const wchar_t* apwcObjName=NULL);
	// ������� � tracer ���� �� �������, ������� ��� ������
	virtual void TraceObjectUnderMouse();
	// ������ ������� 2D-RMML-�������, ������� ��������� ��� ������ (��� 3D-��������, ��������������, viewport)
	virtual mlMedia* GetVisibleUnderNouse();

	// ����������� (��� �������������) ����� � ����� �� ��������
	virtual void OnServerConnected(int aServerMask, bool abRestored); // mlServerMask
	// ��������� (��� ��������������) ����� � ����� �� �������� ����������� ��������
	virtual void OnServerConnectionError(int aServerMask, int aiError); // mlServerMask
	// �������� ����� � ����� �� ��������
	virtual void OnServerDisconnected(int aServerMask, int aiCause); // mlServerMask

	// ���������� ������������
	virtual void OnPreciseURL(const wchar_t* aURL);

	virtual void OnSeancesInfo( const wchar_t* aSeancesInfo, bool bFull);
	virtual void OnEventsInfoIntoLocation( const wchar_t* aEventsInfoIntoLocation);

	virtual void OnProxyAuthError( const wchar_t* aSettingName, int aProxyType, const wchar_t* aProxyServer);

	virtual void OnConnectToServerError(int errorCode);


	// ����������, ��� � ������� ������ �������� ���������������� ��������
	virtual void OnMapObjectsLoaded();

	// ��������� ������� � ������ ��� �������������
	virtual void ShowZoneEvent( bool abVal);

	// ��������� ������� � ������ ����������� ���������� ���������� �� ������ � �������
	virtual void SetInfoState( int idInfoState);

	// ��� ������ ��� ������ �����������
	virtual void OnLogout( int iCode);
	
	// ����� �� ������ �� �� �������
	void OnNotifyServerTOInfo( int minutes);

	// ������� � ����� ������������ ��������� ��� 
	// ��� ������ RMML-�������� � ����� � ����������� �� ������ (aiMode)
	//  aiMode: 0 - ��� ������
	virtual void OutRMMLObjectTree(std::ostream* aOStream, int aiMode);

	// ������� � ����� ������������ ��������� ��� 
	// ��� ������ RMML-�������� � ����� � ����������� �� ������ (aiMode)
	//  aiMode: 0 - ��� ������
	virtual void GetRMMLObjectTree( std::vector<mlObjectInfo> &avElements, int aiMode);

	// ������� ��������� ���������� ������� ��� ������ � ������� ��������
	virtual void CopyObjectUnderMouseToGallery();

	virtual std::wstring GetJsonSynchedProps(const wchar_t* apwcTarget);
	virtual std::wstring GetJsonFormatedProps(const wchar_t* apwcTarget, int aIndent = 0);
	virtual void TransposeBinState(const wchar_t* apwcTarget);

	// ������� ������� ������
	// ���� abAsynch == true, �� �� ����������� � ������������ ������
	virtual	void CallJS_GC(bool abAsynch = true, bool abForceCall = false);
	
	#define JSVS_CLASS            0x1
	#define JSVS_MULTILINE_OBJECT 0x2
	#define JSVS_MULTILINE_ARRAY  0x4

	mlString JSVal2String(JSContext* mcx, jsval vProp, int iFormatOptions = JSVS_CLASS, int iIndent = 0, wchar_t* sIndentStr = L"\t");

	unsigned int GetCurSeanceID();
	bool isCurSeancePlaying();
	void SetSeancePlaying(bool playing);

	// ���������� ����� ��������
	virtual void OnAppActivated();
	// ���������� ��������� ���� ��������
	virtual void OnAppDeactivated();

	// ���������� �������� �������
	// ���� ������ false, �� ���������� �� ���� ���������.
	virtual bool OnClose();

	// ������� � tracer � ��� ������� 2D-������� ��� ������ � ��� ���������
	virtual void TraceUnderMouseObjectDepth();

	void SetWorldScene(mlRMMLScene3D* val) { mpWorld3DScene = val; }

	// ������������ ����� �� ������� �� �������� �������
	void OnObjectCreationResult( unsigned int uErrorCode, unsigned int uObjectID, unsigned int uBornRealityID, unsigned int auRmmlHandlerID);

	// ����������, ����� �� ���� ������ ���������� ����
	void OnWindowFileDrop(const wchar_t* apwcFilePath, int x, int y);

	// ����������, ����� �� ������������ �� ��������� ������ 5 ����� ������� ���� � ���������� ��� �� ������� ����  (������ �����)
	virtual void OnUserSleep();

	// ����������, ����� ������������ ������ � ������ ����� � ��������
	virtual void OnUserWakeUp(); 

	// ������������� ����� ������������ � ������ �������
	void OnEventUserRightInfo( unsigned int aEventUserRight);

	void OnIPadSessionChangeSlide( int changeSlide);

private:
	void OnUpdaterKeyByThreadInternal( mlSynchData &oData);
	void OnVersionReceivedInternal( mlSynchData &oData);
public:
	void SetUpdaterKeyByThread(const wchar_t* apwcUpdaterKey);

	//////////////////////////////////////////////////////////////////////////
	//// �������� �������
	//////////////////////////////////////////////////////////////////////////
	// ��������� ���������� �������� Cut ��������� �������
	virtual void OnCutOperationComplete( int aErrorCode, unsigned int aRecordDuration);
	// ��������� ���������� �������� Copy ��������� �������
	virtual void OnCopyOperationComplete( int aErrorCode);
	// ��������� ���������� �������� Paste ��������� �������
	virtual void OnPasteOperationComplete( int aErrorCode, unsigned int aRecordDuration);
	// ��������� ���������� �������� Delete ��������� �������
	virtual void OnDeleteOperationComplete( int aErrorCode, unsigned int aRecordDuration);
	// ��������� ���������� �������� Save ��������� �������
	virtual void OnSaveOperationComplete( int aErrorCode);
	// ��������� ���������� �������� SaveAs ��������� �������
	virtual void OnSaveAsOperationComplete( int aErrorCode);
	// ��������� ���������� �������� Play ��������� �������
	virtual void OnPlayOperationComplete( int aErrorCode);
	// ��������� ���������� �������� Pause ��������� �������
	virtual void OnPauseOperationComplete( int aErrorCode);
	// ��������� ���������� �������� Rewind ��������� �������
	virtual void OnRewindOperationComplete( int aErrorCode);
	// ��������� ���������� �������� Seek ��������� �������
	virtual void OnSeekOperationComplete( int aErrorCode);
	// ��������� ���������� �������� Undo ��������� �������
	virtual void OnUndoOperationComplete( int aErrorCode, unsigned int aRecordDuration);
	// ��������� ���������� �������� Redo ��������� �������
	virtual void OnRedoOperationComplete( int aErrorCode, unsigned int aRecordDuration);
	// ��������� ���������� �������� Open ��������� �������
	virtual void OnOpenOperationComplete( int aErrorCode, std::wstring aRecordName, std::wstring aRecordDesciption, unsigned int aRecordDuration, std::wstring aAuthor, std::wstring aCreationDate, std::wstring aLocation, std::wstring aLanguage, std::wstring aURL);
	// ���������� ������ 
	virtual void SetVersionInfo(wchar_t* version);

private:
	unsigned int uiCreatedAvatarID;
	unsigned int muiMyAvatarID;
	unsigned int mMyAvatarBornRealityID;
	mlRMMLElement* mpMyAvatar;
	mlRMMLScene3D* mpWorld3DScene;
	bool m_isMyAvatarCreated;
	bool m_isSynchUpdatesSuspended;
	MP_STRING m_currentExecutingScriptSrc;
	int m_currentExecutingScriptLine;

	// ��������� �������� ��������
	typedef std::map<unsigned int/*objectID*/, unsigned int/*handler*/> MapObjectCreationResultHandlers;
	MapObjectCreationResultHandlers	mapObjectCreationResultHandlers;
	// end ��������� �������� ��������

	int lastMouseX, lastMouseY;

	// ���������� ��� �������
	unsigned long muiSceneElementCount; // ���-�� ������������ �� ������ ������ RMML-��������� � �����
	unsigned int muiUIElementCount; // ���-�� ������������ �� ������ ������ RMML-��������� � �����
	__int64 mlLastTraceElemCountTime; // ����� ���������� ������ ���������� � ���-�� ������������ RMML-���������

	void OnPreciseURLInternal(mlSynchData& aData);

	int iSceneWidthDefault;
	int iSceneHeightDefault;
	MP_WSTRING sSceneTitleDefault;
	MP_WSTRING sScenePathDefault;
	MP_WSTRING sSceneTempTitleDefault;
	MP_WSTRING sSceneTempPathDefault;

#ifndef CSCL
public:
	void add_updatable (shared_ptr<Updatable> smp_new_updbl);

private:
	typedef list<shared_ptr<Updatable> > UpdatableStorage;

	UpdatableStorage _updbl;
#endif
};

//#ifndef JS_THREADSAFE
extern mlSceneManager* gpSM;
//#endif

class mlDontSendSynchEvents
{
	bool mbDontSendSynchEventsOld;
	mlSceneManager* mpSM;
public:
	mlDontSendSynchEvents(mlSceneManager* apSM){
		mpSM = apSM;
		mbDontSendSynchEventsOld = mpSM->mbDontSendSynchEvents;
		mpSM->mbDontSendSynchEvents = true;
	}
	~mlDontSendSynchEvents(){
		mpSM->mbDontSendSynchEvents = mbDontSendSynchEventsOld;
	}
};

extern JSFunctionSpec shell_functions[];

extern JSClass JSRMMLMouseClass;
extern JSPropertySpec _JSMousePropertySpec[];
extern JSFunctionSpec _JSMouseFunctionSpec[];
extern char GetMouseEventID(const wchar_t* apwcEventName);

extern JSClass JSRMMLKeyClass;
extern JSPropertySpec _JSKeyPropertySpec[];
extern JSFunctionSpec _JSKeyFunctionSpec[];

extern JSClass JSRMMLPlayerClass;
extern void InitPlayerJSObject(JSContext *cx, JSObject *mjsoPlayer);
extern char GetPlayerEventID(const wchar_t* apwcEventName);
extern wchar_t* GetPlayerEventName(char aidEvent);
extern void CreateModuleJSObject(JSContext *cx, JSObject *ajsoPlayer);
extern void CreateServerJSObject(JSContext *cx, JSObject *ajsoPlayer);
extern void CreateRecordEditorJSObject(JSContext *cx, JSObject *ajsoPlayer);
extern void CreateSRecorderJSObject(JSContext *cx, JSObject *ajsoPlayer);
extern void CreateSRCurrentSeanceJSObject(JSContext *cx, JSObject *ajsoPlayer);
//extern void CreateRealityJSObject(JSContext *cx, JSObject *ajsoPlayer);
extern void CreateMapJSObject(JSContext *cx, JSObject *ajsoPlayer);
extern void TraceServerError(JSContext *cx, int aiErrorCode);
extern void CreateUrlJSObject(JSContext *cx, JSObject *ajsoPlayer);

extern char GetModuleEventID(const wchar_t* apwcEventName);
extern wchar_t* GetModuleEventName(char aidEvent);

extern char GetMapEventID(const wchar_t* apwcEventName);
extern wchar_t* GetMapEventName(char aidEvent);

extern char GetUrlEventID(const wchar_t* apwcEventName);
extern wchar_t* GetUrlEventName(char aidEvent);

extern char GetVoiceEventID(const wchar_t* apwcEventName);
extern wchar_t* GetVoiceEventName(char aidEvent);

extern char GetVideoCaptureEventID(const wchar_t* apwcEventName);
extern wchar_t* GetVideoCaptureEventName(char aidEvent);

extern char GetCodeEditorEventID(const wchar_t* apwcEventName);
extern wchar_t* GetCodeEditorEventName(char aidEvent);

extern char GetRecordEventID(const wchar_t* apwcEventName);
extern wchar_t* GetRecordEventName(char aidEvent);

extern char GetServerEventID(const wchar_t* apwcEventName);
extern wchar_t* GetServerEventName(char aidEvent);

extern char GetRecordEditorEventID(const wchar_t* apwcEventName);
extern wchar_t* GetRecordEditorEventName(char aidEvent);

extern JSClass JSRMMLMath3DClass;
extern void InitMath3DJSObject(JSContext *cx, JSObject *ajsoMath3D);

char* WC2MB(const wchar_t* apwc);
char* WC2MBq(const wchar_t* apwc);
wchar_t* MB2WC(const char* apch);
wchar_t* MB2WCq(const char* apch);

extern JSBool TraceRefs(JSContext *cx, JSObject *obj, JSObject* objToFind);
extern JSBool LogRefs(JSContext *cx, JSObject *obj, JSObject* objToFind);extern JSObject* GetRealityObject( JSContext *cx, cm::cmIMapManager* aMapMan, unsigned int aRealityID);
}