#pragma once

namespace rmml {

#define EVNM_onEnterFrame L"onEnterFrame"
#define EVNM_onPlayed L"onPlayed"

// ���� �������� checkpoint-�� ��� ������������� continuous-�� � speech-��
//#define SCHPVT_MS 1
//#define SCHPVT_SEC 2
//#define SCHPVT_FRAME 3

class RMML_NO_VTABLE mlRMMLContinuous: 
										//public mlJSClassProto,
										public mlIContinuous,
										public moIContinuous
{
friend class mlRMMLElement;
public:
	mlRMMLContinuous(void);
	~mlRMMLContinuous(void);
MLJSPROTO_DECL
private:
	enum {JSPROP0_mlRMMLContinuous=TIDB_mlRMMLContinuous-1,
		JSPROP__playing,
		JSPROP_currentFrame,
		JSPROP__length,
		JSPROP_currentPos,
		JSPROP__duration,
		JSPROP_fps,
		JSPROP__loop,
		TIDE_mlRMMLContinuous
	};
	enum { EVID_mlRMMLContinuous=TIDE_mlRMMLContinuous-1,
		EVID_onEnterFrame,
		EVID_onPlayed,
		TEVIDE_mlRMMLContinuous
	};
	enum {
		EVSM_onEnterFrame = (1 << 6),
		EVSM_onPlayed = (1 << 5)
	};
protected:
	bool _playing;
	int currentFrame;
	unsigned int _length;
	unsigned long currentPos; // ms
	long duration; // ms
	char fps;
	bool _loop;

	JSFUNC_DECL(setCheckPoints)
	JSFUNC_DECL(rewind)
	JSFUNC_DECL(play)
	JSFUNC_DECL(stop)
	JSFUNC_DECL(rewindAndPlay)
	JSFUNC_DECL(gotoAndPlay)
	JSFUNC_DECL(gotoAndStop)

public:
	bool Cont_SetEventData(char aidEvent, mlSynchData &Data);
	bool Freeze();
	bool Unfreeze();

protected:
	virtual mlRMMLElement* GetElem_mlRMMLContinuous()=0;
	virtual int GetFrameByName(const wchar_t* apwcName){ return -1; }
	JSBool goTo(jsval avArg, const char* alpcFuncName);
	void FrameChangedDontStop();
	void PosChangedDontStop();

// ����������  mlIContinuous
public:
	// ��������
	bool GetPlaying(){ // �������� ���� ������������
		if(GetElem_mlRMMLContinuous()->IsFrozen()) return false; 
		return _playing; 
	} 
	void SetFPS(const short aFPS){ fps=(char)aFPS; }	// ������������� �������� FPS
	int GetFPS(){ return fps; }	// �������� �������� FPS ��� ����������� ��������
	int GetCurFrame(){ return currentFrame; } // �������� ����� ����� �� ������� ����� �������
	void SetCurFrame(const int aFrame){ currentFrame=aFrame;};	// ������������� �������� �������� �����
	void SetLength(const int alLength){ _length=alLength; }
	int GetLength(){ return _length; }	// �������� ���-�� ������ ����������� ��������
	void SetCurPos(unsigned long alPos){ currentPos=alPos; }
	unsigned long GetCurPos(){ return currentPos; }
//	void SetDuration(const unsigned long alDur){ _duration=alDur; }
	long GetIADuration(){ return duration; } // �������� ����������������� ����������� ��������
	bool GetLoop(){ return _loop; }	// �������� ���� �����������

	// �������
	void onEnterFrame();
	void onPlayed();
	//
	void Rewind();
	void Play();
	void Stop();

// ���������� moIContinuous
protected:
	moIContinuous* GetmoIContinuous();
	// ��������
	virtual void PlayingChanged();
	virtual void FrameChanged();
	virtual void PosChanged();
	virtual void LengthChanged(); // ���������� ���������� ������ � ����������� ��������
	virtual void DurationChanged(); // ���������� ����������������� ����������� �������� (� ms)
	virtual void FPSChanged(); // ��������� FPS ����������� ��������
	virtual long GetDuration();	// ����������� �������� _duration (����������������� � ms)
	//
	void setCheckPoints(long* apfCheckPoints, int aiLength);
	long getPassedCheckPoint();
};

#define MLICONTINUOUS_IMPL \
public: \
	mlRMMLElement* GetElem_mlRMMLContinuous(){ return this; } \
	mlIContinuous* GetIContinuous(){ return this; } \
	mlRMMLContinuous* GetContinuous(){ return this; }

	//void OnEnterFrame(){ mlRMMLContinuous::onEnterFrame(this); } \
	//void OnPlayed(){ mlRMMLContinuous::onPlayed(this); }

}