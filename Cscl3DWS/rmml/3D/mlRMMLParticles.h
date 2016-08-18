#if _MSC_VER > 1000
#pragma once
#endif

#ifndef _mlRMMLParticles_H_
#define _mlRMMLParticles_H_
#include "mlRMML.h"

namespace rmml {

class mlRMMLParticles :	
					public mlRMMLElement,
					public mlJSClass,
					public mlRMMLLoadable,
					public mlRMML3DObject
{
public:
	mlRMMLParticles(void);
	void destroy() { MP_DELETE_THIS }
	~mlRMMLParticles(void);
MLJSCLASS_DECL

private:
	enum {
		JSPROP_maxParticles = 1,
		JSPROP_density,
		JSPROP_minSpeed,
		JSPROP_maxSpeed,
		JSPROP_minSize,
		JSPROP_maxSize,
		JSPROP_minOpacity,
		JSPROP_maxOpacity,
		JSPROP_dirChangeFactor,
		JSPROP_speedChangeFactor,
		JSPROP_defPointSize
	};

protected:
	JSFUNC_DECL(setType)
	JSFUNC_DECL(enableSortVerts)
	JSFUNC_DECL(disableSortVerts)

private:

// ���������� mlRMMLElement
MLRMMLELEMENT_IMPL
	mlresult CreateMedia(omsContext* amcx);
	mlresult Load();
	mlRMMLElement* Duplicate(mlRMMLElement* apNewParent);

private:
// ����������  mlILoadable
MLILOADABLE_IMPL

// ����������  mlI3DObject
MLI3DOBJECT_IMPL
	bool GetBillboard(){ return false; }
	bool GetUnpickable(){ return false; }

// ����������  moI3DObject
	void BillboardChanged(){}
	void UnpickableChanged(){}
	void SetMaterialTiling(int, float) {};
	void EnableShadows(bool isEnabled){};
	void ChangeCollisionLevel(int level){};
	bool IsFullyLoaded() {return false;}
	void LevelChanged(int /*level*/) {};
	int GetCurrentLevel() { return 0; }
	bool IsIntersectedWithOtherObjects() {return false;}
};
}

#endif
