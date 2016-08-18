#if _MSC_VER > 1000
#pragma once
#endif

#ifndef _mlRMMLGroup_H_
#define _mlRMMLGroup_H_

namespace rmml {

/**
 * ����� ����� 3D-�������� RMML
 */

class mlRMMLGroup :	
					public mlRMMLElement,
					public mlJSClass,
					public mlRMMLLoadable,
					public mlRMML3DObject,
					public mlRMMLPButton,
					public mlI3DGroup
{
public:
	mlRMMLGroup(void);
	void destroy() { MP_DELETE_THIS }
	~mlRMMLGroup(void);
MLJSCLASS_DECL
private:
	enum {
		JSPROP_gravitation = 1,
		JSPROP_intangible,
		JSPROP_scripted,		// ���� true, �� �� ��� ������� ������, ����������� �� src ��������� RMML-�������
		JSPROP_color,		// ����, ������� ����������� �� ���� ���������� ������� � �������, �������� color.a
		JSPROP_opacity,		// ������� �������������� ������� (0..0xFFFF)
	};
	bool scripted;

// ���������� mlRMMLElement
MLRMMLELEMENT_IMPL
	mlresult CreateMedia(omsContext* amcx);
	mlresult Load();
	mlRMMLElement* Duplicate(mlRMMLElement* apNewParent);
	bool Freeze(){ 
		if(!mlRMMLElement::Freeze()) return false; 
		return FreezeChildren(); 
	}
	bool Unfreeze(){ 
		if(!mlRMMLElement::Unfreeze()) return false; 
		return UnfreezeChildren(); 
	}

// ����������  mlRMMLLoadable
MLILOADABLE_IMPL

// ����������  mlIButton
MLIBUTTON_IMPL

// ����������  mlI3DObject
MLI3DOBJECT_IMPL
	bool GetBillboard(){ return false; }
	bool GetUnpickable(){ return false; }

	void ChildAdded(mlRMMLElement* apNewChild);
	void ChildRemoving(mlRMMLElement* apChild);

	void SetMaterialTiling(int, float) {};
	void EnableShadows(bool isEnabled){};
	void ChangeCollisionLevel(int level){};

// ����������  moI3DObject
	void BillboardChanged(){}
	void UnpickableChanged(){}
	bool IsFullyLoaded() {return false;}
	bool IsIntersectedWithOtherObjects() {return false;}
	void LevelChanged(int /*level*/) {};
	int GetCurrentLevel() { return 0; }

// ���������� mlI3DGroup
mlI3DGroup* GetI3DGroup(){ return this; }

	bool GetScripted(){ return scripted; }

// ���������� moI3DGroup
protected:
	moI3DGroup* GetmoI3DGroup();
	void ScriptedChanged();

};

}

#endif
