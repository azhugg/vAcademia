#pragma once

//#include "2D\mlRectangleJSO.h"

namespace rmml {

/**
 * ����� �����, ������������ �� ����������� 
 */

class mlRMMLImageFrame :	
					public mlRMMLElement,
					public mlJSClass,
					public mlRMMLLoadable,
					public mlRMMLVisible,
					public mlRMMLPButton,
					public mlIImageFrame,
					public moIImageFrame
{
protected:
	bool tile;
public:
	mlRMMLImageFrame(void);
	void destroy() { MP_DELETE_THIS }
	~mlRMMLImageFrame(void);
MLJSCLASS_DECL
private:
	enum{
		JSPROP_margin = 1, // ������� �������� �� ���� - �����
		JSPROP_marginTop,
		JSPROP_tile,
		JSPROP_marginBottom,
		JSPROP_marginLeft,
		JSPROP_marginRight,
	};
	int margin;
	int marginTop;
	int marginBottom;
	int marginLeft;
	int marginRight;

// ���������� mlRMMLElement
MLRMMLELEMENT_IMPL
	mlresult CreateMedia(omsContext* amcx);
	mlresult Load();
	mlRMMLElement* Duplicate(mlRMMLElement* apNewParent);
	mlIImageFrame* GetIImageFrame(){ return this; }

// ����������  mlILoadable
MLILOADABLE_IMPL

// ����������  mlIVisible
MLIVISIBLE_IMPL

// ����������  mlIButton
MLIBUTTON_IMPL

// ���������� mlIImageFrame
public:
	virtual int GetMargins(int& aiMarginTop, int& aiMarginBottom, int& aiMarginLeft, int& aiMarginRight);
	bool GetTile(){ return tile; }

// ���������� moIImageFrame
public:
	virtual void MarginsChanged();
	void TileChanged();

};

} // namespace rmml
