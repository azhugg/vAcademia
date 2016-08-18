
#include "StdAfx.h"
#include "XMLList.h"
#include "RMContext.h"
#include "iasynchresource.h"
#include "PlatformDependent.h"
#include "ClassScriptsLoader.h"
#include "MapManagerPriority.h"
#include <algorithm>

CClassScriptsLoader::CClassScriptsLoader():
	MP_MAP_INIT(m_localClassesMap)
	, MP_MAP_INIT(m_includedFilesLoadingStateVec)
	, MP_MAP_INIT(m_includesPerFileMap)
{
	CXMLList localClassesList(GetApplicationRootDirectory() + L"local_classes.xml", true);
	std::string className;
	while (localClassesList.GetNextValue(className))
	{
		m_localClassesMap.insert(MP_MAP<MP_STRING, int>::value_type(MAKE_MP_STRING(className), 1)); 
	}
}

std::string CClassScriptsLoader::GetClassFileName(std::string& className)
{
	if( className.find(".xml") != std::string::npos)
		return className;
	return ":scripts\\\\" + className + ".xml";
}

bool CClassScriptsLoader::IsLocalClass(std::string& className)const
{
	MP_MAP<MP_STRING, int>::const_iterator it = m_localClassesMap.find(MAKE_MP_STRING(className));
	return (it != m_localClassesMap.end());
}

/*
��������� ����� ����������:
1. ����� �������� ����� � ������� - ��������� ��� ��� ���������� �������;
2. ��� ������� ������� ������� ����������� ������
3. ����� �������� ����� � �������� ������ ��� ����������
4. ��������� ��� ��� ������� �����������
*/

int CClassScriptsLoader::LoadClass(std::string& className)
{
	// ����� ��� ���������
	if (gRM->mapMan->IsJSObjClassDefined(className.c_str()))
	{
		return CLASS_LOADED;
	}

	// ����� ��������� - ����������
	if (IsLocalClass(className))
	{
		return CLASS_LOADING;
	}

	// ����� �� ��� �������� - ��� ����� �� �������
	if (IsObjectNotExists(className))
	{
		return CLASS_NOT_EXISTS;
	}

	// ���� �������� ��� � ��� ������� - ����� ����� � ��������
	if ( IsObjectLoaded(className))
	{
		std::string classFileName = GetClassFileName(className);
		int auIncludesLoadedState = GetIncludesLoadedState( classFileName);

		switch ( auIncludesLoadedState)
		{
			case INCLUDES_NOT_EXISTS:
				return CLASS_NOT_EXISTS;

			case INCLUDES_LOADING:
				return CLASS_LOADING;

			case INCLUDES_LOADED:
			{
				std::string jsClassLoad = "Player.module.load(\"" + classFileName + "\");";
				gRM->mapMan->ExecJS(jsClassLoad.c_str());
				
				return CLASS_LOADED;
			}
		}
	}

	// ���� ��� �� ����������� - ������� �� ��������
	if (!IsObjectLoading(className))
	{
		std::string classFileName = GetClassFileName(className);

		IAsynchResource* pAsynchResource = LoadObject(classFileName);
		if (pAsynchResource == NULL)
		{
			if (g->rl->GetAsynchResMan()->IsLocalScripts())
			{
				classFileName.erase(classFileName.begin(), classFileName.begin() + 9);
				classFileName = ":rmcommon:/scripts/server/scripts/" + classFileName;

				std::string jsClassLoad = "Player.module.load(\"" + classFileName + "\");";
				gRM->mapMan->ExecJS(jsClassLoad.c_str());			
			}

			return CLASS_LOADED;
		}

		pAsynchResource->SetPriority(PRIORITY_EXTREMELY_BIG);

		AddLoadingObjectIfNeeded(pAsynchResource, className);
	}

	return CLASS_LOADING;
}

void CClassScriptsLoader::OnAsynchResourceLoaded(IAsynchResource* asynch)
{
	std::string className = PopLoadingObject(asynch);
	
	if (className.size() == 0)
	{
		ATLASSERT(FALSE);
	}
	else
	{
		AddLoadedObject(className);

		// ?? ���� ������ ���������� � �������, �� �� ����� ��� � ����, 
		// �� ����� ��� ������ � ������ ������ ���������� ��� XML, 
		// ������� include-� � ��� �� ��������� �� ����������
		std::string classFileName = GetClassFileName(className);

		ExtractAndLoadIncludes( classFileName, asynch);
	}
}

