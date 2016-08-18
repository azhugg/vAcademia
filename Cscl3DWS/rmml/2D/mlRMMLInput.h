#pragma once


namespace rmml {

/**
 * ����� ���������� �������� RMML
 */

class mlRMMLInput :	
					public mlRMMLElement,
					public mlJSClass,
					public mlRMMLVisible,
					public mlRMMLPButton,
					public mlIInput,
					public moIInput
{
	JSString* value;
	bool mbStylesRef; // pStyles is reference to element
	JSObject* mjsoStyles;
	bool	mbInGetValue;
public:
	mlRMMLInput(void);
	void destroy() { MP_DELETE_THIS }
	virtual ~mlRMMLInput(void);
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
		JSPROP_InputWidth,
		// ������, ���������� �������
		JSPROP_InputHeight,
		// ���� "������", �� ������ �������� ������������� �������������
		// � ����������� �� ���������� ������
		JSPROP_adjustHeight,
		JSPROP_password,
		JSPROP_tabGroup,
		JSPROP_tabIndex,
		JSPROP_multiline
	};
	JSString* style;
	bool html;
	JSString* sliceTail;
	bool adjustHeight;
	unsigned int InputWidth, InputHeight;
	bool password;
	bool multiline;
	int tabGroup;
	int tabIndex;

	JSFUNC_DECL(insertText)

	mlRMMLElement* FindStyles(JSString* ajssStylesName);

// ���������� mlMedia
	virtual void* GetInterface(long iid);

// ���������� mlRMMLElement
MLRMMLELEMENT_IMPL
	mlresult CreateMedia(omsContext* amcx);
	mlresult Load();
	mlRMMLElement* Duplicate(mlRMMLElement* apNewParent);
	mlresult SetValue(const wchar_t* apwcValue);

// ����������  mlIVisible
MLIVISIBLE_IMPL

// ����������  mlIButton
MLIBUTTON_IMPL

// ���������� mlIInput
public:
	mlIInput* GetIInput(){ return this; }
	virtual const wchar_t* GetValue(); // �������� �����
	virtual mlMedia* GetParent();
	virtual mlStyle* GetStyle(const char* apszTag=NULL);
	virtual bool IsHtml(){ return true; }
	virtual const wchar_t* GetSliceTail(){ return sliceTail==NULL?NULL:(const wchar_t*)JS_GetStringChars(sliceTail);}
	virtual bool IsAdjustHeight();
	// ���������� ������ ���������� ����
	// (����� �� ��������� � �������� ��������)
	virtual void SetInputSize(unsigned int auWidth, unsigned int auHeight);
	virtual bool GetPassword(){ return password; }
	virtual bool GetMultiline(){ return multiline; }
	virtual int GetTabGroup(){ return tabGroup; }
	virtual int GetTabIndex(){ return tabIndex; }
	virtual void OnKeyDown(int aiCode);
	virtual void OnSetFocus();
	virtual void OnKillFocus();

// ���������� moIInput
public:
	unsigned int GetScrollTop();
	void SetScrollTop(unsigned int auValue);
	unsigned int GetScrollLeft();
	void SetScrollLeft(unsigned int auValue);
	// �������
	void ValueChanged();
	void StylesChanged();
	void PasswordChanged();
	void MultilineChanged();
	void TabGroupChanged();
	void TabIndexChanged();
// ������ ������ ��� ��� �-��� ���� � ������� ����������
	virtual const wchar_t* GetValue() const;
	void HtmlChanged();
	void AdjustChanged();
	//
	bool InsertText(const wchar_t* apwcText, int aiPos=-1);
};

}
