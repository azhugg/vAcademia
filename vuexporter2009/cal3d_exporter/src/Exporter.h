//----------------------------------------------------------------------------//
// Exporter.h                                                                 //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//----------------------------------------------------------------------------//
// This program is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU General Public License as published by the Free //
// Software Foundation; either version 2 of the License, or (at your option)  //
// any later version.                                                         //
//----------------------------------------------------------------------------//

#ifndef EXPORTER_H
#define EXPORTER_H

//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//

#include <string>
#include <vector>
#include <list>
#include "FileEx.h"
#include "FileSys.h"

#include "../gd/gd_XMLArchive.h"
#include "n3dfile.h"
#include "MeshCandidate.h"
#include "SkeletonCandidate.h"
//#include "../../Cscl3DWS/RenderManager/Classes/MapManagers/ZoneObjectsTypes.h"
#include "ZoneObjectsTypes.h"

//----------------------------------------------------------------------------//
// Forward declarations                                                       //
//----------------------------------------------------------------------------//

class CBaseInterface;
class CBaseNode;
class CBoneCandidate;
class CSkeletonCandidate;
struct SObjectCfg;

typedef	std::map<CString/*base object name*/, int/*ref count*/>	MapReferencesNames;
typedef	std::map<CString/*real material name*/, CString/*generated material name*/>	MaterialsMap;

//----------------------------------------------------------------------------//
// Class declaration                                                          //
//----------------------------------------------------------------------------//
class CExportXMLFile : public CFile
{
public:
	CExportXMLFile(){}
	virtual ~CExportXMLFile(){}

	void Write( const TCHAR* pszBuffer, UINT uCount, UINT* puCount )
	{
		*puCount = uCount;
		CFile::Write((BYTE*)pszBuffer, uCount);
	}

	UINT Read( TCHAR* pszBuffer, UINT uCount, UINT* puCount )
	{
		*puCount = CFile::Read((BYTE*)pszBuffer, uCount);
		return *puCount;
	}	
};

/************************************************************************/
/*  Selected objects description
/************************************************************************/
//////////////////////////////////////////////////////////////////////////
#ifdef MAX3DS_BUILD


typedef	struct  _TWO_STRING
{
	CString	key;
	CString	value;
	_TWO_STRING(CString	akey, CString	avalue)
	{
		key = akey;
		value = avalue;
	}
	_TWO_STRING(const _TWO_STRING& item)
	{
		key = item.key;
		value = item.value;
	}
	void operator=(const _TWO_STRING& item)
	{
		key = item.key;
		value = item.value;
	}
} TWO_STRING;

typedef struct _LOD_PROPS
{
	int	lod;
	int distance;
	int shadow;
	CString resource;

	_LOD_PROPS()
	{
		lod = 0;
		resource = "";
		distance = -1;
		shadow = -1;
	}
} LOD_PROPS;

typedef	struct  _MAP_NODE_DESCRIPTION
{
	CBaseNode*	m_pBaseNode;
	//////////////////////////////////////////////////////////////////////////
	// Object name
	CString		m_Name;
	//////////////////////////////////////////////////////////////////////////
	// Object res
	CString		m_Res;
	//////////////////////////////////////////////////////////////////////////
	// Object type
	vespace::VESPACE_OBJECTS_TYPES		m_Type;
	//////////////////////////////////////////////////////////////////////////
	// Database ID
	BOOL		m_bDatabaseSet;
	DWORD		m_DatabaseID;
	//////////////////////////////////////////////////////////////////////////
	// Prefix
	CString		m_Prefix;
	//////////////////////////////////////////////////////////////////////////
	// Postfixes
	int			m_Level;				// object level
	int			m_Bias;					// bias
	int			m_Lod;					// lod
	std::vector<LOD_PROPS> m_LodProps;	// lod properties

	BOOL		m_Common;				// is common object
	BOOL		m_GenerateStencilData;	// generate stencil data shadow
	CString		m_zoneName;
	CString		m_logicObjName;
	//////////////////////////////////////////////////////////////////////////
	// References
	BOOL					m_IsReference;
	CString					m_ReferenceName;
	_MAP_NODE_DESCRIPTION*	m_pReferenceNodeDescription;
	//////////////////////////////////////////////////////////////////////////
	// Groups
	BOOL					m_IsGroupHead;
	BOOL					m_ExplodeGroup;
	INode*					m_pParentGroupNode;
	_MAP_NODE_DESCRIPTION*	m_pParentGroupNodeDescription;
	//////////////////////////////////////////////////////////////////////////
	// Paths
	CString					m_PathObjectDirectory;
	//////////////////////////////////////////////////////////////////////////
	// Bounds
	float	xmin, ymin, zmin;
	float	xmax, ymax, zmax;
	//////////////////////////////////////////////////////////////////////////
	// Geometry
	BOOL					m_NoGeometry;
	//////////////////////////////////////////////////////////////////////////
	// Class
	CString	m_Class;
	//////////////////////////////////////////////////////////////////////////
	// Other properties
	std::vector<TWO_STRING>	m_Properties;
	//////////////////////////////////////////////////////////////////////////
	// User properties
	CString					m_UserDefinedProperties;
	//////////////////////////////////////////////////////////////////////////
	// Grass
	BOOL					m_Grass;
	// zone
	BOOL					m_IsZone;
	//////////////////////////////////////////////////////////////////////////
	_MAP_NODE_DESCRIPTION()
	{
		m_Type = vespace::VE_UNDEFINED;
		m_ExplodeGroup = TRUE;
		m_pBaseNode = NULL;
		m_Name = "";
		m_Prefix;
		m_Level = 0;
		m_Bias = -1;
		m_Lod = 0; // --	
		m_Common = FALSE;
		m_IsReference = FALSE;
		m_ReferenceName = "";
		m_pReferenceNodeDescription = NULL;
		m_pParentGroupNode = NULL;
		m_Class = "";
		m_IsGroupHead = FALSE;
		m_pParentGroupNodeDescription = NULL;
		m_NoGeometry = FALSE;
		m_Grass = FALSE;
		m_IsZone = FALSE;
		m_bDatabaseSet = 0;
		m_DatabaseID = FALSE;
		m_zoneName = "";
	}
} MAP_NODE_DESCRIPTION;

