#if _MSC_VER > 1000
#pragma once
#endif

#ifndef _mlRMMLCharacter_H_
#define _mlRMMLCharacter_H_

namespace rmml {

#define EVNM_onTurned L"onTurned"
#define EVNM_onRightHandPosChanged L"onRightHandPosChanged"
#define EVNM_onLeftHandPosChanged L"onLeftHandPosChanged"

class mlRMMLMovement;
class mlRMMLVisemes;
class mlRMMLIdles;
class mlRMMLSpeech;
class mlRMMLShadows3D;
#define MLELPN_MOVEMENT "movement"
#define MLELPN_VISEMES "visemes"
#define MLELPN_IDLES "idles"
#define MLELPN_SHADOW "shadow"
#define MLELPN_MATERIALS "materials"

/**
 * ����� 3D-���������� RMML
 */

class mlRMMLCharacter :	
					public mlRMMLElement,
					public mlJSClass,
					public mlRMMLLoadable,
					public mlRMML3DObject,
					public mlRMMLPButton,
					public mlICharacter,
					public moICharacter
{
	bool useZCoordinateSynchonization;
	bool unpickable;
	bool mbChildrenMLElsCreated;
	mlRMMLSpeech* mpSayingSpeech; // speech, ������� ������ ���������� ��������
	short miSayingSpeechIdx; // ��� ����������� �������� ���������� mpSayingSpeech
	JSString* mjssMovement;
	JSString* mjssVisemes;
	JSString* mjssIdles;
	JSString* mjssFaceIdles;
public:
	mlRMMLCharacter(void);
	void destroy() { MP_DELETE_THIS }
	~mlRMMLCharacter(void);
MLJSCLASS_DECL
private:
	enum {
		JSPROP_movement = 1,
		JSPROP_visemes,
		JSPROP_idles,
		JSPROP_faceIdles, 
		JSPROP_shadow,
		JSPROP_materials,
		JSPROP_motion,
		JSPROP_speech, // speech/audio, ������� ���������� �������� � ������ ������
		JSPROP_interlocutors, // ������ ������������ 
		JSPROP_gravitation,
		JSPROP_intangible,
		JSPROP_unpickable,
		JSPROP_color,		// ����, ������� ����������� �� ���� ���������� ������� � �������, �������� color.a
		JSPROP_opacity,		// ������� �������������� ������� (0..0xFFFF)
		JSPROP_useZSynchronization,		// ������� �������������� ������� (0..0xFFFF)
		EVID_onTurned,
		EVID_onSaid,
		EVID_onFalling,
		EVID_onFallAndStop,
		EVID_onWreck,
		EVID_onNeedToChangePosition,
		EVID_onRightHandPosChanged,
		EVID_onLeftHandPosChanged
	};
	enum {
		EVSM_onTurned = (1 << 0),
		EVSM_onSaid = (1 << 1),
		EVSM_onFalling = (1 << 2),
		EVSM_onFallAndStop = (1 << 3),
		EVSM_onWreck = (1 << 4),
		EVSM_onNeedToChangePosition = (1 << 8)
	};
	mlRMMLMovement* movement;
	mlRMMLVisemes* visemes;
	mlRMMLIdles* idles;
	mlRMMLIdles* faceIdles;
	JSFUNC_DECL(say)
	JSFUNC_DECL(goTo)
	JSFUNC_DECL(turnTo)
	JSFUNC_DECL(lookAt)
	JSFUNC_DECL(addInterlocutor) // �������� �����������
	JSFUNC_DECL(removeInterlocutor) // ������� �����������
	JSFUNC_DECL(lookAtByEyes) // ���������� ������ �������
	JSFUNC_DECL(setViseme) // ���������� ��������� ����
	JSFUNC_DECL(unsetViseme) // ������ � ���� ������������ ���������
	JSFUNC_DECL(doVisemes) // ��������������� ������� ��� ��������� ����
	JSFUNC_DECL(setToRandomFreePlace) 
	JSFUNC_DECL(goForward) // ��� ������
	JSFUNC_DECL(goBackward) // ������
	JSFUNC_DECL(strafeLeft) // ��� ����� �����
	JSFUNC_DECL(strafeRight) // ��� ����� ������
	JSFUNC_DECL(stopGo) // ��������� ���� (��� goTo(NULL))
	JSFUNC_DECL(turnLeft) // ������������� �����
	JSFUNC_DECL(turnRight) // ������������� ������
	JSFUNC_DECL(stopTurn) // ��������� ��������������
	JSFUNC_DECL(sayRecordedVoice) // ��������� ���������� �����
	JSFUNC_DECL(setLexeme) // ���������� ��������� ���
	JSFUNC_DECL(goLeft) // ��� ������ � ������
	JSFUNC_DECL(goRight) // ��� ������ � �������
	JSFUNC_DECL(goBackLeft) // ��� ����� � ������
	JSFUNC_DECL(goBackRight) // ��� ����� � �������
	JSFUNC_DECL(getFaceVector)
	JSFUNC_DECL(toInitialPose) 
	JSFUNC_DECL(fromInitialPose) 
	JSFUNC_DECL(pointByHandTo) // �������� �� �������� ����� ��� ������
	JSFUNC_DECL(setSleepingState) // ���������� ������/�������� ���������
	JSFUNC_DECL(setRightHandTracking) // ���/���� ������������ ������ ����
	JSFUNC_DECL(setLeftHandTracking) // ���/���� ������������ ����� ����
	JSFUNC_DECL(setKinectDeskPoints)
	JSFUNC_DECL(disableKinectDesk)

private:
	mlRMMLElement* FindVisemes(jsval ajsvVisemes, const char* apcMethodName);
	void CorrectPositionWithSynchronization(ml3DPosition &aPos); 
	bool ZShouldBeSynched();

public:
	virtual void PosChanged(ml3DPosition &aPos); 
	void Said(mlRMMLSpeech* apSpeech);
	void IdlesSrcChanged(const wchar_t* apwcIdlesName, const wchar_t* apwcSrc);	// ���������� �� mlRMMLIdles

	void SetMaterialTiling(int, float) {};
	void EnableShadows(bool isEnabled){};
	void ChangeCollisionLevel(int level){};

// ���������� mlRMMLElement
MLRMMLELEMENT_IMPL
	EventSpec* GetEventSpec2(){ return _EventSpec; } 
	mlresult CreateMedia(omsContext* amcx);
	mlresult Load();
	mlRMMLElement* Duplicate(mlRMMLElement* apNewParent);
	mlresult Update(const __int64 alTime);
	bool Freeze();
	bool Unfreeze();
	virtual bool NeedToClearRefs(){ return true; }
	virtual mlRMMLElement* GetElemToClearRefsIn(){ return (mlRMMLElement*)GetScene3D(); }
	virtual void ClearRefs(){ ClearRefs_mlRMML3DObject(); }
	// ���������� ����������� �������� � Event �� ��������� �������
	virtual void SetButtonEventData(char aidEvent){ mlRMML3DObject::SetButtonEventData(aidEvent); }

// ���������� mlILoadable
MLILOADABLE_IMPL

// ���������� mlI3DObject
MLI3DOBJECT_IMPL
	bool GetBillboard(){ return false; }

// ����������  moI3DObject
	void BillboardChanged(){}	
	void UnpickableChanged();

// ���������� mlIButton
MLIBUTTON_IMPL

// ���������� mlICharacter
	mlICharacter* GetICharacter(){ return this; } 
	// �������� ��������
	mlMedia* GetCloud();
	// ��������
	bool GetUnpickable();
	mlIMovement* GetMovement();
	mlMedia* GetVisemes();
	mlMedia* GetIdles();
	mlIIdles* GetFaceIdles();
	// �������
	mlRMMLElement* GetElem_mlRMMLCharacter(){ return this; }
	void onTurned();
	void onSaid();
	void onFalling();
	void onFallAndStop();
	void onWreck();
	void onNeedToChangePosition();
	virtual void onRightHandPosChanged(ml3DPosition aRightHandPos, ml3DPosition aRightHandDir, 
		ml3DPosition aRightHandDirExt);
	virtual void onLeftHandPosChanged(ml3DPosition aLeftHandPos, ml3DPosition aLeftHandDir, 
		ml3DPosition aLeftHandDirExt);

	bool IsFullyLoaded() {return false;}
	bool IsIntersectedWithOtherObjects() {return false;}
	void LevelChanged(int /*level*/) {};
	int GetCurrentLevel() { return 0; }
	// ������
	void SynchronizeZPosition();


// ���������� moICharacter
public:
	// ��������
	void MovementChanged();
	void VisemesChanged();
	void IdlesChanged();
	void FaceIdlesChanged();
	mlMedia* GetSpeech(); // speech/audio, ������� ������ ������������
	mlICharacter** GetInterlocutors();
	// ������
	bool say(mlMedia* apSpeech=NULL, int* apDuration=NULL);
	bool goTo(mlMedia* apObject, ml3DPosition* apTurnToPos=NULL, mlMedia* apTurnToObject=NULL, unsigned int shiftTime = 0);
	bool goTo(ml3DPosition &aPos, ml3DPosition* apTurnToPos=NULL, mlMedia* apTurnToObject=NULL, unsigned int shiftTime = 0);
	bool turnTo(mlMedia* apObject);
	bool turnTo(ml3DPosition &aPos);
	bool turnTo(int aiFlags);
	bool lookAt(mlMedia* apObject, int aiMs, double adKoef);
	bool lookAt(ml3DPosition &aPos, int aiMs, double adKoef);
	bool lookAtByEyes(mlMedia* apObject, int aiMs, double adKoef);
	bool lookAtByEyes(ml3DPosition &aPos, int aiMs, double adKoef);
	bool lookAtByEyes(const wchar_t* apDest, int aiMs, double adKoef);
	bool addInterlocutor(mlICharacter* apCharacter);
	bool removeInterlocutor(mlICharacter* apCharacter);
	bool setViseme(mlMedia* apVisemes, unsigned int auiIndex, int aiTime=-1, int aiKeepupTime=-1);
	bool setLexeme( unsigned int auiIndex, int aiTime=0);
	bool setSleepingState( bool abSleep);
	void SetTrackingRightHand( bool abTracking);
	void SetTrackingLeftHand( bool abTracking);
	bool unsetViseme(mlMedia* apVisemes, unsigned int auiIndex, int aiTime=-1);
	bool doVisemes(mlMedia* apVisemes, int aiTime=-1);
		// ���������� ��������� � ��������� ����� � �������, �������� 
	bool setToRandomFreePlace(ml3DPosition* apStartPos, double adRadius, bool abInRoom);
		// ��� ������
	bool goForward(unsigned int shiftTime = 0);
		// ������
	bool goBackward(unsigned int shiftTime = 0);
		// ��� ����� �����
	bool strafeLeft(unsigned int shiftTime = 0);
		// ��� ����� ������
	bool strafeRight(unsigned int shiftTime = 0);
		// ��������� ���� (��� goTo(NULL))
	bool stopGo();
		// ������������� �����
	bool turnLeft(unsigned int shiftTime = 0);
		// ������������� ������
	bool turnRight(unsigned int shiftTime = 0);
		// ��������� ��������������
	bool stopTurn();
		// ������������� ���������� ����
	bool sayRecordedVoice( const char* alpcUserName, const char* alpcFileName, LPCSTR alpcCommunicationAreaName, unsigned int auPosition, unsigned int auSyncVersion);
	// ��� ������ � ������
	bool goLeft(unsigned int shiftTime);
	// ��� ������ � �������
	bool goRight(unsigned int shiftTime);
	// ��� ����� � ������
	bool goBackLeft(unsigned int shiftTime);
	// ��� ����� � �������
	bool goBackRight(unsigned int shiftTime);
	bool getFaceVector(float& x, float& y, float& z);
	bool setKinectDeskPoints(const ml3DPosition& aPos, const ml3DPosition& aPos2, const moMedia* moObj);
	void disableKinectDesk();

	void toInitialPose();
	void fromInitialPose();

	// �������� �� �������� ����� ��� ������
	bool pointByHandTo(const ml3DPosition& aPos, mlMedia* apObject = NULL);
};


}

#endif
