#pragma once

//////////////////////////////////////////////////////////////////////////
// ���� ������� ������
// �������������� ���
namespace vespace
{
	enum	VESPACE_OBJECTS_TYPES
	{
		VE_UNDEFINED		= 0,		// �������������� ���
		VE_OBJECT3D			= 1,		// ������� 3D ������
		VE_GROUP3D			= 2,		// ������ 3D ������
		VE_TREE				= 4,		// ������
		VE_SKYBOX			= 5,		// sky box
		VE_SUNLENSFLARE		= 6,		// sun lensflare
		VE_LOCATIONBOUNDS	= 7,		// ������� �������
		VE_AVATAR			= 8,		// ������
		VE_COLLIDER_AABB	= 9,		// collision object AABB
		VE_COLLIDER_OOBB	= 10,		// collision object OOB
		VE_COLLIDER_MESH	= 11,		// collision object MESH
		VE_REFERENCE		= 64,		// 3D ��������� ������
		VE_PRIMOBJECT3D		= 128,		// ������ �� ����������

		VE_COLLIDER_OBJECT	= 12,		// ������������ ��������
		VE_SITTING_PLACE	= 13,		// ����� ��� �������
		VE_CAMERA			= 14,		// ������
		VE_CAMERA_TARGET	= 15,		// ���� ������

	};
}