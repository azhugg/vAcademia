
#include "mlRMML.h"
//#include "mlSynchLink.h"

namespace rmml {

mlSynchLinkJSO::mlSynchLinkJSO(void){
	muObjectID = OBJ_ID_UNDEF;
	muBornRealityID = 0;
	mpParentElem = NULL;
	mpParent = NULL;
	ref = NULL;
	status = SLS_UNDEFINED;
	mbLinkIsTracking = false;
}

JSBool mlSynchLinkJSO::AlterJSConstructor(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval){
	if(mlSynchLinkJSO::JSConstructor(cx, obj, argc, argv, rval) == JS_FALSE)
		return JS_FALSE;
	if(argc > 0){
		mlSynchLinkJSO* pThis = (mlSynchLinkJSO*)JS_GetPrivate(cx, obj);
		jsval vArg = argv[0];
		if(JSVAL_IS_OBJECT(vArg)){
			if(vArg != JSVAL_NULL){
				JSObject* jso = JSVAL_TO_OBJECT(vArg);
				if(JSO_IS_MLEL(cx, jso)){
					mlRMMLElement* pMLEl = GET_RMMLEL(cx, jso);
					pThis->muObjectID = pMLEl->muID;
					pThis->muBornRealityID = pMLEl->muBornRealityID;
					pThis->ref = jso;
					pThis->status = SLS_EXISTS;
					// ���� ��� ������ �� ������������������ ������
					if(pThis->muObjectID == OBJ_ID_UNDEF){
						// �� ����������� �� �� ������
						GPSM(cx)->TrackLink(pThis);
						pThis->mbLinkIsTracking = true;
					}
				}else{
					JS_ReportWarning(cx, "Link constructor argument must be a RMML object reference");
				}
			}
		}else{
			JS_ReportWarning(cx, "Link constructor argument must be an object reference");
		}
	}
	return JS_TRUE;
}

mlSynchLinkJSO::~mlSynchLinkJSO(void){
	if(mbLinkIsTracking){
		GPSM(mcx)->LinkDestroying(this);
	}
}

///// JavaScript Variable Table
JSPropertySpec mlSynchLinkJSO::_JSPropertySpec[] = {
//	JSPROP_RW(ref)
	JSPROP_RO(status)
	{ 0, 0, 0, 0, 0 }
};

///// JavaScript Function Table
JSFunctionSpec mlSynchLinkJSO::_JSFunctionSpec[] = {
	JSFUNC(getRef,1) // ���������� ������ �� �������� ������
	{ 0, 0, 0, 0, 0 }
};

MLJSCLASS_IMPL_BEGIN(Link,mlSynchLinkJSO,0)
	MLJSCLASS_ADDPROPFUNC
//	MLJSCLASS_ADDPROPENUMOP
	MLJSCLASS_ADDALTERCONSTR(AlterJSConstructor)
MLJSCLASS_IMPL_END(mlSynchLinkJSO)

#define ON_STATUS_FUNC_NAME L"onStatus"
#define ON_LOST_FUNC_NAME L"onLost"
#define ON_LOSING_FUNC_NAME L"onLosing"

JSBool mlSynchLinkJSO::JSSetProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp) {
	SET_PROPS_BEGIN2(mlSynchLinkJSO);
	/*switch(iID){
		//case JSPROP_ref:
		//	if(JSVAL_IS_OBJECT(*vp)){
		//		priv->ref = JSVAL_TO_OBJECT(*vp);
		//	}else{
		//		JS_ReportError(cx,"'ref' property value must be a reference to object");
		//		priv->ref = NULL;
		//	}
		////	priv->NameChanged();
		//	break;
	}*/
	SET_PROPS_END2
	else if(JSVAL_IS_STRING(id)){
		JSString* jssPropName = JSVAL_TO_STRING(id);
		// ���� ��������������� �������-���������� ������� onLost ��� onLosing
		if(isEqual(jssPropName, ON_LOST_FUNC_NAME) || isEqual(jssPropName, ON_LOSING_FUNC_NAME)){
			// �� ���� ������ ������� �� ������� �������
			GPSM(cx)->TrackLink(priv);
			priv->mbLinkIsTracking = true;
		}
	}
	return JS_TRUE;
}

