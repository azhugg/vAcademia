#pragma once

#include "CommonAvatarHeader.h"

#include "GlobalSingletonStorage.h"
#include "GPUCompositeTextureManager.h"
#include "CPUCompositeTextureManager.h"
#include "ICompositeTextureManager.h"
#include "AvatarObjPart.h"
#include "Commands.h"
#include "CompressingImageTask.h"
#include "ReadingCacheFileTask.h"

#define ERROR_VALUE -1
#define MAX_LEVELS	20
#define MAX_HEADS	10

class CCommandCache;

typedef struct _SCompositeTexture
{
	int srcTextureID;
	int resultTextureID;
	int levelCount;
	int texturePosX[MAX_LEVELS];
	int texturePosY[MAX_LEVELS];
	bool texturePosYNeedInvert[MAX_LEVELS];
	DWORD colors[MAX_LEVELS];
	DWORD alphaFuncs[MAX_LEVELS];
	BYTE alphaRefs[MAX_LEVELS];
	CSprite* sprites[MAX_LEVELS];
	bool isAddEnabled;
	bool isUpdated;
	int head;

	MP_STRING_DC fileNames[4];
	int skinColors[4][3];

	_SCompositeTexture();
	_SCompositeTexture(const _SCompositeTexture& tmp);	
} SCompositeTexture;

typedef MP_MAP<MP_STRING, int> TextureList;

class CCompositeTextureManager
{
friend class CGPUCompositeTextureManager;
friend class CCPUCompositeTextureManager;

public:
	CCompositeTextureManager();
	~CCompositeTextureManager();

	bool IsUpdated();
	bool IsLoaded();

	void SetUpdatePermission(bool isPermitted);

	void SetSpecialDiffuseKoef(int textureID, float koef);

	// ������� � �������� ����� ��������� �������� �� ������, ��������� �� RMML
	int AddCompositeTexture(int headID, std::string fromRMMLString);
	// ���������, ���������� �� ��� ��������� ����������� ��������
	bool ExistsNamedComposite(std::string& resultTextureName);
	// ���������� ����� ����������� ��������
	int GetNamedCompositeIndex(std::string& resultTextureName);
	// ������� � �������� ����� ��������� ��������
	int AddCompositeTexture(std::string name, std::string resultTextureName, std::string backgroundFileName, std::string secondFileName, int headID);
	// �������� ��� ���� ����� (��������) � ��������� ��������
	void AddLevelToCompositeTexture(unsigned int textureID, std::string fileName);
	// �������� �������� � ����� �� �������
	void ReplaceLevelInCompositeTexture(unsigned int textureID, int level, std::string fileName);

	void CopyTextureFromLevel(unsigned int textureID, int levelTo, int levelFrom);
	void EnableAddMode(unsigned int textureID);

	// ���������� ID ��������� �������� �� � �����
	int GetCompositeTextureID(std::string name);

	bool LoadCompositeTextureStrings(std::string fileName, CCommandCache* commandCache);

	//�������� � ����������� ���������� ������ ��������, ����������� ���������� � �����, � ������� ������
	void LoadAsynchCompressedTexture();

	// ���������� ��������� ����
	void SetDiffuse(unsigned int textureID, int nLevel, DWORD color);
	// ���������� ��������� ����
	DWORD GetDiffuse(unsigned int textureID, int nLevel);
	// ���������� �������� ����� ��� ����������
	void SetAlphaRef(unsigned int textureID, int nLevel, BYTE alphaRef);
	//���������� ���������� ��������
	void SetTexturePosX(unsigned int uitextureID, int iLevel, int uipos);
	void SetTexturePosY(unsigned int uitextureID, int iLevel, int uipos, bool isNeedInvert);

	// ���������� ������������ ��� ����������
	void SetAlphaFunc(unsigned int textureID, int nLevel, BYTE alphaFunc);

	// ����������� ��������� ��������
	void UpdateTexture(unsigned int textureID);

	// ������ ������ �� �������� �������� �� ������ �� �����������,
	// ���� ���-�� ��� ��� �� ���� �������
	void UpdateLinksToCompositeTextures(C3DObject* pObj, std::vector<SAvatarObjPart>& avatarParts);
	void DeleteAllCompositeTextures();

	void SetMyAvatar(bool isMyAvatar);
	bool IsMyAvatar();
	void SetEditorVisible(bool isEditorVisible);

	void SetHeadID(C3DObject* obj3d, int headID);
	int  GetHeadID()const;

	void SaveOptions(int textureID);
	void RestoreOptions(int textureID);

	void UpdateLinksToCompositeTextures(C3DObject* pObj, bool isLink = true);

	void SetSkinColors(unsigned int textureID, unsigned char* colors);

	CReadingCacheFileTask* GetReadingCacheTask();
	void SetReadingCacheTask(CReadingCacheFileTask* task);

	std::string GetCacheFileName(int textureID);	

	bool IsNeeded();

private:
	void ClearCompressedTexture(CTexture* texture);
	void DeleteCompressingTasks();
	void DeleteCompositeTexture(SCompositeTexture* texture);
	void DeleteCompositeTextures();

	std::string GetAvatarName();

	bool CheckForCompressedData(CTexture* texture, std::string& cacheFileName);
	void CompressTexture(CTexture* texture, std::string& cacheFileName);
	void CompressTexture(unsigned char* textureData, CTexture* texture, std::string& cacheFileName, bool isTextureRGBA = true);
	void CheckTextureForMyAvatar(CTexture* texture, std::string& cacheFileName);	
	CSprite* LoadSprite(std::string fileName, int level);
	int GetNextColorPart(int initialPos, std::string str, std::string& result);
	int GetNextStringPart(int initialPos, std::string str, std::string& result);
	DWORD GetDiffuseFromString(std::string str);	

	MP_VECTOR<SCompositeTexture> m_textures;
	TextureList m_texturesByID;

	MP_STRING m_lastStr;

	unsigned int m_timeForLevel[MAX_LEVELS][2];

	MP_STRING m_backgroundTextureName;
	bool m_isUpdatePermitted;
	bool m_isCompressed;

	bool m_isMyAvatar;

	int m_headID;
	int m_lastHeadID;

	MP_WSTRING m_name;

	MP_STRING_DC m_str[MAX_HEADS];

	C3DObject* m_obj3d;

	SCompositeTexture m_savedTexture;

	ICompositeTextureManager* m_renderImpls[2];
	int m_currentRenderImplID;

	MP_VECTOR<CCompressingImageTask*> m_compressingImageTasks;

	bool m_isEditorVisible;

	CReadingCacheFileTask* m_cacheTask;
};
