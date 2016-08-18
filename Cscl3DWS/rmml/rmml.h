
#ifndef rmml_h___
#define rmml_h___

#include <memory>
#include <string>
#include <vector>
#include <list>
#include "BSSCCode.h"

#include "..\..\nmemoryprofiler\IMemoryProfiler.h"
#include "..\..\nmemoryprofiler\memoryprofilerFuncs.h"
#include "..\..\nmemoryprofiler\memoryprofilerDefines.h"
#include <XInput.h>

#ifndef CSCL
#include "oms/oms_context.h"

namespace mme{
	class Updatable;
}
using namespace mme;
#endif

using oms::omsresult;
using oms::omsContext;

struct ZONE_OBJECT_INFO;
                                             
namespace rmml{

struct syncObjectState;

#ifndef WIN32
#define RMML_NO_VTABLE
#else
#define RMML_NO_VTABLE __declspec(novtable)
//#define RMML_NO_VTABLE
#endif

// ��� �������/�������� ������� � ��������
#if defined (RMML_BUILD)
#  define RMML_DECL  __declspec(dllexport)
#else
# define RMML_DECL   __declspec(dllimport)
#endif

#include "mlSynchData.h"

#define MLWORD unsigned short
#define MLBYTE unsigned char
#define MLDWORD unsigned long
#define MLUSHORT unsigned short
#define MLULONG unsigned long
#define MLLONG long

/**
 * ����������� ���� ������
 */
struct mlPoint{
	int x,y;
};
#define ML_INIT_POINT(pnt) pnt.x=0; pnt.y=0;
struct mlSize{
	int width,height;
};
#define ML_INIT_SIZE(sz) sz.width=0; sz.height=0;
struct mlRect{
	int left,top,right,bottom;
};
#define ML_INIT_RECT(rc) rc.left=0; rc.top=0; rc.right=0; rc.bottom=0;
struct mlBounds{
	int x, y, width, height;
};
#define ML_INIT_BOUNDS(bnd) bnd.x = 0; bnd.y = 0; bnd.width = 0; bnd.height = 0;
struct ml3DPosition{
	double x,y,z;
};
#define ML_INIT_3DPOSITION(pos) {(pos).x=0.0; (pos).y=0.0; (pos).z=0.0;}
#define ML_COPY_3DPOSITION(a, b) {a.x = b.x; a.y = b.y; a.z = b.z;}
struct ml3DScale{
	double x,y,z;
};
#define ML_INIT_3DSCALE(scl) {(scl).x=0.0; (scl).y=0.0; (scl).z=0.0;}
struct ml3DRotation{
	double x,y,z,a; // ���������� 
};
#define ML_INIT_3DROTATION(rot) {(rot).x=0.0; (rot).y=0.0; (rot).z=1.0; (rot).a=0.0;}
struct ml3DPlane{
	double x,y,z,d;
};
#define ML_INIT_3DPLANE(plane) {(plane).x=0.0; (plane).y=0.0; (plane).z=1.0; (plane).d=0.0;}
struct ml3DBounds{
	double xMin, xMax, yMin, yMax, zMin, zMax;
};
#define ML_INIT_3DBOUNDS(bnd) {(bnd).xMin=0.0; (bnd).xMax=0.0; (bnd).yMin=0.0; (bnd).yMax=0.0; (bnd).zMin=0.0; (bnd).zMax=0.0;}
struct mlColor{
	unsigned char r,g,b,a;
};
#define ML_INIT_COLOR(col) {(col).r=0; (col).g=0; (col).b=0; (col).a=0xFF;}

typedef std::wstring mlString;

/**
 * �����-����
 */
#define MLMT_UNDEFINED 0
#define MLMPT_LOADABLE (1<<(8+0))
#define MLMPT_VISIBLE (1<<(8+1))
#define MLMPT_AUDIBLE (1<<(8+2))
#define MLMPT_CONTINUOUS (1<<(8+3))
#define MLMPT_BUTTON (1<<(8+4))
#define MLMPT_3DOBJECT (1<<(8+5))
#define MLMT_VISIBLE (MLMPT_LOADABLE | \
	                MLMPT_VISIBLE | \
	                MLMPT_BUTTON | \
					)
// �����������
#define MLMT_IMAGE (MLMPT_LOADABLE | \
	                MLMPT_VISIBLE | \
	                MLMPT_BUTTON | \
					1 \
					)
// ��������
#define MLMT_ANIMATION (MLMPT_LOADABLE | \
	                MLMPT_VISIBLE | \
	                MLMPT_CONTINUOUS | \
	                MLMPT_BUTTON | \
					2 \
					)
// �����
#define MLMT_VIDEO (MLMPT_LOADABLE | \
	                MLMPT_VISIBLE | \
	                MLMPT_CONTINUOUS | \
	                MLMPT_BUTTON | \
					3 \
					)
// �����
#define MLMT_TEXT (MLMPT_LOADABLE | \
	                MLMPT_VISIBLE | \
	                MLMPT_BUTTON | \
					4 \
					)
//#define MLMT_HTML (MLMPT_LOADABLE | \
//	                MLMPT_VISIBLE | \
//	                MLMPT_BUTTON | \
//					5 \
//					)
#define MLMT_BUTTON (MLMPT_LOADABLE | \
	                MLMPT_VISIBLE | \
	                MLMPT_BUTTON | \
					6 \
					)
#define MLMT_ACTIVEX (MLMPT_LOADABLE | \
	                MLMPT_VISIBLE | \
					7 \
					)
#define MLMT_PLUGIN (MLMPT_LOADABLE | \
					8 \
					)
#define MLMT_AUDIO (MLMPT_LOADABLE | \
	                MLMPT_AUDIBLE | \
	                MLMPT_CONTINUOUS | \
					1 \
					)
// XML-��������
#define MLMT_XML   (MLMPT_LOADABLE | \
					1 \
					)
// ���������������� ����
#define MLMT_VIEWPORT ( MLMPT_VISIBLE | \
					20 \
					) 
// 3D �����
#define MLMT_SCENE3D ( MLMPT_LOADABLE | \
					21 \
					) 
// ������
#define MLMT_CAMERA ( MLMPT_LOADABLE | \
					MLMPT_3DOBJECT | \
					22 \
					) 
// ���������
#define MLMT_LIGHT ( MLMPT_LOADABLE | \
					MLMPT_3DOBJECT | \
					23 \
					) 
// 3D-������
#define MLMT_OBJECT ( MLMPT_LOADABLE | \
					MLMPT_3DOBJECT | \
					24 \
					) 
// �������� 3D-�������
#define MLMT_MOTION ( MLMPT_LOADABLE | \
					25 \
					) 
// 3D-��������
#define MLMT_CHARACTER ( MLMPT_LOADABLE | \
					MLMPT_3DOBJECT | \
					26 \
					) 
// ���� 3D-���������
#define MLMT_SPEECH ( MLMPT_LOADABLE | \
					27 \
					) 
// ������ 3D-��������
#define MLMT_GROUP  ( MLMPT_LOADABLE | \
					MLMPT_3DOBJECT | \
					28 \
					)
// "������" 3D-��������� 
#define MLMT_CLOUD  ( MLMPT_LOADABLE | \
					29 \
					) 
// ������
#define MLMT_VISEMES ( MLMPT_LOADABLE | \
					30 \
					) 
// ���������
#define MLMT_HINT   ( MLMPT_LOADABLE | \
					31 \
					) 
// ���� ��� 3D-��������
#define MLMT_PATH3D ( MLMPT_LOADABLE | \
					32 \
					) 
// ����� ��� 3D-��������
#define MLMT_MAP	( \
					33 \
					) 
// �������� ��� 3D-��������
#define MLMT_MATERIAL ( MLMPT_LOADABLE | \
					34 \
					) 
// ��������� ��� 3D-��������
#define MLMT_MATERIALS ( MLMPT_LOADABLE | \
					35 \
					) 

// ���� ������
#define MLMT_FLASH ( MLMPT_LOADABLE |   \
					MLMPT_VISIBLE | \
					MLMPT_CONTINUOUS | \
					36 \
					)

// Browser ������
#define MLMT_BROWSER ( MLMPT_LOADABLE | \
					MLMPT_VISIBLE | \
					37 \
					)
// QuickTime
#define MLMT_QT		(MLMPT_LOADABLE | \
					MLMPT_VISIBLE | \
					MLMPT_CONTINUOUS | \
					38 \
					)

// Input-������� ��� ����� ������
#define MLMT_INPUT ( MLMPT_VISIBLE | \
	                MLMPT_BUTTON | \
					39 \
					)

// ����� �� �����������
#define MLMT_IMAGE_FRAME (MLMPT_LOADABLE | \
					MLMPT_VISIBLE | \
					MLMPT_BUTTON | \
					40 \
					)
// 3D-������ ���� �������
#define MLMT_COMMINICATION_AREA ( MLMPT_LOADABLE | \
				MLMPT_3DOBJECT | \
					41 \
					)
// �����������
#define MLMT_LINE ( \
					MLMPT_VISIBLE | \
					MLMPT_BUTTON | \
					42 \
					)
// �������
#define MLMT_PARTICLES (MLMPT_LOADABLE | \
					MLMPT_3DOBJECT | \
					43 \
					)

// ����������
#define MLMT_COMPOSITION	(MLMPT_VISIBLE | \
					MLMPT_BUTTON | \
					250 \
					)

#if (defined(RMML_BUILD) || defined(RMML_DEBUG))
// ���������� ���� RMML-��������� 
// (������������ ������ SceneManager-�� � Debugger-��)

#define MLMPT_ELEMENT	(1<<(8+7))
#define MLMPT_CUSTOM	(MLMPT_ELEMENT-1)

// ������
#define MLMT_RESOURCE		( MLMPT_LOADABLE | \
							242 \
							) 

// ���� 3D-�������
#define MLMT_SHADOW3D		( \
							243 \
							) 
// ���� 3D-��������
#define MLMT_SHADOWS3D		( \
							244 \
							) 
// ��������� idle-����� ��������
#define MLMT_IDLE			( \
							245 \
							) 
// ����� idle-���� ��������
#define MLMT_IDLES			( \
							246 \
							) 
// ��������� ������ 3D-���������
#define MLMT_MOVEMENT		( \
							247 \
							) 
// ����� ������
#define MLMT_STYLES			(MLMPT_LOADABLE | \
							248 \
							)
// ��������� ������������������ 
#define MLMT_SEQUENCER		(MLMPT_CONTINUOUS | \
							249 \
							)
// �����
#define MLMT_SCENE			(MLMT_COMPOSITION+1)
#define MLMT_SCENECLASS		(MLMT_COMPOSITION+2)
// ������
#define MLMT_SCRIPT			(255)

// �������������� ���� ������� (event, watch, event expression)
#define MLST_EVENT  (MLMT_SCRIPT | (1<<(8+0)))
#define MLST_WATCH  (MLMT_SCRIPT | (1<<(8+1)))
#define MLST_SYNCHMS  (MLMT_SCRIPT | (1<<(8+2)))
#define MLST_EVEXPR  (MLMT_SCRIPT | (1<<(8+3)))
#define MLST_SYNCH  (MLMT_SCRIPT | (1<<(8+4)))
#define MLST_SYNCHPROP  (MLMT_SCRIPT | (1<<(8+5)))
#define MLST_SYNCHEVENT	(MLMT_SCRIPT | (1<<(8+6)))
#define MLSTF_NOTSET  ((1<<(8+7)))
#endif

enum {
	MLPT_UNKNOWN,
	MLPT_NULL,
	MLPT_INT,
	MLPT_BOOLEAN,
	MLPT_STRING,
	MLPT_DOUBLE,
	MLPT_POINT,
	MLPT_SIZE,
	MLPT_RECT,
	MLPT_3DPOS,
	MLPT_3DSCL,
	MLPT_3DROT,
	MLPT_FUNCTION,
	MLPT_ELEMREF,
	MLPT_JSOREF,
	MLPT_LIMIT,
	MLPT_DWORDARGB
};

#define MLCF_COLLISION_BEGIN 1
#define MLCF_COLLISION_END   2

class mlMedia;
class moMedia;

/**
 * ����� ������������ � ������� �����������, ����� ���� ������� ������.
 * (�.�. ���� ������ ��������� ������ �� std::wstring, �� ���������� ������ � 
 * ���� ������ dll-��, � ����� �������� ������� � ���� ���� dll-�� � ������)
 */
#pragma warning( disable : 4251 )
class RMML_DECL mlOutString{
public:
	mlOutString():
		MP_WSTRING_INIT(mstr)
	{
	}

	mlOutString(const mlOutString& other):
		MP_WSTRING_INIT(mstr)
	{
		mstr = other.mstr;
	}

	MP_WSTRING mstr;
	mlOutString& operator=(const wchar_t* apwc);
	mlOutString& operator=(const char* apsz);
	mlOutString& operator=(mlString as);
};

/**
 * ��������� ���� RMML-���������
 */
struct RMML_NO_VTABLE mlIElement{

};

/**
 * ��������� ���� ����������� RMML-���������
 */