JSBool mlSynchLinkJSO::JSGetProperty(JSContext *cx, JSObject *obj, jsval id, jsval *vp) {
	GET_PROPS_BEGIN2(mlSynchLinkJSO);
	switch(iID){
		//case JSPROP_ref:{
		//	priv->UpdateRef();
		//	if(priv->ref == NULL)
		//		*vp = JSVAL_NULL;
		//	else
		//		*vp = OBJECT_TO_JSVAL(priv->ref);
		//	}break;
		case JSPROP_status:
			// ���� ������ �� ���������, 
			if(priv->status == 0){
				// �� ��������� ��������� ����������� �������
				//priv->GetStatus();
			}
			*vp = INT_TO_JSVAL(priv->status);
			break;
	}
	GET_PROPS_END2;
	return JS_TRUE;
}

void mlSynchLinkJSO::Init(mlSynchLinkJSO* apSrc){
	muObjectID = apSrc->muObjectID;
	muBornRealityID = apSrc->muBornRealityID;
	mpParentElem = NULL;
	mpParent = NULL;
	ref = apSrc->ref;
	status = apSrc->status;
}

unsigned int mlSynchLinkJSO::CorrectBornRealityID(){
	oms::omsContext* pContext = GPSM(mcx)->GetContext();
	unsigned int uCorrectBornRealityID = muBornRealityID; 
	cm::cmIRealityInfo* realityInfo;
	if (pContext->mpMapMan->GetRealityInfo( muBornRealityID, realityInfo))
	{
		if (realityInfo->GetType() == cm::RT_ZERO 
			|| realityInfo->GetType() == cm::RT_CUSTOM
			|| realityInfo->GetType() == cm::RT_USER)
		{
			unsigned int uRealityID = pContext->mpMapMan->GetCurrentRealityID();; 
			while (uRealityID != 0 && uRealityID != muBornRealityID)
			{
				if (!pContext->mpMapMan->GetRealityInfo( uRealityID, realityInfo))
					break;
				if (realityInfo->GetType() == cm::RT_ZERO)
					break;
				if (realityInfo->GetType() == cm::RT_CUSTOM)
					break;
				if (realityInfo->GetType() == cm::RT_USER)
					break;
				uCorrectBornRealityID = uRealityID;
				uRealityID = realityInfo->GetParentRealityID();
			}
		}
	}
	return uCorrectBornRealityID;
}

void mlSynchLinkJSO::UpdateRef(bool abElemDestroyed){
	if(abElemDestroyed){
		CallEventJSFunc(ON_LOSING_FUNC_NAME);	// �� ���� ������ ������ ����� ���� ��� �����, �� ����� ������
	}
	JSObject* jsoWasRef = ref;
	if(muObjectID == OBJ_ID_UNDEF){
		if(abElemDestroyed)
			ref = NULL;
	}else{
		ref = NULL;
		// ������������ uLinkBornRealityID
		unsigned int uCorrectBornRealityID;
		if (mpParent != NULL)      // ���� ��� ������ �������� �������� ������� synch, �.�. ������ ����������������
			uCorrectBornRealityID = CorrectBornRealityID();  // ����� ��������������� muBornRealityID
		else
			uCorrectBornRealityID = muBornRealityID;           // �������������� muBornRealityID �� �����

		mlRMMLElement* pMLEl = GPSM(mcx)->GetScriptedObjectByID(muObjectID, uCorrectBornRealityID, false);
		if(pMLEl != NULL && pMLEl->GetMO() == PMOV_DESTROYED) // ���� �����-������ ��� ������, �� ����� �������, ��� ������ ������ �� ����������
			pMLEl = NULL;
		if(pMLEl != NULL){
			ref = pMLEl->mjso;
			status = SLS_EXISTS;
		}
	}
	if(jsoWasRef != NULL && ref == NULL){
		status = SLS_LOST;
		CallEventJSFunc(ON_STATUS_FUNC_NAME);
		CallEventJSFunc(ON_LOST_FUNC_NAME);
	}
}

void mlSynchLinkJSO::UpdateObjectID(){
	if((ref != NULL) && (JSO_IS_MLEL(mcx, ref))){
		mlRMMLElement* pMLEl = GET_RMMLEL(mcx, ref);
		if(pMLEl != NULL){
			muObjectID = pMLEl->muID;
			muBornRealityID = pMLEl->muBornRealityID;
		}
	}
}

