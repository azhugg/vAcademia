
#pragma once

// ��������� � ������� ������� bounding box ������� ������������ � bounding box ��� �������� ������,
// ������ ����������� �� ��� Z
#define SHADOW_BOUNDING_BOX_OFFSET		0.5f
#define SHADOW_BOUNDING_BOX_SCALE_XY	1.2f
#define SHADOW_BOUNDING_BOX_SCALE_Z		3.0f

// ������� ��������� ��� ������
#define ANIM_DIMENSIONS			35.0f, 40.0f, 180.0f

// ����������� ��������� ������ ���������, ������� � �������� ��������������� ���������� ������
// ���� �� ����������, ����� ��������������� ��, ��� ����� � ����������� ������� � � ���������
// �� ���������
#define MIN_Z_CHANGE			2.0f

// ����������� ������ �� ������ �� ��������� � bounding box �������
#define MIN_SIZE				15.0f

// ��������� ��� ��������� bounding box-�� ��������� ��������
#define MIN_BIG_OBJECT_SIZE		100.0f
#define COLLISION_CORRELATION	0.9f