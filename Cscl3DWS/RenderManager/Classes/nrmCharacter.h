// nrmCharacter.h: interface for the nrmCharacter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_nrmCharacter_H__A94D4F09_D0E9_480F_A889_757EF331EA0A__INCLUDED_)
#define AFX_nrmCharacter_H__A94D4F09_D0E9_480F_A889_757EF331EA0A__INCLUDED_

#include "../CommonRenderManagerHeader.h"

//////////////////////////////////////////////////////////////////////////
#include "nrmanager.h"
#include "VoipManClient.h"
#include "nrm3DObject.h"
#include "WalkController.h"
#include "SkeletonAnimationObject.h"
#include "Recognize.h"
//////////////////////////////////////////////////////////////////////////
class CEyesBlinkCtrl;
class C3DCharacter;
class CSCloudCtrl;
class nrmAudio;
class nrmVisemes;


//////////////////////////////////////////////////////////////////////////
//using namespace natura3d;

//////////////////////////////////////////////////////////////////////////
class nrmCharacter :	public nrm3DObject,
						public moICharacter,
						public moIBinSynch,
						public voip::IVoipCharacter

{
	friend class CEyesBlinkCtrl;
public:
	nrmCharacter(mlMedia* apMLMedia);
	virtual ~nrmCharacter();

public:
	/*BEGIN_INTERFACE_IMPLEMENTATION
	END_INTERFACE_IMPLEMENTATION*/

// ���������� imessage_handler
public:
	bool	handle_message(LPVOID target, DWORD message, LPVOID data);

	// ���������� moIBinSynch, ��� ������ � Kinect
public:
	// 
	void ResetSynchKinectState();
	// � ������� ������ ������ ���������: ���������� ���
	bool SetFullState(unsigned int auStateVersion, const unsigned char* apState, int aiSize);
	// � ������� ������ ��������� ���������: �������� ���������
	bool UpdateState(unsigned int auStateVersion, BSSCCode aCode, const unsigned char* apState, int aiSize
						, unsigned long aulOffset, unsigned long aulBlockSize);
	// ����� ��������� �� ������ ������ ���������: ��������� ���
	// (���������� ������ ������, apState ����� ���� null)
	unsigned long GetFullState(unsigned int& auStateVersion, const unsigned char* apState, int aiSize);
	bool GetFullStateImpl(unsigned int& auStateVersion, const unsigned char* apState, int& aiSize);
	// �������� ��������� �������, �������� ��� ������
	void Reset();
	// ������� ���� ��� ���������� ��������� ���������
	moIBinSynch* GetIBinSynch();

	// ���������� ieventslistener
public:
	// ET_POSITIONCHANGED
	//void	on_position_changed( icollisionobject* apicollisionobject){}
	// ET_GROUPOBJECTSASYNCHCREATED
	//void	on_group_objects_asynch_created( igroup3d* apigroup3d){}
	// ������������ �������
	//void	handle_event( events::EVENTS_TYPE event, ibase* apiobject, LPVOID apdata);

public:
	int				uID;

// ���������� moMedia
public:
	void GetSynchData(mlSynchData &aSynchData);
	void SetSynchData(mlSynchData &aSynchData);
	moI3DObject* GetI3DObject(){ return this; }
	moIButton* GetIButton(){ return this; }

	bool setKinectDeskPoints(const ml3DPosition& aPos, const ml3DPosition& aPos2, const moMedia* moObj);
	void disableKinectDesk();

	void toInitialPose();
	void fromInitialPose();

	void OnChanged(int eventID);

private:
	void OnLoaded();

// ����������  moILoadable
public:
	void loaded();
	bool SrcChanged(); // �������� ������ �������
	bool Reload(){return false;} // ���������� ������� �� �������� src

// ����������  moIButton
public:
	virtual void onKeyDown();
	virtual void onKeyUp();	

	// ����������  moI3DObject
public:
	void FrozenChanged();
	void UnpickableChanged();

	bool	setSleepingState(bool isSleeped);

// ����������  moICharacter
public:
	void onTurnArrived();
	void onPointArrived();
	//bool idleCanStart(mlMedia* apmlMotion);
	virtual void update(DWORD	dwTime, void* data);
	virtual	void HandleCal3DAction(ACTIONTYPE type, int actvalue);
	moICharacter* GetICharacter(){ return this; }
	void IdlesSrcChanged(const wchar_t* apwcIdlesName, const wchar_t* apwcSrc);

	void MovementChanged();
	void VisemesChanged();
	void IdlesChanged();
	void FaceIdlesChanged();
	mlMedia* GetSpeech(); // speech/audio, ������� ������ ������������
	mlICharacter** GetInterlocutors();
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
	void setIndexedVisemes(int iFirstFomen, int iSecondFonem, float fMiddleFactor);
	void handleKinectState();

		// ���������� ��������� ���� � �������� auiIndex, ������ �� apVisemes.
		// aiTime - ������ ������� ���������/������, 
		// aiKeepupTime - ������ �������, ������� "�������" ��� ��������� �� ����
	bool setViseme(mlMedia* apVisemes, unsigned int auiIndex, int aiTime=-1, int aiKeepupTime=-1);
		// ������ � ���� ��������� � �������� auiIndex, ������ �� apVisemes.
		// aiTime - ������ ������� "��������"
	bool unsetViseme(mlMedia* apVisemes, unsigned int auiIndex, int aiTime=-1);
		// ��������������� ������� ��� ��������� ����, ��������� � apVisemes, �� ������ ������� aiTime
	bool doVisemes(mlMedia* apVisemes, int aiTime=-1);
	// ���������� ��������� ��� �� ��������� �����
	bool setLexeme( unsigned int auiIndex, int aiTime=0);
		// ���������� ��������� � ��������� ����� � �������, �������� �����������:
		// apStartPos - ��������� �������, adRadius - ������ �������
		// abInRoom - ��������� � �������� �������
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
	bool sayRecordedVoice( LPCSTR alpcUserName, LPCSTR alpcFileName, LPCSTR alpcCommunicationAreaName, unsigned int auPosition, unsigned int auSyncVersion);
		// ��� ������ � ������
	bool goLeft(unsigned int shiftTime);
		// ��� ������ � �������
	bool goRight(unsigned int shiftTime);
		// ��� ����� � ������
	bool goBackLeft(unsigned int shiftTime);
		// ��� ����� � �������
	bool goBackRight(unsigned int shiftTime);
	bool getFaceVector(float& x, float& y, float& z);
	//	void doMotion(mlMedia* apMotion);
	//	void setMotion(mlMedia* apMotion, int aiDurMS);
	void goPath(mlMedia* apPath, int aiFlags);
	void goPath(ml3DPosition* apPath, int aiPointCount, unsigned int auShiftTime);
		// ��������� �������� �������� ������� ����� ���� (%/���)
	void setPathVelocity(float afVelocity);
		// ��������� ���������� ����� (� %-�� ����), �� ������� ����� �����������
		// onPassed. getPassedCheckPoint() ������ �������� ����������� �����
	void setPathPercent(float afPercent);
	float getPathPercent();
	// �������� �� �������� ����� ��� ������
	bool pointByHandTo(const ml3DPosition& aPos, mlMedia* apObject);

	// ����������  IVoipCharacter
public:
	void handleVoiceData( int nSampleRate, const short* pRawAudio, int nSamples);
	ml3DPosition getAbsolutePosition();
	ml3DRotation getAbsoluteRotation();	
	bool isSayingAudioFile(LPCSTR  alpcFileName);
	int m_FirstLexem;
	CRecognize m_Recognize;

public:
	bool attachSound(mlMedia* pAudio, ml3DPosition& pos3D);
	void OnSetRenderManager();
	void VisibleChanged();

	void PrepareForDestroy();
	CWalkController* GetWalkController()const;

	void SetTrackingRightHand(bool isEnabled);
	void SetTrackingLeftHand(bool isEnabled);

protected:
	void stopIdles();
	// Properties
	nrmVisemes*				visemes;		// Character visemes
	nrmAudio*				m_pLeftStepSound;	// Sound for left foot step
	nrmAudio*				m_pRightStepSound;	// Sound for right foot step
	//CSCloudCtrl*			cloud_ctrl;		// For "cloud text saying"
	//CEyesBlinkCtrl*			eyesblink_ctrl;	// Blink/Lips controller
	mlMedia*				mpSpeech;		// Speech object to say
	mlIIdles*				mpIdles;		// Current idles array
	byte					keys[255];
	//std::vector<IDLEDESC>	idlesDesc;
	// Methods
	void	SetIdles(mlMedia* apIdles);
	void	SetFaceIdles(mlIIdles* apIdles);
	void	ApplyFrozenToSpeech();
	
	// Event handlers
	//virtual	void	OnDoneAction(int mID);	// Overload because of idles routine

	void ShiftTime( unsigned int shiftTime);

private:
	DWORD	dwSpeechDuration;
	bool	bPlayEnd;
	bool	m_isAsynch;
	bool	m_isTrackingRightHandEnabled;
	bool	m_isTrackingLeftHandEnabled;
	bool	m_isEmptyBinState;
	MP_STRING	sCurrentVoiceFileName;
	MP_STRING	sVoiceUserName;

	CWalkController* m_walkController;	
};

#endif // !defined(AFX_nrmCharacter_H__A94D4F09_D0E9_480F_A889_757EF331EA0A__INCLUDED_)
