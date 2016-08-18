//****************************************************************************//
// bone.cpp                                                                   //
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

#include "error.h"
#include "bone.h"
#include "corebone.h"
#include "skeleton.h"

 /*****************************************************************************/
/** Constructs the bone instance.
  *
  * This function is the default constructor of the bone instance.
  *****************************************************************************/

CalBone::CalBone()
{
  m_pCoreBone = 0;
  m_pSkeleton = 0;
}

 /*****************************************************************************/
/** Destructs the bone instance.
  *
  * This function is the destructor of the bone instance.
  *****************************************************************************/

CalBone::~CalBone()
{
}

 /*****************************************************************************/
/** Interpolates the current state to another state.
  *
  * This function interpolates the current state (relative translation and
  * rotation) of the bone instance to another state of a given weight.
  *
  * @param weight The blending weight.
  * @param translation The relative translation to be interpolated to.
  * @param rotation The relative rotation to be interpolated to.
  *****************************************************************************/

void CalBone::blendState(float weight, const CalVector& translation, const CalQuaternion& rotation)
{
	if (weight > 0.5f)
	{
		int ii = 0;
	}

	if(m_accumulatedWeightAbsolute == 0.0f)
	{
		// it is the first state, so we can just copy it into the bone state
		//m_translationAbsolute = translation;

		if( m_pCoreBone && getParentId()!=-1)
		{
			m_translationAbsolute = m_translation;
		}
		else
			m_translationAbsolute = translation;

		m_rotationAbsolute = rotation;

		m_accumulatedWeightAbsolute = weight;
	}
	else
	{
		// it is not the first state, so blend all attributes
		float factor;
		if( m_accumulatedWeightAbsolute + weight == 0)
			factor = 0.0;
		else
			factor = weight / (m_accumulatedWeightAbsolute + weight);

		if( !m_pCoreBone )
		{
			m_translationAbsolute.blend(factor, translation);
		}
		else
		{
			m_rotationAbsolute.blend(factor, rotation);
		}


		m_accumulatedWeightAbsolute += weight;
	}
} 

 /*****************************************************************************/
/** Calculates the current state.
  *
  * This function calculates the current state (absolute translation and
  * rotation, as well as the bone space transformation) of the bone instance
  * and all its children.
  *****************************************************************************/

void CalBone::calculateState()
{
	Cal3DComString name = getCoreBone()->getName();
	if (name.Find("teach bip Head") != -1) {
		int ii=0;
	}
  // check if the bone was not touched by any active animation
  if(m_accumulatedWeight == 0.0f)
  {
    // �� ���� ��� �� ����� ����������� ����� �������� 
	// ����� ��������������� � ��������� ���������
    //m_translation = m_pCoreBone->getTranslation();
    //m_rotation = m_pCoreBone->getRotation();
  }

  // get parent bone id
  int parentId;
  parentId = m_pCoreBone->getParentId();

  if(( parentId == -1)&( !m_pSkeleton->haveOwner()))
  {
	// ���� ��� ������������ �����, �� ���. ������. = �����. ������.
    m_translationAbsolute = m_translation;
    m_rotationAbsolute = m_rotation;
  }
  else
  {
    // get the parent bone
    CalBone *pParent;
	if( m_pSkeleton->haveOwner() && ( parentId == -1))
		pParent = m_pSkeleton->m_pOwnerBone;
	else
		pParent = m_pSkeleton->getBone(parentId);

    // transform relative state with the absolute state of the parent
    m_translationAbsolute = m_translation;
    m_translationAbsolute *= pParent->getRotationAbsolute();
    m_translationAbsolute += pParent->getTranslationAbsolute();
	
    m_rotationAbsolute = m_rotation;
	m_rotationAbsolute *= pParent->getRotationAbsolute();
	/*if( m_pSkeleton->haveOwner()){
		m_rotationAbsolute = CalQuaternion(-0.022, -0.084, 0.64, 0.75);
		m_translationAbsolute = CalVector(-56.825, -7.573, 168.753);
		
	}*/
  }

  // calculate the bone space transformation
  if( getCoreBone()->getName() == "slaver"){
	  int i=0;
  }
  m_translationBoneSpace = m_pCoreBone->getTranslationBoneSpace();
  m_translationBoneSpace *= m_rotationAbsolute;
  m_translationBoneSpace += m_translationAbsolute;

  m_rotationBoneSpace = m_pCoreBone->getRotationBoneSpace();
  m_rotationBoneSpace *= m_rotationAbsolute;

  // calculate all child bones
  std::list<int>::iterator iteratorChildId;
  for(iteratorChildId = m_pCoreBone->getListChildId().begin(); iteratorChildId != m_pCoreBone->getListChildId().end(); ++iteratorChildId)
  {
    m_pSkeleton->getBone(*iteratorChildId)->calculateState();
  }
}

 /*****************************************************************************/
/** Clears the current state.
  *
  * This function clears the current state (absolute translation and rotation)
  * of the bone instance and all its children.
  *****************************************************************************/

void CalBone::clearState()
{
  m_accumulatedWeight = 0.0f;
  m_accumulatedWeightAbsolute = 0.0f;
}

 /*****************************************************************************/
/** Creates the bone instance.
  *
  * This function creates the bone instance based on a core bone.
  *
  * @param pCoreBone A pointer to the core bone on which this bone instance
  *                  should be based on.
  *
  * @return One of the following values:
  *         \li \b true if successful
  *         \li \b false if an error happend
  *****************************************************************************/

