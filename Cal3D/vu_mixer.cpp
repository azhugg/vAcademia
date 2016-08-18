//****************************************************************************//
// CharMixer.cpp                                                                  //
// Copyright (C) 2001, 2002 Bruno 'Beosil' Heidelberger                       //
//****************************************************************************//
// This library is free software; you can redistribute it and/or modify it    //
// under the terms of the GNU Lesser General Public License as published by   //
// the Free Software Foundation; either version 2.1 of the License, or (at    //
// your option) any later version.                                            //
//****************************************************************************//

#include "StdAfx.h"
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

//****************************************************************************//
// Includes                                                                   //
//****************************************************************************//

#include "error.h"
#include "vu_mixer.h"
#include "coremodel.h"
#include "corebone.h"
#include "coreanimation.h"
#include "coretrack.h"
#include "model.h"
#include "skeleton.h"
#include "bone.h"
#include "animation.h"
#include "animation_action.h"
#include "animation_cycle.h"
#include "calanimationmorph.h"
//#include "stepctrl.h"
#include "corekeyframe.h"
#include "coreskeleton.h"
#include "animsequencer.h"
#include "Mpeg4FDP.h"
#include "coreanimationflags.h"
#include "memory_leak.h"


CalVUMixer::CalVUMixer()
{
}

CalVUMixer::~CalVUMixer()
{
}

/************************************************************************/
/* inanimPlace - ���������� ������ �� �������, ������ �� ��������
/************************************************************************/
void CalVUMixer::CheckAnimPlace(CalAnimation* pAnim)
{
	CalQuaternion	animBasisRtn;
	CalVector		animBasisTrn;

	if( !pAnim->getBoneStartState(Cal::PROJECTION_BONE_ID, animBasisTrn, animBasisRtn))
		return;	// �������� �� ����� ������������ ������

	basis_animation = pAnim;

	// ���� ���������� ����� ��������� ���������� �������� �� ���������� ������,
	// �� ���������� ���� �������������� �������� ������ � ��������� ��������
	// � ����� ���������� ������
	if( !animation_basis_used)
	{
		moveAnimationToPosition(pAnim, basis_position, basis_rotation);
		animBasisTrn = basis_position;
		animBasisRtn = basis_rotation;
	}

	// ���� ���������� ����� ��������� ������ �� ��������, ��
	// ���������� ���� �������������� �������� ������ � ��������� ���
	// � ����� ������ ��������
	m_pModel->getSkeleton()->set_frame_move(animBasisTrn, animBasisRtn);
	m_pModel->relative_pos_changed = false;
}

void CalVUMixer::moveAnimationToPosition(CalAnimation* pAnim, const CalVector& externPosition, const CalQuaternion& externRotation)
{
	if (!m_pModel->getSkeleton())
		return;

	CalVector animationBipTranslation;
	CalQuaternion animationBipRotation;
	pAnim->getBoneStartState(Cal::PROJECTION_BONE_ID, animationBipTranslation, animationBipRotation);

	// �������
	CalQuaternion	projectionBoneRotation;

	// �������
	CalVector xmodel(1,0,0);
	CalVector xanim(1,0,0);

	// ����������� "�������" ������
	xmodel *= externRotation;

	// ����������� "�������" ��������
	xanim *= animationBipRotation;

	if( xanim.dot(xmodel) > 0.99)
	{
		projectionBoneRotation = CalQuaternion(0,0,0,1);
	}
	else	if( xanim.dot(xmodel) < -0.99)
	{
		projectionBoneRotation = CalQuaternion(CalVector(0,0,1),3.141692f);
	}
	else	
	{
		projectionBoneRotation = CalQuaternion(xanim.getNormal(xmodel), -xanim.getAngle(xmodel));
	}

	CalVector projectionBoneTranslation = animationBipTranslation;
	projectionBoneTranslation *= projectionBoneRotation;
	projectionBoneTranslation = externPosition - projectionBoneTranslation;

	pAnim->setProjectionBoneTransformation( projectionBoneTranslation, projectionBoneRotation);


	pAnim->getBoneStartState(Cal::PROJECTION_BONE_ID, animationBipTranslation, animationBipRotation);
	pAnim->applyProjectionBoneTransformation( animationBipTranslation, animationBipRotation);
}