typedef struct _PRIMITIVE_DESCRIPTION
{
	CalVector		translation;
	CalQuaternion	rotation;
	CalVector		scale;	

	float	xmin, ymin, zmin;
	float	xmax, ymax, zmax;

	int	type;

	_PRIMITIVE_DESCRIPTION()
	{
		translation.set(0, 0, 0);
		rotation.set(0, 0, 0, 1);
		scale.set(0, 0, 0);

		xmin = 0; ymin = 0; zmin = 0;
		xmax = 0; ymax = 0; zmax = 0;

		int type = 0;
		

	}

} PRIMITIVE_DESCRIPTION;

typedef struct _SITPLACE_DESCRIPTION 
{
	CalVector		translation;
	CalQuaternion	rotation;

	_SITPLACE_DESCRIPTION()
	{
		translation.set(0, 0, 0);
		rotation.set(0, 0, 0, 1);
	}	
} SITPLACE_DESCRIPTION;

typedef struct _CAMERA_DESCRIPTION
{
	CalVector	cameraPos; // ���������� ������� ������
	CalVector	targetPos; // ���������� ������� �������
	float			fov; // ��� ������
	int			id; // ���������� �����
	CString		name; // ��� ������

	_CAMERA_DESCRIPTION()
	{
		cameraPos.set(0, 0, 0);
		targetPos.set(0, 0, 0);
		fov = 0;
		id = 0;
		name = "";
	}

	std::vector<TWO_STRING> params;

} CAMERA_DESCRIPTION;

#endif

class CExporter
{
// member variables
protected:
	CBaseInterface *m_pInterface;
	std::string m_strLastError;

// constructors/destructor
public:
	CExporter();
	virtual ~CExporter();

	BOOL	overAmbient, overDiffuse;
	DWORD	dwAmbient, dwDiffuse;

// member functions
public:
	bool AddCoreKeyframe(int boneCandidateId, float settime, float time, CSkeletonCandidate* skeletonCandidate, CalCoreAnimation* coreAnimation);
	bool CreateCoreTrack(CBoneCandidate* pBoneCandidate, int boneCandidateId, bool bCommon, CalCoreAnimation* coreAnimation);
	void load_config(CFileEx& file, std::vector<Cal3DComString>& types, std::vector<Cal3DComString>& names);
	void SaveNode(std::ofstream& file, CBaseNode* node, int ilevel, Cal3DComString name);
#ifdef MAX3DS_BUILD
	bool SaveXMLScene(const Cal3DComString& fileName);
#endif
	void StartLog(CString fname);
	void WriteLog(CString info);
	void SaveMeshConfig(const std::string& strFilename, int smc, bool skel, CString txPath, CString txParent, CalVector* tr=NULL, CalQuaternion* rtn=NULL, BOOL m_bKeepConfig=TRUE);
	void SaveSceneConfig(CString fname, std::vector<CString>& names);
	bool Create(CBaseInterface *pInterface);
	bool ExportAnimation(const std::string& strFilename);
#ifdef MAX3DS_BUILD
	bool ExportMaterial(const std::string& strFilename, CalCoreMaterial* coreMaterial = NULL, StdMat* pMaxMtrl = NULL, BOOL abSaveMaps=TRUE);
#else
	bool ExportMaterial(const std::string& strFilename, BOOL abSaveMaps=TRUE);
#endif

