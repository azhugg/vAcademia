// ����� ��� ���� ���������� ���������

// ��� ini-�����
#define INI_FILE_NAME _T( "setup.ini")

// ����� ������ � ini-�����
#define INI_GENERAL _T( "general")
// ��� ��������� �  ini-�����, ������� �������� ���� � ����� � �������� ��� ����������
#define INI_REG_KEY _T( "regSection")

#define MAX_CACHE_SIZE	300000000
	
// ������ ��������
enum EQueryError {
	qeNoError = 0, qeServerError, qeQueryNotFound, qeScriptError, qeReadError,
	qeNotImplError, qeServerNotResolved, qeTimeOut, qeNotExistFileReadError
};
