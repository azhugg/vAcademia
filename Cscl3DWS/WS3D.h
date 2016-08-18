// WS3D.h : Declaration of the CWS3D

#ifndef __WS3D_H_
#define __WS3D_H_

#define __WS3D_H_

#include "resource.h"       // main symbols
#include <atlctl.h>

#include "wsthread.h"	// thread class
#include "nrm.h"
#include "SoundWrapper.h"

#include "synch.h"
#include "tracer.h"
#include "rmml\rmmld.h"
#include "../PlayerLib/notifies.h"
#include "../LogWriter/LogWriterLastMsgAsync.h"
#include "../Voip/VoipManClient/include/VoipManClient.h"
#include "CopyDataPoster.h"

#define WS3D_LOG_CHANNEL 1

using namespace rmml;
using namespace res;

namespace sm
{
	struct smISoundManager;
}

class CDumpParamsSaver;

class CLogWriter;

class CExecCommandThread;

class CGlobalKeyboardHook;


/////////////////////////////////////////////////////////////////////////////
// CWS3D
class CWS3D : public CWSThread,
			public oms::mwIWindow,
			public rmml::mlISinchronizer,
			public oms::omsIApplication,
			public ws3d::IC3DWSModule
{
public:
	CWS3D();
	~CWS3D();

BEGIN_PROP_MAP(CWS3D)
	PROP_ENTRY_TYPE("moduleBase", 15, CLSID_NULL, VT_BSTR)
	PROP_ENTRY_TYPE("userBase", 16, CLSID_NULL, VT_BSTR)
	PROP_ENTRY_TYPE("UIBase", 17, CLSID_NULL, VT_BSTR)
	PROP_ENTRY_TYPE("UISkinBase", 18, CLSID_NULL, VT_BSTR)
	PROP_ENTRY_TYPE("regKey", 28, CLSID_NULL, VT_BSTR)
END_PROP_MAP()

private:
	HINSTANCE						m_hResourceInstance;
	HWND							m_hParentWnd;
	HWND							m_FrameWorkWnd;
	RECT							m_FrameWorkRECT;
	CSoundWrapper					sWrapper;
	//res::resResourceManagerWrapper* mpResManWrapper;
	class miGuardAScope{
		CRITICAL_SECTION* mpCS;
	public:
		miGuardAScope(CRITICAL_SECTION &aCS){ mpCS=&aCS; ::EnterCriticalSection(mpCS); }
		~miGuardAScope(){ ::LeaveCriticalSection(mpCS); }
	};
	class miWin32Input: public oms::omsInput<miGuardAScope,CRITICAL_SECTION>{
	public:
		miWin32Input(){ ::InitializeCriticalSection(&mMutex); }
		~miWin32Input(){ ::DeleteCriticalSection(&mMutex); }
		void* AllocMem(unsigned int auBlockSize){
			if(auBlockSize==NULL) return NULL;
			char* pNew = MP_NEW_ARR( char, auBlockSize);
			return pNew;
		}
		void FreeMem(void* apBlock){
			if(apBlock==NULL) return;
			MP_DELETE_ARR( apBlock);
		}
	}mInput;
	std::auto_ptr<oms::omsContext>	pOMSContext;
	std::auto_ptr<wsSynchronizer> mpSynch; // �������� �������������� (���� spLogMan==NULL)
	DWORD mtStartTime;
	DWORD	avatarID;
	BOOL	bFullScreen;
	HCURSOR mhCursor;
	CComString sMicrophoneFileName;
	CComString sMyUserName;
	
	// Communication Manager
	cm::cmICommunicationManager *mpICommunicationManager;
	resIResourceManager *mpIResourceManager;
	
	cm::cmIComManCallbacks* pCallbacks;
	CRITICAL_SECTION csQuery;
	//
	HANDLE mhEventGetSetState;
	BYTE* mpSMState;
	DWORD mSMStateSize;
	MP_WSTRING mwsUIBase;
	MP_WSTRING mwsUISkinBase;
	MP_WSTRING msSysCommand;
	MP_WSTRING msRegKey;
	MP_WSTRING msPathIni;
	rmml::mlEModes mSMLastMode;
	bool mbMouseCaptured;
	RECT mrcClient;
	std::vector<wchar_t*> mvArguments; // ��������� ��������� ������ ����������

	enum ELogInitialized { liLogNotInitialized, liLogInitialized, liLogInitHandled};
	ELogInitialized nLogInitialized;

	/*struct CChildWnd
	{
		HWND hWnd;
		WORD wType;
	};
	//typedef CChlidrenList std::list< CWS3D::CChildWnd>;

	std::list< CChildWnd> oChildrenWnd;*/

	BOOL mbLangAlreadyGot;
	CComString msLanguage;

	BOOL mbVersionAlreadyGot;
	CComString msVersion;

	bool mbAppActive; // ����: ������� ���������� ��� ���?

	// ������ ���������� ����� ����.
	// ��������, ����� ��� ����������� ������ ����������� ������� (��������, ���� ���� ��������������)
	unsigned muWindowClientWidth, muWindowClientHeight;

	virtual UINT Thread();
	void SetEditMode(BOOL abEdit);

	HRESULT STDMETHODCALLTYPE GetWindow(HWND *phwnd){
		*phwnd = m_FrameWorkWnd;
		return S_OK;
	}
	    
	HRESULT STDMETHODCALLTYPE ContextSensitiveHelp( BOOL fEnterMode){
		return S_OK;
	}

public: 
	void SetDllInstance(HINSTANCE ahDllInstance){
		m_hResourceInstance = ahDllInstance;
	}

// IWS3D
public:
	int		initCounter;
	bool needExit;
	void activateCave();
	BOOL initOMSContext();
	oms::omsContext *getOMSContext();
	BOOL createManagers();		
	void Destroy();
	//CNRManager*	get_RM(){ return pnrManager;}
	//void	CreateRenderManager();
	HRESULT ToggleFullScreen(UINT auOn);
	unsigned long initialize(HWND wnd);
	HRESULT setAvatarName(/*[in]*/ BSTR sName);
	HRESULT SetAvatarID(UINT aid);
	HRESULT ExecScript(BSTR sScript, VARIANT abImmediately);				/* + */
	unsigned long loadUI(BSTR sSrc);
	unsigned long loadScene(BSTR sSrc);										/* + */
	HRESULT loadSceneN(UINT numScene);									/* - */
	unsigned long initResManager();
	HRESULT get_UISkinBase(/*[out, retval]*/ BSTR *pVal);
	HRESULT put_UISkinBase(/*[in]*/ BSTR newVal);
	unsigned long put_PathIni( BSTR newVal);
	// �������� ����������� ������ ��� ��������� ��������� ����
	HRESULT GetCSForWindow(HWND ahWnd, IUnknown** apCS);
// IEventHandler2
	HRESULT getSysCommand(BSTR* psCmd);
	HRESULT Resize(short aiWidth, short aiHeight);
	unsigned long Update(DWORD adwTime);
	unsigned long loadModule(BSTR sModuleID, BSTR sScene);
	bool SetScreenMode(DWORD adwWidth, DWORD adwHeight, DWORD adwBPP, BOOL abFullscreen);
	void SetScreenModeForOculus(DWORD adwWidth, DWORD adwHeight, DWORD x, DWORD y);
	bool SetArguments(const wchar_t* apwcArgs);

	unsigned long OnLMouseDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL* bHandled);
	unsigned long OnRMouseDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL* bHandled);
	unsigned long OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL* bHandled);
	unsigned long OnKeyUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL* bHandled);
	unsigned long OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL* bHandled);
	unsigned long OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL* bHandled);
	unsigned long OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL* bHandled);
	unsigned long OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL* bHandled);
	unsigned long OnRButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL* bHandled);
	unsigned long OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL* bHandled);
	unsigned long OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL* bHandled);
	unsigned long OnMButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL* bHandled);
	unsigned long OnMButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL* bHandled);
	unsigned long OnCopyData(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL* bHandled);
	bool OnActivateApp(bool abActivated);
		// ����������� ������ ���������� ����� ���� ���������
	bool OnWindowSizeChanged(unsigned int auiWidth, unsigned int auiHeight);
	
	unsigned long PrepareForDestroy();
	
	// ���������� �������� �������
	// ���� ������ false, �� ���������� �� ���� ���������.
	bool OnClose();

	// ����������, ����� �� ���� ������ ���������� ����
	void OnFileDrop(LPCWSTR alpcFilePath);

	unsigned long getSettingValue(BSTR sName, BSTR* psVal);
	unsigned long SetLogAndTracerMode(BOOL bUseLog, int gSendLog, BOOL bUseTracer, BOOL bUseSyncLog);

	void SetApplicationRoot(LPCWSTR alpcRoot);
	LPCWSTR GetApplicationRoot();

	omsresult setSoundManager( sm::smISoundManager *aSoundMan);

	////////////////////////////
	// ���������� oms::mwIWindow
	//
	// ���������� ����� �������
	// 0 - ��� ������� (������)
	// 1 - ���������� ������
	// 2 - ������ �������� ����
	// 3 - ������ ��������
	void SetCursor(int aiType);
	int GetClientWidth();
	int GetClientHeight();
	unsigned int GetMainWindowHandle();
	void MouseMoveToPos( short aX, short aY);
	void SendMouseMessage( unsigned int msg, unsigned int wParam, short aX, short aY);

	/**
	* ���������� trc::trcITracer
	*/
	wsTracer mTracer;

	/**
	* ���������� rmml::mlISinchronizer
	*/
	omsresult CallEvent(unsigned int auID, MLBYTE* apData, MLWORD auDataLength);
	omsresult GetObjectID(const wchar_t* apwcStringID, unsigned int &auID);
	omsresult GetObjectStringID( unsigned int auID, rmml::mlOutString &asStringID);
	void NotifyGetState(MLBYTE* apData, MLDWORD alSize);
	// ��������� �� ������ ��������� � ���������� (synch-��������) ���������������� ��������
	omsresult SendStateChanges(MLBYTE* apData, int aiDataLength);

	/**
	* ���������� rmml::omsIWSWrapper
	*/
	//void	Initialize();
	//bool	LoadSceneN( UINT numScene);
	void	SetUIBase( BSTR apUIBasePath);
	void	SetModuleBase( BSTR apModuleBasePath);
	//bool	LoadUI(BSTR sSrc);

	void Refresh();
	//const char* RefineResPathWithAlias(const char* apcResSrcPath);

	// ����� ����� ������, ����������� � ������������ ����������� �������*/
	int GetFilesFromCFG(int iType, char* apszCFGFileName, char* appszFileNames, int bufCount);
	/**
	* ���������� oms::omsIApplication
	*/
	// �������� �� ������ ���������� ������� ��
	unsigned int GetError();
	// ��������� ������� ��
	bool execSysCommand(const wchar_t* apwcCommandLine, unsigned int* apErrorCode);
	bool execSysCommandExeOnly(const wchar_t* apwcCommandLine, unsigned int* apErrorCode);
	//
	bool IsExecAnySysCommand();
	// ��������� ������� "��������" ����������
	void ExternCommand(const wchar_t* apwcCmd, const wchar_t* apwcPar);
	// ������� ���������� ���������� ���� ���������
	void SendDebugInfo(unsigned char acInfoType, void* hwndDebugger, 
							unsigned char* apData, unsigned int auDataLength);
	// �������������� ������� ����
	void minimize();
	// ������� ������� ���� (����� �� ���������)
	void exit();
	// ��������� dll
	unsigned long LoadLibrary(const wchar_t* apwcDllName);
	// ���������� dll
	void FreeLibrary(unsigned long auHModule);
	// �������� ����� ������� � dll
	void* GetProcAddress(unsigned long auHModule, const char* apwcProcName);
	// �������� ������ GUID
	void GetStringGUID( wchar_t* apwcGUID, unsigned short aSizeGUID);
	// �������� ������ �������� ����
	bool ResizeWindow(unsigned auClientWidth, unsigned auClientHeight, int aiFullScreen);
	void ResizeWindowForOculus(unsigned auClientWidth, unsigned auClientHeight, unsigned x, unsigned y);
	// �������� ������� ����� (timeGetTime ��� �������)
	unsigned long GetTimeStamp();
	// �������� ������ ������ � ��������� ����
	bool GetVersion(wchar_t* apwcVersion, unsigned short aSizeVersion);
	// �������� ���� � ��������� ����, �� ������� ������� �������� ��� ��������� 
	bool GetLanguage(wchar_t* apwcLanguage, unsigned short aSizeLanguage);
	// ��������� ���������� � �������
	bool SetSetting(const wchar_t* apwcName, const wchar_t* apwcValue);
	// �������� ���������� �� �������
	bool GetSetting(const wchar_t* apwcName, wchar_t* apwcValue, unsigned short aSizeValue, const wchar_t* apwcSectionName = L"");
	// �������� ���������� �� ini
	virtual bool GetSettingFromIni(const char* apwcSection, const char* apwcName,char* apwcValue, unsigned short aSizeValue);
	// �������� ���� � ������ � ��������� ������
	bool GetDebugVersion();
	// ����� ����������� ���������� ���� �������� ���� � ����� �� ��������� ���������� 
	bool GetLocalFilePath(const wchar_t* apszFilter, const wchar_t* apwcStartPath, bool abExists, wchar_t* pwcPath, unsigned short aSizePath, const wchar_t* apszDefaultName);
	// (���� aiX < 0, �� ������������ ������� ���������� ��������� ����)
	bool FireMouseDown(int aiX, int aiY);
	// �������� ��������� �������� (�������� � ��������� ������, ��������)
	wchar_t** GetArguments();
	// �������� ���������� ����-����� � ascii � unicode
	void TranslateKeyCode(unsigned short muKeyCode, unsigned short& muKeyStates, unsigned short muScanCode, unsigned short& muAscii, unsigned short& muUnicode, bool& mbAlt, bool& mbCtrl, bool& mbShift);
	// ������ ��������� ������
	void GetDesktopSize(int& offsetX, int& width, int& height);
	// ������ ������� ������� �����
	void GetToolbarParams(int& toolbarOffsetX, int& toolbarOffsetY, int& toolbarWidth, int& toolbarHeight);
	// �������� �� �������� �����������
	void OnAuthorizated( int aiTypeSendLog);

	// ������� �� ���� ���������� �� ������ ������?
	bool IsActive();

	void GetWideDesktopParams(int& width, int& height, int& startOffset);

	// ���������, ������ �� ��������� ������� �� ����������?
	bool IsKeyDown(unsigned short muKeyCode, unsigned short muScanCode);

	// �������� ���������� ���� � ����������
	int ShowMessageBox(const wchar_t* apwcMessage, const wchar_t* apwcTitle = NULL);

	// ��� ������ ��� ������ �����������
	virtual void OnLogout( int iCode);

	// ������ ������, ������� ����������
	virtual bool CWS3D::GetConsumedMemorySize(unsigned long& PageFaultCount
											, unsigned long& PeakWorkingSetSize
											, unsigned long& WorkingSetSize
											, unsigned long& QuotaPeakPagedPoolUsage
											, unsigned long& QuotaPagedPoolUsage
											, unsigned long& QuotaPeakNonPagedPoolUsage
											, unsigned long& QuotaNonPagedPoolUsage
											, unsigned long& PagefileUsage
											, unsigned long& PeakPagefileUsage);

	// ����������, ����� �� ������� ����������������		
	void onLoginFinished( long sessionID);
	long GetSessionID(); 
	void SendMessage2Launcher( int id, unsigned long message);
	void OnWindowMoving();
	void HandleDebugBreakKeyboardRequest( );
	void HandleMakeDumpKeyboardRequest();
	// ���������� true - ���� ������������ ������� ����� ctrl + alt + shift + d
	bool GetKeysShortcutsEnabled( );

