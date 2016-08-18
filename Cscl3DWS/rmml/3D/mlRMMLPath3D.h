#if _MSC_VER > 1000
#pragma once
#endif

#ifndef _mlRMMLPath3D_H_
#define _mlRMMLPath3D_H_

namespace rmml {

/**
 * ����� ���� ����������� ��� 3D-�������� RMML
 */

class mlRMMLPath3D :	
					public mlRMMLElement,
					public mlJSClass,
					public mlRMMLLoadable,
					public moIPath3D
{
public:
	mlRMMLPath3D(void);
	void destroy() { MP_DELETE_THIS }
	~mlRMMLPath3D(void);
MLJSCLASS_DECL

	JSFUNC_DECL(getPosition) // (������ �� ������� (����������� ����� ("���������")))
	JSFUNC_DECL(getLinearPosition) // (������ �� ����� (���� �������� ������ � �����))
	JSFUNC_DECL(getNormal) // �������� ������� � ������������ ������� (0..1) �� ����
	JSFUNC_DECL(getTangent) // �������� ����������� � ������������ ������� (0..1) �� ����
	JSFUNC_DECL(getNormalByLinearPos) // �������� ������� � ������������ �������� ������� (0..1) �� ����
	JSFUNC_DECL(getTangentByLinearPos) // �������� ����������� � ������������ �������� ������� (0..1) �� ����

// ���������� mlRMMLElement
MLRMMLELEMENT_IMPL
	mlresult CreateMedia(omsContext* amcx);
	mlresult Load();
	mlRMMLElement* Duplicate(mlRMMLElement* apNewParent);

// ����������  mlILoadable
MLILOADABLE_IMPL

// ���������� moIPath3D
public:
	ml3DPosition getPosition(double adKoef);
	ml3DPosition getLinearPosition(double adKoef);
	ml3DPosition getNormal(double adKoef);
	ml3DPosition getTangent(double adKoef);
	ml3DPosition getNormalByLinearPos(double adKoef);
	ml3DPosition getTangentByLinearPos(double adKoef);
};

}

#endif
