//****************************************************************************//
// submesh.h                                                                  //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_SUBMESH_H
#define CAL_SUBMESH_H

#include "CommonCal3DHeader.h"

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//
#include "windows.h"
#include "global.h"
#include "vector.h"
#include "coresubmesh.h"

//****************************************************************************//
// Forward declarations                                                       //
//****************************************************************************//


//****************************************************************************//
// Class declaration                                                          //
//****************************************************************************//

 /*****************************************************************************/
/** The submesh class.
  *****************************************************************************/

class CAL3D_API CalSubmesh
{
// misc
public:
  /// The submesh PhysicalProperty.
  typedef struct
  {
    CalVector position;
    CalVector positionOld;
    CalVector force;
  } PhysicalProperty;

  struct TangentSpace
  {
	  CalVector tangent;
	  float crossFactor;
  };

  /// The submesh Face.
  typedef struct
  {
#ifdef FACESIZEISWORD
    WORD vertexId[3];
#else
	int vertexId[3];
#endif
  } Face;

// member variables
protected:
  CalCoreSubmesh *m_pCoreSubmesh;  
  MP_VECTOR<CalCoreSubmesh::STangentSpaceLevel > m_vectorvectorTangentSpace;
  MP_VECTOR<CalVector> m_vectorVertex;
  MP_VECTOR<CalVector> m_vectorNormal;
  MP_VECTOR<Face> m_vectorFace;
  MP_VECTOR<PhysicalProperty> m_vectorPhysicalProperty;
  int m_vertexCount;
  int m_faceCount;
  int m_coreMaterialId;
  bool m_bInternalData;

// constructors/destructor
public:
  CalSubmesh();
	virtual ~CalSubmesh();

// member functions	
public:
	void fill_indices(WORD* pdata, bool localCounter=false);
	void fill_vertices(LPVOID pdata);
	void lockVertices();
	void releaseVertices();
	void attachVB(IDirect3DDevice8 *pD3D8);
  float* getVertices();
  bool create(CalCoreSubmesh *pCoreSubmesh);
  void destroy();
  CalCoreSubmesh *getCoreSubmesh();
  int getCoreMaterialId();
  int getFaceCount();
#ifdef FACESIZEISWORD
  int getFaces(unsigned short* pFaceBuffer);
  int getFaces(unsigned short** pFaceBuffer);
#else
  int getFaces(int *pFaceBuffer);
  int getFaces(int **pFaceBuffer);
#endif
  std::vector<CalVector>& getVectorNormal();
  std::vector<CalCoreSubmesh::STangentSpaceLevel>& getVectorVectorTangentSpace();
  std::vector<PhysicalProperty>& getVectorPhysicalProperty();
  std::vector<CalVector>& getVectorVertex();
  std::vector<Face>& getVectorFace();
  bool isTangentsEnabled(int mapId) const;
  bool enableTangents(int mapId, bool enabled);
  int getVertexCount();
  bool hasInternalData();
  void setCoreMaterialId(int coreMaterialId);
  void setLodLevel(float lodLevel);

  // ���������� ��� ������ � VertexBuffer`��
//  LPDIRECT3DVERTEXBUFFER8	pVBUntransformed;
  int						vbStartIndex;
  // ���������� ��� ������ � IndexBuffer`��
//  LPDIRECT3DINDEXBUFFER8	pVBIndices;
  int						ibStartIndex;

  bool						bUseVB;
  //LPVOID					pVertData;
  int						stride;
  //D3DMATERIAL8				material;
};

#endif

//****************************************************************************//
