#pragma once

namespace rmml {

//struct mlLocation3D
//{
//};

// �����, � ������� �������� �������� ����� �������� ��������� � 3D-����

class mlLocation3D:	
					public mlJSClass
{
public:
	mlLocation3D(void);
	void destroy() { MP_DELETE_THIS; }
	~mlLocation3D(void);
	static JSBool AlterJSConstructor(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
MLJSCLASS_DECL2(mlLocation3D)
private:
	enum {
		JSPROP_url, // ��������� (��� ���������) ���� ������� ������� ����� URL-��
		JSPROP_location, // ��������� ������������� ������� 
		JSPROP_place, // �������� (��� ���������) ������������� ����� � �������
		JSPROP_zone, // �������� ������������� ����
		JSPROP_reality, // �������� ������������� ����������
		JSPROP_point, // check point (����� ��� ������ ���������� �������) - ���������� ������� � �������� �����
		// ���������� ��� ������������� ����������
		JSPROP_x,
		JSPROP_y,
		JSPROP_z,
	};

	#define MLL3D_DM_LOCATION	0x001
	#define MLL3D_DM_PLACE_I	0x002
	#define MLL3D_DM_PLACE_S	0x004
	#define MLL3D_DM_X			0x008
	#define MLL3D_DM_Y			0x010
	#define MLL3D_DM_Z			0x020
	#define MLL3D_DM_ZONE		0x040
	#define MLL3D_DM_REALITY	0x080
	#define MLL3D_DM_POINT_I	0x100
	#define MLL3D_DM_POINT_S	0x200
	int miDefined; // �����, ������������ ��, ����� ��������� ��������� ������

	// ���������� URL
	bool parseURL(const wchar_t* apwcURL);
	// ������� URL
	const wchar_t* makeURL();

public:
//	mlLocation3D GetLocation(); // ������ 
	const wchar_t* GetLocationURL(); // ������ ��������� ������ URL-��

}; // class mlLocation3D

}
