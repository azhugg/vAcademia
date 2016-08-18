#include "mlRMML.h"
#include "config/oms_config.h"
#include "config/prolog.h"


namespace rmml {

mlScriptPreloader::mlScriptPreloader(mlSceneManager* apSceneManager):
	mpSceneManager(apSceneManager),
	miNextScriptToProcessIdx(0),
	MP_VECTOR_INIT(mvScripts),
	MP_VECTOR_INIT(mvExtractedIncludes),
	MP_MAP_INIT(mAllIncludedScripts)
{

}

mlScriptPreloader::~mlScriptPreloader()
{
	VecScriptInfo::iterator vi = mvScripts.begin();
	VecScriptInfo::iterator viEnd = mvScripts.begin();
	for(; vi != viEnd; vi++){
		mlScriptInfoInternal* pSII = *vi;
		MP_DELETE( pSII);
	}
	mvScripts.clear();
}

mlScriptPreloader::mlIncludeInfoInternal::mlIncludeInfoInternal(const mlScriptInfo& aScriptInfo, mlSceneManager* apSceneManager):
	mlScriptInfo(aScriptInfo),
	mpSceneManager(apSceneManager),
	mbIncludesExtracted(false),
	mbAllSubIncludesExtracted(false),
	mbExtractResult(false),
	mbLoaded(false),
	MP_VECTOR_INIT(mvIncludes)
{
	//mbExtractResult = ExtractIncludes();
}

mlScriptPreloader::mlScriptInfoInternal::mlScriptInfoInternal(const mlScriptInfo& aScriptInfo, mlSceneManager* apSceneManager):
	mlIncludeInfoInternal(aScriptInfo, apSceneManager), 
	mbNeedToBeLoaded(false),
	mbAllIncludesLoaded(false)
{
}

mlScriptPreloader::mlScriptInfoInternal::~mlScriptInfoInternal()
{
}

mlScriptPreloader::mlIncludeInfoInternal::~mlIncludeInfoInternal(){
	VecIncludeInfo::iterator vi = mvIncludes.begin();
	VecIncludeInfo::iterator viEnd = mvIncludes.begin();
	for(; vi != viEnd; vi++){
		mlIncludeInfoInternal* pSII = *vi;
		MP_DELETE( pSII);
	}
	mvIncludes.clear();
}

#ifdef RMML_USE_LIBXML2

xmlSAXHandler mlSAX2SIIHandlerStruct =
{
	NULL, // internalSubsetDebug,
	NULL, // isStandaloneDebug,
	NULL, // hasInternalSubsetDebug,
	NULL, // hasExternalSubsetDebug,
	NULL, // resolveEntityDebug,
	NULL, // getEntityDebug,
	NULL, // entityDeclDebug,
	NULL, // notationDeclDebug,
	NULL, // attributeDeclDebug,
	NULL, // elementDeclDebug,
	NULL, // unparsedEntityDeclDebug,
	NULL, // setDocumentLocatorDebug,
	NULL, // startDocumentDebug,
	NULL, // endDocumentDebug,
	NULL, 
	NULL, 
	NULL, // referenceDebug,
	NULL, // charactersDebug,
	NULL, // ignorableWhitespaceDebug,
	NULL, // processingInstructionDebug,
	NULL, // commentDebug,
	NULL, // mlRMMLBuilder::warningDebug, // warningDebug,
	NULL, // mlRMMLBuilder::errorDebug, // errorDebug,
	NULL, // mlRMMLBuilder::fatalErrorDebug, // fatalErrorDebug,
	NULL, // getParameterEntityDebug,
	NULL, // cdataBlockDebug,
	NULL, // externalSubsetDebug,
	XML_SAX2_MAGIC, 
	NULL, 
	mlScriptPreloader::mlIncludeInfoInternal::startElementNs, // startElementNsDebug,
	mlScriptPreloader::mlIncludeInfoInternal::endElementNs, // endElementNsDebug,
	NULL
};

xmlSAXHandlerPtr mlSAX2SIIHandler = &mlSAX2SIIHandlerStruct;

/*
* SAX2 specific callbacks
*/

struct mlScriptPreloaderSAXContext
{
	mlScriptPreloader::mlIncludeInfoInternal* mpSSI;
	int miDepth;
	mlXMLArchive* mpAr;