#define	RMML_LOAD_STATE_SRC_LOADED				0
#define	RMML_LOAD_STATE_SRC_LOADING				2
#define	RMML_LOAD_STATE_ALL_RESOURCES_LOADED	1
#define	RMML_LOAD_STATE_LOADERROR				-1

enum ELoadingPriority
{
	ELP_UI = 0,	// ������������ ���������
	ELP_ShowNow = 1,
	ELP_ShowLater = 2,
	ELP_Garbage = 3,
	ELP_UI_Visible = 4,
	ELP_XML = 5,
	ELP_BACKGROUND_SOUND = 6,
	ELP_Last
};

struct RMML_NO_VTABLE mlILoadable{
	// ��������
	virtual const wchar_t* GetRawSrc() const= 0;
	virtual const wchar_t* GetSrc() = 0;
	virtual ELoadingPriority GetLoadingPriority() const = 0;
	virtual bool IsStreaming() = 0;
	virtual bool IsLoading() = 0;
	// �������
	/* aiState: 0 - �������� ����, ��������� � src,
				2 - ������� ������� �������� �����, ��������� � src,
				1 - ��������� ��� ������� ������� (�������� ����������� ��� ������� ������ ������),
				-1 - �������� ������ 
	*/
	virtual void onLoad(int aiState) = 0;
	virtual void onUnload() = 0;
	virtual void onLoading(int amiLoadedBytes, int amiTotalBytes) = 0;
	//
};

/**
 * ��������� ���� ����������� �����-��������
 */
struct RMML_NO_VTABLE moILoadable{
	// ��������
	virtual bool SrcChanged()=0; // �������� ������ �������
	virtual bool Reload()=0; // ���������� ������� �� �������� src
	virtual void LoadingPriorityChanged( ELoadingPriority aPriority)=0; // ����� ���������� �������� �������
};

struct RMML_NO_VTABLE mlIVisibleNotifyListener{
	// ���������� ���������� ������������� �������� ��������
	virtual void VisualRepresentationChanged(mlMedia* map)=0;
	// ������� ������� ����������� � ������ �� ���� ����������� �� �����
	virtual void VisibleDestroyed(mlMedia* map)=0;
};

/**
 * ��������� ���� ������� RMML-���������
 */
struct RMML_NO_VTABLE mlIVisible{
	// ��������
	virtual mlPoint GetAbsXY()=0; // �������� ���������� ��������� (� ������ ��������� ���������)
	virtual void SetSize(mlSize asz)=0; 
	virtual mlSize GetSize()=0; 
	virtual int GetAbsDepth()=0; // �������� ���������� ������� (� ������ ������� ���������)
	virtual bool GetAbsVisible()=0; // �������� ���������� �������� ��������� (� ������ ��������� ���������)
	virtual float GetAbsOpacity()=0; //�������� ��������������
	// �������
	//
	virtual void Show()=0;
	virtual void Hide()=0;
	//
	//virtual bool IsVisualRepresentationChanged()=0;
	// ���������� ��������� ����������� ������� ���������
	virtual void SetVListener(mlIVisibleNotifyListener* apListener)=0;
	// ������� ��������� ����������� ������� ���������
	virtual void RemoveVListener(mlIVisibleNotifyListener* apListener)=0;

	virtual mlMedia* GetMLMedia()=0;
};

/**
 * ��������� ���� ������� �����-��������
 */
struct RMML_NO_VTABLE moIVisible{
	// ��������
	virtual void AbsXYChanged()=0; // ���������� ���������� ��������� 
	virtual void SizeChanged()=0;
	virtual void AbsDepthChanged()=0; // ���������� ���������� �������
	virtual void AbsVisibleChanged()=0; // ���������� ���������� ���������
	virtual void AbsOpacityChanged()=0; // ���������� ���������� ���������
	// ������
	virtual mlSize GetMediaSize()=0; 
	virtual void Crop(short awX, short awY, MLWORD awWidth, MLWORD awHeight)=0; 
	virtual mlRect GetCropRect()=0; 
	virtual void SetMask(mlMedia* aMask, int aiX=0, int aiY=0)=0;
	virtual void SetMask(int aiX, int aiY, int aiW, int aiH)=0;
	virtual int IsPointIn(const mlPoint aPnt)=0; // -1 - not handled, 0 - point is out, 1 - point is in
};

/**
 * ��������� ���� �������� RMML-���������
 */
struct RMML_NO_VTABLE mlIContinuous{
	// ��������
	virtual bool GetPlaying()=0; // �������� ���� ������������
	virtual void SetFPS(const short aFPS)=0;	// ������������� �������� FPS
	virtual int GetFPS()=0;	// �������� �������� FPS ��� ����������� ��������
	virtual void SetCurFrame(const int aFrame)=0;	// ������������� �������� currentFrame
	virtual int	GetCurFrame()=0;	// �������� ����� ����� �� ������� ����� �������
	virtual void SetLength(const int alLength)=0;	// ������������� �������� _length (����� ���-�� ������)
	virtual int GetLength()=0;	// �������� ���-�� ������ ����������� ��������
	virtual void SetCurPos(unsigned long alPos)=0;	// ���������� currentPos
	virtual unsigned long GetCurPos()=0;	// �������� currentPos (� ms), �� ������� ����� ����������
//	virtual void SetDuration(const unsigned long alDur)=0;	// ������������� �������� _duration (����������������� � ms)
	virtual long GetIADuration()=0; // �������� ����������������� ����������� ��������
	virtual bool GetLoop()=0;	// �������� ���� �����������
	// �������
	virtual void onEnterFrame()=0;
	virtual void onPlayed()=0;
	//
	virtual void Rewind()=0;
	virtual void Play()=0;
	virtual void Stop()=0;
};

/**
 * ��������� ���� �������� �����-��������
 */
struct RMML_NO_VTABLE moIContinuous{
	// ��������
	virtual void PlayingChanged()=0; // ��������� ���� _playing
	virtual void FrameChanged()=0; // ��������� ����� �������� �����
	virtual void PosChanged()=0; // ��������� currentPos
	virtual void LengthChanged(){} // ���������� ���������� ������ � ����������� ��������
	virtual void DurationChanged(){} // ���������� ����������������� ����������� �������� (� ms)
	virtual void FPSChanged(){} // ��������� FPS ����������� ��������
	virtual unsigned long GetCurPos()=0;	// ����������� ������� �������� (����������������� � ms)
	virtual long GetDuration()=0;	// ����������� �������� _duration (����������������� � ms)
	// �������
	//
	virtual void setCheckPoints(long* apfCheckPoints, int aiLength)=0;
	virtual long getPassedCheckPoint()=0;
};

/**
 * ��������� ���� RMML-���������, ������� ���� ��������
 */
struct RMML_NO_VTABLE mlIButton{
	virtual unsigned int GetKeyCode()=0;
	virtual unsigned int GetKeyStates()=0;
	virtual unsigned int GetKeyScanCode()=0;
	virtual unsigned int GetKeyUnicode()=0;
	// ��������
	virtual bool GetAbsEnabled()=0;
	// �������
	//virtual void OnPress()=0;
	//virtual void OnRelease()=0;
	//virtual void OnReleaseOutside()=0;
	//virtual void OnRollOver()=0;
	//virtual void OnRollOut()=0;

};

/**
 * ��������� ���� �����-��������, ������� ���� ��������
 */
struct RMML_NO_VTABLE moIButton{
	// ��������
	virtual void AbsEnabledChanged()=0;
	// �������
	virtual void onSetFocus()=0;
	virtual void onKillFocus()=0;
	virtual void onKeyDown()=0;
	virtual void onKeyUp()=0;
};

/**
 * ��������� ���� RMML-���������, �������������� 3D-�����
 */
struct RMML_NO_VTABLE mlIScene3D{
	virtual bool GetUseMapManager()=0;
};

/**
 * ��������� 3D-����
 */
struct RMML_NO_VTABLE moIScene3D{
	virtual void UseMapManagerChanged()=0;
};

struct mlIMaterial;

/**
 * ��������� ���� RMML-���������, �������������� 3D-�������
 */
struct RMML_NO_VTABLE mlI3DObject{
	virtual mlMedia* GetParentGroup()=0;
	virtual mlMedia* GetScene3D()=0;
	// ��������
	virtual bool GetVisible()=0;
	virtual bool GetInCameraSpace()=0;
	virtual bool GetCheckCollisions()=0;
	virtual int GetBoundingBox()=0;
	virtual void IntersectionChanged()=0;
	virtual mlMedia* GetShadows()=0;
	virtual mlIMaterial* GetMaterial(unsigned char auID)=0;
	virtual mlMedia* GetMotion()=0;
	virtual bool GetGravitation()=0;
	virtual bool GetIntangible()=0;
	virtual bool GetBillboard()=0;
	virtual bool GetUnpickable()=0;
	virtual mlColor GetBoundingBoxColor()=0;
	virtual mlColor GetColor()=0;	// ������������� �� ���� ���������� ������� ����
	virtual unsigned char GetOpacity()=0; // 0..0xFF  ������� ��������������
	virtual void SyncronizeCoords()=0; // ����� ���������������� ����������
	// �������
	virtual void onPassed()=0;
	virtual void onCollision(mlMedia** aapObjects, int aFlags, ml3DPosition endPoint, int pathPointCount, bool isStatic)=0;
	virtual void onLODChanged(double adVisionDistance)=0;
	// ������
	virtual bool GetAbsVisible()=0;
	virtual wchar_t* GetClassName()=0;
};

/**
 * ����� ���������� 3D-��������
 */
struct RMML_NO_VTABLE mlIMap{
	// ��������
	virtual mlMedia* GetSource()=0;
};

/**
 * ��������� 3D-��������
 */
struct RMML_NO_VTABLE moIMaterial{
	// ��������
	virtual mlColor GetDiffuse()=0;
//	virtual void SetDiffuse(mlColor &aColor)=0;
	virtual mlColor GetAmbient()=0;
//	virtual void SetAmbient(mlColor &aColor)=0;
	virtual mlColor GetSpecular()=0;
//	virtual void SetSpecular(mlColor &aColor)=0;
	virtual double GetPower()=0; // 0..255
//	virtual void SetPower(float auVal)=0; // >= 0
	virtual mlColor GetEmissive()=0;
//	virtual void SetEmissive(mlColor &aColor)=0;
	virtual unsigned char GetOpacity()=0; // 0 - ����������, 255 - ������������
//	virtual void SetOpacity(unsigned char auVal)=0;
	virtual mlMedia* GetMap(const wchar_t* apwcIndex)=0;
//	virtual void SetMap(const wchar_t* apwcIndex, mlIVisible* apVisible)=0;
	//
};

struct RMML_NO_VTABLE mlIMaterial : public moIMaterial{
};

/**
 * ��������� ���� 3D-��������
 */
