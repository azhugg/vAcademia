#include <string>

class CStartUpdater
{
public:
	CStartUpdater(void);
	~CStartUpdater(void);

	// ���������� ���� � ������������ ���������
	std::wstring prepareUpdate(const std::wstring& sRootDir, const std::wstring& sAppDir, unsigned int& auErrorCode, unsigned int& auSystemErrorCode);	

private:
	unsigned int  copyFiles(const std::wstring& asFromDir, const std::wstring& asToDir, const wchar_t** alpcFilesNames, unsigned int auCount, unsigned int& auSystemErrorCode);
	// ���������� ���� � ����� � ����������
	std::wstring createDirectories(const std::wstring& asBaseDir, unsigned int& auErrorCode, unsigned int& auSystemErrorCode);
};