	bool ExportMesh(std::string& strFilename);
	bool ExportTownMesh(std::string& strFilename);
	void WeldSelected( float dist);
	bool ExportSkeleton(const std::string& strFilename);
	CBaseInterface *GetInterface();
	const std::string& GetELastError();
	void SetLastError(const std::string& strText, const std::string& strFilename, int line);
	void SetLastErrorFromCal(const std::string& strFilename, int line);
	void RemoveFakeFrames( CalCoreAnimation* apCoreAnimation);

	void clearObjectCfg();
//#ifdef MAX3DS_BUILD
	bool parseXML(CExportXMLFile& cfgFile, const Cal3DComString& fn)	;
	bool saveXMLConfig(Cal3DComString fn);
	bool saveRefXMLConfig(const Cal3DComString& fn);
	bool saveStringToFile(CFile* file, Cal3DComString str);
	bool parse_reference(gd_xml_stream::CXMLArchive<CExportXMLFile>& ar);
	bool parse_meterial(gd_xml_stream::CXMLArchive<CExportXMLFile>& ar);
	bool parse_property(gd_xml_stream::CXMLArchive<CExportXMLFile>& ar);
	bool parse_collBox(gd_xml_stream::CXMLArchive<CExportXMLFile>& ar);

	void OpenZoneDescFile(CString strFilename);
	void WriteZoneDesc(std::vector<MAP_NODE_DESCRIPTION>& descs);
	void CloseZoneDescFile();

	void OpenLocationsDescFile(CString strFilename);
	void WriteLocationDesc(MAP_NODE_DESCRIPTION& location);
	void CloseLocationsDescFile();

	CString  GetLocationXmlDesc(MAP_NODE_DESCRIPTION* locObject, std::vector<CAMERA_DESCRIPTION>* cams);
	

//#endif

public:
	static	BOOL	calexpoptEXPORTSHADOWDATA;	// ������� ������ ��� ����������� �����
	static	BOOL	calexpoptUSELOCALCOORDS;	// ������ ������� ������ � ��������� ����������� ��������
	static	BOOL	calexpoptSTAYATLAST;		// 
	static	BOOL	calexpoptKEEPFRAME;			// 
	static	BOOL	calexpoptKEEPROTATIONSINANIMATION;			// 
	static	BOOL	calexpoptKeepConfig;
	static	BOOL	calexpoptNORMALIZEVERTS;	// 
	static	BOOL	calexpoptGenerateGlobalCoords;
	static	float	calexportFLOORZPOS;			// 
	static	int		maxMaterialIndex;			// 
	static	BOOL	calexpoptAutoexportToServer;	// ������������� ����������� ��������� � ������ �� ������, ���� � �������� � cal3d.cfg
	static	BOOL	calexpoptExportMaterials;	// �������������� ���������
	static	BOOL	calexpoptUseTexturesPath;	// ������������ ���� ��� ������ ������ � ���������
	static	BOOL	calexpoptUseMaterialsPath;	// ������������ ���� ��� ������ ������ � ����������
	static	BOOL	calexpoptGetnMtrlsNames;	// ������������ �������� ����������
	static	int		calexpoptAutoReplaceSameMaterials;	// 0 - ����������, 1 - ������������, 2 - �� ������������
	static	int		calexpoptAutoReplaceSameTextures;	// 0 - ����������, 1 - ������������, 2 - �� ������������
	static	CString	calexportTexturesPath;		// ���� ��� ��������������� �������
	static	CString	calexportMaterialsPath;		// ���� ��� ��������������� ����������
	static	CString	calexportExportDir;			// ������� ���� ��� ��������������� [root]
	static	BOOL	calexpoptUseBakedIfExists;	// ������������ baked ��������, ���� �� ����
	static	BOOL	calexpoptOptimizeAnimations;// �������������� �������� (���������� "���������������" �����)
	static	BOOL	calexportUsePrefixAsDirName;		// ������������ ������� ����� ������� ��� ��� ��� ����������
	static	BOOL	calexportSeparateCommonObjects;		// ��������� "����� �������" � ��������� �����
	static	BOOL	calexportSeparateCommonTextures;	// ��������� "����� ��������" � ��������� �����
	static	BOOL	calexportFindLODs;					// ������ � ��������� ����
	static	BOOL	calexportGenerateSQL;				// ������� SQL ��������� �� ����
	static	BOOL	calexportGenerateDBID;				// ��������� �������� DBID