struct RMML_NO_VTABLE moI3DObject{
	// ��������
	virtual ml3DPosition GetPos()=0; 
	virtual void PosChanged(ml3DPosition &pos)=0; // ���������� ��������� 
	virtual ml3DScale GetScale()=0;
	virtual void ScaleChanged(ml3DScale &scl)=0;
	virtual ml3DRotation GetRotation()=0;
	virtual void RotationChanged(ml3DRotation &rot)=0;
	virtual void VisibleChanged()=0; // ���������� ���������
	virtual void InCameraSpaceChanged()=0;
	virtual void CheckCollisionsChanged()=0; // ��������� ���� �������� �� �����������
	virtual void BoundingBoxChanged()=0;
	virtual void ShadowChanged()=0;
	virtual moIMaterial* GetMaterial(unsigned auID)=0;
	virtual void MaterialsChanged()=0;
	virtual void MaterialChanged(unsigned auID)=0;
	virtual void MaterialMapChanged(unsigned auID, const wchar_t* apwcIndex)=0;
	virtual void MotionChanged()=0;
	virtual void GravitationChanged()=0;
	virtual void IntangibleChanged()=0;
	virtual void BillboardChanged()=0;
	virtual void UnpickableChanged()=0;
	virtual void BoundingBoxColorChanged()=0;
	virtual void ColorChanged()=0;
	virtual void OpacityChanged()=0;
	// ������
	virtual void doMotion(mlMedia* apMotion)=0;
	virtual void setMotion(mlMedia* apMotion,int aiDurMS)=0;
	virtual void removeMotion(mlMedia* apMotion)=0;
		// avObjects - ������ �� ��������� �� ������ ���������� �� �������, 
		// � �������� ������������ ������ (0-� ������� - ��������� �� ������,
		// � ������� ��������� ��������� ������������)
		// (������ ��������� � ������������ RenderManager-��)
		// aiLen - ����� �������
		// aPos3D - ��������� �������, ��� ������� ������������ � 0-� �������� ��� �� ����
	virtual bool getCollisions(mlMedia** &avObjects, int &aiLength, ml3DPosition &aPos3D)=0;
	virtual bool getIntersections(mlMedia** &avObjects, int &aiLength)=0;
	virtual bool getIntersectionsIn(mlMedia** &avObjects, int &aiLength)=0;
	virtual bool getIntersectionsOut(mlMedia** &avObjects, int &aiLength)=0;
	virtual void goPath(mlMedia* apPath, int aiFlags)=0;
	virtual void goPath(ml3DPosition* apPath, int aiPointCount, unsigned int auShiftTime)=0;
	virtual mlMedia* getPath()=0;
		// ����� ���� �� ���������� �����
	virtual bool findPathTo(ml3DPosition &aFrom, ml3DPosition &aTo, ml3DPosition* &avPath, int &aiLength)=0;
		// ��������� �������� �������� ������� ����� ���� (%/���)
	virtual void setPathVelocity(float afVelocity)=0;
		// ��������� ���������� ����� (� %-�� ����), �� ������� ����� �����������
		// onPassed. getPassedCheckPoint() ������ �������� ����������� �����
	virtual void setPathCheckPoints(float* apfCheckPoints, int aiLength)=0;
	virtual float getPassedCheckPoint()=0;
	virtual void setPathPercent(float afPercent)=0;
	virtual float getPathPercent()=0;
	virtual bool attachTo(mlMedia* ap3DODest,const char* apszBoneName)=0;
	virtual ml3DBounds getBounds(mlMedia* ap3DOCoordSysSrc)=0;
		// ��������� bounds-�� ����������, �� ������� ������� �������� � ��������������� aiMatID
	virtual ml3DBounds getSubobjBounds(int aiMatID)=0;
	virtual int moveTo(ml3DPosition aPos3D, int aiDuaration, bool abCheckCollisions)=0;
	virtual ml3DPosition getAbsolutePosition()=0;
	virtual ml3DRotation getAbsoluteRotation()=0;
	virtual bool attachSound(mlMedia* apAudio, ml3DPosition& pos3D)=0;
	virtual bool detachSound(mlMedia* apAudio)=0;
	virtual bool addPotentialTexture(mlMedia* apVisibleObject)=0;
	virtual bool addPotentialTexture(const wchar_t* apwcSrc)=0;
	// Authoring
	virtual bool Get3DPropertiesInfo(mlSynchData &aData)=0;
	virtual bool Get3DProperty(char* apszName,char* apszSubName, mlOutString &sVal)=0;
	virtual bool Set3DProperty(char* apszName,char* apszSubName,char* apszVal)=0;
	// ������
		// ��������� ��������� �� ��� �������� (������ XML)
	virtual void SetMaterialDescription(int aiMatIdx, const wchar_t* apwcDescr)=0;
	// ���������� �� ������ � ���� ����
	virtual bool HasAttachParent()=0;
	// ������ ������� �������
	virtual std::string getAllTextures()=0;
	// �������� ��� ������
	virtual int detectObjectMaterial(int x, int y)=0;
	// ���������� ������� ���������
	virtual void SetMaterialTiling(int textureSlotID, float tilingKoef) = 0;
	// ��������/������ ������������ ���
	virtual void showAxis();
	virtual void hideAxis();
	// ���/���� ����
	virtual void EnableShadows(bool isEnabled) = 0;
	// �������� ��� ��������
	virtual void ChangeCollisionLevel(int level) = 0;
	virtual bool IsFullyLoaded() = 0;
	virtual bool IsIntersectedWithOtherObjects() = 0;
	// �������� ���� � ����� ����� ��������, ���� �� �������� ����� ResManager
	virtual bool GetMaterialTextureSrc(int aiMatIdx, const wchar_t* apwcMapName, mlOutString &asSrc)=0;
	virtual void LevelChanged(int level) = 0;
	virtual int GetCurrentLevel() = 0;
};

/**
 * ��������� 3D-������
 */
struct RMML_NO_VTABLE moI3DGroup{
	// ��������
	virtual void ScriptedChanged()=0;
	// Authoring
	virtual bool ChildAdded(mlMedia* apChild)=0;
	virtual bool ChildRemoving(mlMedia* apChild)=0;
};

/**
 * ��������� RMML-��������� 3D-�����
 */
struct RMML_NO_VTABLE mlI3DGroup{
	// ��������
	virtual bool GetScripted()=0;
};

struct RMML_NO_VTABLE moIParticles{
	virtual void SetMaxParticles(int count)=0;
	virtual int GetMaxParticles() const=0;
	virtual void SetDensity(float density)=0;
	virtual float GetDensity() const=0;
	virtual void SetMinSpeed(float speed)=0;
	virtual float GetMinSpeed() const=0;
	virtual void SetMaxSpeed(float speed)=0;
	virtual float GetMaxSpeed() const=0;
	virtual void SetMinSize(float size)=0;
	virtual float GetMinSize() const=0;
	virtual void SetMaxSize(float size)=0;
	virtual float GetMaxSize() const=0;
	virtual void SetMinOpacity(float opacity)=0;
	virtual float GetMinOpacity() const=0;
	virtual void SetMaxOpacity(float opacity)=0;
	virtual float GetMaxOpacity() const=0;
	virtual void SetDirChangeFactor(float factor)=0;
	virtual float GetDirChangeFactor() const=0;
	virtual void SetSpeedChangeFactor(float factor)=0;
	virtual float GetSpeedChangeFactor() const=0;
	virtual bool SetType(const char* name)=0;
	virtual void SetSortVerts(bool sort)=0;
	virtual void SetDefPointSize(float size)=0;
	virtual float GetDefPointSize() const=0;
};

/**
 * ��������� �������� RMML-���������
 */
struct mlIAudible{
	//
	virtual unsigned long GetRMCurPos() = 0;
	virtual const wchar_t* GetEmitterType() = 0;
	virtual const wchar_t* GetSplineSrc() = 0;
	virtual const int GetVolume() = 0;
	virtual const wchar_t* GetLocation() = 0;
};

/**
 * ��������� �������� ��������
 */
struct moIAudible{
	virtual bool EmitterTypeChanged() = 0;
	virtual bool SplineSrcChanged() = 0;
	virtual bool VolumeChanged() = 0;
	virtual bool LocationChanged() = 0;
};

/**
 * ��������� RMML-�������� ������
 */
struct RMML_NO_VTABLE mlICamera{
	// ��������
	virtual int GetFOV()=0;
	virtual bool GetFixedPosition()=0;
	virtual void SetFOV(int aiFOV)=0;
	virtual ml3DPosition GetDest()=0;
	virtual ml3DPosition GetDestOculus()=0;
	virtual ml3DPosition GetUp()=0;
	virtual void SetDest(ml3DPosition aiDest)=0;
	virtual mlMedia* GetLinkTo()=0;
	virtual double GetMaxLinkDistance()=0;
	// �������
	virtual void onTrackFilePlayed()=0;
};

/**
 * ��������� ������
 */
struct RMML_NO_VTABLE moICamera{
	// ��������
	virtual void FOVChanged()=0;
	virtual void FixedPositionChanged()=0;
	virtual void DestChanged()=0;
	virtual void DestOculusChanged()=0;
	virtual void UpChanged()=0;
	virtual void ResetDestOculus()=0;
	virtual void LinkToChanged()=0;
	virtual ml3DPosition GetDest()=0; 
	virtual ml3DPosition GetDestOculus()=0;
	virtual ml3DPosition GetUp()=0;
	virtual void MaxLinkDistanceChanged()=0;
	virtual void SetNearPlane(float nearPlane)=0;
	virtual float GetZLevel()=0;
	// ������
	virtual int destMoveTo(ml3DPosition aPos3D, int aiDuaration, bool abCheckCollisions)=0;
	virtual void Rotate(int aiDuaration, int aiTime)=0;
	virtual void Move(int aiDuaration, int aiTime)=0;
	virtual void ClearOffset() = 0;
	virtual bool HasOffset() = 0;
	virtual void CorrectPosition() = 0;
	// ������������� �������� ������ �� ����� �� �����
	virtual void PlayTrackFile(const wchar_t* apwcFilePath) = 0;
	// �������
};

/**
 * ��������� RMML-�������� viewport'�
 */
struct RMML_NO_VTABLE mlIViewport{
	// �������� ��������
	virtual mlMedia* GetHint()=0;
	// ��������
	virtual mlMedia* GetCamera()=0;
	virtual mlMedia** GetInclude()=0;
	virtual mlMedia** GetExclude()=0;
	virtual double GetSoundVolume()=0;
	virtual bool GetTraceObjectOnMouseMove()=0;
	// �������
	virtual void onShowHint(mlMedia* apObject, mlPoint aScreenXY, const wchar_t* apwcText)=0;
	virtual void onHintShown(mlMedia* apObject)=0;
	virtual void onHideHint()=0;
};

// ��������� 3D-�������
struct ml3DLocation
{
	mlOutString ID;
	mlOutString params;
};

#define NO_OBJECT		0xFFFFFFFF

/**
 * ��������� viewport'�
 */
struct RMML_NO_VTABLE moIViewport{
		// abGroundOnly - ������������ ��� ������� ����� �����
		// apaCheckObjects - ������ 3D-��������, ������� ����������� ����� ���������
		// �� ����������� � �����, ���� ���� ��� ����� ���������� ��������
		// appTextureMedia - ������������ ������ �� rmml-2D-������, ��������� �� ��������, � ������� ������
		// apXYTextureMedia - 2D-���������� ��������� ���� ������������ ����� 2D-�������
	virtual mlMedia* GetObjectUnderMouse(mlPoint &aMouseXY, bool abGroundOnly = false,
		ml3DPosition *apXYZ=NULL, ml3DLocation *apLocation=NULL, int *apiType=NULL, 
		ml3DPosition *apXYTexture=NULL, mlMedia** apaCheckObjects=NULL, 
		mlMedia** appTextureMedia=NULL, mlPoint* apXYTextureMedia=NULL, bool* abpGround=NULL)=0;
	virtual bool GetProjection(ml3DPosition &aXYZ, mlPoint &aXY)=0;
	virtual bool GetProjection(mlMedia* ap3DO, mlPoint &aXY, mlRect &aBounds)=0;
	virtual void GetRay(mlPoint &aXY, ml3DPosition &aXYZ0, ml3DPosition &aXYZ)=0;
	virtual void SetLODScale(float lodScale)=0;
	// ��������
	virtual void CameraChanged()=0;
	virtual void InclExclChanged()=0;
	virtual void SoundVolumeChanged()=0;
	virtual void TraceObjectOnMouseMoveChanged()=0;
	// �������
	virtual void onPress(mlPoint apntXY)=0; // ������ ���-�� (apntXY) �� viewport-�
	virtual void onRelease()=0;				// ��������� ������ ����
	// ������
	virtual mlRect GetBounds(mlMedia* apObject)=0;
		// �������� ����� ����������� ���� � ������ � ����������, �������� ������ � ��������
	virtual ml3DPosition GetIntersectionWithPlane(mlPoint aXY, ml3DPosition aPlane, ml3DPosition aNormal, bool& abBackSide)=0;
		// �������� ����� �������� 3D-����� �� ����� � ������ ����������
	virtual ml3DPosition GetProjectionToLand(ml3DPosition a3DPoint)=0;
	// �������� ����� �������� 3D-����� �� ����� � ������ ��������
	virtual ml3DPosition GetProjectionToObjects(ml3DPosition a3DPoint)=0;
		// �������� ��������� ������������� ������� �� 2D-�����������
	virtual bool GetObjectStringIDAt(mlPoint aXY, mlOutString& asStringID)=0;
};

struct mlStyle{
	enum Units{
		U_undef,
		U_pt,		// ������
		U_px,		// �������
		U_percent	// ��������
	};
	struct IntWithUnits{
		short val; // -1 - default
		Units units;
	};
	enum{
		FS_undef,
		FS_normal,
		FS_italic
	} fontStyle;
	enum{
		FW_undef,
		FW_normal,
		FW_bold
	} fontWeight;
	IntWithUnits fontSize;
	IntWithUnits lineHeight;
	char* fontFamily;
	enum{
		TA_undef,
		TA_left,
		TA_right,
		TA_center,
		TA_justify
	} textAlign;
	IntWithUnits textIndent;
	enum{
		TD_undef,
		TD_none,
		TD_underline
	} textDecoration;
	struct Color{
		bool def; // defined
		unsigned char r,g,b;
	} color;
	enum{
		AA_undef,
		AA_none,
		AA_smooth
	} antialiasing;

	mlStyle(){
		fontStyle=FS_undef;
		fontWeight=FW_undef;
		fontSize.val=-1;
		fontSize.units=U_undef;
		lineHeight.val=-1;
		lineHeight.units=U_undef;
		fontFamily=NULL;
		textAlign=TA_left;
		textIndent.val=-1;
		textIndent.units=U_undef;
		textDecoration=TD_undef;
		color.def=false;
		antialiasing=AA_undef;
	}

	// !!! the strucure must be destroying in the DLL it was created in !!!
	~mlStyle();	
	mlStyle& operator=(mlStyle& aStyle);
};

/**
 * ��������� RMML-��������� �����������
 */
