#pragma once


namespace rmml {

/**
 * ����� ���������� �������� RMML
 */

class mlRMMLText :
					public mlRMMLElement,
					public mlJSClass,
					public mlRMMLLoadable,
					public mlRMMLVisible,
					public mlRMMLPButton,
					public mlIText,
					public moIText
{
	JSString* value;
	bool mbStylesRef; // pStyles is reference to element
	JSObject* mjsoStyles;
	bool mbWidthIsSetInJS;
	bool mbHeightIsSetInJS;
public:
	mlRMMLText(void);
	void destroy() { MP_DELETE_THIS }
	~mlRMMLText(void);
MLJSCLASS_DECL
private:
	enum {
		JSPROP_value = 1,
		JSPROP_style,
		JSPROP_html,
		JSPROP_scrollTop,
		JSPROP_scrollLeft,
		JSPROP_sliceTail,
		// ������, ���������� �������
		JSPROP_textWidth,
		// ������, ���������� �������
		JSPROP_textHeight,
		// ���� "������", �� ������ �������� ������������� �������������
		// � ����������� �� ���������� ������
		JSPROP_adjustHeight,
		// ���� �������� �� ������� ���������� �����
		// '#RGB' '#RRGGBB' '#AARRGGBB'
		JSPROP_bkgColor,
		JSPROP_pageSize, // ������ 
		JSPROP_textSize, // ������ ������ � �������� (������������ ������� ��������� � ��������)
	};

	JSFUNC_DECL(scrollToLine);
	JSFUNC_DECL(addText);
	JSFUNC_DECL(getCursorPos);
	JSFUNC_DECL(increaseCursor);
	JSFUNC_DECL(decreaseCursor);
	JSFUNC_DECL(getVisibleBounds);
	JSFUNC_DECL(setCursorPos);
	JSFUNC_DECL(toClipboard);
	JSFUNC_DECL(fromClipboard);
	JSFUNC_DECL(setSelection);
	JSFUNC_DECL(isSliced);
	JSFUNC_DECL(getSymbolIDByXY);
	JSFUNC_DECL(areWordBreaksExists);

	JSString* style;
	bool html;
	JSString* sliceTail;
	bool adjustHeight;
	unsigned int textWidth, textHeight;
	mlColor bkgColor;
	unsigned int textSize, pageSize;

	mlRMMLElement* FindStyles(JSString* ajssStylesName);

	void CreateOwnStylesObject(const wchar_t* apwcStyle);

	void BeforeTextReformatting();
	void AfterTextReformatting();

// ���������� mlRMMLElement
MLRMMLELEMENT_IMPL
	mlresult CreateMedia(omsContext* amcx);
	mlresult Load();
	mlRMMLElement* Duplicate(mlRMMLElement* apNewParent);
	mlresult SetValue(wchar_t* apwcValue);
	mlString GetValue(int aiIndent);
	static void PropIsSet(mlRMMLElement* apThis, char idProp);

// ����������  mlILoadable
MLILOADABLE_IMPL

// ����������  mlIVisible
MLIVISIBLE_IMPL

// ����������  mlIButton
MLIBUTTON_IMPL

// ���������� mlIText
public:
	mlIText* GetIText(){ return this; }
	const wchar_t* GetValue(); // �������� �����
	//mlMedia* GetParent();
	mlStyle* GetStyle(const char* apszTag=NULL);
	bool IsHtml(){ return html; }
	const wchar_t* GetSliceTail(){ return sliceTail==NULL?NULL:(const wchar_t*)JS_GetStringChars(sliceTail);}
	bool IsAdjustHeight();
	mlSize GetFormatSize();
	// ���������� ������ ���������� ����
	// (����� �� ��������� � �������� ��������)
	void SetTextSize(unsigned int auWidth, unsigned int auHeight);
	const mlColor GetBkgColor();
	// ���������� ������ ������ � ���������� �����
	void SetScrollSizeInLines(unsigned int auPageSize, unsigned int auTextSize);

// ���������� moIText
public:
	// ��������
	unsigned int GetScrollTop();
	void SetScrollTop(unsigned int auValue);
	unsigned int GetScrollLeft();
	void SetScrollLeft(unsigned int auValue);
	void ScrollToLine(unsigned int auLine);
	void AddText(wchar_t* str);
	void IncreaseCursor(unsigned int delta);
	void DecreaseCursor(unsigned int delta);
	void GetVisibleBounds(int& _begin, int& _end);
	void SetSelection(int from, int to);
	int GetCursorX();
	int GetCursorY();
	int GetCursorPos();
	int GetCursorHeight();
	void ToClipboard(wchar_t* str);
	std::wstring FromClipboard();
	void SetCursorPos(int pos);
	int GetSymbolIDByXY(int x, int y);
	bool GetSliced();
	// �������
	void ValueChanged();
	void StylesChanged();
	void HtmlChanged();
	void AdjustChanged();
	void BkgColorChanged();
	// ������
	bool GetTextFormattingInfo(mlSynchData &aData); // ��� Authoring-�
	bool AreWordBreaksExists();
};

}
