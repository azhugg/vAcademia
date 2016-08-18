#pragma once


namespace rmml {

/**
 * ����� ������ RMML
 */

class mlRMMLButton :	
					public mlRMMLElement,
					public mlJSClass,
					public mlRMMLLoadable,
					public mlRMMLVisible,
					public mlRMMLPButton
{
public:
	mlRMMLButton(void);
	void destroy() { MP_DELETE_THIS }
	~mlRMMLButton(void);
MLJSCLASS_DECL

// ���������� mlRMMLElement
MLRMMLELEMENT_IMPL
	mlresult CreateMedia(omsContext* amcx);
	mlresult Load();
	mlRMMLElement* Duplicate(mlRMMLElement* apNewParent);

// ����������  mlILoadable
MLILOADABLE_IMPL

// ����������  mlIVisible
MLIVISIBLE_IMPL

// ����������  mlIButton
MLIBUTTON_IMPL

};

}