struct RMML_NO_VTABLE mlIImage{
	enum Filter{
		AA_NONE,
		AA_LINEAR,
		AA_ANISOTROPIC
	};
	virtual Filter GetAntialiasing()=0;
	virtual bool GetEditable()=0;
	virtual bool GetEditableNow()=0;
	virtual bool GetRemoteControlNow()=0;
	virtual mlColor GetColor()=0;
	virtual bool GetTile()=0;
	virtual bool GetSmooth()=0;
	virtual mlBounds GetCrop()=0;
	virtual bool GetCompressed()=0;
	virtual int GetAngle()=0;
	virtual unsigned char GetMaxDecreaseKoef()=0;
	virtual bool GetSysmem()=0;
};

/**
 * ��������� �����������
 */
struct RMML_NO_VTABLE moIImage{
	virtual void EditableChanged()=0;
	virtual void EditableNowChanged()=0;
	virtual void ColorChanged()=0;
	virtual void TileChanged()=0;
	virtual void SmoothChanged()=0;
	virtual void CropChanged()=0;
	virtual void CompressedChanged()=0;
	virtual void AngleChanged()=0;
	virtual void MaxDecreaseKoefChanged()=0;
	virtual void SysmemChanged()=0;
	virtual bool MakeSnapshot(mlMedia** appVisibles, const mlRect aCropRect)=0;
	virtual void DrawLine(int aiX1, int aiY1, int aiX2, int aiY2, const mlColor color, int aiWeight)=0;
	virtual void FillRectangle(int aiX1, int aiY1, int aiX2, int aiY2, const mlColor color)=0;
	virtual void SetCanvasSize(int aiWidth, int aiHeight)=0;
	virtual void DrawEllipse(int aiX1, int aiY1, int aiX2, int aiY2, const mlColor aColor, int aiWeight, const mlColor aFillColor)=0;
	virtual void DrawRectangle(int aiX1, int aiY1, int aiX2, int aiY2, const mlColor aColor, int aiWeight, const mlColor aFillColor)=0;
	virtual void SetActiveTool(int aToolNumber)=0;
	virtual void UndoAction()=0;
	virtual void PasteText(bool isPasteOnIconClick = false)=0;
	virtual void SetLineWidth(int aLineWidth)=0;
	virtual void SetLineColor(const mlColor aColor)=0;
	virtual void SetBgColor(const mlColor aColor)=0;
	virtual void SetToolOpacity(int aOpacity)=0;
	virtual void SetTextFontName(wchar_t* aFontName)=0;
	virtual void SetTextBold(bool isBold)=0;
	virtual void SetTextItalic(bool isItalic)=0;
	virtual void SetTextUnderline(bool isUnderline)=0;
	virtual void SetTextAntialiasing(bool isAntialiasing)=0;
	virtual void SetToolMode(int aMode)=0;
	virtual void SetLineDirection(int aDirection)=0;
	virtual void Resample(int aiWidth, int aiHeight)=0;
	virtual void ClearOldScreen()=0;
	virtual mlColor GetPixel(int aiX, int aiY)=0;
	// ������� ���� ����� �����������
	virtual void DuplicateFrom(mlMedia* apSourceImage)=0;
	virtual bool isPaintToolUsed()=0;
	virtual bool isFirstSharingFrameDrawed()=0;
	virtual void CopyToClipboard() = 0;
	virtual bool ChangeState( unsigned char type, unsigned char *aData, unsigned int aDataSize) = 0;
	virtual bool GetBinState( BYTE*& aBinState, int& aBinStateSize) = 0;
	virtual bool SetIPadDrawing( bool abIPadDrawing) = 0;
	virtual bool ApplyIPadState() = 0;
	virtual bool IsBinaryStateReceived() = 0;
	virtual void Save() = 0;
};

/**
 * ��������� ��������� RMML-���������
 */
struct RMML_NO_VTABLE mlIText{
	// ��������
	virtual const wchar_t* GetValue()=0; // �������� �����
	virtual mlStyle* GetStyle(const char* apszTag=NULL)=0;
	virtual bool IsHtml()=0;
	virtual const wchar_t* GetSliceTail()=0;
	virtual bool IsAdjustHeight()=0;
	virtual const mlColor GetBkgColor()=0;
	virtual mlSize GetFormatSize()=0; 
	// ���������� ������ ���������� ����
	// (����� �� ��������� � �������� ��������)
	virtual void SetTextSize(unsigned int auWidth, unsigned int auHeight)=0;
	// ���������� ������ ������ � ���������� �����
	virtual void SetScrollSizeInLines(unsigned int auPageSize, unsigned int auTextSize)=0;
	virtual int GetSymbolIDByXY(int x, int y) = 0;
	// �������
};

/**
 * ��������� ������
 */
struct RMML_NO_VTABLE moIText{
	// ��������
	virtual void ValueChanged()=0;
	virtual void StylesChanged()=0;
	virtual void HtmlChanged()=0;
	virtual void AdjustChanged()=0;
	virtual void BkgColorChanged()=0;
	virtual unsigned int GetScrollTop()=0;
	virtual void SetScrollTop(unsigned int auValue)=0;
	virtual unsigned int GetScrollLeft()=0;
	virtual void SetScrollLeft(unsigned int auValue)=0;
	// �������
	// ������
	virtual void ScrollToLine(unsigned int auLine)=0;
	virtual void AddText(wchar_t* str)=0;
	//added nikitin
	virtual void DecreaseCursor(unsigned int delta)=0;
	virtual void IncreaseCursor(unsigned int delta)=0;
	virtual void GetVisibleBounds(int& b, int& e)=0;
	virtual void SetCursorPos(int pos)=0;
	virtual void ToClipboard(wchar_t* str)=0;
	virtual std::wstring FromClipboard()=0;
	virtual int GetCursorX()=0;
	virtual int GetCursorY()=0;
	virtual int GetCursorPos()=0;
	virtual int GetCursorHeight()=0;
	virtual void SetSelection(int from, int to)=0;
	virtual bool GetSliced()=0;
	virtual int GetSymbolIDByXY(int x, int y) = 0;
	////added nikitin
	// ������
	virtual bool GetTextFormattingInfo(mlSynchData &aData)=0; // ��� Authoring-�
	virtual bool AreWordBreaksExists() = 0;
};

/**
 * ��������� RMML-��������� �����
 */
struct RMML_NO_VTABLE mlIInput{
	virtual const wchar_t* GetValue()=0; // �������� �����
	virtual mlMedia* GetParent()=0;
	virtual mlStyle* GetStyle(const char* apszTag=NULL)=0;
	virtual const wchar_t* GetSliceTail()=0;
	virtual bool IsAdjustHeight()=0;
	virtual void SetInputSize(unsigned int auWidth, unsigned int auHeight) = 0;
	virtual bool GetPassword()=0;
	virtual bool GetMultiline()=0;
	virtual int GetTabGroup()=0;
	virtual int GetTabIndex()=0;
	virtual void OnKeyDown(int aiCode)=0;
	virtual void OnSetFocus()=0;
	virtual void OnKillFocus()=0;
};

/**
 * ��������� �������� �����
 */
struct RMML_NO_VTABLE moIInput{
	virtual void ValueChanged()=0;
	virtual void StylesChanged()=0;
	virtual void PasswordChanged()=0;
	virtual void MultilineChanged()=0;
	virtual void TabGroupChanged()=0;
	virtual void TabIndexChanged()=0;

	virtual const wchar_t* GetValue() const = 0;

	virtual bool InsertText(const wchar_t* apwcText, int aiPos=-1)=0;
};

/**
 * ��������� ����� RMML-���������
 */
struct RMML_NO_VTABLE mlIVideo{
	// ��������
	virtual bool IsWindowed()=0;
	// �������
};

/**
 * ��������� �����
 */
struct RMML_NO_VTABLE moIVideo{
	// ��������
	// �������
	virtual void WindowedChanged()=0;
};

/**
 * ��������� RMML-��������� 2D-�����
 */
struct RMML_NO_VTABLE mlILine{
	virtual mlPoint GetAbsXY2()=0;
	virtual mlColor GetColor()=0;
	virtual int GetThickness()=0;
};

/**
 * ��������� 2D-�����
 */
struct RMML_NO_VTABLE moILine{
	virtual void ColorChanged()=0;
	virtual void ThicknessChanged()=0;
	virtual void SmoothChanged()=0;
};

#define ML_UNDEF_MARGIN_VALUE -1

/**
* ��������� RMML-��������� ����� �� �����������
*/
struct RMML_NO_VTABLE mlIImageFrame{
	// 
	virtual int GetMargins(int& aiMarginTop, int& aiMarginBottom, int& aiMarginLeft, int& aiMarginRight)=0;
	virtual bool GetTile()=0;
};

struct RMML_NO_VTABLE moIImageFrame{
	// 
	virtual void MarginsChanged()=0;
	virtual void TileChanged()=0;
};

/**
 * ��������� RMML-��������� ������� 3D-����������
 */
struct RMML_NO_VTABLE mlIMovement{
	virtual mlMedia* GetMotion(const wchar_t* apwcName)=0;
	// ��������
	virtual double	GetStepLength()=0;
	virtual double	GetRotSpeed()=0;
	// �������
};

/**
 * ��������� ������ idle-�� ��� 3D-����������
 */
struct RMML_NO_VTABLE mlIIdles{
	virtual mlMedia* GetIdle(int aiNum)=0;
};

/**
 * ��������� idle-� ��� 3D-����������
 */
struct RMML_NO_VTABLE mlIIdle{
	virtual mlMedia* GetMotion()=0;
	virtual int GetBetween()=0;
	virtual double GetFreq()=0;
};

enum mlE3DShadowType{
	MLSHT_NONE,
	MLSHT_PROJECTIVE,
	MLSHT_STENCIL
};

/**
 * ��������� ���� ��� 3D-��������
 */
struct RMML_NO_VTABLE mlIShadow{
	virtual mlE3DShadowType GetType()=0;
	virtual ml3DPlane GetPlane()=0;
	virtual mlColor GetColor()=0;
	virtual mlMedia* GetLight()=0;
};

/**
 * ��������� ������ ����� ��� 3D-��������
 */
struct RMML_NO_VTABLE mlIShadows: public mlIShadow{
	virtual mlMedia* GetShadow(int aiNum)=0;
};

/**
 * ��������� RMML-��������� 3D-����������
 */
struct RMML_NO_VTABLE mlICharacter{
	// �������� ��������
	virtual mlMedia* GetCloud()=0;
	// ��������
	virtual mlIMovement* GetMovement()=0;
	virtual mlMedia* GetVisemes()=0;
	virtual mlMedia* GetIdles()=0;
	virtual mlIIdles* GetFaceIdles()=0;
	virtual bool GetUnpickable();
	// �������
	virtual void onTurned()=0;
	virtual void onSaid()=0;
	virtual void onFalling()=0;
	virtual void onFallAndStop()=0;
	virtual void onWreck()=0;
	virtual void onNeedToChangePosition()=0;
	virtual void onRightHandPosChanged(ml3DPosition aRightHandPos, ml3DPosition aRightHandDir, 
		ml3DPosition aRightHandDirExt)=0;
	virtual void onLeftHandPosChanged(ml3DPosition aLeftHandPos, ml3DPosition aLeftHandDir, 
		ml3DPosition aLeftHandDirExt)=0;
	// ������
	virtual void SynchronizeZPosition()=0;
};

/**
 * ��������� 3D-����������
 */
