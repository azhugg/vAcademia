#pragma once

namespace rmml {

struct mlMask{
	enum EMaskType{
		MASKT_UNDEF,
		MASKT_ELEM, // ����� ������� ���������
		MASKT_RECT  // ����� ������������� ��������
	} meType;
	mlRMMLElement* mpElem;
	int miX;
	int miY;
	int miW;
	int miH;
	bool mbLocal;	// ���� =true, ����� ������ ������������ ��������� ������� � ����� ������������ ������ � ���
	mlMask(){
		mpElem = NULL;
		miX = miY = miW = miH = 0;
		mbLocal = false;
	}
};

#define EVNM_onMouseDown L"onMouseDown"
#define EVNM_onRMouseDown L"onRMouseDown"
#define EVNM_onMouseMove L"onMouseMove"
#define EVNM_onRMouseUp L"onRMouseUp"
#define EVNM_onMouseUp L"onMouseUp"
#define EVNM_onMouseWheel L"onMouseWheel"

/**
 * RMML-�������� ��� ������� RMML-�������
 * (Composition, Image, Animation, ...)
 * (�� ������ � ����������� JavaScript!)
 */
class mlRMMLVisible: 
					//public mlJSClassProto,
					public mlIVisible,
					public moIVisible
{
friend class mlRMMLPButton;
friend class mlRMMLElement;
friend class mlRMMLViewport;
friend class mlRMMLContinuous;
friend class mlRMMLComposition;
friend class mlRMMLText;
friend class mlRMMLLine;
friend class mlSceneManager;
public:
	mlRMMLVisible(void);
	~mlRMMLVisible(void);
MLJSPROTO_DECL
private:
	enum {JSPROP0_mlRMMLVisible=TIDB_mlRMMLVisible-1,
		JSPROP__x,
		JSPROP__y,
		JSPROP__width,
		JSPROP__height,
		JSPROP__depth,
		JSPROP__visible,
		JSPROP__xmouse,
		JSPROP__ymouse,
		JSPROP_opacity,
		TIDE_mlRMMLVisible
	};
public:
	enum { EVID_mlRMMLVisible=TIDE_mlRMMLVisible-1,
		EVID_onMouseDown,
		EVID_onRMouseDown,
		EVID_onMouseMove,
		EVID_onRMouseUp,
		EVID_onMouseUp,
		EVID_onMouseWheel,
		TEVIDE_mlRMMLVisible
	};
protected:
	mlPoint mXY;
	mlSize mSize;
	int miDepth;
	bool mbVisible;
	unsigned short musOpacity;

	JSFUNC_DECL(getMediaSize)
	JSFUNC_DECL(crop)
	JSFUNC_DECL(setMask)
	JSFUNC_DECL(show)
	JSFUNC_DECL(hide)
	JSFUNC_DECL(localToGlobal)
	JSFUNC_DECL(globalToLocal)
	JSFUNC_DECL(getAbsVisible)

	mlMask* mpMask;

protected:
	struct mlVisibleNotifications{
		// ���������� ������� ������������� ������� 
		// (���� �������� ���������, �� ���� ����� ������������ ��������)
		bool mbVisualRepresentationChanged;
		// 
		mlMedia* mpThis;
		// ������ ���������� 
		typedef std::set<mlIVisibleNotifyListener*> mlVisibleNotifyListeners;
		mlVisibleNotifyListeners mVNListeners;
	public:
		mlVisibleNotifications(mlMedia* apThis);
		~mlVisibleNotifications();
		// ���������� ���������� � ���, ��� ���������� ���������� ������������� �������
		// (���� ������ ����������� �������� ������ ��������� 3D-��������, �� ������� �������� 2D-������� � �������� �������)
		void NotifyVisualRepresentationChanged();
	};
	// ���-�� ���������� � ���������� ������������� �� ���� Update-�
	//#define MLVRFL_THIS_UPDATE 1
	// ���-�� ���������� � ���������� ������������� �� ������� Update-�
	//#define MLVRFL_LAST_UPDATE 2
	// ������ ��� ���������� ����������� ���������� ������� ���������
	mlVisibleNotifications* mpVisibleNotifications;

	virtual mlRMMLElement* GetElem_mlRMMLVisible()=0;
	virtual mlRMMLElement* GetParent_mlRMMLVisible()=0;
	virtual void InvalidBounds();
	// ������������ ����������� �� mlRMMLElement � ���, ��� parent ��� �� null, � ���� null
	//void ParentNulled();
	virtual void SetMask(const mlMask* apMask = NULL);		// �������� �����
	void ClearMask(bool abDestroying = false);	// ������� �����
public:
	virtual mlRect GetBounds();
	mlRect GetAbsBounds();
	void SetAbsXY(const mlPoint apntXY);
	void SetAbsBounds(const mlRect arcBounds);
	mlPoint GetMouseXY();
	bool Visible_IsPointInBoundingBox(const mlPoint aPnt);
	bool Visible_IsPointIn(const mlPoint aPnt);
	void Visible_NotifyParentChanged();
	virtual mlRMMLElement* GetVisibleAt(const mlPoint aPnt, mlIVisible** appVisible=NULL){
		if(Visible_IsPointIn(aPnt))
			return GetElem_mlRMMLVisible();
		return NULL;
	}
	void LocalToGlobal(mlPoint &aPnt);
	void GlobalToLocal(mlPoint &aPnt);

	bool CanHandleEvent(char aidEvent){
		if(aidEvent < TEVIDE_mlRMMLVisible && aidEvent > EVID_mlRMMLVisible){
			if(!GetAbsVisible()) return false;
			Vis_SetVisibleEventData(aidEvent);
		}
		return true;
	}

	void Vis_SetVisibleEventData(char aidEvent);
	void Vis_VisualRepresentationChanged();
	// ���������� �� ������ Update-� ��������
	mlresult UpdateVisualRepresentationChanged();
	// ������������� ���������� Show() � Hide() ��� ���������� ���� rmml-������
	// (������ ������������ ������ � mlRMMLViewport::GetButtonUnderMouse)
	void ShowInternal(){ mbVisible = true; }
	void HideInternal(){ mbVisible = false; }

	// ���������� ����� ��� ��� ���� ������� onPress � ��������� �������� viewport-� ��� �������� 3D-�������
	// ���������� true, ���� onPress � ��������� �������� �� ���� ��������
	virtual bool CallOnBeforePress(mlButtonUnderMouse &aBUM){ return false; } // mlRMMLElement* ap3DObject=NULL, ml3DPosition* apXYZ=NULL, mlRMMLElement* apTextureObject=NULL, mlPoint* appntTextureXY=NULL){ return false; }

	// ��������� �������������
	static void Vis_GetData4Synch(JSContext* cx, char aidEvent, mlSynchData &avSynchData);
	static void Vis_SetData4Synch(JSContext* cx, char aidEvent, mlSynchData &avSynchData);
//	static void Vis_SetData4Synch(char aidEvent, unsigned char* apSynchData, int &aiIdx);
	int GetDepth(){ return miDepth; }

public:
	//mlresult GetPropsFrom(mlRMMLVisible* apElSrc){
	//	mXY=apElSrc->mXY;
	//	mSize=apElSrc->mSize;
	//	miDepth=apElSrc->miDepth;
	//	mbVisible=apElSrc->mbVisible;
	//	return ML_OK;
	//}

// ���������� mlIVisible
public:
	// ��������
	mlPoint GetAbsXY();
	void SetSize(mlSize asz){ mSize=asz; SizeChanged();}
	mlSize GetSize();
	int GetAbsDepth();
	bool GetAbsVisible();
	float GetAbsOpacity();
	// �������
	//
	void Show();
	void Hide();
	//
	//bool IsVisualRepresentationChanged(){ return mucVisualRepresentationChanged != 0; }
	void SetVListener(mlIVisibleNotifyListener* apListener);
	void RemoveVListener(mlIVisibleNotifyListener* apListener);
	mlMedia* GetMLMedia(){ return GetElem_mlRMMLVisible(); }

// ���������� moIVisible
protected:
	moIVisible* GetmoIVisible();
	// ��������
	void AbsXYChanged(); // ���������� ���������� ��������� 
	void SizeChanged();
	void AbsDepthChanged(); // ���������� ���������� �������
	void AbsVisibleChanged(); // ���������� ���������� ���������
	void AbsOpacityChanged(); // ���������� ���������� ��������������
	// ������
	mlSize GetMediaSize();
	void Crop(short awX, short awY, MLWORD awWidth, MLWORD awHeight); 
	mlRect GetCropRect();
	void SetMask(mlMedia* aMask, int aiX=0, int aiY=0);
	void SetMask(int aiX, int aiY, int aiW, int aiH);
	virtual int IsPointIn(const mlPoint aPnt); // -1 - not handled, 0 - point is out, 1 - point is in
};

#define MLIVISIBLE_IMPL \
public: \
	mlRMMLElement* GetElem_mlRMMLVisible(){ return this; } \
	mlRMMLElement* GetParent_mlRMMLVisible(){ return mpParent; } \
	mlIVisible* GetIVisible(){ return this; } \
	mlRMMLVisible* GetVisible(){ return this; } \
	virtual void ParentNulled(){ InvalidBounds(); AbsVisibleChanged(); }

}
