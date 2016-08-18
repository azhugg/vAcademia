// nrm3DGroup.h: interface for the nrm3DGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_nrm3DGroup_H__A94D4F09_D0E9_480F_A889_757EF331EA0A__INCLUDED_)
#define AFX_nrm3DGroup_H__A94D4F09_D0E9_480F_A889_757EF331EA0A__INCLUDED_

#include "../CommonRenderManagerHeader.h"

#include "nrm3DObject.h"
#include "iasynchresourcehandler.h"

class n3dGroup;

class nrm3DGroup :	public nrm3DObject,
					public moI3DGroup
{
public:
	nrm3DGroup(mlMedia* apMLMedia);
	virtual ~nrm3DGroup();


public:
	void OnChanged(int eventID);

	// ���������� iasynchresourcehandler
public:
	// �������� �������� �������� �������
	virtual void OnAsynchResourceLoadedPersent(IAsynchResource* asynch, byte percent);
	// ������ ��������
	virtual void OnAsynchResourceLoaded(IAsynchResource* asynch);
	// ������ ��������
	virtual void OnAsynchResourceError(IAsynchResource* asynch);

// ���������� moMedia
public:
	moILoadable* GetILoadable(){ return this; }
	moI3DObject* GetI3DObject(){ return this; }
	moIButton* GetIButton(){ return this; }

// ���������� moI3DGroup
	void ScriptedChanged();
public:
	bool ChildAdded(mlMedia* apChild);
	bool ChildRemoving(mlMedia* apChild);

// ����������  moILoadable
public:
	bool SrcChanged(); // �������� ������ �������
	bool Reload(){return false;} // ���������� ������� �� �������� src

// ����������  moI3DObject
public:
	// ��������
	void PosChanged(ml3DPosition &pos); // ���������� ��������� 
	void ScaleChanged(ml3DScale &scl);
	void RotationChanged(ml3DRotation &rot);
	//void VisibleChanged(); // ���������� ���������
	void CheckCollisionsChanged(); // ��������� ���� �������� �� �����������
	virtual void update(DWORD dwTime, void *data);
	// Some STUBS
public:
	void LevelChanged(int /*level*/){};
	int GetCurrentLevel() { return 0; }
	void	BoundingBoxChanged(){}
	void	ShadowChanged(){}
	void	MaterialsChanged(){}
	void	MaterialChanged(unsigned /*auID*/){}
	void	MaterialMapChanged(unsigned /*auID*/, const wchar_t* /*apwcIndex*/){}
	void	MotionChanged(){}
	void	GravitationChanged(){}
	void	IntangibleChanged(){}
	bool	getCollisions(mlMedia**& /*avObjects*/, int& /*aiLength*/, ml3DPosition& /*aPos3D*/){ return false; }
	bool	getIntersections(mlMedia**& /*avObjects*/, int& /*aiLength*/){ return false; }
	void	doMotion(mlMedia* /*apMotion*/){}
	void	setMotion(mlMedia* /*apMotion*/, int /*aiDurMS*/){}
	void	removeMotion(mlMedia* /*apMotion*/){}
	bool	getIntersectionsIn(mlMedia**& /*avObjects*/, int& /*aiLength*/){ return false; }
	bool	getIntersectionsOut(mlMedia**& /*avObjects*/, int& /*aiLength*/){ return false; }
	void	goPath(mlMedia* /*apPath*/, int /*aiFlags*/){}
	moIMaterial*	GetMaterial(unsigned /*auID*/){ return NULL; }
	// ������
	mlMedia* getPath(){ return NULL; }
		// ��������� �������� �������� ������� ����� ���� (%/���)
	void setPathVelocity(float /*afVelocity*/){}
		// ��������� ���������� ����� (� %-�� ����), �� ������� ����� �����������
		// onPassed. getPassedCheckPoint() ������ �������� ����������� �����
	void setPathCheckPoints(float* /*apfCheckPoints*/, int /*aiLength*/){}
	float getPassedCheckPoint(){ return 0.0; }
	void setPathPercent(float /*afPercent*/){}
	float getPathPercent(){ return 0.0; }
	bool attachTo(mlMedia* /*ap3DODest*/, const char* /*apszBoneName*/){ return false; }
	// Authoring
	bool Get3DPropertiesInfo(mlSynchData& /*aData*/){ return false; }
	bool Get3DProperty(char* /*apszName*/, char* /*apszSubName*/, mlOutString& /*sVal*/){ return false; }
	bool Set3DProperty(char* /*apszName*/, char* /*apszSubName*/, char* /*apszVal*/){ return false; }

// ����������  moIButton
public:
	void onKeyDown(){}
	void onKeyUp(){}
	void onSetFocus(){}
	void onKillFocus(){}
	void AbsEnabledChanged(){}

public:
	//	����������� �� �������� ����������� �������
	void	auto_scripted_object_is_destroing(nrm3DObject* apnrm3DObject);

protected:
	// ������� rmml-����������� ������ ��� ������� Natura3D
	bool CreateRMMLObjectFor3DObject(C3DObject* obj3d, const char* name);
	// ������� ��� ���������� �������� �� ������
	bool ClearScriptedInternalObjects();
	// ������� ���������� ������� �� ������
	bool ClearScriptedInternalObject(C3DObject* obj3d);
	// ��������� �������, ���������� � �������
//	void ScriptSelfChildren();

	// ������ RMML �������� - ������� ��������� � ������. �������� ���������� �����������, 
	// ����������� � ��������������� ������������� �������
	std::vector<C3DObject *> auto_scripted_object;

	// ��������� ��� �������, ������� ��������� � ������� � ����
	void AttachChildrenFromScript();

public:
	bool AttachScriptedChild(mlMedia* apChild);

private:
	bool DetachScriptedChild(mlMedia* apChild);
	void LoadGroup(ifile* file);

	MP_VECTOR<C3DObject *> m_childObjects;
	bool m_rmmlLoadAllCalled; // (�������) ����, ��� onLoad(RMML_LOAD_STATE_ALL_RESOURCES_LOADED) � rmml-������� ��� ���������
};

#endif // !defined(AFX_nrm3DGroup_H__A94D4F09_D0E9_480F_A889_757EF331EA0A__INCLUDED_)
