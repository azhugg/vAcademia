
#pragma once

class IDynamicTexture;

class IBrowser
{
public:

	// ���������� ������������ ��������
	virtual IDynamicTexture* GetDynamicTexture() = 0;
	// ���������� ������ "����" ��������
	virtual bool SetSize(int aiWidth, int aiHeight) = 0;
	// ��������� ������� ����
	virtual bool FireMouseDown(int x, int y)=0; // x,y ������������ ������ �������� ���� ���� ��������
	virtual bool FireMouseMove(int x, int y)=0;
	virtual bool FireMouseUp(int x, int y)=0;
	virtual bool FireMouseWheel(int x, int y, int distance)=0;
	// ��������� ������� ����������
	virtual bool KeyDown(unsigned int keyCode, unsigned int keyStates, unsigned int scanCode)=0;
	virtual bool KeyUp(unsigned int keyCode, unsigned int keyStates, unsigned int scanCode)=0;
	virtual bool KeyPress(unsigned int charCode, unsigned long scanCode)=0;
	virtual bool SetFocus()=0;
	virtual bool KillFocus()=0;
	// 
	virtual bool Navigate(const wchar_t* apwcURL, int flags=0, const wchar_t* postData=NULL, const wchar_t* headers=NULL)=0;
	//
	virtual bool ExecScript(const wchar_t* scriptCode, VARIANT* result)=0;
	// ���������� ����������� ������, ������� ���� ������������ ��� ��������� ��������� 
	// ��������������� �������� ���� ��������
	virtual void* GetCSForChildWindows() = 0;
	// �������� ����� �������� ����
	virtual bool SetParentWindow(HWND aHwnd) = 0;
	// ��������� � �������� HTML-�������� ������
	virtual bool LoadHTMLString(const wchar_t* apwcHTMLString, const wchar_t* apwcEncoding = NULL)=0;
};

#ifndef ML_VARIANT
namespace rmml{
	struct ML_VARIANT;
}
#endif

struct IBrowserCallBack
{
	//////////////////////////////////////////////////////////////////////////
	virtual void on_scrolled()=0; // �������� ���������������� html-�� (������� ����, ����������...)
	// Fired when a new hyperlink is being navigated to.
	virtual void on_before_navigate(const wchar_t* URL, int flags, const wchar_t* targetFrameName, const wchar_t* postData, const wchar_t* headers, bool &cancel)=0;
	// Fired when the document being navigated to becomes visible and enters the navigation stack
	virtual void on_navigate_complete(const wchar_t* URL)=0;
	virtual void on_document_complete(const wchar_t* URL)=0;
	// Fired when download progress is updated
	virtual void on_progress_change(int progress, int progressMax)=0;
	virtual void on_download_begin()=0; // Download of a page started
	virtual void on_download_complete()=0; // Download of page complete
	virtual void on_command_state_change(int command, bool enable)=0; // The enabled state of a command changed 
	// Fires when an error occurs during navigation
	virtual void on_navigate_error(const wchar_t* URL, const wchar_t* frame, int StatusCode, bool &cancel)=0;
	// ����� �� JavaScript ����� window.external
	virtual void external_callback(const wchar_t* apProcName, int aiArgC, 
		VARIANT* pvarArgs, VARIANT* varRes, bool abEvent)=0;
	// ������ �������� ���������, ���� ��������� ������
	virtual void on_texture_size_changed(int width, int height) = 0;
};