bool CalBone::create(CalCoreBone *pCoreBone)
{
  if(pCoreBone == 0)
  {
    CalError::setLastError(CalError::INVALID_HANDLE, __FILE__, __LINE__);
    return false;
  }

  m_pCoreBone = pCoreBone;

  return true;
}

 /*****************************************************************************/
/** Destroys the bone instance.
  *
  * This function destroys all data stored in the bone instance and frees all
  * allocated memory.
  *****************************************************************************/

void CalBone::destroy()
{
  m_pCoreBone = 0;
  m_pSkeleton = 0;
}

 /*****************************************************************************/
/** Provides access to the core bone.
  *
  * This function returns the core bone on which this bone instance is based on.
  *
  * @return One of the following values:
  *         \li a pointer to the core bone
  *         \li \b 0 if an error happend
  *****************************************************************************/

CalCoreBone *CalBone::getCoreBone()
{
  return m_pCoreBone;
}

 /*****************************************************************************/
/** Returns the current rotation.
  *
  * This function returns the current relative rotation of the bone instance.
  *
  * @return The relative rotation to the parent as quaternion.
  *****************************************************************************/

const CalQuaternion& CalBone::getRotation()
{
  return m_rotation;
}

 /*****************************************************************************/
/** Returns the current absolute rotation.
  *
  * This function returns the current absolute rotation of the bone instance.
  *
  * @return The absolute rotation to the parent as quaternion.
  *****************************************************************************/

const CalQuaternion& CalBone::getRotationAbsolute()
{
  return m_rotationAbsolute;
}

 /*****************************************************************************/
/** Returns the current bone space rotation.
  *
  * This function returns the current rotation to bring a point into the bone
  * instance space.
  *
  * @return The rotation to bring a point into bone space.
  *****************************************************************************/

const CalQuaternion& CalBone::getRotationBoneSpace()
{
  return m_rotationBoneSpace;
}

 /*****************************************************************************/
/** Returns the current translation.
  *
  * This function returns the current relative translation of the bone instance.
  *
  * @return The relative translation to the parent as quaternion.
  *****************************************************************************/

const CalVector& CalBone::getTranslation()
{
  return m_translation;
}

 /*****************************************************************************/
/** Returns the current absolute translation.
  *
  * This function returns the current absolute translation of the bone instance.
  *
  * @return The absolute translation to the parent as quaternion.
  *****************************************************************************/

const CalVector& CalBone::getTranslationAbsolute()
{
  return m_translationAbsolute;
}

 /*****************************************************************************/
/** Returns the current bone space translation.
  *
  * This function returns the current translation to bring a point into the
  *bone instance space.
  *
  * @return The translation to bring a point into bone space.
  *****************************************************************************/

const CalVector& CalBone::getTranslationBoneSpace()
{
  return m_translationBoneSpace;
}

 /*****************************************************************************/
/** Locks the current state.
  *
  * This function locks the current state (absolute translation and rotation)
  * of the bone instance and all its children.
  *****************************************************************************/

void CalBone::pushState(){
	m_savetranslation = m_translation;
	m_saverotation = m_rotation;
}
void CalBone::popState(){
	m_translation = m_savetranslation;
	m_rotation = m_saverotation;
}

void CalBone::lockState()
{
  // clamp accumulated weight
  if(m_accumulatedWeightAbsolute > 1.0f - m_accumulatedWeight)
  {
    m_accumulatedWeightAbsolute = 1.0f - m_accumulatedWeight;
  }

  if(m_accumulatedWeightAbsolute > 0.0f)
  {
    if(m_accumulatedWeight == 0.0f)
    {
      // it is the first state, so we can just copy it into the bone state
      m_translation = m_translationAbsolute;
      m_rotation = m_rotationAbsolute;

      m_accumulatedWeight = m_accumulatedWeightAbsolute;
    }
    else
    {
      // it is not the first state, so blend all attributes
      float factor;
      factor = m_accumulatedWeightAbsolute / (m_accumulatedWeight + m_accumulatedWeightAbsolute);

      m_translation.blend(factor, m_translationAbsolute);
      m_rotation.blend(factor, m_rotationAbsolute);

      m_accumulatedWeight += m_accumulatedWeightAbsolute;
    }

    m_accumulatedWeightAbsolute = 0.0f;
  }
}

 /*****************************************************************************/
/** Sets the skeleton.
  *
  * This function sets the skeleton to which the bone instance is attached to.
  *
  * @param pSkeleton The skeleton to which the bone instance should be
  *                  attached to.
  *****************************************************************************/

void CalBone::setSkeleton(CalSkeleton *pSkeleton)
{
  m_pSkeleton = pSkeleton;
}

//****************************************************************************//

void CalBone::setUpState()
{
    m_translation = m_pCoreBone->getTranslation();
    m_rotation = m_pCoreBone->getRotation();
}

void CalBone::saveState()
{
	m_translation = m_translationAbsolute;
	m_rotation = m_rotationAbsolute;
}

void CalBone::setRotation(CalQuaternion qrtn)
{
	m_rotation = qrtn;
}

void CalBone::setTranslation(CalVector atrn)
{
	m_translation = atrn;
	m_translationAbsolute = atrn;
}

int CalBone::getParentId()
{
	return m_pCoreBone->getParentId();
}