struct RMML_NO_VTABLE moICharacter{
	// ��������
	virtual void UnpickableChanged()=0;
	virtual void MovementChanged()=0;
	virtual void VisemesChanged()=0;
	virtual void IdlesChanged()=0;
	virtual void FaceIdlesChanged()=0;
	virtual mlMedia* GetSpeech()=0; // speech/audio, ������� ������ ������������
	virtual mlICharacter** GetInterlocutors()=0;
	virtual void IdlesSrcChanged(const wchar_t* apwcIdlesName, const wchar_t* apwcSrc)=0;
	// ������
	virtual bool say(mlMedia* apSpeech=NULL, int* apDuration=NULL)=0;
	virtual bool goTo(mlMedia* apObject, ml3DPosition* apTurnToPos=NULL, mlMedia* apTurnToObject=NULL, unsigned int shiftTime = 0)=0;
	virtual bool goTo(ml3DPosition &aPos, ml3DPosition* apTurnToPos=NULL, mlMedia* apTurnToObject=NULL, unsigned int shiftTime = 0)=0;
	virtual bool turnTo(mlMedia* apObject)=0;
	virtual bool turnTo(ml3DPosition &aPos)=0;
	virtual bool turnTo(int aiFlags)=0;
	virtual bool lookAt(mlMedia* apObject, int aiMs, double adKoef)=0;
	virtual bool lookAt(ml3DPosition &aPos, int aiMs, double adKoef)=0;
	virtual bool lookAtByEyes(mlMedia* apObject, int aiMs, double adKoef)=0;
	virtual bool lookAtByEyes(ml3DPosition &aPos, int aiMs, double adKoef)=0;
	virtual bool lookAtByEyes(const wchar_t* apDest, int aiMs, double adKoef)=0;
	virtual bool addInterlocutor(mlICharacter* apCharacter)=0;
	virtual bool removeInterlocutor(mlICharacter* apCharacter)=0;
		// ���������� ��������� ���� � �������� auiIndex, ������ �� apVisemes.
		// aiTime - ������ ������� ���������/������, 
		// aiKeepupTime - ������ �������, ������� "�������" ��� ��������� �� ����
	virtual bool setViseme(mlMedia* apVisemes, unsigned int auiIndex, int aiTime=-1, int aiKeepupTime=-1)=0;
		// ������ � ���� ��������� � �������� auiIndex, ������ �� apVisemes.
		// aiTime - ������ ������� "��������"
	virtual bool unsetViseme(mlMedia* apVisemes, unsigned int auiIndex, int aiTime=-1)=0;
		// ��������������� ������� ��� ��������� ����, ��������� � apVisemes, �� ������ ������� aiTime
	virtual bool doVisemes(mlMedia* apVisemes, int aiTime=-1)=0;
		// ���������� ��������� � ��������� ����� � �������, �������� �����������:
		// apStartPos - ��������� �������, adRadius - ������ �������
		// abInRoom - ��������� � �������� �������
	virtual bool setToRandomFreePlace(ml3DPosition* apStartPos, double adRadius, bool abInRoom)=0;
		// ��� ������
	virtual bool goForward(unsigned int shiftTime)=0;
		// ������
	virtual bool goBackward(unsigned int shiftTime)=0;
		// ��� ����� �����
	virtual bool strafeLeft(unsigned int shiftTime)=0;
		// ��� ����� ������
	virtual bool strafeRight(unsigned int shiftTime)=0;
		// ��������� ���� (��� goTo(NULL))
	virtual bool stopGo()=0;
		// ������������� �����
	virtual bool turnLeft(unsigned int shiftTime)=0;
		// ������������� ������
	virtual bool turnRight(unsigned int shiftTime)=0;
		// ��������� ��������������
	virtual bool stopTurn()=0;
		// ������������� ���������� ����
	virtual bool sayRecordedVoice( const char* alpcUserName, const char* alpcFileName, LPCSTR alpcCommunicationAreaName, unsigned int auPosition, unsigned int auSyncVersion) = 0;
		// ���������� ��������� ��� �� ��������� �����
	virtual bool setLexeme( unsigned int auiIndex, int aiTime=0) = 0;
		// ��� ������ � ������
	virtual bool goLeft(unsigned int shiftTime)=0; 
		// ��� ������ � �������
	virtual bool goRight(unsigned int shiftTime)=0; 
		// ��� ����� � ������
	virtual bool goBackLeft(unsigned int shiftTime)=0; 
		// ��� ����� � �������
	virtual bool goBackRight(unsigned int shiftTime)=0; 

	virtual bool getFaceVector(float& x, float& y, float& z) = 0;

    virtual void toInitialPose() = 0;
	virtual void fromInitialPose() = 0;
		// �������� �� �������� ����� ��� ������
	virtual bool pointByHandTo(const ml3DPosition& aPos, mlMedia* apObject = NULL) = 0;

	virtual bool setSleepingState(bool abSleep) = 0;
	virtual void SetTrackingRightHand(bool isEnabled) = 0;
	virtual void SetTrackingLeftHand(bool isEnabled) = 0;
	virtual bool setKinectDeskPoints(const ml3DPosition& aPos, const ml3DPosition& aPos2, const moMedia* moObj) = 0;
	virtual void disableKinectDesk() = 0;
	// �������
};

/**
 * ��������� RMML-��������� �������� 3D-��������
 */
struct RMML_NO_VTABLE mlIMotion{
	// ��������
	virtual void SetDuration(unsigned int aiDuration)=0;
	virtual unsigned int GetDuration()=0;
	virtual bool GetInverse()=0;
	virtual bool GetPrimary()=0;
	// �������
	virtual void onSet()=0;
	virtual void onDone()=0;
};

/**
 * ��������� �������� 3D-��������
 */
struct RMML_NO_VTABLE moIMotion{
	// ��������
	virtual void DurationChanged()=0;
	virtual void InverseChanged()=0;
	virtual void PrimaryChanged()=0;
	virtual mlMedia* GetObject()=0; // ����� ������ ��������� ��� �������� ��� ��������
	// �������
	// ������
	virtual ml3DPosition GetFramePos(const wchar_t* apwcTrack, unsigned int auFrame)=0; 
	virtual void SetFramePos(const wchar_t* apwcTrack, unsigned int auFrame, ml3DPosition &pos)=0;
	virtual ml3DScale GetFrameScale(const wchar_t* apwcTrack, unsigned int auFrame)=0;
	virtual void SetFrameScale(const wchar_t* apwcTrack, unsigned int auFrame, ml3DScale &scl)=0;
	virtual ml3DRotation GetFrameRotation(const wchar_t* apwcTrack, unsigned int auFrame)=0;
	virtual void SetFrameRotation(const wchar_t* apwcTrack, unsigned int auFrame, ml3DRotation &rot)=0;
	//
	virtual ml3DPosition GetFramePos(int aiTrack, unsigned int auFrame)=0; 
	virtual void SetFramePos(int aiTrack, unsigned int auFrame, ml3DPosition &pos)=0;
	virtual ml3DScale GetFrameScale(int aiTrack, unsigned int auFrame)=0;
	virtual void SetFrameScale(int aiTrack, unsigned int auFrame, ml3DScale &scl)=0;
	virtual ml3DRotation GetFrameRotation(int aiTrack, unsigned int auFrame)=0;
	virtual void SetFrameRotation(int aiTrack, unsigned int auFrame, ml3DRotation &rot)=0;
	virtual bool moveTo(ml3DPosition& aPos3D)=0;
	virtual bool rotateTo(ml3DRotation& aRot3D)=0;
};

/**
 * ��������� RMML-��������� '��������' 3D-����������
 */
struct RMML_NO_VTABLE mlICloud{
	// �������� ��������
	virtual mlMedia* GetText()=0;
	virtual mlMedia* GetNicknameText()=0;
	// ��������
	virtual bool GetVisibility()=0;
	virtual mlMedia* GetContent()=0;
	virtual mlMedia* GetFootnote()=0;
	virtual const wchar_t* GetAlign()=0;
	virtual bool GetMoveOutOfScreen()=0;
	virtual bool GetAllowOverlap()=0;
	virtual bool GetSortOnDepth()=0;
	virtual int GetMarginX()=0;
	virtual int GetMarginY()=0;
	virtual double GetMaxVisibleDistance()=0;
	virtual mlMedia* GetAttachedTo()=0;
};

/**
* ��������� '��������' 3D-��������
*/
struct RMML_NO_VTABLE moICloud{
	virtual void VisibleChanged()=0;
	virtual void ContentChanged()=0;
	virtual ml3DPosition GetPos()=0;
	virtual void SetPos(ml3DPosition &aPos)=0;
	virtual void PosChanged(ml3DPosition &pos)=0;
	virtual void FootnoteChanged()=0;
	virtual void AlignChanged()=0;
	virtual void MoveOutOfScreenChanged()=0;
	virtual void AllowOverlapChanged()=0;
	virtual void SortOnDepthChanged()=0;
	virtual void MaxVisibleDistanceChanged()=0;
	virtual void MarginXChanged()=0;
	virtual void MarginYChanged()=0;
	virtual void AttachedToChanged(mlMedia* pmlMedia)=0;
	virtual void SetSpecParameters(int adeltaNoChangePos, int ashiftAtAvatarPosZ)=0;
	virtual void ViewVecChanged(ml3DPosition &viewVec);
};

/**
 * ��������� hint-� ��� 3D-��������
 */
struct RMML_NO_VTABLE mlIHint{
	// ��������
	virtual mlStyle* GetStyle(const char* apszTag=NULL)=0;
	// �������
};

/**
 * ��������� Path3D-�������
 */
struct RMML_NO_VTABLE moIPath3D{
	// ��������
	// �������
	// ������
	virtual ml3DPosition getPosition(double adKoef)=0;
	virtual ml3DPosition getLinearPosition(double adKoef)=0;
	virtual ml3DPosition getNormal(double adKoef)=0;
	virtual ml3DPosition getTangent(double adKoef)=0;
	virtual ml3DPosition getNormalByLinearPos(double adKoef)=0;
	virtual ml3DPosition getTangentByLinearPos(double adKoef)=0;
};

enum ML_VARENUM{
    MLVT_EMPTY           = 0,
    MLVT_NULL            = 1,
    MLVT_I2              = 2,
    MLVT_I4              = 3,
    MLVT_R4              = 4,
    MLVT_R8              = 5,
    MLVT_CY              = 6,
    MLVT_DATE            = 7,
    MLVT_BSTR            = 8,
    MLVT_DISPATCH        = 9,
    MLVT_ERROR           = 10,
    MLVT_BOOL            = 11,
    MLVT_VARIANT         = 12,
    MLVT_UNKNOWN         = 13,
    MLVT_DECIMAL         = 14,
// VBA reserves 15 for future use
    MLVT_I1              = 16,
    MLVT_UI1             = 17,
    MLVT_UI2             = 18,
    MLVT_UI4             = 19,
    MLVT_I8              = 20,
    MLVT_UI8             = 21,
    MLVT_INT             = 22,
    MLVT_UINT            = 23,
    MLVT_VOID            = 24,
    MLVT_HRESULT         = 25,
    MLVT_PTR             = 26,
    MLVT_SAFEARRAY       = 27,
    MLVT_CARRAY          = 28,
    MLVT_USERDEFINED     = 29,
    MLVT_LPSTR           = 30,
    MLVT_LPWSTR          = 31,
// VBA reserves 32-35 for future use
    MLVT_RECORD          = 36,

    MLVT_FILETIME        = 64,
    MLVT_BLOB            = 65,
    MLVT_STREAM          = 66,
    MLVT_STORAGE         = 67,
    MLVT_STREAMED_OBJECT = 68,
    MLVT_STORED_OBJECT   = 69,
    MLVT_BLOB_OBJECT     = 70,
    MLVT_CF              = 71,
    MLVT_CLSID           = 72,

    MLVT_BSTR_BLOB       = 0x0fff,

    MLVT_VECTOR          = 0x1000,
    MLVT_ARRAY           = 0x2000,
    MLVT_BYREF           = 0x4000,
    MLVT_RESERVED        = 0x8000,

    MLVT_ILLEGAL         = 0xffff,
    MLVT_ILLEGALMASKED   = 0x0fff,
    MLVT_TYPEMASK        = 0x0fff
};
struct ML_VARIANT{
	unsigned short vt;
	MLWORD wReserved1;
	MLWORD wReserved2;
	MLWORD wReserved3;
	union{
		MLLONG lVal;
		MLBYTE bVal;
		short iVal;
		float fltVal;
		double dblVal;
		short boolVal;
		long scode;
		union tagCY {
			struct {
#ifdef _MAC
				long      Hi;
				long Lo;
#else
				unsigned long Lo;
				long      Hi;
#endif
			};
			MLDWORD int64;
		} cyVal;
		double date;
		wchar_t* bstrVal;
		void *punkVal;
		void *pdispVal;
		void *parray;
		MLBYTE *pbVal;
		short *piVal;
		long *plVal;
		float *pfltVal;
		double *pdblVal;
		short *pboolVal;
		short *pbool;
		long *pscode;
		tagCY *pcyVal;
		double *pdate;
		wchar_t* *pbstrVal;
		void* *ppunkVal;
		void* *ppdispVal;
		void* *pparray;
		ML_VARIANT *pvarVal;
		void* byref;
		char cVal;
		unsigned short uiVal;
		unsigned long ulVal;
		int intVal;
		unsigned int uintVal;
		struct tagDEC {
			unsigned short wReserved;
			union {
				struct {
					MLBYTE scale;
					MLBYTE sign;
				};
				unsigned short signscale;
			};
			unsigned long Hi32;
			union {
				struct {
#ifdef _MAC
					MLULONG Mid32;
					MLULONG Lo32;
#else
					MLULONG Lo32;
					MLULONG Mid32;
#endif
				};
				MLDWORD Lo64;
			};
		};
		tagDEC *pdecVal;
		char* *pcVal;
		MLUSHORT *puiVal;
		MLULONG *pulVal;
		int *pintVal;
		unsigned int *puintVal;
		struct  __tagBRECORD{
			void* pvRecord;
			void *pRecInfo;
		};
	};
};

/**
 * ��������� ActiveX-��������� RMML
 */
struct RMML_NO_VTABLE mlIActiveX{
	// ��������
	// �������
	virtual omsresult CallExternal(const wchar_t* apProcName, int aiArgC, 
						   ML_VARIANT* pvarArgs, ML_VARIANT* varRes, bool abEvent)=0;
};

/**
 * ��������� ActiveX-��������
 */
struct RMML_NO_VTABLE moIActiveX{
	// ��������
	// �������
	virtual omsresult Call(wchar_t* apProcName, int aiArgC, 
						   ML_VARIANT* pvarArgs, ML_VARIANT* varRes)=0;
	virtual omsresult Set(wchar_t* apPropName,ML_VARIANT* vVar)=0;
	virtual omsresult Get(wchar_t* apPropName,ML_VARIANT* varRes)=0;
};

