#if _MSC_VER > 1000
#pragma once
#endif

#ifndef _mlRMMLCloud_H_
#define _mlRMMLCloud_H_

namespace rmml {

/**
 * ����� 2D-'��������' ��� 3D ���� 
 */

/*
	content 		ref 	������ �� ���������� � �������
	footnote 		string 	��� �������� ������ (� ����������, �� ������� ��������� content)
	align 			string 	������������ ������������ ����� �������� (�left�, �top;left��)
	moveOutOfScreen bool 	�������� �� ������� ������ ��� ���
	allowOverlap	bool 	��������� ������������ � ������� ��������
	sortOnDepth 	bool 	����������� �� �������
	position 		pos3D 	���������� �������� (������������ ������� attachedTo ��� ����������)
	attachedTo 		ref 	������, � �������� ��������� ������
*/

#define SET_GET_POS_C2(C) \
	void SetPos_##C(double aiNewVal){ \
		CreateJSPos(); pos.C=aiNewVal; \
		PosChanged(pos); \
	} \
	double GetPos_##C(){ return GetPos().C; }

class mlRMMLCloud :	
					public mlRMMLElement,
					public mlJSClass,
					public mlRMMLLoadable,
					// public mlRMMLVisible,
					public mlICloud,
					public moICloud,
					public mlIVisibleNotifyListener
{
	bool mbInverse;
	JSString* mjssContent;
	bool mbMLElsCreated;
public:
	mlRMMLCloud(void);
	void destroy() { MP_DELETE_THIS }
	~mlRMMLCloud(void);
MLJSCLASS_DECL
private:
	enum {
		JSPROP_inverse = 1,
		JSPROP_visible,
		JSPROP_maxVisibleDistance,
		JSPROP_duration,
		JSPROP_content,
		JSPROP_footnote,
		JSPROP_align,
		JSPROP_moveOutOfScreen,
		JSPROP_allowOverlap,
		JSPROP_sortOnDepth,
		JSPROP_position,
		JSPROP_attachedTo,
		JSPROP_marginX,
		JSPROP_marginY,
		JSPROP_viewVec
	};
	bool visible;
	double duration;
	double maxVisibleDistance;
	mlRMMLComposition* content;
	JSString* footnote;
	JSString* align;
	bool moveOutOfScreen;
	bool allowOverlap;
	bool sortOnDepth;
	ml3DPosition pos;
	JSString* attachedTo;
	mlRMMLElement* mp3DOattachedTo;
	int marginX, marginY;
	ml3DPosition viewVec;

	JSFUNC_DECL(attachTo)
	JSFUNC_DECL(setSpecParameters)

private:
	struct mlJS3DPos: public mlI3DPosition{
		mlRMMLCloud* mpCloud;
		JSObject* mjsoPos;
		mlJS3DPos(mlRMMLCloud* apCloud);
		// mlI3DPosition
		void SetPos_x(double adNewVal){ mpCloud->SetPos_x(adNewVal); }
		void SetPos_y(double adNewVal){ mpCloud->SetPos_y(adNewVal); }
		void SetPos_z(double adNewVal){ mpCloud->SetPos_z(adNewVal); }
		ml3DPosition GetPos(){ return mpCloud->GetPos(); }
		void SetPos(ml3DPosition &aPos){ mpCloud->SetPos(aPos); }
	};
	mlJS3DPos* mpJSPos;
	SET_GET_POS_C2(x)
	SET_GET_POS_C2(y)
	SET_GET_POS_C2(z)

	void CreateJSPos();

	void SetAttachedTo(JSString* ajssName);

	mlRMMLScene3D* mpScene3DRegistered;	// 3D-�����, � ������� ��������������� cloud
	// ����������������/����������������� cloud � 3D-�����, ���� �� ��� � ��� ������� �������� ��������/���������
	void UpdateRegistration();
	// ����������������� cloud �� 3D-�����, ���� �� ��� ��� ���������������
	void Unregister();
	// �������� 3D-�����, ������� �������� ����� �� ��������� cloud-�
	mlRMMLScene3D* Get3DScene();
	// ���������� true, ���� � ��� cloud � ��� ������� ��������� ��������
	bool IsVisible();

// ���������� mlRMMLElement
MLRMMLELEMENT_IMPL
	mlresult CreateMedia(omsContext* amcx);
	mlresult Load();
	mlRMMLElement* Duplicate(mlRMMLElement* apNewParent);

// ����������  mlILoadable
MLILOADABLE_IMPL

// ����������  mlIVisible
//MLIVISIBLE_IMPL

// ���������� mlICloud
public:
	mlICloud* GetICloud(){ return this; } 
	mlMedia* GetText();
	mlMedia* GetNicknameText();
	bool GetVisibility(){ return visible; }
	mlMedia* GetContent();
	mlMedia* GetFootnote();
	const wchar_t* GetAlign();
	bool GetMoveOutOfScreen();
	bool GetAllowOverlap();
	bool GetSortOnDepth();
	mlMedia* GetAttachedTo();
	int GetMarginX();
	int GetMarginY();
	double GetMaxVisibleDistance();

// ���������� moICloud
public:
	// ��������
	moICloud* GetmoICloud();
	void VisibleChanged();
	void ContentChanged();
	void FootnoteChanged();
	void MarginXChanged();
	void MarginYChanged();
	void AlignChanged();
	void MoveOutOfScreenChanged();
	void AllowOverlapChanged();
	void SortOnDepthChanged();
	void MaxVisibleDistanceChanged();
	void ViewVecChanged(ml3DPosition &viewVec);
	ml3DPosition GetPos();
	void SetPos(ml3DPosition &aPos);
	void PosChanged(ml3DPosition &pos); // ���������� ��������� 
	void AttachedToChanged(mlMedia* pmlMedia);
	void SetSpecParameters(int adeltaNoChangePos, int ashiftAtAvatarPosZ);

	// ���������� mlIVisibleNotifyListener
public:
	// ���������� ���������� ������������� �������� ��������
	virtual void VisualRepresentationChanged(mlMedia* map);
	// ������� ������� ����������� � ������ �� ���� ����������� �� �����
	virtual void VisibleDestroyed(mlMedia* map);
};

}

#endif
