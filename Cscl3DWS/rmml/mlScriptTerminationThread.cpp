#include "mlRMML.h"
#include "config/oms_config.h"
#include "mlScriptTerminationThread.h"
#include "config/prolog.h"

#include "Windows.h"
#include "crtdbg.h"
#include "ViskoeThread.h"

namespace rmml {

	// timeout ���������� �������, ����� �������� �������� ��������������
	#define SCRIPT_EXECUTION_SLOW_LIMIT_MS 200

	// timeout ���������� �������, ����� �������� ��������� ���������� �������
	#define SCRIPT_EXECUTION_TIME_LIMIT_MS 10000

	JSBool mlJSOperationCallback(JSContext *cx){
		mlScriptTerminationThread* pSTThread = GPSM(cx)->GetScriptTerminationThread();
		int iExceededTimeout = pSTThread->GetExceededTimeout();
		if(iExceededTimeout == 0)
			return JS_TRUE;
		bool bInterrupted = false;
		if(iExceededTimeout == SCRIPT_EXECUTION_TIME_LIMIT_MS)
			bInterrupted = true;
		GPSM(cx)->JSScriptExecutionTimeout(bInterrupted, iExceededTimeout);
		if(bInterrupted)
			return JS_FALSE;
		return JS_TRUE;
	}

	class mlScriptTerminationThread::mlSTThreadInternal:
		public ViskoeThread::CThreadImpl<mlScriptTerminationThread::mlSTThreadInternal>
	{
		JSOperationCallback mNativeOperationCallback;
		JSContext* mcx;
		HANDLE mevSleep;
		HANDLE mevTimeout;
		bool mbSleeping;	// ����� ���� � ����� � �������� timeout-�� ���������� �������
		bool mbShouldSleep;	// ���� ==true, ������ ������ ��� ���������� � �� ���� ������ ����� timeout-��
		int miExceededTimeout;
		void* mFirstStartScript;
	public:
		mlSTThreadInternal(JSContext* acx):mcx(acx),mbSleeping(false), mbShouldSleep(false){
			mevSleep = CreateEvent(NULL, FALSE, FALSE, NULL);
			mevTimeout = CreateEvent(NULL, FALSE, FALSE, NULL);
			mNativeOperationCallback = JS_SetOperationCallback(mcx, mlJSOperationCallback);
			mFirstStartScript = NULL;
		}
		~mlSTThreadInternal(){
			// JS_SetOperationCallback(mcx, mNativeOperationCallback); // �������� JSContext ��� �� ������� � ���� ������
		}

		int GetExceededTimeout(){
			return miExceededTimeout;
		}

		DWORD Run(){
			while( !ShouldStop() ) {
				mbShouldSleep = false;
				mbSleeping = true;
				::WaitForSingleObject(mevSleep, INFINITE);
				mbSleeping = false;
				if(ShouldStop())
					break;
				if(mbShouldSleep)
					continue;
				// ���� timeout
				miExceededTimeout = 0;
				if(::WaitForSingleObject(mevTimeout, SCRIPT_EXECUTION_SLOW_LIMIT_MS) != WAIT_OBJECT_0){
				//if(!mevTimeout.WaitForEvent(SCRIPT_EXECUTION_SLOW_LIMIT_MS)){
					// ����� �� timeout-�: ��������� ���������� �������
					miExceededTimeout = SCRIPT_EXECUTION_SLOW_LIMIT_MS;
					JS_TriggerOperationCallback(mcx);
				}
				if(ShouldStop())
					break;
				if(mbShouldSleep)
					continue;
				if(::WaitForSingleObject(mevTimeout, SCRIPT_EXECUTION_TIME_LIMIT_MS) != WAIT_OBJECT_0){
					// ����� �� timeout-�: ��������� ���������� �������
					miExceededTimeout = SCRIPT_EXECUTION_TIME_LIMIT_MS;
					JS_TriggerOperationCallback(mcx);
				}
				if(ShouldStop())
					break;
			}
			return 0;
		}
		void ScriptExecutionStarted(void* aCurrentScript){
			//���� �� ���������� �������� ���������� mFirstStartScript ����� �� ����������
			if (mFirstStartScript)
				return;
			if(!mbSleeping){	// ����� ��� �� ����� ������� � ��������� ���
				// ������� ����� ������� ������� �� ����, ������� ��� �� ��������
				mbShouldSleep = true;
				::SetEvent(mevTimeout);
			}
			mFirstStartScript = aCurrentScript;
			::SetEvent(mevSleep);
		}
		void ScriptExecuted(void* aCurrentScript){
			//�� ���� ������ ScriptExecutionStarted ��� ��� JSScriptCancelTerimation ��� ����������� ������ ����������
			if (!mFirstStartScript || !aCurrentScript || mFirstStartScript == aCurrentScript)
			{
				mFirstStartScript = NULL;
			
				mbShouldSleep = true;
				if(!mbSleeping){
					::SetEvent(mevTimeout);
				}
			}
		}

		virtual void Stop(){
			SignalStop();
			::SetEvent(mevSleep);
			::SetEvent(mevTimeout);
			ViskoeThread::CThreadImpl<mlScriptTerminationThread::mlSTThreadInternal>::Stop();
		}
	};

	mlScriptTerminationThread::mlScriptTerminationThread(JSContext* acx){
		mpThread.reset(new mlSTThreadInternal(acx));
		mpThread->Start();
	}

	mlScriptTerminationThread::~mlScriptTerminationThread(){
		mpThread->Stop();
	}

	void mlScriptTerminationThread::ScriptExecutionStarted(void* aCurrentScript){
		mpThread->ScriptExecutionStarted(aCurrentScript);
	}

	void mlScriptTerminationThread::ScriptExecuted(void* aCurrentScript){
		mpThread->ScriptExecuted(aCurrentScript);
	}

	int mlScriptTerminationThread::GetExceededTimeout(){
		return mpThread->GetExceededTimeout();
	}

}

