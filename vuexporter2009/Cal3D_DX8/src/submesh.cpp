//****************************************************************************//
// submesh.cpp                                                                //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "submesh.h"
#include "error.h"
#include "coresubmesh.h"

 /*****************************************************************************/
/** Constructs the submesh instance.
  *
  * This function is the default constructor of the submesh instance.
  *****************************************************************************/

CalSubmesh::CalSubmesh()
{
  m_pCoreSubmesh = 0;
  m_pVertices = NULL;
  //pVertData = NULL;
  stride=sizeof(D3DVERTEX);
  bUseVB = true;
  ibStartIndex = vbStartIndex = 0;
}

 /*****************************************************************************/
/** Destructs the submesh instance.
  *
  * This function is the destructor of the submesh instance.
  *****************************************************************************/

CalSubmesh::~CalSubmesh()
{
	if(m_pVertices != NULL)
		free(m_pVertices);
	if( bUseVB){
		//pVBUntransformed->Release();
		//pVBTransformed->Release();
	}
}

 /*****************************************************************************/
/** Creates the submesh instance.
  *
  * This function creates the submesh instance based on a core submesh.
  *
  * @param pCoreSubmesh A pointer to the core submesh on which this submesh
  *                     instance should be based on.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalSubmesh::create(CalCoreSubmesh *pCoreSubmesh)
{
  if(pCoreSubmesh == 0)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return false;
  }

  m_pCoreSubmesh = pCoreSubmesh;

  // reserve memory for the face vector
  m_vectorFace.reserve(m_pCoreSubmesh->getFaceCount());
  m_vectorFace.resize(m_pCoreSubmesh->getFaceCount());

  // set the initial lod level
  setLodLevel(1.0f);

  // set the initial material id
  m_coreMaterialId = -1;

  //�� �� ����� ���������� ����������� ������
  //m_pVertices = (D3DVECTOR*)malloc(sizeof(D3DVECTOR)*m_pCoreSubmesh->getVertexCount());
  m_pVertices = (D3DVERTEX*)malloc(sizeof(D3DVERTEX)*m_pCoreSubmesh->getVertexCount());
  /*int vertexId;
  for(vertexId = 0; vertexId < m_pCoreSubmesh->getVertexCount(); vertexId++){
      m_pVertices[vertexId].x = vectorVertex[vertexId].position.x;
	  m_pVertices[vertexId].y = vectorVertex[vertexId].position.y;
	  m_pVertices[vertexId].z = vectorVertex[vertexId].position.z;
  }*/

  // check if the submesh instance must handle the vertex and normal data internally
  if(m_pCoreSubmesh->getSpringCount() > 0)
  {
    m_vectorVertex.reserve(m_pCoreSubmesh->getVertexCount());
    m_vectorVertex.resize(m_pCoreSubmesh->getVertexCount());
    m_vectorNormal.reserve(m_pCoreSubmesh->getVertexCount());
    m_vectorNormal.resize(m_pCoreSubmesh->getVertexCount());
    m_vectorPhysicalProperty.reserve(m_pCoreSubmesh->getVertexCount());
    m_vectorPhysicalProperty.resize(m_pCoreSubmesh->getVertexCount());

    // get the vertex vector of the core submesh
    std::vector<CalCoreSubmesh::Vertex>& vectorVertex = m_pCoreSubmesh->getVectorVertex();

    // copy the data from the core submesh as default values
    int vertexId;
    for(vertexId = 0; vertexId < m_pCoreSubmesh->getVertexCount(); vertexId++)
    {
      // copy the vertex data
      m_vectorVertex[vertexId] = vectorVertex[vertexId].position;
      m_vectorPhysicalProperty[vertexId].position = vectorVertex[vertexId].position;
      m_vectorPhysicalProperty[vertexId].positionOld = vectorVertex[vertexId].position;

      // copy the normal data
      m_vectorNormal[vertexId] = vectorVertex[vertexId].normal;
    }

    m_bInternalData = true;
  }
  else
  {
    m_bInternalData = false;
  }

  return true;
}

 /*****************************************************************************/
