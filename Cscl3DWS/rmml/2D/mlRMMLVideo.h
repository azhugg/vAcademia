#pragma once


namespace rmml {

/**
 * ����� ����� RMML
 */

class mlRMMLVideo :
					public mlRMMLElement,
					public mlJSClass,
					public mlRMMLLoadable,
					public mlRMMLVisible,
					public mlRMMLContinuous,
					public mlRMMLPButton,
					public mlIVideo,
					public moIVideo
{
public:
	mlRMMLVideo(void);
	void destroy() { MP_DELETE_THIS }
	~mlRMMLVideo(void);
MLJSCLASS_DECL
private:
	enum {
		JSPROP_windowed = 1
	};
	bool windowed;
	
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

// ���������� mlIVideo
public:
	mlIVideo* GetIVideo(){ return this; }
	bool IsWindowed(){ return windowed; }

// ���������� moIVideo
public:
	void WindowedChanged();
};

}