	static	BOOL	calexprotUseLeftAssigment2Right;	// ������������ ����� �������� �� ������
	static	BOOL	calexprotUseRigthAssigment2Left;	// ������������ ������ �������� �� �����
	static	float	calexprotPosCompareValueExact;		// �������� ��������� ������� ������ � ������ ������
	static	BOOL	calexprotIsUseSide2SideEffect;		// ��������� ����� ����� � ����� ������� ��������� ��� ����� ������ �������

	static	BOOL	calexprotAutomatedExport;			// �������������� ������� ��� ������ �������������

	static	BOOL	calexportAbsoluteTexturesPaths;		// ��� ��������� � true - ��� �������� ���������� � ����� calexportTexturesPath ������������ ����� ��������

	static	CString	MaxInstallPath; // ���� ���������
	static	CString materialExportFormat; // ������ �������������� �������
	static	int		texturesExportQuality; // �������� �������������� �������

	static	BOOL	useFloatVertexCoords; // ������������� float ��� ���������� ��������� ������

	static	BOOL	exportGeometryOnly; // �������������� ������ ��������� (� �� ������ �� ������)
	static	BOOL	exportWithoutTextures; // �������������� ��� �������

	static	BOOL	libExport; // ������� � ����������


#ifdef MAX3DS_BUILD

protected:
	CString GetTexturesPath(const CString& objectPath, const CString& texturesPath);
	void	WeldVertices(  Mesh* mesh1, Matrix3 m1, 
								Mesh* mesh2, Matrix3 m2, 
								float dist);
	void	GetSelectedObjectsDesc(std::vector<MAP_NODE_DESCRIPTION>*	aSelectedObjects, MapReferencesNames& mapReferencesNames);
	bool	CreateCal3DObjects( MAP_NODE_DESCRIPTION* objectDesc, CMeshCandidate& meshCandidate, CSkeletonCandidate& skeletonCandidate, CalCoreMesh& coreMesh);

	PRIMITIVE_DESCRIPTION	GetColliderObject(MAP_NODE_DESCRIPTION* objectDesc); // ������� �������� ��������� �� �������
	SITPLACE_DESCRIPTION	GetSitplaceObject(MAP_NODE_DESCRIPTION* objectDesc); // �������� ��������� ����� �������
	
	void	GetNodeTransform( MAP_NODE_DESCRIPTION* objectDesc, CalVector& translation, CalQuaternion& rotation, CalVector& scale, bool localToParent = false);
	void	GetGroupBounds(MAP_NODE_DESCRIPTION* objectDesc, std::vector<MAP_NODE_DESCRIPTION>* vecSelectedObjects, CalVector& amin, CalVector& amax);
	void	GetNodeParentBounds(MAP_NODE_DESCRIPTION* objectDesc, std::vector<MAP_NODE_DESCRIPTION>* vecSelectedObjects, CalVector& amin, CalVector& amax);
	void	CreateBMFFile(const CString& meshFileName, MAP_NODE_DESCRIPTION* objectDesc,
									CMeshCandidate& meshCandidate,
									MaterialsMap& mapMaterialsNames,
									std::vector<int>& materialsIds,
									CString&	pathAbsoluteObjectDirectory,
									CString&	pathMaterials,
									CString&	pathObjectDirectory,
									std::vector<PRIMITIVE_DESCRIPTION>& colliderObjects,
									CString& maxPath);
	void	SaveGroupConfig(const CString& meshFileName, MAP_NODE_DESCRIPTION* objectDesc, std::vector<MAP_NODE_DESCRIPTION>*	aSelectedObjects);
	CString	GetObjectDirName( MAP_NODE_DESCRIPTION* objectDesc, MapReferencesNames& mapReferencesNames);
	void	FillObjectConfig( MAP_NODE_DESCRIPTION* objectDesc, CString meshFileName, CString skelFileName, SObjectCfg* objectCfg);

	CString GetMaterialName(int mId, CMeshCandidate& meshCandidate, std::vector<int>& materialsIds,
		MaterialsMap& mapMaterialsNames, MAP_NODE_DESCRIPTION*	objectDesc,	SECFileSystem* fs, 
		CString& pathAbsoluteObjectDirectory, CString& pathMaterials);

#endif

private:
	bool			keepFrameRotationSet;
	CalQuaternion	keepFrameRotation;
	CString	logFileName;
	FILE* ZoneDescFile;
	FILE* LocationsDescFile;
	
	std::vector<WORD> m_faces;
	std::vector<CalVector> m_vertex;
	std::vector<CalVector> m_texCoords;
	std::vector<int> m_facesInMaterial;
	std::vector<int> m_vertexInMaterial;
	std::vector<int> m_texLevelsForVertex;
	std::map<Cal3DComString, CalVector> m_internalZonePivots;
};

extern CExporter theExporter;

#endif

//----------------------------------------------------------------------------//