JSObject* mlSynchLinkJSO::GetRef(){
	if(ref != NULL){
		return ref;
	}
	if(muObjectID == OBJ_ID_UNDEF)
		return NULL;
	unsigned int uCorrectBornRealityID = CorrectBornRealityID(); // added by Tandy 29.05.09
	mlRMMLElement* pMLEl = GPSM(mcx)->GetScriptedObjectByID(muObjectID, uCorrectBornRealityID, false);
	if(pMLEl != NULL){
		return pMLEl->mjso;
	}else
		return NULL;
}

// ��������� ��������� ����������� ������� �������
void mlSynchLinkJSO::GetStatus(){
	// ���� ������ ��� ��������� ��� ��������� ������ �� ����������� �������
	if(status != 0)
		return; // �� ������ �������
	// ���� ������ ��� ����, �� ���� ������� ������ �� ����������
	if(ref != NULL){
		status = SLS_EXISTS;
		return;
	}
	UpdateRef(); // ���� ������ ����, �� �� ���� ������ �� ����, �� ������ ����� ����������� � ������ ����
	if(status != 0)
		return; // �� ������ �������
	// �������� ������ �� ��������� ������� ������� �� ������
	if(!GPSM(mcx)->SendRequestForObjectStatus(muObjectID, muBornRealityID, this)){
		status = SLS_CANT_BE_DEFINED; // -4 - ������ ������� �� ����� ���� ���������
		return;
	}
	status = SLS_WAITING_STATUS; // 2 - ��������� ������ �� ��������� ������� �������, �� ����� ��� �� ������
}

JSBool mlSynchLinkJSO::JSFUNC_getRef(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval){
	mlSynchLinkJSO* pThis = (mlSynchLinkJSO*)JS_GetPrivate(cx, obj);
	pThis->UpdateRef();
	if(pThis->ref == NULL)
		*rval = JSVAL_NULL;
	else
		*rval = OBJECT_TO_JSVAL(pThis->ref);
	return JS_TRUE;
}

int mlSynchLinkJSO::GetObjectID(unsigned int& aBornRealityID){
	aBornRealityID = muBornRealityID;
	return muObjectID;
}

void mlSynchLinkJSO::CallEventJSFunc(const wchar_t* apwcName){
	if(GPSM(mcx)->IsDestroying())
		return;
	// �������� ����� onStatus, ���� �� ���������
	jsval vFunc=JSVAL_NULL;
//	#define ON_STATUS_FUNC_NAME L"onStatus"
	if(!JS_GetUCProperty(mcx, mjso, (const jschar*)apwcName, -1, &vFunc))
		return;
	if(vFunc == JSVAL_NULL)
		return;
	if(!JSVAL_IS_REAL_OBJECT(vFunc)){
		mlTraceWarning(mcx, "%S is not a function\n", apwcName);
		return;
	}
	if(!JS_ObjectIsFunction(mcx,JSVAL_TO_OBJECT(vFunc))){
		mlTraceWarning(mcx, "%S is not a function\n", apwcName);
		return;
	}
	unsigned char ucArgC = 0;
	jsval* vArgs=NULL;
	jsval v;
	//if(apwcResponse != NULL)
	//mlTrace(mcx, "onStatus: resp: %S\n", apwcResponse);
	//else 
	//mlTrace(mcx, "onStatus: resp: NULL\n", apwcResponse);
	JS_CallFunctionValue(mcx, mjso, vFunc, ucArgC, vArgs, &v);
}

// ���������� SceneManager-��, ����� ������� ����� � MapServer-�
// auObjectID � auBornRealityID - ��� ��������, ��� � ���� Link-� ��� ��� ��������� ������ ������ ����� �������
void mlSynchLinkJSO::onStatusReceived(unsigned int auObjectID, unsigned int auBornRealityID, int aiStatus){
	// ���� �������� ������ �� �� ������� ������
	if(muObjectID != auObjectID || muBornRealityID != auBornRealityID)
		return; // �� ������ ������� 
	// ���� ������ ��� �� ���� (��������, ������ ��� ������)
	if(status != SLS_WAITING_STATUS)
		return; // �� ���� �������
	status = aiStatus;
	UpdateRef();
	// �������� ����� onStatus, ���� �� ���������
	CallEventJSFunc(ON_STATUS_FUNC_NAME);
}

}

