#if _MSC_VER > 1000
#pragma once
#endif

#ifndef _mlRMMLScene3D_H_
#define _mlRMMLScene3D_H_

namespace rmml {

class mlRMMLCloud;

/**
 * ����� 3D-���� RMML
 */

class mlRMMLScene3D :	
					public mlRMMLElement,
					public mlJSClass,
					public mlIScene3D
{
public:
	mlRMMLScene3D(void);
	void destroy() { MP_DELETE_THIS }
	~mlRMMLScene3D(void);
MLJSCLASS_DECL
private:
	enum {
		JSPROP_useMapManager
	};
	bool useMapManager;

//	JSFUNC_DECL(setEditableBoundsColor)	// ���������� ���� boundbox-� ���� ������������� �������� � �������� ��, ���� ���� ������������

// ���������� mlRMMLElement
MLRMMLELEMENT_IMPL
	mlresult CreateMedia(omsContext* amcx);
	mlRMMLElement* Duplicate(mlRMMLElement* apNewParent);
	bool Freeze(){ 
		if(!mlRMMLElement::Freeze()) return false; 
		return FreezeChildren(); 
	}
	bool Unfreeze(){ 
		if(!mlRMMLElement::Unfreeze()) return false; 
		return UnfreezeChildren(); 
	}
	virtual void ClearRefs(){ ClearIntersections(); }
	mlIScene3D* GetIScene3D(){ return this; }

private:
	bool mbClearingIntersections;
//	mlColor mEditableBBoxColor;	// ���� boundingbox-� �� ��������� ��� ������������� ��������
//	mlColor mBBoxColor; // ���� boundingbox-� �� ��������� ��� ��������������� ��������
	typedef std::vector<mlRMMLCloud*> CloudVector;
	CloudVector mvVisibleClouds; // ������ ���� ������� cloud-�� 3D-��������

//	void ResetEditableBBoxColor();
//	bool IsEditableObject(mlRMMLElement* apElem);
public:
	bool ClearingIntersections(){ return mbClearingIntersections; }
	void ClearIntersections();
	// �������� ���� �� ��������� ��� bounding-box-� ��������� �������
//	mlColor GetDefaultBoundingBoxColor(mlRMMLElement* apElem);

	// ���������������� ������� cloud
	bool RegisterVisibleCloud(mlRMMLCloud* apCloud);
	// ����������������� cloud
	void UnregisterCloud(mlRMMLCloud* apCloud);
	// �������� ������ � cloud-�, ������� ��� ������
	mlRMMLElement* GetCloudButtonUnderMouse(mlPoint &aXY, mlButtonUnderMouse &aBUM, mlRMMLCloud* &pCloud);

// ���������� mlIScene3D
	bool GetUseMapManager(){ return useMapManager; }

// ���������� moIScene3D
	void UseMapManagerChanged();

};

}

#endif
