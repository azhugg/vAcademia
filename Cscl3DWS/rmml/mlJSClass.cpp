
#include "mlRMML.h"

namespace rmml {

mlJSClass::mlJSClass(void)
{
}

mlJSClass::~mlJSClass(void)
{
}

const wchar_t* aEventNames[256];
// ���� aEventIsPW[id]==true, �� ������� aEventNames[id] ����� ��� char*
// � ������� �������� 'watch'-� ��������
bool aEventIsPW[256];
// ����� ������� (aEventIsPW[?]==1) � �������
mlString aPropNames[256];

void DumpEventNames(JSContext* cx){
	for(int ii=0; ii<256; ii++){
		TRACE(cx, "%d:",ii);
		if(aEventIsPW[ii]){
			char* pch=(char*)aEventNames[ii];
			TRACE(cx, "P:%s\n",pch);
		}else{
			const wchar_t* pwc=aEventNames[ii];
			TRACE(cx, "E:%S\n",pwc);
		}
	}
}

mlresult mlJSClass::CreatePropFuncSpecs(JSPropertySpec* &pJSPropertySpec,JSFunctionSpec* &pJSFunctionSpec){
	pJSPropertySpec = MP_NEW_ARR( JSPropertySpec, 1);
	memset(pJSPropertySpec,0,sizeof(JSPropertySpec));
	pJSFunctionSpec = MP_NEW_ARR( JSFunctionSpec, 1);
	memset(pJSFunctionSpec,0,sizeof(JSFunctionSpec));
	return ML_OK;
}

mlresult mlJSClass::DestroyPropFuncSpecs(JSPropertySpec* &pJSPropertySpec,JSFunctionSpec* &pJSFunctionSpec){
	MP_DELETE_ARR( pJSPropertySpec);
	MP_DELETE_ARR( pJSFunctionSpec);
//DumpEventNames();
	return ML_OK;
}

mlresult mlJSClass::AddPropFuncSpecs(JSPropertySpec* &pJSPropertySpec,JSFunctionSpec* &pJSFunctionSpec,
									 JSPropertySpec* apJSPropSpec, int anProps, 
									 JSFunctionSpec* apJSFuncSpec, int anFuncs){
	if(anProps>0){
		JSPropertySpec* pJSPS=pJSPropertySpec;
		int nP;
		for(nP=0; pJSPS->name; nP++,pJSPS++);
		JSPropertySpec* pJSPropSpecNew = MP_NEW_ARR( JSPropertySpec, nP+anProps+1);
		if(nP>0)
			memcpy(pJSPropSpecNew, pJSPropertySpec, nP*sizeof(JSPropertySpec));
		memcpy(pJSPropSpecNew+nP, apJSPropSpec, anProps*sizeof(JSPropertySpec));
		memcpy(pJSPropSpecNew+nP+anProps, pJSPropertySpec+nP, sizeof(JSPropertySpec));
		pJSPS=pJSPropertySpec;
		pJSPropertySpec=pJSPropSpecNew;
		MP_DELETE_ARR( pJSPS);
	}
	if(anFuncs>0){
		JSFunctionSpec* pJSFS=pJSFunctionSpec;
		int nF;
		for(nF=0; pJSFS->name; nF++,pJSFS++);
		JSFunctionSpec* pJSFuncSpecNew = MP_NEW_ARR( JSFunctionSpec, nF+anFuncs+1);
		if(nF>0)
			memcpy(pJSFuncSpecNew, pJSFunctionSpec, nF*sizeof(JSFunctionSpec));
		memcpy(pJSFuncSpecNew+nF, apJSFuncSpec, anFuncs*sizeof(JSFunctionSpec));
		memcpy(pJSFuncSpecNew+nF+anFuncs, pJSFunctionSpec+nF, sizeof(JSFunctionSpec));
		pJSFS=pJSFunctionSpec;
		pJSFunctionSpec=pJSFuncSpecNew;
		MP_DELETE_ARR( pJSFS);
	}
	return ML_OK;
}

mlresult mlJSClass::AddEventSpecs(EventSpec* &pEventSpec, EventSpec* apEventSpec, int anEvents){
	if(anEvents > 0){
		EventSpec* pES=pEventSpec;
		int nP;
		for(nP=0; pES->name; nP++,pES++);
		EventSpec* pEventSpecNew = MP_NEW_ARR( EventSpec, nP+anEvents+1);
		if(nP>0)
			memcpy(pEventSpecNew, pEventSpec, nP*sizeof(EventSpec));
		memcpy(pEventSpecNew+nP, apEventSpec, anEvents*sizeof(EventSpec));
		memcpy(pEventSpecNew+nP+anEvents, pEventSpec+nP, sizeof(EventSpec));
		pES=pEventSpec;
		pEventSpec=pEventSpecNew;
		MP_DELETE_ARR( pES);
	}
	return ML_OK;
}

void mlJSClass::Init(){
	memset(aEventNames,0,sizeof(const wchar_t*)*256);
	memset(aEventIsPW,0,sizeof(bool)*256);
}

mlresult mlJSClass::InitEventIDMap(MapEventID &mMapEventID, EventSpec* apEventSpec, JSPropertySpec* apJSPropertySpec){
	EventSpec* pEventSpec=apEventSpec;
	// ���� ���� ��������
	if(apJSPropertySpec!=NULL && apJSPropertySpec->name!=NULL){
		// ������� ���������� �������
		JSPropertySpec* pJSPS=apJSPropertySpec;
		int nP;
		for(nP=0; pJSPS->name; nP++,pJSPS++);
//		ML_ASSERTION(nP>0,"mlJSClass::InitEventIDMap");
		// ������� ���������� �������
		EventSpec* pEvSp=apEventSpec;
		int nE;
		for(nE=0; pEvSp->name; nE++,pEvSp++);
		// ����������� ������ ������� ��� ���������� ���� �������
		pEventSpec = MP_NEW_ARR( EventSpec, nE+nP+1);
		memcpy(pEventSpec, apEventSpec, nE*sizeof(EventSpec));
		// ��������� ������� �� ��������� (watch)
		pJSPS=apJSPropertySpec;
		pEvSp=pEventSpec+nE;
		for(; pJSPS->name; pJSPS++){
			if(pJSPS->flags!=0 || *((short*)pJSPS->name)!=0x6E6F){ // !="on..." 
				pEvSp->id=pJSPS->tinyid;
				pEvSp->prop=true;
				pEvSp->name=(wchar_t*)pJSPS->name;
				aEventNames[pEvSp->id]=pEvSp->name;
				aEventIsPW[pEvSp->id]=pEvSp->prop; // name - char*, � �� wchar_t*
				pEvSp++;
			}
		}
		pEvSp->id=0;
		pEvSp->prop=false;
		pEvSp->name=NULL;
		//
		MP_DELETE_ARR( apEventSpec);
		apEventSpec=pEventSpec;
	}
	mMapEventID = std::auto_ptr<EventSpec>(pEventSpec);
	while(apEventSpec->id){
		aEventNames[apEventSpec->id]=apEventSpec->name;
		aEventIsPW[apEventSpec->id]=apEventSpec->prop;
		apEventSpec++;
	}
	return ML_OK;
}

char mlJSClass::GetEventID(MapEventID &amMapEventID, wchar_t* apwcIDName){ 
	EventSpec* pEventSpec=amMapEventID.get();
//DumpEventNames();
	while(pEventSpec->id){
		if(pEventSpec->prop){
			if(isEqualOld(apwcIDName,(char*)pEventSpec->name)){
				return pEventSpec->id;
			}
		}else{
			if(isEqual(apwcIDName,pEventSpec->name)){
				return pEventSpec->id;
			}
		}
		pEventSpec++;
	}
	return 0; 
}

wchar_t* mlJSClass::GetEventName(MapEventID &amMapEventID, char acID){
	EventSpec* pEventSpec=amMapEventID.get();
//DumpEventNames();
	while(pEventSpec->id){
		if(pEventSpec->id==acID)
			return (wchar_t*)pEventSpec->name;
		pEventSpec++;
	}
	return NULL;
}

void mlJSClass::SetInstanceContext(JSContext* cx, JSObject* obj){
//DumpEventNames(cx);
	if(JSO_IS_MLEL(cx, obj)){
		mlRMMLElement* pMLEl = (mlRMMLElement*)JS_GetPrivate(cx, obj);
		pMLEl->SetJSContext(cx);
	}
}

// ����� ������������ ������� (�������) ��� ��������, ������� �� �������� RMML-����������
void mlJSClass::CallListenersStatic(JSContext* mcx, JSObject* mjso, const wchar_t* apwcEventName)
{
	jsval vFunc = JSVAL_NULL;
	if(apwcEventName == NULL) 
		return;
	// ���� ���� �������� onXXX
	if(wr_JS_GetUCProperty(mcx, mjso, apwcEventName, wcslen(apwcEventName), &vFunc) == JS_FALSE)
		return;
	if(!JSVAL_IS_STRING(vFunc)){
		if(!JSVAL_IS_REAL_OBJECT(vFunc))
			return;
		if(!JS_ObjectIsFunction(mcx, JSVAL_TO_OBJECT(vFunc)))
			return;
	}
	if(vFunc != JSVAL_NULL){
		// �������� �������
		jsval v;
		if(JSVAL_IS_STRING(vFunc)){
			JSString* jss = JSVAL_TO_STRING(vFunc);
			JS_EvaluateUCScript(mcx, mjso, JS_GetStringChars(jss), JS_GetStringLength(jss), 
				cLPCSTRq(apwcEventName), 1, &v);
		}else{
			JS_CallFunctionValue(mcx, mjso, vFunc, 0, NULL, &v);
		}
	}
}

}
