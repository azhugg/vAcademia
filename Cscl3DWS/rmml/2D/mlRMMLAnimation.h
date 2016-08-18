#pragma once


namespace rmml {

/**
 * ����� �������� RMML
 */

class mlRMMLAnimation :
					public mlRMMLElement,
					public mlJSClass,
					public mlRMMLLoadable,
					public mlRMMLVisible,
					public mlRMMLContinuous,
					public mlRMMLPButton
{
public:
	mlRMMLAnimation(void);
	void destroy() { MP_DELETE_THIS }
	~mlRMMLAnimation(void);
MLJSCLASS_DECL

// ���������� mlRMMLElement
MLRMMLELEMENT_IMPL
	mlresult CreateMedia(omsContext* amcx);
	mlresult Load();
	mlRMMLElement* Duplicate(mlRMMLElement* apNewParent);
	mlresult Update(const __int64 alTime);
	bool Freeze();
	bool Unfreeze();

// ����������  mlILoadable
MLILOADABLE_IMPL

// ����������  mlIVisible
MLIVISIBLE_IMPL

// ����������  mlIContinuous
MLICONTINUOUS_IMPL

// ����������  mlIButton
MLIBUTTON_IMPL

};

}