#pragma once

namespace rmml {

// ����� ���������� � �������
class mlScriptInfo{
public:
	MP_WSTRING mwsSrc;
	bool mbInclude;	// =true, ���� ������ �������� include-�� �� ��������� � ������������� �������
public:
	mlScriptInfo(const wchar_t* apwcSrc):
		MP_WSTRING_INIT(mwsSrc)
	{
		if(apwcSrc != NULL && *apwcSrc != L'\0')
			mwsSrc = apwcSrc;
		mbInclude = false;
	}
//	~mlScriptInfo();
};

class mlSceneManager;

// ����� �������, ������������� ���� ��������������� include-��
class mlScriptPreloader
{
public:
	// ����� ���������� �� include-�
	class mlIncludeInfoInternal: public mlScriptInfo{
	public:
		typedef MP_VECTOR<mlIncludeInfoInternal*> VecIncludeInfo;
		VecIncludeInfo mvIncludes;
		mlSceneManager* mpSceneManager;
		//mlScriptInfoInternal* mpParent;
		bool mbExtractResult; // ���������� �� ������� ������ include-��?
		bool mbIncludesExtracted; // ����������� ������� ������� include-� �� �������
		bool mbAllSubIncludesExtracted; // ����������� ������� ������� include-� �� ���� include-��
		bool mbLoaded; // ������ �������� (� ��� ����� � �������������)
	public:

		static void startElementNs(void* ctx ATTRIBUTE_UNUSED,
			const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
			int nb_namespaces, const xmlChar** namespaces, int nb_attributes,
			int nb_defaulted, const xmlChar** attributes);
		static void endElementNs(void* ctx ATTRIBUTE_UNUSED,
			const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI);

	private:
public: // �������� ��� �������
		// ��������� ������� ������� � ����������� ������ include-��
		bool FindInclude(const mlScriptInfo& aScriptInfo);
private:
		// ������� ��������� ������ � ����������� ������ include-��
		mlIncludeInfoInternal* FindIncludeInternal(const mlScriptInfo& aScriptInfo);
	public:
		mlIncludeInfoInternal(const mlScriptInfo& aScriptInfo, mlSceneManager* apSceneManager);
		virtual ~mlIncludeInfoInternal();

		// ������������ include-�� ���������� ������� ������ �� ���������: ������ �� ������
		//bool RemoveScript(const mlScriptInfo& aScriptInfo);

		// ������� �� ����� mwsSrc ������ include-��
		bool ExtractIncludes();

		// ��������� �� ��� include-� ��� ���������� �������?
		// ���������� -1, ���� ������ �� ������
		//int IsIncludesPreloadedFor(const mlScriptInfo& aScriptInfo);
	protected:
		// ������� ��� include-� � ���� ������, ������ ����������
		//void CollectIncludesIntoOneList();

		// ���� � ������ �������������� include � ������
		virtual bool ExtractOneInclude();

		// ���� ������������� include � ������ ���
		virtual bool LoadOneInclude(mlScriptPreloader* apScriptPreloader);
	};

	// ����� ���������� � �������
	class mlScriptInfoInternal: public mlIncludeInfoInternal{
	public:
		bool mbNeedToBeLoaded; // ������ ����� ���������
		bool mbAllIncludesLoaded; // ��� include-� ���������
	public:
		mlScriptInfoInternal(const mlScriptInfo& aScriptInfo, mlSceneManager* apSceneManager);
		~mlScriptInfoInternal();

		// ���� � ������ �������������� include � ������
		virtual bool ExtractOneInclude();

		// ���� ������������� include � ������ ���
		virtual bool LoadOneInclude(mlScriptPreloader* apScriptPreloader);

		// ��������� �� ��� include-� ��� ����� �������?
		bool IsIncludesAllPreloaded();
	};

private:
	mlSceneManager* mpSceneManager;

	typedef MP_VECTOR<MP_WSTRING> VecExtractedIncludeInfo;
	VecExtractedIncludeInfo mvExtractedIncludes;

	//// �������� ������: �������� ������ �������, ��� ������� ������������ include-�
	//mlScriptInfoInternal mRoot;
	typedef MP_VECTOR<mlScriptInfoInternal*> VecScriptInfo;
	VecScriptInfo mvScripts;

	// ������ ������� (� mRoot.mvIncludes), 
	// �������� �������� ���� ���������� �� ��������� update-�
	int miNextScriptToProcessIdx;

	// ���� ������ ���� ����������� �� ����� ������ ��������� include-��
	typedef MP_MAP<MP_WSTRING, int> MapAllIncludedScripts;
	MapAllIncludedScripts mAllIncludedScripts; 

public:
	mlScriptPreloader(mlSceneManager* apSceneManager);
	~mlScriptPreloader();

private:
	bool UpdateInternal();

	// ����� ������ � ������
	VecScriptInfo::iterator FindScriptInternal(const mlScriptInfo& aScriptInfo);

	// �������� ���� � ������� � ���������������� ����
	// ����� �� ��� ���������� ���������� �� ����, ������ �� �����������
	mlString NormalizeSrcPath(const wchar_t* apwcSrc);

public:
	// �������� ������ ��� ������������ ��� include-��
	// ���� abPreloadIncludes=true, �� ��������� ������ include-�, � ��� ������ ���
	// ���������� true, ���� ������ ������������� �������� (���� include-�, ������� ����� �������)
	bool AddScript(const mlScriptInfo& aScriptInfo, bool abPreloadIncludes);

	// ������������ include-�� ���������� ������� ������ �� ���������: ������ �� ������
	bool RemoveScript(const mlScriptInfo& aScriptInfo);

	// ��������� �� ��� include-� ��� ���������� �������?
	int IsIncludesPreloadedFor(const mlScriptInfo& aScriptInfo);

	// ��������� ��������� ��� ������������
	// aiTimeQuota (ms) - ����� �� �������, ������� �������� ��� ��������� ��� ������������
	void Update(int aiTimeQuota);

	// �������� ������ � ���� ���� ����������� include-��
	void RegIncludedScript(const wchar_t* apwcSrc);

	// ������ true, ���� ������ ��� ���������� � �������� ������ ���������
	bool IsAlreadyIncluded(const wchar_t* apwcSrc);

	// ������ ������� �� ���������� �����
	int ExtractIncludes( const wchar_t* apcFileSrc, const wchar_t** appIncludes, int count);
};

}
