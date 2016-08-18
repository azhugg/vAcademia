#pragma once

class IPriorityUpdater;
class IChangesListener;
class ILogWriter;

#define		DOWNLOAD_MODE_AGRESSIVE_LOADING		0
#define		DOWNLOAD_MODE_MODERATE_LOADING		1

#define		DOWNLOAD_SCHEME_MESH_MAINLY			0
#define		DOWNLOAD_SCHEME_NORMAL				1

#include <vector>

#include "ireslibrary.h"

#define MAX_COORDS_PER_RESOURCES	16

struct resourceLoadStruct
{
	char	filename[MAX_PATH]; //IAsynchResource::GetFileName();
	long	fileSize;
	long	loadedPercent;
	float	x[MAX_COORDS_PER_RESOURCES];
	float	y[MAX_COORDS_PER_RESOURCES];
	float	z[MAX_COORDS_PER_RESOURCES];
	int		coordsSize;
	IAsynchResource*	pIAsynchResource;

	long	bytesPerSecond;
};

struct IAsyncResourcesLoadingStat
{
	//************************************
	// Method:    GetResourcesLoadInfo
	// Returns:   ���������� ����������� ��������
	// Qualifier: ���������� ���������� �� ����������� ��������.
	//************************************
	virtual int GetResourcesLoadInfo(resourceLoadStruct** pResourceLoadStruct) = 0;
};

struct IAsynchResourceManager
{
	// ������ ������ �� �������� � ���������� ��������� ��� ���������� ���������
	virtual IAsynchResource* GetAsynchResource(LPCSTR fileName, bool isInMainThread, DWORD mainPriority,
		IAsynchResourceHandler* handler, IResLibrary* resLibrary, std::vector<void*>* owners = NULL, void* exactObject = NULL,
		char scriptPriority = -1) = 0;

	// ��������� ������ �� ���������� �������� �������
	virtual void	AddLoadHandler(IAsynchResource* async, IAsynchResourceHandler* handler) = 0;

	virtual void	SetDownloadMode(int mode) = 0;

	// ������� ������ �� ���������� �������� �������
	virtual void	RemoveLoadHandler(IAsynchResource* async, IAsynchResourceHandler* handler/*, IResLibrary* resLibrary*/) = 0;

	// ������� ������ �� ���������� �������� �������
	virtual void	RemoveLoadHandler(IAsynchResourceHandler* handler/*, IResLibrary* resLibrary*/) = 0;

	// ���� ������ ����� ����������� � ���������� �� ���� ������
	virtual IAsynchResource* FindAsynchResource(LPCSTR fileName) = 0;

	// ��������� �������� ������� � ������� ������
	virtual void DeleteAsyncResourse( IAsynchResource* apIAsynchResource, IAsynchResourceHandler* handler/*, IResLibrary* resLibrary*/) = 0;

	// ���������������� �������� �������
	virtual void PauseDownloadAsyncResourse( IAsynchResource* apIAsynchResource) = 0;	
	
	virtual void Update() = 0;

	// ��������� �������� ������������ �������. ������������ ��� �������� �������, ���
	//	������� ������������ ����������. 
	virtual void SetLoadingMesh(void* mesh) = 0;
	virtual void* GetLoadingMesh() = 0;

	virtual void GetAllAsynchResources(std::vector<IAsynchResource *>& resources) = 0;
	virtual int GetAllAsynchResourcesCount() = 0;

	virtual void SetILogWriter(ILogWriter* lw) = 0;

	virtual void AddPotentionalTexture(void* object, char* src, float koef) = 0;
	virtual void DeletePotentionalTexture(void* object, char* src) = 0;

	virtual void	SetSeriousChangesListener(IChangesListener* seriousChangesListener) = 0;

	virtual bool	GetModifyTime(LPCWSTR fileName, __int64& modifyTime, bool abLocal = false) = 0;

	virtual void	EnableTimeLogging(bool isEnabled) = 0;

	virtual bool	IsLocalFileExists( LPCSTR fileName) = 0;

	virtual void	SetPriorityUpdater(IPriorityUpdater* updater) = 0;

	virtual bool	IsLocalScripts() = 0;
	virtual void	SetLocalScripts(bool isLocalScripts) = 0;
};