private:
	bool IsDebuggerPresent();
	bool RunRecordEditorApp(); 
	void GetDisplayParamsIfNeeded();

	void GetFileFilter(CWComString &sFilte, CWComString &sExt);

	void Tick();

	CComString GetRegKeyValueStr( LPCTSTR lpKey);
	CComString GetValueStrFromIni( LPCTSTR lpSection, LPCTSTR lpKey);
	wstring	GetRegVersion();
	wstring	GetRegLanguage();
	void DestroyContext();
	// Logmanager manager
	void CreateLogManagersImpl( oms::omsContext* context);
	void DestroyLogManagersImpl( oms::omsContext* context);
	// Voip manager
	void CreateVoipManagerImpl( oms::omsContext* context);
	void DestroyVoipManagerImpl( oms::omsContext* context);
	HMODULE	hVoipModule;
	// Desktop manager
	void CreateDesktopManagerImpl( oms::omsContext* context);
	void DestroyDesktopManagerImpl( oms::omsContext* context);
	HMODULE	hDesktopModule;

	// Desktop manager
	void CreateSoundManagerImpl( oms::omsContext* context);
	void DestroySoundManagerImpl( oms::omsContext* context);
	HMODULE	hSoundModule;

	// Comman manager
	void CreateComManagerImpl( oms::omsContext* context);
	void DestroyComManagerImpl( oms::omsContext* context);
	HMODULE	hComModule;

	// Comman manager
	void CreateLoggerImpl( oms::omsContext* context);
	void DestroyLoggerImpl( oms::omsContext* context);
	HMODULE	hLoggerModule;

	// Log functions
	void	HandleOnManagerModuleInitialized( const  wchar_t* alpcModuleName);
	void	HandleOnManagerModuleNotLoaded( const  wchar_t* alpcModuleName);
	void	HandleOnManagerCreateFunctionNotFound( const wchar_t* alpcModuleName, const char* alpcFuncitonName);
	void	HandleOnManagerCreateFunctionFailed( const wchar_t* alpcModuleName, const char* alpcFuncitonName);

	void UpdateSleepingState();

	CWComString sApplicationRoot;
	CLogWriter* m_pLogWriter;
	CLogWriterLastMsgAsync* m_pLogWriterLastMsgAsync;

	bool m_isAllowedDebug;

	bool m_isAlt;
	bool m_isCtrl;
	bool m_isShift;
	long sessionID;
	CDumpParamsSaver *m_pDumpParamsSaver;
	typedef MP_VECTOR<CExecCommandThread*> CExecCommandThreadVec;
	typedef CExecCommandThreadVec::iterator CExecCommandThreadVecIt;
	CExecCommandThreadVec m_execCommandThreadVec;
	CGlobalKeyboardHook*   m_pkeybHook;

	unsigned int m_lastUserActivityTime;
	bool m_isSleepingUserState;

	DEVMODE m_defaultDM;
	MP_VECTOR<DEVMODE> m_displayParams;
	bool m_isDisplayParamsInitialized;
	int m_windowMovingTime;
	DWORD	m_screenWidth;
	DWORD	m_screenHeight;
};


////////////////////////
// DELME

//}
#endif //__WS3D_H_