/**
* ��������� Browser-��������� RMML
*/
struct RMML_NO_VTABLE mlIBrowser{
	// ��������
	virtual mlColor GetBkgColor()=0;
	virtual const wchar_t* GetDefaultCharset()=0;
	virtual bool GetAXFrame()=0;
	virtual const wchar_t* GetValue()=0;
	// ������
	// �������
	virtual void onScrolled()=0; // �������� ���������������� html-�� (������� ����, ����������...)
		// Fired when a new hyperlink is being navigated to.
	virtual void onBeforeNavigate(const wchar_t* URL, int flags, const wchar_t* targetFrameName, 
				const wchar_t* postData, const wchar_t* headers, bool &cancel)=0;
		// Fired when the document being navigated to becomes visible and enters the navigation stack
	virtual void onNavigateComplete(const wchar_t* URL)=0;
	virtual void onDocumentComplete(const wchar_t* URL)=0;
		// Fired when download progress is updated
	virtual void onProgressChange(int progress, int progressMax)=0;
	virtual void onDownloadBegin()=0; // Download of a page started
	virtual void onDownloadComplete()=0; // Download of page complete
	virtual void onCommandStateChange(int command, bool enable)=0; // The enabled state of a command changed 
		// Fires when an error occurs during navigation
	virtual void onNavigateError(const wchar_t* URL, const wchar_t* frame, int StatusCode, bool &cancel)=0;
};

/**
* ��������� Browser-��������
*/
struct RMML_NO_VTABLE moIBrowser{
	// ��������
	virtual const wchar_t* GetDocumentType()=0;
	virtual const wchar_t* GetLocationName()=0;
	virtual const wchar_t* GetLocationURL()=0;
	virtual bool GetBusy()=0;
	virtual int GetReadyState()=0;
	virtual bool GetOffline()=0;
	virtual void SetOffline(bool abValue)=0;
	virtual bool GetSilent()=0;
	virtual void SetSilent(bool abValue)=0;
	virtual int GetScrollTop()=0;
	virtual void SetScrollTop(int aiValue)=0;
	virtual int GetScrollLeft()=0;
	virtual void SetScrollLeft(int aiValue)=0;
	virtual void BkgColorChanged()=0;
	virtual void ValueChanged()=0;
	// ������
		// flags: 2 - NoHistory, 4 - NoHistory, 8 - NoWriteToCache
	virtual omsresult Navigate(const wchar_t* URL, int flags, const wchar_t* postData, const wchar_t* headers)=0;
	virtual void GoBack()=0; // Navigates to the previous item in the history list
	virtual void GoForward()=0; // Navigates to the next item in the history list
	virtual void GoHome()=0; // Go home/start page (src)
	virtual void Refresh(int level)=0; // Refresh the currently viewed page (NORMAL = 0, IFEXPIRED = 1, CONTINUE = 2, COMPLETELY = 3)
	virtual void Stop()=0; // Stops opening a file
	virtual bool ScrollTo(int x, int y)=0; // ����������
	virtual bool ScrollBy(int x, int y)=0; // ������������� 
	virtual void ScrollToAnchor(const wchar_t* anchor)=0;
	virtual omsresult ExecScript(const wchar_t* code, const wchar_t* language, ML_VARIANT* result)=0;
	virtual bool ExecCommand(const wchar_t* cmdID, bool showUI, ML_VARIANT value)=0;
	virtual bool QueryCommandEnabled(const wchar_t* cmdID)=0;
	virtual bool FireMouseDown(int x, int y)=0; // x,y ������������ ������ �������� ���� ���� ��������
	virtual bool FireMouseMove(int x, int y)=0;
	virtual bool FireMouseUp(int x, int y)=0;
	virtual bool FireMouseWheel(int x, int y, int distance)=0;
	virtual bool KeyDown(unsigned int keyCode, unsigned int keyStates, unsigned int scanCode)=0;
	virtual bool KeyUp(unsigned int keyCode, unsigned int keyStates, unsigned int scanCode)=0;
	virtual bool KeyPress(unsigned int charCode, unsigned long scanCode)=0;
//	virtual bool SetFocus()=0;
//	virtual bool KillFocus()=0;
	// �������
};

struct mlIFlash
{
    virtual void onFSCommand(const wchar_t *command, const wchar_t *args) = 0;

	virtual int GetWModeOnCreate()=0;
};

struct moIFlash
{
    virtual void  setVariable(const wchar_t *var, const wchar_t *value) = 0;

	virtual int getScaleMode()=0;
	virtual void setScaleMode(int)=0;

	virtual int getWMode()=0;
	virtual void setWMode(int)=0;

// ���������� ����� ������
    virtual long getVariable(const wchar_t *var, mlOutString &sRetVal) = 0;
// ���������� ����� ����������
    virtual long callFunction(const wchar_t *request, mlOutString &sRetVal) = 0;
};

/**
 * ��������� ActiveX-��������� RMML
 */
struct RMML_NO_VTABLE mlIPlugin{
	// ��������
	virtual const wchar_t*	GetUID()=0;
	// �������
	virtual omsresult Call(char* apProcName, char* aArgTypes, va_list aArgsV)=0;
};

/**
 * ��������� ActiveX-��������
 */
struct RMML_NO_VTABLE moIPlugin{
	// ��������
	// �������
	//
	virtual omsresult Call(char* apProcName, int aiArgC, va_list aArgsV, MLDWORD &aRes)=0;
};

/**
 * ��������� RMML-��������� ������ ����������
 */
struct RMML_NO_VTABLE mlISpeech{
	// �������
	virtual void onSaid()=0;
};

/**
 * ��������� ������ ����������
 */
struct RMML_NO_VTABLE moISpeech{
	virtual long GetCurrentPos()=0;
};

/**
 * ��������� RMML-��������������
 */
struct RMML_NO_VTABLE mlISinchronizer{
	virtual omsresult CallEvent( unsigned int aiID, MLBYTE* apData, MLWORD auDataLength)=0;
	virtual omsresult GetObjectID( const wchar_t* apwcStringID, unsigned int &auID)=0;
	virtual omsresult GetObjectStringID( unsigned int aiID, mlOutString &asStringID)=0;
	virtual void NotifyGetState(MLBYTE* apData, MLDWORD alSize)=0;
	// ��������� �� ������ ��������� � ���������� (synch-��������) ���������������� ��������
	virtual omsresult SendStateChanges(MLBYTE* apData, int aiDataLength)=0;
};

// ���������, ����������� RMML-��������, �������������� �������� ������������� ���������
// (��������, ��������-��������)
// (��� ��������� ������ RM-������)
struct RMML_NO_VTABLE mlIBinSynch{
	// ����������� � ���, ��� ��������� ���������� 
	// � ������� ���������� ������� ���������
	// (���������� false, ���� ��������� �������� ������: ������� ����� ��������)
	// (aulOffset � aulBlockSize ����� ����� ������ ��� BSSCC_REPLACE)
	// (auStateVersion - ������ ���������, � ������� ��������� ��� ���������)
	virtual bool StateChanged(unsigned int auStateVersion, BSSCCode aCode, void* apData, unsigned long aulSize, 
			unsigned long aulOffset=0, unsigned long aulBlockSize=0)=0;
	virtual bool GetBinState( BYTE*& aBinState, int& aBinStateSize)=0;
};

// ���������, ����������� RM-��������, �������������� �������� ������������� ���������
// (��������, ��������-��������)
struct RMML_NO_VTABLE moIBinSynch{
	// � ������� ������ ������ ���������: ���������� ���
	virtual bool SetFullState(unsigned int auStateVersion, const unsigned char* apState, int aiSize)=0;
	// � ������� ������ ��������� ���������: �������� ���������
	virtual bool UpdateState(unsigned int auStateVersion, BSSCCode aCode, const unsigned char* apState, int aiSize, 
		unsigned long aulOffset=0, unsigned long aulBlockSize=0)=0;
	// ����� ��������� �� ������ ������ ���������: ��������� ���
	// (���������� ������ ������, apState ����� ���� null)
	virtual unsigned long GetFullState(unsigned int& auStateVersion, const unsigned char* apState, int aiSize)=0;
	// �������� ��������� �������, �������� ��� ������
	virtual void Reset()=0;
};

/**
 * ��������� ��� Debugger-�
 */
// ��������� ��� �������� �������
//struct mldPropertyInfo{
//	int type;
//}; 
struct mldPropertiesInfo{
	short numGroup;
};
struct mldPropGroupInfo{
	short numPropeties;
	int type;
};
// ��������� ��� �������� ������ � ��������
struct mldEventsInfo{
	short numGroup;
};
struct mldEventsGroupInfo{
	short numEvents;
	int type;
};

/**
 * ������� ����� ���� RMML-���������, ���������� � �����-���������
 */
class moMedia;

#define ML_MO_IS_NULL(PMO) ((((unsigned)PMO)&~3)==0)

class RMML_NO_VTABLE mlMedia{
//friend class moMedia;
public:
	unsigned short mType; // ��� �����-������� (�����������, �������, ��������, ��������, ����, ...)
                          // ���� ��� ����� �������� � ����������� �� ���� ������������ �������
//	shared_ptr<moMedia> mMOMedia; // ��������� �� �����-������
protected:
	moMedia* mpMO; // ��������� �� �����-������
public:
//	void SetMO(shared_ptr<moMedia> aMOMedia){ mMOMedia=aMOMedia; }
	mlMedia(){ mpMO=NULL; }
	void SetMO(moMedia* apMO){ mpMO = apMO; }
	moMedia* GetMO(){ return mpMO; }
	moMedia* GetSafeMO(){ if (ML_MO_IS_NULL(mpMO)) return NULL; return mpMO; }
	virtual mlMedia* GetParentElement()=0;
	virtual mlMedia* GetScene()=0;
	virtual bool IsAllParentComposLoaded()=0;
	virtual bool GetFrozen()=0;
	virtual int EnumPlugins(std::vector<mlMedia*>& avPlugRefs)=0;
	virtual unsigned int GetSynchID(unsigned int &auiBornReality, bool abGetIfUndefined = true)=0;
	virtual mlMedia** GetChildren()=0;
	// ���������� ��� ������� ���������� ���������� (����������� ����� FreeChildrenArray ��)
	virtual mlMedia** GetAllVisibleChildren()=0;
	virtual void FreeChildrenArray(mlMedia** apaChildren)=0;
	// �������� ���� �����-�������
	bool IsLoadable(){ return (mType & MLMPT_LOADABLE)?true:false; }
	bool IsVisible(){ return (mType & MLMPT_VISIBLE)?true:false; }
	bool IsAudible(){ return (mType & MLMPT_AUDIBLE)?true:false; }
	bool IsContinuous(){ return (mType & MLMPT_CONTINUOUS)?true:false; }
	bool IsButton(){ return (mType & MLMPT_BUTTON)?true:false; }
	// 
	void SetType(int aiType){ mType=(unsigned short)aiType; }
		// ��������� ���� �����-�������
		// (�������� ������� �����-������ ��� �������� ����������� �����; 
		//  ����� �� �������� SetType(MLMT_VIDEO))
	int GetType(){ return mType; }
	// ������������ ������ � ��������� 
	virtual int GetPropType(const char*)=0;
	virtual int GetIntProp(const char*)=0;
	virtual bool GetBooleanProp(const char*)=0;
	virtual double GetDoubleProp(const char*)=0;
	virtual wchar_t* GetStringProp(const char*)=0;
	virtual mlMedia* GetRefProp(const char*)=0;
	virtual void SetProp(const char*, int)=0;
	virtual void SetProp(const char*, double)=0;
	virtual void SetProp(const char*, const char*)=0;
	virtual void SetProp(const char*, const wchar_t*)=0;
	// ������ � ����������� ����������
	virtual mlILoadable* GetILoadable(){ return NULL; }
	virtual mlIVisible* GetIVisible(){ return NULL; }
	virtual mlI3DGroup* GetI3DGroup(){ return NULL; }
	virtual mlIAudible* GetIAudible(){ return NULL; }
	virtual mlIContinuous* GetIContinuous(){ return NULL; }
	virtual mlIButton* GetIButton(){ return NULL; }
	virtual mlI3DObject* GetI3DObject(){ return NULL; }
	// ������ � ����������� ��������� �����
	virtual mlIImage* GetIImage() { return NULL; }
	virtual mlIText* GetIText(){ return NULL; }
	virtual mlIInput* GetIInput(){ return NULL; }
	virtual mlICamera* GetICamera(){ return NULL; }
	virtual mlIViewport* GetIViewport(){ return NULL; }
	virtual mlICharacter* GetICharacter(){ return NULL; }
	virtual mlICloud* GetICloud(){ return NULL; }
	virtual mlIMotion* GetIMotion(){ return NULL; }
	virtual mlIActiveX* GetIActiveX(){ return NULL; }
	virtual mlIBrowser* GetIBrowser(){ return NULL; }
	virtual mlIFlash* GetIFlash(){ return NULL; }
	virtual mlIPlugin* GetIPlugin(){ return NULL; }
	virtual mlISpeech* GetISpeech(){ return NULL; }
	virtual mlIIdle* GetIIdle(){ return NULL; }
	virtual mlIIdles* GetIIdles(){ return NULL; }
	virtual mlIVideo* GetIVideo(){ return NULL; }
	virtual mlIShadows* GetIShadows(){ return NULL; }
	virtual mlIShadow* GetIShadow(){ return NULL; }
	virtual mlIMap* GetIMap(){ return NULL; }
	virtual mlIMaterial* GetIMaterial(){ return NULL; }
	virtual mlIHint* GetIHint(){ return NULL; }
	virtual mlIScene3D* GetIScene3D(){ return NULL; }
	virtual mlIImageFrame* GetIImageFrame(){ return NULL; }
	virtual mlILine* GetILine(){ return NULL; }
	// ������ � �������������� �����������
	virtual mlIBinSynch* GetIBinSynch(){ return NULL; }
	// ������ � ����������� ����������� ������ �����
	virtual void* GetInterface(long /*iid*/){ return NULL; }
	//// ��������� �������������
	// ����������������� ��������� ������� �� ���� ��������
	virtual bool synchronize(mlSynchData* apData=NULL)=0;
	// ������ ������������ �� ����� ���� � ������ (���������� MapManager-��)
	virtual void ZoneChanged(int aiZone)=0;
	// ������ ������� � ������ ���������� (���������� MapManager-��)
	virtual void RealityChanged(unsigned int auReality)=0;
	// ���������� ���������� ������������� �������
	// (������������ ��� ������ ��� ����������� ��������-������� 3D-��������)
	virtual void VisualRepresentationChanged()=0;
	// ������ �������� ��������������������
	virtual bool IsSynchronized() = 0;
	// ������������� ������� ���������, ������ ����� � ������
	virtual bool IsSynchInitialized() = 0;
	// 
	virtual void ReadyForBinState()=0;
	// 
	virtual void SetLocationSID(const wchar_t* pszLocationID)=0;
	virtual void SetCommunicationAreaSID(const wchar_t* pszCommunicationAreaID)=0;	
	//
	virtual bool IsForcedUpdateBinState()=0;
};