	mlScriptPreloaderSAXContext(mlScriptPreloader::mlIncludeInfoInternal* apSII, mlXMLArchive* apAr):
		mpSSI(apSII),
		mpAr(apAr)
	{
		miDepth = 0;
	}
};

void mlScriptPreloader::mlIncludeInfoInternal::startElementNs(void* ctx ATTRIBUTE_UNUSED,
								   const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
								   int nb_namespaces, const xmlChar** namespaces, int nb_attributes,
								   int nb_defaulted, const xmlChar** attributes)
{
	mlScriptPreloaderSAXContext* pContext = (mlScriptPreloaderSAXContext*) ctx;
	pContext->miDepth++;

	if(isEqual((const char*)localname, "rmml")){
		return; // skip <rmml ...
	}
	if(isEqual((const char*)localname, "include")){
		if(pContext->miDepth > 2) // ���, ��� ������ ������� ������ ����������
			return;
		mlString wsIncludeSrc;
		for (int i = 0; i < nb_attributes*5; i += 5){
			if(isEqual((const char*)attributes[i], "src")){
				int iLen = (int) (attributes[i + 4] - attributes[i + 3]);
				UTF82String((const char*)attributes[i + 3], iLen, wsIncludeSrc);
				break;
			}
		}
		if(!wsIncludeSrc.empty()){
			int iCurXMLSrcIdx = pContext->mpAr->miSrcIdx;
			mlString sCurXMLSrc = pContext->mpSSI->mpSceneManager->mSrcFileColl[iCurXMLSrcIdx];
			mlString sPath = pContext->mpSSI->mpSceneManager->RefineResPath(sCurXMLSrc, wsIncludeSrc.c_str());

			MP_NEW_P2( pIncludeSSI, mlIncludeInfoInternal, sPath.c_str(), pContext->mpSSI->mpSceneManager);
			pIncludeSSI->mbInclude = true;
			pContext->mpSSI->mvIncludes.push_back(pIncludeSSI);
		}
		return;
	}
}


void mlScriptPreloader::mlIncludeInfoInternal::endElementNs(void* ctx ATTRIBUTE_UNUSED,
	const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI)
{
	mlScriptPreloaderSAXContext* pContext = (mlScriptPreloaderSAXContext*) ctx;
	pContext->miDepth--;
/*
	if(isEqual((const char*)localname, "rmml")){
		return; // skip </rmml>
	}
	if(isEqual((const char*)localname, "define")){
		return;
	}
	if(isEqual((const char*)localname, "include")){
		//if(!pRMB->msIncludeSrc.empty()){
		//	if(ML_FAILED(pRMB->Include(pRMB->msIncludeSrc.c_str())))
		//		return;
		//}
		return;
	}
*/
}

#endif

bool mlScriptPreloader::mlIncludeInfoInternal::ExtractIncludes()
{
	if(mbIncludesExtracted)
		return mbExtractResult;
	mbIncludesExtracted = true;
	mbExtractResult = false;
	if(mwsSrc.empty())
		return false;
#ifdef RMML_USE_LIBXML2
	mlXMLArchive* pAr = NULL;
	if(ML_FAILED(mpSceneManager->OpenXMLArchive(mwsSrc.c_str(), pAr, false))){
		return false;
	}

	int ret = 0;

	//int res = SAXUserParseStream(mlSAX2Handler, pAr);

	mlRMMLBuilder oBuilder(mpSceneManager->GetJSContext());
	oBuilder.SetSceneManager(mpSceneManager);
	pAr->mpBuilder = &oBuilder;
	xmlParserCtxtPtr mpParserCtxt = oBuilder.CreateStreamParserCtxt(pAr);
	if (mpParserCtxt == NULL)
		return false;
	oBuilder.SetParserContext(mpParserCtxt);
	xmlSAXHandlerPtr pOldSaxHandler = mpParserCtxt->sax;
	mpParserCtxt->sax = mlSAX2SIIHandler;
	//mpParserCtxt->begin_line = 0;
	//mpParserCtxt->end_line = 0;
	mlScriptPreloaderSAXContext oSPSCtxt(this, pAr);
	mpParserCtxt->userData = &oSPSCtxt;

	mlSceneManager::mlLibXml2Funcs* pLibXml2Funcs = mpSceneManager->GetLibXml2Funcs();
	int iResult = pLibXml2Funcs->xmlParseDocument(mpParserCtxt);


	//	free(mpParserCtxt->node);

	if (mpParserCtxt->wellFormed)
		ret = 0;
	else{
		if (mpParserCtxt->errNo != 0)
			ret = mpParserCtxt->errNo;
		else
			ret = -1;
	}
	if (mlSAX2SIIHandler != NULL)
		mpParserCtxt->sax = NULL;
	if (mpParserCtxt->myDoc != NULL){
		pLibXml2Funcs->xmlFreeDoc(mpParserCtxt->myDoc);
		mpParserCtxt->myDoc = NULL;
	}
	mpParserCtxt->sax = pOldSaxHandler;
	pLibXml2Funcs->xmlFreeParserCtxt(mpParserCtxt);

	if (ret != 0){ // error
		xmlErrorPtr pErr = pLibXml2Funcs->xmlGetLastError();
		if (pErr != NULL && pErr->message != NULL){ //  && pErr->code == XML_IO_LOAD_ERROR
			//			mlTraceError(cx, "XML load error: %s\n",pErr->message);
		}
	}

	//mpSceneManager->CloseXMLArchive(pAr); // ����������� � mlRMMLBuilder::InputCloseCallback
	//CollectIncludesIntoOneList();
	if(mvIncludes.size() > 0){
		mbExtractResult = true;
		return true;
	}
	// ��� ��� include-�� ��� ������, �� ����� �������, ��� ��� ��� ��� ���������
	mbAllSubIncludesExtracted = true;
#endif
	return false;
}

// ����� ������ � ������
mlScriptPreloader::VecScriptInfo::iterator mlScriptPreloader::FindScriptInternal(const mlScriptInfo& aScriptInfo){
	VecScriptInfo::iterator vi = mvScripts.begin();
	VecScriptInfo::iterator viEnd = mvScripts.end();
	for(; vi != viEnd; vi++){
		mlScriptInfoInternal* pSSI = *vi;
		if(pSSI->mwsSrc == aScriptInfo.mwsSrc)
			return vi;
	}
	return viEnd;
}

// �������� ������ ��� ������������ ��� include-��
// ���� abPreloadIncludes=true, �� ��������� ������ include-�, � ��� ������ ���
// ���������� true, ���� ������ ������������� �������� (���� include-�, ������� ����� �������)
bool mlScriptPreloader::AddScript(const mlScriptInfo& aScriptInfo, bool abPreloadIncludes){
	// ���� ������ ��� ���� � ������
	VecScriptInfo::iterator vi = FindScriptInternal(aScriptInfo);
	if(vi != mvScripts.end()){
		// �� ��� � ���������
		(*vi)->mbNeedToBeLoaded = !abPreloadIncludes;
		if((*vi)->mbNeedToBeLoaded){
			(*vi)->mbAllIncludesLoaded = false;
			(*vi)->mbLoaded = false;
		}
		return true;
	}
	MP_NEW_P2( pSSI, mlScriptInfoInternal, aScriptInfo, mpSceneManager);
	pSSI->mbNeedToBeLoaded = !abPreloadIncludes;
	// ����� ��������� ������ include-�� �������� ������
	pSSI->ExtractIncludes();
	// ���� ���� include-�, ������� ��� �� ���������� 
	if(pSSI->mbExtractResult){
		// �� ����� ��������� ������� ��
		mvScripts.push_back(pSSI);
		return true;
	}
	// ����� ��� ������ ������� � �������. ����� ����������� ��� ������
	return false;
}

// ������������ include-�� ���������� ������� ������ �� ���������: ������ �� ������
bool mlScriptPreloader::RemoveScript(const mlScriptInfo& aScriptInfo){
	VecScriptInfo::iterator vi = FindScriptInternal(aScriptInfo);
	if(vi != mvScripts.end()){
		mlScriptInfoInternal* pSSI = *vi;
		mvScripts.erase(vi);
		MP_DELETE( pSSI);
		return true;
	}
	return false;
}

// ��������� ������� ������� � ����������� ������ include-��
bool mlScriptPreloader::mlIncludeInfoInternal::FindInclude(const mlScriptInfo& aScriptInfo){
	VecIncludeInfo::const_iterator cvi = mvIncludes.begin();
	VecIncludeInfo::const_iterator cviEnd = mvIncludes.end();
	for(; cvi != cviEnd; cvi++){
		const mlIncludeInfoInternal* pSSI = *cvi;
		if(pSSI->mwsSrc == aScriptInfo.mwsSrc)
			return true;
	}
	return false;
}

/*
// ������� ��� include-� � ���� ������, ������ ����������
void mlScriptPreloader::mlIncludeInfoInternal::CollectIncludesIntoOneList()
{
	while(true){
		bool bChanged = false;
		VecIncludeInfo::iterator vi = mvIncludes.begin();
		VecIncludeInfo::iterator viEnd = mvIncludes.end();
		for(; vi != viEnd; vi++){
			mlIncludeInfoInternal* pSSI = *vi;
			pSSI->CollectIncludesIntoOneList(); 
			if(pSSI->mvIncludes.size() > 0){
				// ������������ ��� ���-include-� � ���������� ������ include-��
				VecIncludeInfo::iterator vi2 = pSSI->mvIncludes.begin();
				VecIncludeInfo::iterator vi2End = pSSI->mvIncludes.end();
				for(; vi2 != vi2End; vi2++){
					mlIncludeInfoInternal* pSSI2 = *vi;
					// ���� include ��� ���� � ������ ��� �� �������� ������ � ������ �������
					if(FindInclude(*pSSI2) || mpSceneManager->IsAlreadyIncluded(pSSI2->mwsSrc.c_str())){
						// �� ��� ����� �������� �� �������
						 MP_DELETE( pSSI2;
					}else{
						mvIncludes.push_back(pSSI2);
					}
				}
				pSSI->mvIncludes.clear();
				bChanged = true;
				break;
			}
		}
		if(!bChanged)
			break;
	}
}
*/

mlScriptPreloader::mlIncludeInfoInternal* 
mlScriptPreloader::mlIncludeInfoInternal::FindIncludeInternal(const mlScriptInfo& aScriptInfo)
{
	VecIncludeInfo::iterator vi = mvIncludes.begin();
	VecIncludeInfo::iterator viEnd = mvIncludes.end();
	for(; vi != viEnd; vi++){
		mlIncludeInfoInternal* pSSI = *vi;
		if(pSSI->mwsSrc == aScriptInfo.mwsSrc)
			return pSSI;
	}
	return NULL;
}

// ���� � ������ �������������� include � ������
bool mlScriptPreloader::mlIncludeInfoInternal::ExtractOneInclude()
{
	if(!mbIncludesExtracted){
		ExtractIncludes();
		return true;
	}
	if(mbAllSubIncludesExtracted)
		return false;
	VecIncludeInfo::iterator vi = mvIncludes.begin();
	VecIncludeInfo::iterator viEnd = mvIncludes.end();
	for(; vi != viEnd; vi++){
		mlIncludeInfoInternal* pII = *vi;
		if(pII->mbAllSubIncludesExtracted)
			continue;
		if(pII->ExtractOneInclude())
			return true;
	}
	mbAllSubIncludesExtracted = true;
	return false;
}

// ���� � ������ �������������� include � ������
bool mlScriptPreloader::mlScriptInfoInternal::ExtractOneInclude()
{
	bool bRet = mlIncludeInfoInternal::ExtractOneInclude();
	// �� �������� �������������� include-��
	if(!bRet){
		// �������� ��� include-�, ������� ���� ��������� � ���� ������
		//CollectIncludesIntoOneList(); // �������� ������, ��� ��� ����� ������� ��������
	}
	return bRet;
}

// ���� ������������� include � ������ ���
bool mlScriptPreloader::mlIncludeInfoInternal::LoadOneInclude(mlScriptPreloader* apScriptPreloader)
{
	VecIncludeInfo::iterator vi = mvIncludes.begin();
	VecIncludeInfo::iterator viEnd = mvIncludes.end();
	for(; vi != viEnd; vi++){
		mlIncludeInfoInternal* pII = *vi;
		if(pII->LoadOneInclude(apScriptPreloader))
			return true;
		// ��� �������� include-� ���������
		if(!pII->mbLoaded){
			if(!apScriptPreloader->IsAlreadyIncluded(pII->mwsSrc.c_str())){
//if(isEqual(pII->mwsSrc.c_str(), L":scripts/slides_cache.xml"))
//pII->mbLoaded = true;
				mpSceneManager->Load(pII->mwsSrc.c_str(), false);
				apScriptPreloader->RegIncludedScript(pII->mwsSrc.c_str());
				pII->mbLoaded = true;
			}
			mvIncludes.erase(vi);
			MP_DELETE( pII);
			return true;
		}
	}
	return false;
}

// ���� ������������� include � ������ ���
bool mlScriptPreloader::mlScriptInfoInternal::LoadOneInclude(mlScriptPreloader* apScriptPreloader)
{
	if(mbAllIncludesLoaded && mbLoaded)
		return false;
	if(mlIncludeInfoInternal::LoadOneInclude(apScriptPreloader))
		return true;
	mbAllIncludesLoaded = true;
	if(mbNeedToBeLoaded){
		mpSceneManager->Load(mwsSrc.c_str(), false);
		mbLoaded = true;
	}
	return false;
}

/*
// ��������� �� ��� include-� ��� ���������� �������?
bool mlScriptPreloader::mlIncludeInfoInternal::IsIncludesPreloadedFor()
{
	mlIncludeInfoInternal* pSII = FindIncludeInternal(aScriptInfo);
	if(pSII == NULL)
		return -1;
	VecIncludeInfo::const_iterator cvi = pSII->mvIncludes.begin();
	VecIncludeInfo::const_iterator cviEnd = pSII->mvIncludes.end();
	for(; cvi != cviEnd; cvi++){
		const mlIncludeInfoInternal* pSII2 = *cvi;
		if(!pSII2->mbLoaded)
			return 0;
	}
	return 1;
}
*/

// ��������� �� ��� include-� ��� ����� �������?
bool mlScriptPreloader::mlScriptInfoInternal::IsIncludesAllPreloaded()
{
	if(mbAllIncludesLoaded)
		return true;
	VecIncludeInfo::const_iterator cvi = mvIncludes.begin();
	VecIncludeInfo::const_iterator cviEnd = mvIncludes.end();
	for(; cvi != cviEnd; cvi++){
		const mlIncludeInfoInternal* pII = *cvi;
		if(!pII->mbLoaded)
			return false;
	}
	mbAllIncludesLoaded = true;
	return true;
}

// ��������� �� ��� include-� ��� ���������� �������?
// ���������� -1, ���� ������ �� ������
int mlScriptPreloader::IsIncludesPreloadedFor(const mlScriptInfo& aScriptInfo)
{
	VecScriptInfo::iterator vi = FindScriptInternal(aScriptInfo);
	if(vi == mvScripts.end())
		return -1;
	mlScriptInfoInternal* pSII = *vi;
	return pSII->IsIncludesAllPreloaded()?1:0;
}

// ��������� ��������� ��� ������������
// aiTimeQuota (ms) - ����� �� �������, ������� �������� ��� ��������� ��� ������������
void mlScriptPreloader::Update(int aiTimeQuota)
{
	// ���� ��� �� ��������� ���������� �����
	unsigned long ulStartTime = 0;
	unsigned long ulNowTime = 0;
	if(aiTimeQuota > 0)
		ulNowTime = ulStartTime = mpSceneManager->GetContext()->mpApp->GetTimeStamp();
	while(ulNowTime == ulStartTime || (ulNowTime - ulStartTime) < (unsigned)aiTimeQuota){
		if (!UpdateInternal())
			return;	// ������ ������: �������
		if(aiTimeQuota > 0){
			ulNowTime = mpSceneManager->GetContext()->mpApp->GetTimeStamp();
		}else
			ulNowTime = 1;
	}

}

// ���������� true, ���� ���-�� �������
bool mlScriptPreloader::UpdateInternal()
{
	if(mvScripts.size() == 0)
		return false;
	int iStartScriptToProcessIdx = miNextScriptToProcessIdx;
	while(true){
		// ���������� ������� �� ������ � mRoot
		mlScriptInfoInternal* pSII = mvScripts[miNextScriptToProcessIdx];
		miNextScriptToProcessIdx++;
		if(miNextScriptToProcessIdx >= (int)mvScripts.size())
			miNextScriptToProcessIdx = 0;
		// ���� � ������ ���� �������, �� ������� ��� �� ��� include-� ���������� �� ������� 
		// �� ����������� ��������, �� ���������� ��������� include
		bool wasWorked = false;
		if(!pSII->mbAllSubIncludesExtracted){
			// ���� � ������ �������������� include � ������
			if(pSII->ExtractOneInclude())
				return true;
		}else{
			// ���� ��� ������� ����������, �������� �� ��� �� ��� ���������.
			if(!pSII->mbAllIncludesLoaded){
				// ���� ���, �� ��������� ���� include
				if(pSII->LoadOneInclude(this))
					return true;
			}else{
				//?? �������� ����� ��� ������� ������ �� ������
			}
		}
		if(miNextScriptToProcessIdx == iStartScriptToProcessIdx)
			return false; // ��������� ��� ������� � ������ �� �������
	}
}

inline std::wstring replace(std::wstring text, std::wstring s, std::wstring d)
{
	for(unsigned index=0; index=text.find(s, index), index!=std::wstring::npos;)
	{
		text.replace(index, s.length(), d);
		index+=d.length();
	}
	return text;
}

// �������� ���� � ������� � ���������������� ����
// ����� �� ��� ���������� ���������� �� ����, ������ �� �����������
mlString mlScriptPreloader::NormalizeSrcPath(const wchar_t* apwcSrc)
{
	mlString wsSrc(apwcSrc);
	wsSrc = mpSceneManager->ReplaceAliasesToModuleIDs(wsSrc);
	int iLastCommaPos = wsSrc.rfind(L':');
	if(iLastCommaPos != mlString::npos){
		if(wsSrc[iLastCommaPos+1] != L'\\')
			wsSrc.insert(iLastCommaPos+1, L"\\");
	}
	wsSrc = replace(wsSrc, L"/", L"\\");
	wchar_t* pwc = (wchar_t*)wsSrc.c_str();
	for(; *pwc != L'\0'; pwc++){ *pwc = tolower(*pwc); }
	return wsSrc;
}

// �������� ������ � ���� ���� ����������� include-��
void mlScriptPreloader::RegIncludedScript(const wchar_t* apwcSrc)
{
	mlString wsSrc = NormalizeSrcPath(apwcSrc);
	mAllIncludedScripts.insert(MapAllIncludedScripts::value_type(MAKE_MP_WSTRING(wsSrc), 1));  //std::pair<mlString, int>
}

// ������ true, ���� ������ ��� ���������� � �������� ������ ���������
bool mlScriptPreloader::IsAlreadyIncluded(const wchar_t* apwcSrc)
{
	mlString wsSrc = NormalizeSrcPath(apwcSrc);
	if(mAllIncludedScripts.find(MAKE_MP_WSTRING(wsSrc)) != mAllIncludedScripts.end())
		return true;
	return false;
}

// ������ ������� �� ���������� �����
int mlScriptPreloader::ExtractIncludes( const wchar_t* apcFileSrc, const wchar_t** appIncludes, int count)
{
	mvExtractedIncludes.clear();
	mlScriptInfo	scriptInfo( apcFileSrc);
	mlScriptPreloader::mlScriptInfoInternal scriptInfoInternal( scriptInfo, mpSceneManager);
	if( scriptInfoInternal.ExtractIncludes())
	{
		mlScriptPreloader::mlIncludeInfoInternal::VecIncludeInfo::iterator it = scriptInfoInternal.mvIncludes.begin()
								, end = scriptInfoInternal.mvIncludes.end();
		for ( ; it!=end; it++)
		{
			mlString wsSrc = NormalizeSrcPath((*it)->mwsSrc.c_str());
			mvExtractedIncludes.push_back( MAKE_MP_WSTRING(wsSrc.c_str()));
		}

		int includesCount = 0;
		VecExtractedIncludeInfo::iterator it2 = mvExtractedIncludes.begin(), end2 = mvExtractedIncludes.end();
		for ( ; it2 != end2; it2++)
		{
			if( count > 0)
			{
				*appIncludes = it2->c_str();
				appIncludes++;
				includesCount++;
			}
			else
			{
				break;
			}
		}
		return includesCount;
	}
	return 0;
}

}
