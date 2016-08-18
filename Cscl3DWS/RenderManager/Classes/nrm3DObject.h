// nrm3DObject.h: interface for the nrm3DObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_nrm3DObject_H__A94D4F09_D0E9_480F_A889_757EF331EA0A__INCLUDED_)
#define AFX_nrm3DObject_H__A94D4F09_D0E9_480F_A889_757EF331EA0A__INCLUDED_

#include "../CommonRenderManagerHeader.h"

#include "nrmanager.h"
#include "nrmObject.h"
#include <vector.h>
#include <quaternion.h>
#include "3DObject.h"
#include "IAsynchResourceHandler.h"

class nrmAudio;
class nrmObject;
class nrmMaterial;
enum ACTIONTYPE;

#define	RESTORE_POSITION_AFTER_LOAD_ALEX_2009_08_01_CHANGES	// ������� ��� ����, ����� ����� ������������ ������� ��� ���������� �����������������

class nrm3DObject :	
					public nrmObject,
					public moI3DObject,
					public moIButton,
					public IChangesListener,
					public IAsynchResourceHandler
{
	friend	class nrmCharacter;
public:
	nrm3DObject(mlMedia* apMLMedia);
	virtual ~nrm3DObject();

	virtual void update(DWORD dwTime, void* data);	

	virtual void OnChanged(int eventID);

	void SetBoundsFromDB(ml3DPosition& pos);

	void SetMaterialTiling(int textureSlotID, float tilingKoef);

	void EnableShadows(bool isEnabled);
	void ChangeCollisionLevel(int level);

	void LevelChanged(int level);
	int GetCurrentLevel();

	void UnpickableCheck();

	// ���������� IAsynchResourceHandler
public:
	// �������� �������� �������� �������
	virtual void OnAsynchResourceLoadedPersent(IAsynchResource* asynch, byte percent);
	// ������ ��������
	virtual void OnAsynchResourceLoaded(IAsynchResource* asynch);
	// ������ ��������
	virtual void OnAsynchResourceError(IAsynchResource* asynch);

	bool IsFullyLoaded();
	bool IsIntersectedWithOtherObjects();

// ���������� moMedia
public:
	moILoadable* GetILoadable(){ return this; }
	moI3DObject* GetI3DObject(){ return this; }
	moIButton* GetIButton(){ return this; }
	void PropIsSet();

// ����������  moILoadable
public:
	bool SrcChanged(); // �������� ������ �������
	bool Reload(){return false;} // ���������� ������� �� �������� src

// ����������  moI3DObject
public:
	void showAxis();
	void hideAxis();

	// ��������
	void FrozenChanged();
	bool HasAttachParent();

	virtual ml3DPosition GetPos(); 
	void PosChanged(ml3DPosition &pos); // ���������� ��������� 
	ml3DScale GetScale(); 
	void ScaleChanged(ml3DScale &scl);
	ml3DRotation GetRotation(); 
	void RotationChanged(ml3DRotation &rot);
	void VisibleChanged(); // ���������� ���������
	void InCameraSpaceChanged();
	void CheckCollisionsChanged(); // ��������� ���� �������� �� �����������
	void BoundingBoxChanged();
	void ShadowChanged();
	moIMaterial* GetMaterial(unsigned auID);
	void MaterialsChanged();
	void MaterialChanged(unsigned auID);
	void MaterialMapChanged(unsigned auID, const wchar_t* apwcIndex);
	void MotionChanged();
	void GravitationChanged();
	void IntangibleChanged();
	void BoundingBoxColorChanged();
		// avObjects - ������ �� ��������� �� ������ ���������� �� �������, 
		// � �������� ������������ ������ (0-� ������� - ��������� �� ������,
		// � ������� ��������� ��������� ������������)
		// (������ ��������� � ������������ RenderManager-��)
		// aiLen - ����� �������
		// aPos3D - ��������� �������, ��� ������� ������������ � 0-� �������� ��� �� ����
	bool getCollisions(mlMedia** &avObjects, int &aiLength, ml3DPosition &aPos3D);
	bool getIntersections(mlMedia** &avObjects, int &aiLength);
	void ColorChanged();
	void OpacityChanged();
	// ������
	void doMotion(mlMedia* apMotion);
	void setMotion(mlMedia* apMotion, int aiDurMS);
	void removeMotion(mlMedia* apMotion);
	bool getIntersectionsIn(mlMedia** &avObjects, int &aiLength);
	bool getIntersectionsOut(mlMedia** &avObjects, int &aiLength);
	void goPath(mlMedia* apPath, int aiFlags);
	void goPath(ml3DPosition* apPath, int aiPointCount, unsigned int auShiftTime);
	mlMedia* getPath();
		// ����� ���� �� ���������� �����
	bool findPathTo(ml3DPosition &aFrom, ml3DPosition &aTo, ml3DPosition* &avPath, int &aiLength);
		// ��������� �������� �������� ������� ����� ���� (%/���)
	void setPathVelocity(float afVelocity);
		// ��������� ���������� ����� (� %-�� ����), �� ������� ����� �����������
		// onPassed. getPassedCheckPoint() ������ �������� ����������� �����
	void setPathCheckPoints(float* apfCheckPoints, int aiLength);
	float getPassedCheckPoint();
	void setPathPercent(float afPercent);
	float getPathPercent();
	bool attachTo(mlMedia* ap3DODest,const char* apszBoneName);
	ml3DBounds getBounds(mlMedia* ap3DOCoordSysSrc);
		// ��������� bounds-�� ����������, �� ������� ������� �������� � ��������������� aiMatID
	virtual ml3DBounds getSubobjBounds(int aiMatID);
	int moveTo(ml3DPosition aPos3D, int aiDuaration, bool abCheckCollisions);
	virtual ml3DPosition getAbsolutePosition();
	virtual ml3DRotation getAbsoluteRotation(); 
	virtual bool attachSound(mlMedia* pAudio, ml3DPosition& pos3D);
	virtual bool detachSound(mlMedia* pAudio); 
	virtual void detachSound(nrmAudio* pAudio);
	std::string getAllTextures();
	int detectObjectMaterial(int x, int y);
	bool addPotentialTexture(mlMedia* apVisibleObject);
	bool addPotentialTexture(const wchar_t* apwcSrc);
	// Authoring
	bool Get3DPropertiesInfo(mlSynchData &aData);
	bool Get3DProperty(char* apszName,char* apszSubName, mlOutString &sVal);
	bool Set3DProperty(char* apszName,char* apszSubName,char* apszVal);
	void BillboardChanged();
	void UnpickableChanged();
	// 
	void SetMaterialDescription(int aiMatIdx, const wchar_t* apwcDescr);
	void on_clear();

	bool InitializePlugins();

	void AddAsynchSound(IAsynchResource* piasynchresource);

	// �������� ���� � ����� ����� ��������, ���� �� �������� ����� ResManager
	bool GetMaterialTextureSrc(int aiMatIdx, const wchar_t* apwcMapName, mlOutString &asSrc);

// ����������  moIButton
public:
	virtual void nrm3DObject::HandleCal3DAction(ACTIONTYPE type, int actvalue);
	void		SetMaterialMapMedia(int smid, mlMedia* apmlMedia);
	mlMedia*	GetMaterialMapMedia(int smid);
	void onKeyDown(){};
	void onKeyUp(){};
	void onSetFocus(){}
	void onKillFocus(){}
	void AbsEnabledChanged();
	void onPointArrived();

	C3DObject*		m_obj3d;

public:
	void	set_auto_scripted_parent( nrm3DGroup* apnrm3DObject);

	bool	m_wasCallbackPerformed;

protected:
	MP_VECTOR<nrmMaterial*>		m_materials;
	MP_MAP<int, mlMedia*>		m_materialMapTypes;
	nrm3DGroup*					autoscripted_parent;
	
	void				removeMotionTimed(mlMedia* apMotion, float fadeTime=0.0f);
	void				removeIdleTimed(mlMedia* apMotion, float fadeTime=0.0f);
	bool				isObjectLoaded();

	// Event handlers
	virtual	void	OnDoneAction(int mID);
	virtual	void	OnSetAction(int mID);

	// Shadows
	void CreateShadow(mlE3DShadowType shadowType, ml3DPlane shadowPlane, mlColor shadowColor, mlMedia* shadowLight);

	// Misc
	bool			m_bScriptedTransformApplied;
	bool			m_bScriptedPositionSet;
	bool			m_bScriptedRotationSet;
	bool			m_isLoaded;

	int				m_level;

	CalVector		m_ScriptedPosition;
	CalQuaternion	m_ScriptedRotation;
	CalVector		m_boundsFromDB;		// ������� �������, ���������� �� ��
										// ������������, ����� ��������� ��� �� ����������	

	MP_STRING		m_objectName;

	MP_VECTOR<nrmAudio *> m_soundList;

	MP_VECTOR<MP_STRING> m_potentionalTextures;
};

#endif // !defined(AFX_nrm3DObject_H__A94D4F09_D0E9_480F_A889_757EF331EA0A__INCLUDED_)