/**
 * ������� ����� ���� �����-��������, �������������� RMML
 */
class moMedia{
protected:
	mlMedia* mpMLMedia; // ��������� �� RMML-�������
public:
	unsigned short mType; // ��� �����-�������. ���������� �� mpMLMedia->mType ������ ����� �����-������ ������������ �� RMML-������� ��� ������������ ��������
	moMedia(){ mpMLMedia = NULL; mType = 0; }
	moMedia(mlMedia* apMLMedia){ SetMLMedia(apMLMedia); mType = 0; }
	virtual ~moMedia(){}
public:
	int GetType(){ return mType; }
	__forceinline void SetMLMedia(mlMedia* apMLMedia){ mpMLMedia=apMLMedia; }
	__forceinline mlMedia* GetMLMedia(){ return mpMLMedia; }
	virtual moILoadable* GetILoadable(){ return NULL; }
	virtual moIVisible* GetIVisible(){ return NULL; }
	virtual moIAudible* GetIAudible(){ return NULL; }
	virtual moIContinuous* GetIContinuous(){ return NULL; }
	virtual moIButton* GetIButton(){ return NULL; }
	virtual moI3DObject* GetI3DObject(){ return NULL; }
	//
	virtual moICamera* GetICamera(){ return NULL; }
	virtual moIViewport* GetIViewport(){ return NULL; }
	virtual moIText* GetIText(){ return NULL; }
	virtual moIInput* GetIInput(){ return NULL; }
	virtual moICharacter* GetICharacter(){ return NULL; }
	virtual moIMotion* GetIMotion(){ return NULL; }
	virtual moIActiveX* GetIActiveX(){ return NULL; }
	virtual moIBrowser* GetIBrowser(){ return NULL; }
	virtual moIFlash* GetIFlash(){ return NULL; }
	virtual moIPlugin* GetIPlugin(){ return NULL; }
	virtual moISpeech* GetISpeech(){ return NULL; }
	virtual moIVideo* GetIVideo(){ return NULL; }
	virtual moIImage* GetIImage(){ return NULL; }
	virtual moIPath3D* GetIPath3D(){ return NULL; }
	virtual moI3DGroup* GetIGroup3D(){ return NULL; }
	virtual moIScene3D* GetIScene3D(){ return NULL; }
	virtual moICloud* GetICloud(){ return NULL; }
	virtual moIBinSynch* GetIBinSynch(){ return NULL; }
	virtual moIImageFrame* GetIImageFrame(){ return NULL; }
	virtual moILine* GetILine(){ return NULL; }
	virtual moIParticles* GetIParticles(){ return NULL; }
	virtual void* GetInterface(long /*iid*/){ return NULL; }
	// ���� ���������� false, ������ ����� ������ �� ������ � ����� ��� � ���������� �� ��������
	virtual bool Update(const __int64 /*alTime*/){ return false; }
	virtual void GetSynchData(mlSynchData& /*aSynchData*/){ return; }
	virtual void SetSynchData(mlSynchData& /*aSynchData*/){ return; }
	virtual void FrozenChanged()= 0;
	virtual void PropIsSet(){ return; }
	virtual void AllParentComposLoaded(){ return; }
	virtual void ParentChanged(){ return; }
	//
};


enum mlEModes{
	smmInitilization,
	smmLoading,
	smmNormal,
	smmPlay,
	smmPause,
	smmAuthoring,
	smmDisconnection
};

enum mlETestModes{
	smNoTest,
	smNormalAutoTests,
	smAutoClassTest
};

typedef void (*mlCallbackFunc)(void* apObj, void* apParams);

struct mlCallback{
	mlCallbackFunc mFunc;
	void* mpParams;		// �������� ��� mCallbackFunc
};

// �������� ��� muID, ������� ����������, ��� ������� �� �������� �������� ����
#define OBJ_ID_UNDEF 0xFFFFFFFF
// �������� ��� muID, ������� ����������, ��� ������� �������� �������
#define OBJ_ID_CLASS 0xFFFFFFFE
// �������� ��� muID, ������� ������ ��� ����� ������� ����������, ��� ������� �������� ��������
#define OBJ_ID_AVATAR 4000000000
// �������� ��� muID, ������� ������ ��� ����� ������� ����������, ��� ������� �������� ������������� (������������) ������������� ��������
#define OBJ_ID_EDITABLE 1000000000

#define REALITY_ID_UNDEF 0xFFFFFFFF

struct mlObjectInfo4Creation
{
	// ��� ��� ������?
	bool isMyAvatar;
	// ������������� �������
	unsigned int muObjectID;			
	// ������������� ����������� ��������
	unsigned int muBornRealityID;		
	// ������������� ����������� ���������������
	unsigned int muRealityID;
	// RMML-��� ������� (������ MLMT_OBJECT ��� MLMT_GROUP)
	unsigned short musType;	
	// ��� �������
	const char* mszName;	
	// ��� ������ �������
	const char* mszClassName;	
	// ��� ����� ������ ������� (��� ��������)
	const char* mszClassFileName;	
	// ���� � ��������
	const wchar_t* mwcSrc;
	// ������������ 3D-����� ��� 3D-������
	const mlMedia* mpParent;
	// ������ ������������� (userProps)
	const wchar_t* mwcParams;
	mlCallback mCallback;
	// ��� callback-�
	void* mpMapManager;
	// ��� callback-�
	const ZONE_OBJECT_INFO* mpZoneObjectInfo;
	//����� ��������
	long long creatingTime;
	// ������� �������
	unsigned int muLevel;
	// ������� ������� (�� ��)
	ml3DPosition mBounds;
	// ������� ������� (�������� ���� ���������� CreateAvatar, ���� � ������� �������������)
	ml3DPosition mPos;
	// ������ ��������� ����������������� �������
	syncObjectState* objectFullState;
	// ID �������, � �������� ���� ������� onCreate ��� onError ����� �������� �������
	int miCreationHandlerID;
	// ����� �� ������� ��� � ������� �����������
	bool isHighPriority;

	mlObjectInfo4Creation()
	{
		muObjectID = OBJ_ID_UNDEF;
		muBornRealityID = REALITY_ID_UNDEF;
		muRealityID = REALITY_ID_UNDEF;
		musType = 0;
		mszName = NULL;
		mszClassName = NULL;
		mszClassFileName = NULL;
		mwcSrc = NULL;
		mpParent = NULL;
		mwcParams = NULL;
		mCallback.mFunc = NULL;
		mCallback.mpParams = NULL;
		mpMapManager = NULL;
		mpZoneObjectInfo = NULL;
		muLevel = 0;
		creatingTime = 0;
		ML_INIT_3DPOSITION(mBounds);
		ML_INIT_3DPOSITION(mPos);
		objectFullState = NULL;
		miCreationHandlerID = 0;
		isHighPriority = false;
	}
};

struct mlObjectInfo4OnServerCreation: public mlObjectInfo4Creation
{
	ml3DPosition mPos;	// ������� �������
	ml3DRotation mRot;	// ������� �������
	ml3DScale mScl;		// ������� �������
	ml3DScale mBounds;	// ������� ������� �� ����
	int miLevel;		// "�������" �������
	const char* mszLocation;	// ������������� �������
	mlObjectInfo4OnServerCreation(){
		ML_INIT_3DPOSITION(mPos);
		ML_INIT_3DROTATION(mRot);
		ML_INIT_3DSCALE(mScl);
		mScl.x = mScl.y = mScl.z = 1.0;
		ML_INIT_3DSCALE(mBounds);
		miLevel = 0;
		mszLocation = "";
	}
};

/*
this.LOGIN_SERVER_STATUS_BIT = 1;
this.RES_SERVER_STATUS_BIT = 2;
this.LOG_SERVER_STATUS_BIT = 4;
this.MAP_SERVER_STATUS_BIT = 8;
this.SYNC_SERVER_STATUS_BIT = 16;
this.SERVICE_SERVER_STATUS_BIT = 32;
#define	VOIP_RECORD_SERVER_STATUS_BIT	0x40
#define	VOIP_PLAY_SERVER_STATUS_BIT		0x80
*/
enum mlServerMask{
	ML_SERVER_MASK_WORLD_SERVICE = (1 << 0),	// ������ �����������
	ML_SERVER_MASK_RES = (1 << 1),		// ��������� ������
	ML_SERVER_MASK_LOG = (1 << 2),		// (�� ������������)
	ML_SERVER_MASK_MAP = (1 << 3),		// ������ �����
	ML_SERVER_MASK_SYNC = (1 << 4),		// ������ �������������
	ML_SERVER_MASK_SERVICE = (1 << 5),	// ������ ��������
	ML_SERVER_MASK_VOIP_RECORD = (1 << 6),	// ������ ������ ����
	ML_SERVER_MASK_VOIP_PLAY = (1 << 7),	// ������ ��������������� ����
	ML_SERVER_MASK_SHARING = (1 << 8),		//������ �������� �����
};

struct RMML_NO_VTABLE mlObjectInfo{
	unsigned int id;
	mlString name;
	mlString src;
	mlString parent;
};

/**
 * ���������, ������������ ��� �������� ��������� ����
 */
struct RMML_NO_VTABLE mlISceneManager{
	// ��������� ���������� ����������������� ����������
	virtual omsresult LoadUI(const char* asSrc)=0;
	virtual omsresult LoadUI(const wchar_t* asSrc)=0;
	// ��������� ������
	virtual omsresult LoadModuleMeta()=0;
	// ��������� ������ � �����
	virtual omsresult LoadModuleMetaAndLoadScene( unsigned int aID, wchar_t *asModuleID, wchar_t* asScene)=0;
	// ��������� ����� ������ �� ������
	virtual omsresult LoadScene(int aiNum)=0;
	// ��������� �����
	virtual omsresult LoadScene(const char* asSrc)=0;
	virtual omsresult LoadScene(const wchar_t* asSrc)=0;
	// �������� �����
	virtual omsresult Update(const __int64 alTime, int &aiDeltaTime, const bool isNormalMode)=0;
	// ������ ��������� �� ���������� ����������������� ����������
	virtual mlMedia* GetUI()=0;
	// ������ ��������� �� �������� �����
	virtual mlMedia* GetActiveScene()=0;
	// ��������� JavaScript
	virtual omsresult ExecScript(const wchar_t* apwcScript, int aiLength=-1, wchar_t* *appwcRetVal=NULL)=0;
	// ���������� ����� ������ ��������� ����
	virtual omsresult SetMode(mlEModes aeMode)=0;
	// �������� ����� ������ ��������� ����
	virtual mlEModes GetMode()=0;
	// ���������� �������� ����� ������ ��������� ����
	virtual void SetTestMode(mlETestModes aeTesmMode)=0;
	// ���������� ���������� �������� ��������
	virtual void SetTestResDir(const wchar_t* apwcTestResDir)=0;	
	// ����� ��������� �����
	virtual omsresult GetState(void* &apData, unsigned long &aSize)=0;
	// ������������ ��������� �����
	virtual omsresult SetState(void* aData, unsigned long aDataSize)=0;
	// ������ ��������� �� ���������� ���������
	virtual mlMedia* GetAuthScene()=0;
	// ������ ��������� �� ������� �� ��� ������� �����
	virtual mlMedia* GetObjectByTarget(const wchar_t* apwcTarget)=0;
	//������ RealityID �������a �� ��� ������� �����
	virtual unsigned int GetObjectRealityIDByTarget(const wchar_t* apwcTarget) = 0;
	// ������ ��������� �� ����� ����
	virtual mlMedia* GetWorldScene() const=0;

