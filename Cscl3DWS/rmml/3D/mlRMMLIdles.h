#if _MSC_VER > 1000
#pragma once
#endif

#ifndef _mlRMMLIdles_H_
#define _mlRMMLIdles_H_

namespace rmml {

/**
 * ����� ��� ������� ������ ���������� RMML
 */

class mlRMMLIdles :	
					public mlRMMLElement,
					public mlJSClass,
					public mlRMMLLoadable,
					public mlIIdles
{
public:
	mlRMMLIdles(void);
	void destroy() { MP_DELETE_THIS }
	~mlRMMLIdles(void);
MLJSCLASS_DECL
private:
	enum {
		JSPROP_step_length = 1,
		JSPROP_rot_speed
	};
	double step_length;
	double rot_speed;

// ���������� mlRMMLElement
MLRMMLELEMENT_IMPL
	mlresult CreateMedia(omsContext* amcx);
	mlRMMLElement* Duplicate(mlRMMLElement* apNewParent);

// ����������  mlILoadable
MLILOADABLE_IMPL
	// ��������
	virtual bool SrcChanged();

// ���������� mlIIdles
public:
	mlIIdles* GetIIdles(){ return this; }
	mlMedia* GetIdle(int aiNum){
		return GetChild(MLMT_IDLE,aiNum);
	}
};

}

#endif