void	CClassScriptsLoader::ExtractAndLoadIncludes( std::string fileName, IAsynchResource* asynch)
{
	// ��� ��� ��������
	if( m_includesPerFileMap.find(MAKE_MP_STRING(fileName)) != m_includesPerFileMap.end())
		return;

	// ������� ��� ������� �������� ������
	std::vector<std::string>	includeFilesVector;
	gRM->mapMan->ExtractIncludes( asynch, includeFilesVector);

	// ���������� ��� ������� �������� ������ �������� �����
	m_includesPerFileMap.insert( MP_MAP<MP_STRING, TIncludesMapValue>::value_type(MAKE_MP_STRING(fileName), includeFilesVector));

	// ��������, ��� ���� ���� ����������� (��� ��� ������� ����������)
	m_includedFilesLoadingStateVec.insert( MP_MAP<MP_STRING, int>::value_type(MAKE_MP_STRING(fileName), INCLUDES_LOADING));

	// ������� �� ���� �������� � �������� �� �� ��������
	std::vector<std::string>::iterator it = includeFilesVector.begin(), end = includeFilesVector.end();
	for ( ; it!=end; it++)
	{
		MP_STRING includeFileName = MAKE_MP_STRING(*it);

		// ���� ������ �������� ��� ���������, ���������
		if( m_includedFilesLoadingStateVec.find( includeFileName) != m_includedFilesLoadingStateVec.end())
			continue;

		if ( IsObjectNotExists(includeFileName))
		{
			m_includedFilesLoadingStateVec.insert( MP_MAP<MP_STRING, int>::value_type(includeFileName, INCLUDES_NOT_EXISTS));
			continue;
		}

		IAsynchResource* pAsynchResource = LoadObject(includeFileName);
		if (pAsynchResource == NULL)
		{
			m_includedFilesLoadingStateVec.insert( MP_MAP<MP_STRING, int>::value_type(includeFileName, INCLUDES_LOADED));
			continue;
		}

		pAsynchResource->SetPriority(PRIORITY_EXTREMELY_BIG);

		AddLoadingObjectIfNeeded(pAsynchResource, includeFileName);

		m_includedFilesLoadingStateVec.insert( MP_MAP<MP_STRING, int>::value_type(includeFileName, INCLUDES_LOADING));
	}
}

int		CClassScriptsLoader::GetIncludesLoadedState(std::string& fileName)
{
	TIncludesMapValue recursiveIncludesFilter;
	recursiveIncludesFilter.push_back( fileName);

	return GetIncludesLoadedStateImpl( fileName, recursiveIncludesFilter);
}

int		CClassScriptsLoader::GetIncludesLoadedStateImpl(std::string& fileName, TIncludesMapValue& recursiveIncludesFilter)
{
	MP_MAP<MP_STRING, int>::iterator loadedStateIt = m_includedFilesLoadingStateVec.find( MAKE_MP_STRING(fileName));
	if( loadedStateIt == m_includedFilesLoadingStateVec.end())
		return INCLUDES_LOADING;

	if( loadedStateIt->second != INCLUDES_LOADING)
		return loadedStateIt->second;

	MP_MAP<MP_STRING, TIncludesMapValue>::iterator it = m_includesPerFileMap.find( MAKE_MP_STRING(fileName));
	// �� ����� ������ � ������� �� ��������
	if( it == m_includesPerFileMap.end())
	{
		if( IsObjectNotExists( fileName))
			return INCLUDES_NOT_EXISTS;

		return INCLUDES_LOADING;
	}

	// ���� ���� ������, �� �� loaded
	int recursiveLoadedResult = INCLUDES_LOADED;

	TIncludesMapValue::iterator itbegin = it->second.begin(), itend = it->second.end();
	for( ; itbegin != itend; itbegin++)
	{
		std::string includeName = *itbegin;

		TIncludesMapValue::iterator recursiveIt = std::find( recursiveIncludesFilter.begin(), recursiveIncludesFilter.end(), includeName);
		if( recursiveIt != recursiveIncludesFilter.end())
			continue;

		recursiveIncludesFilter.push_back( includeName);
		int oneIncludeLoadedResult = GetIncludesLoadedStateImpl( includeName, recursiveIncludesFilter);
		recursiveIncludesFilter.erase( recursiveIncludesFilter.begin() + recursiveIncludesFilter.size() - 1);

		// ���� ���� �� ���� ����� ������ INCLUDES_NOT_EXISTS - �� ����, ����� ������ ������
		if( oneIncludeLoadedResult == INCLUDES_NOT_EXISTS)
		{
			//??
		}
		// ���� ���� �� ���� ����� ������ INCLUDES_LOADING - �� � ���� �������
		if( oneIncludeLoadedResult == INCLUDES_LOADING)
		{
			recursiveLoadedResult = INCLUDES_LOADING;
		}
	}

	loadedStateIt->second = recursiveLoadedResult;

	return recursiveLoadedResult;
}

CClassScriptsLoader::~CClassScriptsLoader()
{
}