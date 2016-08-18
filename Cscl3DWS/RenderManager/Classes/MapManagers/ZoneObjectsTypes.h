#pragma once

//////////////////////////////////////////////////////////////////////////
// ���� ������� ������
// �������������� ���
namespace vespace
{
	enum	VESPACE_OBJECTS_TYPES
	{
		VE_UNDEFINED					= 0,		// �������������� ���
		VE_OBJECT3D						= 1,		// ������� 3D ������
		VE_GROUP3D						= 2,		// ������ 3D ������
		VE_PARTICLES					= 3,		// �������
		VE_TREE							= 4,		// ������
		VE_GROUP_LECTORE_TYPE			= 5,		// ������ ���� ���������
		VE_GROUP_PIVOT_TYPE				= 6,		// ������ ���� ����� ��������
		VE_LOCATIONBOUNDS				= 7,		// ������� �������
		VE_AVATAR						= 8,		// ������
		VE_COLLIDER_AABB				= 9,		// collision object AABB
		VE_COLLIDER_OOBB				= 10,		// collision object OOB
		VE_COLLIDER_MESH				= 11,		// collision object MESH
		VE_TEMPLOCATIONBOUNDS			= 14,		// ������� ��������� �������
		VE_COMMINICATIONAREABOUNDS		= 15,		// ������� ��������� ������� �������
		VE_REFERENCE					= 64,		// 3D ��������� ������
		VE_PRIMOBJECT3D					= 128		// ������ �� ����������
	};
}