/** Destroys the submesh instance.
  *
  * This function destroys all data stored in the submesh instance and frees
  * all allocated memory.
  *****************************************************************************/

void CalSubmesh::destroy()
{
  m_pCoreSubmesh = 0;
}

 /*****************************************************************************/
/** Returns the core material ID.
  *
  * This function returns the core material ID of the submesh instance.
  *
  * @return One of the following values:
  *         \li the \b ID of the core material
  *         \li \b -1 if an error happend
  *****************************************************************************/

int CalSubmesh::getCoreMaterialId()
{
  return m_coreMaterialId;
}

 /*****************************************************************************/
/** Provides access to the core submesh.
  *
  * This function returns the core submesh on which this submesh instance is
  * based on.
  *
  * @return One of the following values:
  *         \li a pointer to the core submesh
  *         \li \b 0 if an error happend
  *****************************************************************************/

CalCoreSubmesh *CalSubmesh::getCoreSubmesh()
{
  return m_pCoreSubmesh;
}

 /*****************************************************************************/
/** Returns the number of faces.
  *
  * This function returns the number of faces in the submesh instance.
  *
  * @return The number of faces.
  *****************************************************************************/

int CalSubmesh::getFaceCount()
{
  return m_faceCount;
}

 /*****************************************************************************/
/** Provides access to the face data.
  *
  * This function returns the face data (vertex indices) of the submesh
  * instance. The LOD setting of the submesh instance is taken into account.
  *
  * @param pFaceBuffer A pointer to the user-provided buffer where the face
  *                    data is written to.
  *
  * @return The number of faces written to the buffer.
  *****************************************************************************/