	//// ��������� ������� ������ �������������
	// 
	// ������ ������ �������
	virtual void LocationChanged(const wchar_t* apwcLocationName,  unsigned int auiObjectID, unsigned int auiBornRealityID)=0;
	virtual void CommunicationAreaChanged(const wchar_t* apwcCommunicationAreaName,  unsigned int auiObjectID, unsigned int auiBornRealityID)=0;
	// � ������� ����������� ���������� ���������� xy
	// ������� ����� ������� (���������)
	virtual mlMedia* CreateAvatar(mlObjectInfo4Creation &aAvatarInfo)=0;
	// ������� ������ (���������)
	virtual mlMedia* CreateObject(mlObjectInfo4Creation &aObjectInfo)=0;
	// ������, ���������� � �������, ���� ��� ������ �� ������
	virtual omsresult HandleObjectNotFound( unsigned int aObjectID, unsigned int aBornReality, unsigned int aRealityID)=0;
	// ��������������� ������������� ����� ������ ����� � ��������
	virtual omsresult RestoreSynchronization( const wchar_t* apwcRequestedUrl) = 0;
	// ���������� ������������� ������� � ����
	virtual bool IsObjectExist( unsigned int auObjectID, unsigned int auBornRealityID)=0;
	// ������� ������
	virtual omsresult DeleteObject( unsigned int auObjectID, unsigned int auBornRealityID)=0;
		// appaObjects - ��������� �� ����������, � ������� ���� �������� ������ �� ������ �����-�������� ��� ������������ ��������
	virtual omsresult DeleteObject( mlMedia* apObject, moMedia** *appaObjects = NULL)=0;

	virtual omsresult DeletePreviousRealityObjects(const std::vector<unsigned int>& aExludeOnjects)=0;
	// ������������ ��������� �� ������� �������
	virtual omsresult HandleSyncMessage(unsigned int auObjectID, unsigned int auBornRealityID, const wchar_t* apwcSender, unsigned char* apData, unsigned short aiDataSize)=0;
	// ���������� ������ ��������
	virtual int ExtractIncludes( const wchar_t* apcFileSrc, const wchar_t** appIncludes, int count)=0;

	/*
	// Vladimir 2011/04/14
	*/
	// ���������� ����� ��� ��������������� �����
	virtual void SetLogin(const wchar_t* apwcLogin) = 0;
	// ���������� ������ ��� ��������������� �����
	virtual void SetPassword(const wchar_t* apwcPassword) = 0;
	// ���������� ��� ������: �������������� - 1 ��� �� �������������� - 0 - ��� ��������������� �����
	virtual void SetPasswordEncoding(int iPasswordEncodingMode) = 0;
	// ���������� ���� ��� ��������������� �����
	virtual void SetLoginKey(const wchar_t* apwcLoginKey) = 0;
	// ���������� ���� ��� ��������������� ����� ����� ���������� ����� loginKey
	virtual void SetUpdaterKey(const wchar_t* apwcUpdaterKey) = 0;
	// ���������� ���� ��� ��������������� ����� ����� ���������� ����� loginKey, ������� ��� ������ �� ������
	virtual void SetUpdaterKeyByThread(const wchar_t* apwcUpdaterKey) = 0;
	/*
	// end
	*/

	virtual void OnWindowMoved() = 0;
	virtual void OnWindowSizeChanged( const int& auClientWidth, const int& auClientHeight) = 0;
	virtual void OnKinectFound() = 0;
	virtual void OnOculusRiftFound() = 0;
	virtual bool OnControllerFound() = 0;
	virtual void OnControllerLost() = 0;
	
	// ���������� ������� �� URL
	virtual void SetLocationUrl(const wchar_t* apwcURL) = 0;
	virtual void SetLocationUrlAndTeleport(const wchar_t* apwcURL) = 0;
	virtual void SetLocationUrl(const wchar_t* apwcLocation, const wchar_t* apwcPlace
		, float x, float y, float z, unsigned long zoneID
		, const wchar_t* apwcAction, const wchar_t* apwcActionParam)=0;
	virtual const wchar_t* GetLocationURL()=0;

	//virtual void ChangeObjectBornReality( mlMedia* apMedia, unsigned int auBornRealityID) = 0;
	virtual void NotifyMapOnMyAvatarCreatedListeners() = 0;

	virtual const wchar_t* GetClientStringID() = 0;

//#ifndef CSCL
//	virtual void add_updatable (shared_ptr<Updatable> smp_new_updbl) = 0;
//#endif
	// ���������� �� ����������� ��� ������ rmml-��������
	virtual bool IsJSObjClassDefined(const wchar_t* apwcClassName)=0;
	// ������� ���� � ������� �� �������� ������� � trace
	// abLost - �������� ���� ������ �� ���������� RMML-��������� (parent==null)
	// aiType - 0: ���, 1: 3D-�������, 2: 3D-���������, 3: 3D-������, 4: ����������
	// apwcObjName - ��� �������
	virtual bool TraceObjectsRefs(bool abLost=true, int aiType=0, const wchar_t* apwcObjName=NULL)=0;
	// ������� � tracer ���� �� �������, ������� ��� ������
	virtual void TraceObjectUnderMouse()=0;
	// ������ ������� 2D-RMML-�������, ������� ��������� ��� ������ (��� 3D-��������, ��������������, viewport)
	virtual mlMedia* GetVisibleUnderNouse()=0;
	// ����������� (��� �������������) ����� � ����� �� ��������
	virtual void OnServerConnected(int aServerMask, bool abRestored)=0; // mlServerMask
	// ��������� (��� ��������������) ����� � ����� �� �������� ����������� ��������
	virtual void OnServerConnectionError(int aServerMask, int aiError)=0; // mlServerMask
	// �������� ����� � ����� �� ��������
	virtual void OnServerDisconnected(int aServerMask, int aiCause)=0; // mlServerMask

	virtual void SetSyncCallbacks();

	virtual void SetClientStringID(const wchar_t* apwcStringID)=0;
	virtual void SetServiceCallbacks() = 0;
	// ������� � ����� ������������ ��������� ��� 
	// ��� ������ RMML-�������� � ����� � ����������� �� ������ (aiMode)
	//  aiMode: 0 - ��� ������
	virtual void OutRMMLObjectTree(std::ostream* aOStream, int aiMode)=0;

	// ������� ��������� ���������� ������� ��� ������ � ������� ��������
	virtual void CopyObjectUnderMouseToGallery()=0;

	// �������� ��������� ��� 
	// ��� ������ RMML-�������� � ����� � ����������� �� ������ (aiMode)
	//  aiMode: 0 - ��� ������
	virtual void GetRMMLObjectTree( std::vector<mlObjectInfo> &avElements, int aiMode) = 0;

	// �������� �������� ������������������� �������
	virtual std::wstring GetJsonFormatedProps(const wchar_t* apwcTarget, int aIndent = 0) = 0;
	//virtual bool HasBinaryState(const wchar_t* apwcTarget) = 0;
	// �������� ID �������� ������
	virtual unsigned int GetCurSeanceID() = 0;
	// �������� ��������� �������� ������
	virtual bool isCurSeancePlaying() = 0;
	// ���������� ��������� �������� ������
	virtual void SetSeancePlaying(bool playing) = 0;
	// ���������� ����� ��������
	virtual void OnAppActivated() = 0;
	// ���������� ��������� ���� ��������
	virtual void OnAppDeactivated() = 0;
	// ���������� �������� �������
	// ���� ������ false, �� ���������� �� ���� ���������.
	virtual bool OnClose() = 0;

	// ������� � tracer � ��� ������� 2D-������� ��� ������ � ��� ���������
	virtual void TraceUnderMouseObjectDepth() = 0;

	//virtual void SetSynchObjectCount(int count) = 0;	

	// ���������� ������������
	virtual void OnPreciseURL(const wchar_t* aURL) = 0;

	virtual void OnSeancesInfo( const wchar_t* aSeancesInfo, bool bFull) = 0;

	//  ������ ����������� ������ �������
	virtual void OnProxyAuthError( const wchar_t* aSettingName, int aProxyType, const wchar_t* aProxyServer) = 0;

	//  ������ ���������� � ��������
	virtual void OnConnectToServerError(int errorCode) = 0;

	// ����������, ��� � ������� ������ �������� ���������������� ��������
	virtual void OnMapObjectsLoaded() = 0;

	// ��������� ������� � ������ ��� �������������
	virtual void ShowZoneEvent( bool abVal) = 0;

	// ��� ���������� � �������� ����
	virtual void SetInfoState( int iInfoState) = 0;

	// ��� ������ ��� ������ �����������
	virtual void OnLogout( int iCode) = 0;

	// ����������, ����� �� ���� ������ ���������� ����
	virtual void OnWindowFileDrop(const wchar_t* apwcFilePath, int x, int y) = 0;

	// ����������, ����� �� ������������ �� ��������� ������ 5 ����� ������� ���� � ���������� ��� �� ������� ����  (������ �����)
	virtual void OnUserSleep() = 0;

	virtual void OnVideoCaptureError(const unsigned int errorCode) = 0;

	virtual void OnCodeEditorClose(const std::wstring& text) = 0;

	// ����������, ����� ������������ ������ � ������ ����� � ��������
	virtual void OnUserWakeUp() = 0;

	virtual void OnNotifyServerTOInfo( int minutes) = 0;

	// ������������� �������� ��������� �������. ���������� ������ �� Ctrl+I
	virtual void TransposeBinState(const wchar_t* apwcTarget) = 0;

	// ������� ������� ������
	// ���� abAsynch == true, �� �� ����������� � ������������ ������
	virtual void CallJS_GC(bool abAsynch = true, bool abForceCall = false) = 0;

	virtual void OnEventsInfoIntoLocation( const wchar_t* aSeancesInfo) = 0;

	virtual void OnIPadSessionChangeSlide( int changeSlide) = 0;

	virtual void OnVisibilityDistanceKoefChanged(int koef) = 0;
	
	// ��������� ���������� �������� Cut ��������� �������
	virtual void OnCutOperationComplete( int aErrorCode, unsigned int aRecordDuration) = 0;
	// ��������� ���������� �������� Copy ��������� �������
	virtual void OnCopyOperationComplete( int aErrorCode) = 0;
	// ��������� ���������� �������� Paste ��������� �������
	virtual void OnPasteOperationComplete( int aErrorCode, unsigned int aRecordDuration) = 0;
	// ��������� ���������� �������� Delete ��������� �������
	virtual void OnDeleteOperationComplete( int aErrorCode, unsigned int aRecordDuration) = 0;
	// ��������� ���������� �������� Save ��������� �������
	virtual void OnSaveOperationComplete( int aErrorCode) = 0;
	// ��������� ���������� �������� SaveAs ��������� �������
	virtual void OnSaveAsOperationComplete( int aErrorCode) = 0;
	// ��������� ���������� �������� Play ��������� �������
	virtual void OnPlayOperationComplete( int aErrorCode) = 0;
	// ��������� ���������� �������� Pause ��������� �������
	virtual void OnPauseOperationComplete( int aErrorCode) = 0;
	// ��������� ���������� �������� Rewind ��������� �������
	virtual void OnRewindOperationComplete( int aErrorCode) = 0;
	// ��������� ���������� �������� Seek ��������� �������
	virtual void OnSeekOperationComplete( int aErrorCode) = 0;
	// ��������� ���������� �������� Undo ��������� �������
	virtual void OnUndoOperationComplete( int aErrorCode, unsigned int aRecordDuration) = 0;
	// ��������� ���������� �������� Redo ��������� �������
	virtual void OnRedoOperationComplete( int aErrorCode, unsigned int aRecordDuration) = 0;
	// ��������� ���������� �������� Open ��������� �������
	virtual void OnOpenOperationComplete( int aErrorCode, std::wstring aRecordName, std::wstring aRecordDesciption, unsigned int aRecordDuration, std::wstring aAuthor, std::wstring aCreationDate, std::wstring aLocation, std::wstring aLanguage, std::wstring aURL) = 0;
	// ���������� ������ 
	virtual void SetVersionInfo(wchar_t* version) = 0;
};

RMML_DECL omsresult CreateSceneManager(omsContext* apmcx);
RMML_DECL void DestroySceneManager(omsContext* amcx);

inline void AppendPointer(std::string& aStr, void* aPtr)
{
	char strPtr[ 10];
	aStr += "0x";
	_itoa_s((int)aPtr, strPtr, 10, 16);
	aStr += strPtr;
}

}

#endif