#pragma once

namespace rmml {

struct mlI3DPosition{
	virtual void SetPos_x(double adNewVal)=0;
	virtual void SetPos_y(double adNewVal)=0;
	virtual void SetPos_z(double adNewVal)=0;
	virtual ml3DPosition GetPos()=0;
	virtual void SetPos(ml3DPosition &pos)=0;
};

/**
 * JS-����� ��� 3D-��������� RMML
 */

class mlPosition3D:	
					public mlJSClass
{
public:
	mlPosition3D(void);
	void destroy() { MP_DELETE_THIS; }
	~mlPosition3D(void);
	static JSBool AlterJSConstructor(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval);
MLJSCLASS_DECL2(mlPosition3D)
private:
	enum {
		JSPROP_x,
		JSPROP_y,
		JSPROP_z,
	};
	bool bRef;
	union{
		ml3DPosition* pPos;
		mlI3DPosition* mpI3DPos;
	};
	void CreatePos(){
		if(!bRef && pPos==NULL){ 
			pPos = MP_NEW( ml3DPosition);
			ML_INIT_3DPOSITION(*pPos);
		}
	}
	JSFUNC_DECL(clone)
	JSFUNC_DECL(add)
	JSFUNC_DECL(sub)
	JSFUNC_DECL(mult)	// �� ����� ��� Rotation3D
	JSFUNC_DECL(div)
	JSFUNC_DECL(dot)	// ��������� ������������
	JSFUNC_DECL(cross)	// ��������� ������������
	JSFUNC_DECL(blend)	// ������������
	JSFUNC_DECL(normalize)	// �������������
	JSFUNC_DECL(length)	// �����
	JSFUNC_DECL(angle)	// ���������� ������� � �������� ����� ���� �������� � ������ � �������� ���������
	JSFUNC_DECL(angleSigned)  // ���������� ������ ������� � �������� ������������ ������� up (�� ��������� (0, 0, 1)) ����� ���� �������� � ������ � �������� ��������� 
public:
	void Set3DPosRef(mlI3DPosition* apI3DPos){
		if(!bRef && pPos!=NULL) MP_DELETE( pPos);
		bRef=true;
		mpI3DPos=apI3DPos;
	}
	ml3DPosition GetPos();
	void SetPos(ml3DPosition &pos);
	double GetLength();
	static void Cross(ml3DPosition const& v1, ml3DPosition const& v2, ml3DPosition & ret);
	static double Dot(ml3DPosition const& v1, ml3DPosition const& v2);
};

}