#ifdef FACESIZEISWORD
int CalSubmesh::getFaces(LPDIRECT3DINDEXBUFFER8 pFaceBuffer)
{
  // copy the face vector to the face buffer
   //pFaceBuffer = pVBIndices;
	assert(false);
#else
int CalSubmesh::getFaces(int *pFaceBuffer)
{
  // copy the face vector to the face buffer
  memcpy(pFaceBuffer, &m_vectorFace[0], m_faceCount * sizeof(int) * 3);
#endif

  return m_faceCount;
}

#ifdef FACESIZEISWORD
int CalSubmesh::getFaces(LPDIRECT3DINDEXBUFFER8 *pFaceBuffer)
{
  // copy the face vector to the face buffer
  // memcpy(pFaceBuffer, &m_vectorFace[0], m_faceCount * sizeof(WORD) * 3);
	//*pFaceBuffer = pVBIndices;
	assert(false);
#else
int CalSubmesh::getFaces(int **pFaceBuffer)
{
  // copy the face vector to the face buffer
  //memcpy(pFaceBuffer, &m_vectorFace[0], m_faceCount * sizeof(int) * 3);
	*pFaceBuffer = (WORD*)&m_vectorFace[0];
#endif

  return m_faceCount;
}

 /*****************************************************************************/
/** Returns the normal vector.
  *
  * This function returns the vector that contains all normals of the submesh
  * instance.
  *
  * @return A reference to the normal vector.
  *****************************************************************************/

std::vector<CalVector>& CalSubmesh::getVectorNormal()
{
  return m_vectorNormal;
}

 /*****************************************************************************/
/** Returns the physical property vector.
  *
  * This function returns the vector that contains all physical properties of
  * the submesh instance.
  *
  * @return A reference to the physical property vector.
  *****************************************************************************/

std::vector<CalSubmesh::PhysicalProperty>& CalSubmesh::getVectorPhysicalProperty()
{
  return m_vectorPhysicalProperty;
}

 /*****************************************************************************/
/** Returns the vertex vector.
  *
  * This function returns the vector that contains all vertices of the submesh
  * instance.
  *
  * @return A reference to the vertex vector.
  *****************************************************************************/

std::vector<CalVector>& CalSubmesh::getVectorVertex()
{
  return m_vectorVertex;
}

 /*****************************************************************************/
/** Returns the number of vertices.
  *
  * This function returns the number of vertices in the submesh instance.
  *
  * @return The number of vertices.
  *****************************************************************************/

int CalSubmesh::getVertexCount()
{
  return m_vertexCount;
}

 /*****************************************************************************/
/** Returns if the submesh instance handles vertex data internally.
  *
  * This function returns wheter the submesh instance handles vertex data
  * internally.
  *
  * @return One of the following values:
  *         \li \b true if vertex data is handled internally
  *         \li \b false if not
  *****************************************************************************/

bool CalSubmesh::hasInternalData()
{
  return m_bInternalData;
}

 /*****************************************************************************/
/** Sets the core material ID.
  *
  * This function sets the core material ID of the submesh instance.
  *
  * @param coreMaterialId The core material ID that should be set.
  *****************************************************************************/

void CalSubmesh::setCoreMaterialId(int coreMaterialId)
{
  m_coreMaterialId = coreMaterialId;
}

 /*****************************************************************************/
/** Sets the LOD level.
  *
  * This function sets the LOD level of the submesh instance.
  *
  * @param lodLevel The LOD level in the range [0.0, 1.0].
  *****************************************************************************/

void CalSubmesh::setLodLevel(float lodLevel)
{
  // clamp the lod level to [0.0, 1.0]
  if(lodLevel < 0.0f) lodLevel = 0.0f;
  if(lodLevel > 1.0f) lodLevel = 1.0f;

  // get the lod count of the core submesh
  int lodCount;
  lodCount = m_pCoreSubmesh->getLodCount();

  // calculate the target lod count
  lodCount = (int)((1.0f - lodLevel) * lodCount);

  // calculate the new number of vertices
  m_vertexCount = m_pCoreSubmesh->getVertexCount() - lodCount;

  // get face vector of the core submesh
  std::vector<CalCoreSubmesh::Face>& vectorFace = m_pCoreSubmesh->getVectorFace();

  // get face vector of the core submesh
  std::vector<CalCoreSubmesh::Vertex>& vectorVertex = m_pCoreSubmesh->getVectorVertex();

  // calculate the new number of faces
  m_faceCount = vectorFace.size();

  int vertexId;
  for(vertexId = vectorVertex.size() - 1; vertexId >= m_vertexCount; vertexId--)
  {
    m_faceCount -= vectorVertex[vertexId].faceCollapseCount;
  }

  // fill the face vector with the collapsed vertex ids
  int faceId;
  for(faceId = 0; faceId < m_faceCount; faceId++)
  {
    int vertexId;
    for(vertexId = 0; vertexId < 3; vertexId++)
    {
      // get the vertex id
      int collapsedVertexId;
      collapsedVertexId = vectorFace[faceId].vertexId[vertexId];

      // collapse the vertex id until it fits into the current lod level
      while(collapsedVertexId >= m_vertexCount) collapsedVertexId = vectorVertex[collapsedVertexId].collapseId;

      // store the collapse vertex id in the submesh face vector
      m_vectorFace[faceId].vertexId[vertexId] = collapsedVertexId;
    }
  }
}

//****************************************************************************//

float* CalSubmesh::getVertices()
{
	assert(false);
	/*if( !bUseVB)
		return (float*)m_pVertices;
	else
		return (float*)pVertData;*/
	return NULL;
}

void CalSubmesh::attachVB(IDirect3DDevice8 *pD3D8)
{
	assert(false);
	/*HRESULT hr;

	//
	if( getCoreSubmesh()->NoSkel()){
		lockVertices();
		// get vertex vector of the core submesh
		std::vector<CalCoreSubmesh::Vertex>& vectorVertex = getCoreSubmesh()->getVectorVertex();
		int vertexCount;
		vertexCount = getVertexCount();
		// calculate all submesh vertices
		int vertexId;
		int vstride = stride/sizeof(float) - 2;
		float* pvertices = (float*)pVertData;

		for(vertexId = 0; vertexId < vertexCount; vertexId++){
			// get the vertex
			CalCoreSubmesh::Vertex& vertex = vectorVertex[vertexId];
			*pvertices = vertex.position.x;	pvertices++;
			*pvertices = vertex.position.y;	pvertices++;
			*pvertices = vertex.position.z;
			pvertices += vstride;
		}
		releaseVertices();
	}*/
	///////////////////////////////////////////////////////////////////////
	/*WORD*	pIndices;
	pVBIndices->Lock( 0, 3*m_faceCount*sizeof(WORD), (BYTE**)&pIndices, 0);
	std::vector<CalCoreSubmesh::Face>& vectorFace = getCoreSubmesh()->getVectorFace();
	int faceCount = getFaceCount();
	std::vector<CalCoreSubmesh::Face>::iterator	it;
	for( it = vectorFace.begin(); it != vectorFace.end(); it++){
		*pIndices = (*it).vertexId[0]; pIndices++;
		*pIndices = (*it).vertexId[1]; pIndices++;
		*pIndices = (*it).vertexId[2]; pIndices++;
	}
	pVBIndices->Unlock();*/
	///////////////////////////////////////////////////////////////////////

	bUseVB = true;
	//
	// Set the RGBA for diffuse reflection. 
/*	material.Diffuse.r = 1.0f; 
	material.Diffuse.g = 1.0f; 
	material.Diffuse.b = 1.0f; 
	material.Diffuse.a = 1.0f; 

	// Set the RGBA for ambient reflection. 
	material.Ambient.r = 0.5f; 
	material.Ambient.g = 0.5f;
	material.Ambient.b = 0.5f; 
	material.Ambient.a = 1.0f; 

	// Set the color and sharpness of specular highlights. 
	material.Specular.r = 1.0f; 
	material.Specular.g = 1.0f; 
	material.Specular.b = 1.0f; 
	material.Specular.a = 1.0f; 
	material.Power = 50.0f; 

	// Set the color and sharpness of specular highlights. 
	material.Emissive.r = 0.0f; 
	material.Emissive.g = 0.0f; 
	material.Emissive.b = 0.0f; 
	material.Emissive.a = 0.0f; */

	// This code example uses the material properties defined for 
    return;
}

void CalSubmesh::releaseVertices()
{
	if( bUseVB){
		//pVBUntransformed->Unlock();
	}
}

void CalSubmesh::lockVertices()
{
	assert(false);
	/*int bcount = stride;
    if( FAILED( pVBUntransformed->Lock( bcount*vbStartIndex, getVertexCount()*bcount, (BYTE**)&pVertData, 0)))
		assert(false);*/
		//pVertData = NULL;
}

std::vector<CalSubmesh::Face>& CalSubmesh::getVectorFace()
{
  return m_vectorFace;
}

void CalSubmesh::fill_indices(WORD *pdata, bool localCounter)
{
	int	addval = localCounter ? 0 : vbStartIndex;
	std::vector<CalCoreSubmesh::Face>& vectorFace = getCoreSubmesh()->getVectorFace();
	int faceCount = getFaceCount();
	std::vector<CalCoreSubmesh::Face>::iterator	it;
	for( it = vectorFace.begin(); it != vectorFace.end(); it++){
		*pdata = (*it).vertexId[0]+addval; pdata++;
		*pdata = (*it).vertexId[1]+addval; pdata++;
		*pdata = (*it).vertexId[2]+addval; pdata++;
	}
}

void CalSubmesh::fill_vertices(LPVOID pdata)
{
	std::vector<CalCoreSubmesh::Vertex>& vectorVertex = getCoreSubmesh()->getVectorVertex();
	// calculate all submesh vertices
	int vertexId;
	int vstride = stride/sizeof(float) - 2;
	float* pvertices = (float*)pdata;

	for(vertexId = 0; vertexId < m_vertexCount; vertexId++)
	{
		// get the vertex
		CalCoreSubmesh::Vertex& vertex = vectorVertex[vertexId];
		*pvertices = vertex.position.x;	pvertices++;
		*pvertices = vertex.position.y;	pvertices++;
		*pvertices = vertex.position.z;
		pvertices += vstride;
	}
}

