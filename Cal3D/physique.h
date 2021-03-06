//****************************************************************************//
// physique.h                                                                 //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#ifndef CAL_PHYSIQUE_H
#define CAL_PHYSIQUE_H

#include "CommonCal3DHeader.h"

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "global.h"
#include "vector.h"
#include "hitverts.h"

//****************************************************************************//
// Forward declarations                                                       //
//****************************************************************************//

class CalModel;
class CalSubmesh;
class CFace;

//****************************************************************************//
// Class declaration                                                          //
//****************************************************************************//

 /*****************************************************************************/
/** The physique class.
  *****************************************************************************/

class CAL3D_API CalPhysique
{
	typedef std::vector<HIBOXVERT> vecHIBOXVERT;
// member variables
public:
  CalModel *m_pModel;

// constructors/destructor
public:
  CalPhysique();
	virtual ~CalPhysique();

// member functions	
public:
  D3DVECTOR getVertexPosition(CalSubmesh *pSubmesh, WORD vNum);
  void calculateFace(CFace*, float*,WORD, int lex1=0, int lex2=0, float phase=0, bool bUseVB=false);
  int CalPhysique::calculateTangentSpaces(CalSubmesh *pSubmesh, int mapId, float *pTangentSpaceBuffer/*, int stride*/);
  int calculateNormals(CalSubmesh *pSubmesh, float *pNormalBuffer);
  int calculateVertices(CalSubmesh *pSubmesh, float *pVertexBuffer, bool isUsualOrder);
  bool create(CalModel *pModel);
  void destroy();
  void update();

public:
	bool get_bbox_by_hitbs(CalVector *max, CalVector *min, std::vector<std::string>& names);
	void update_FAP();
	void createHitBoxes();
	MP_VECTOR<BOUNDVERTS> m_vectorHitBoxes;
	HITBOX	*pHitBoxes;
};

#endif

//****************************************************************************//
