// SoundPtr.h: interface for the CSoundPtr and CSoundPtrMap classes
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_SOUNDPTR_H__54B0AECF_085B_4AC2_9BEE_FD00DC0B82A4__INCLUDED_)
#define AFX_SOUNDPTR_H__54B0AECF_085B_4AC2_9BEE_FD00DC0B82A4__INCLUDED_

#include "ResFile.h"

using namespace audiere;

struct smISoundNotify;
class CSndFileLoaderPlayer;
class CStopCallback;

template <class TSound>
class CSoundPtrBase : public ISoundPtrBase
{ 
public:	
	CSoundPtrBase();	

	virtual ~CSoundPtrBase();	

public:
	virtual void init();

	void set_notify( sm::smISoundNotify *apISoundNotify);
	sm::smISoundNotify* get_notify();

	void set_id( int id);

	int get_id();

	void set_name( const wchar_t *fileName);

	const wchar_t *get_name();

public:
	void lock();

	void un_lock();

	virtual DWORD get_pos( int flag);

	virtual void set_pos( DWORD pos);

	virtual float get_volume();

	virtual void set_volume( float volume);

	virtual float get_pan();

	virtual void set_pan( float pan);

	virtual BOOL play();

	virtual BOOL is_play();

	virtual BOOL is_playing();

	virtual BOOL is_stopped(){ return !is_playing();};

	virtual void stop();

#if AUDIERE_RELEASE_P
	virtual void stop_and_wait();
#endif

	virtual void set_repeat();

	virtual void set_loop( BOOL bLoop);

	virtual BOOL is_loop();

	virtual BOOL is_seekable();

	virtual DWORD get_frequency();

	virtual DWORD get_length_in_ms();

	virtual DWORD get_length();

#if AUDIERE_BUILD_H
	virtual DWORD get_size();	
#endif

#if SOUND_MAN_RELEASE_H
	virtual DWORD get_current_size();
#endif

	virtual DWORD get_buffer_size();

	virtual LPSTR get_buffer( DWORD *size);

	virtual void set_pitch_shift( float shift);

	virtual float get_pitch_shift( float shift);

public:	
	virtual BOOL open( AudioDevicePtr device, File *resFile, const wchar_t *fileName) = 0;
	virtual BOOL open_async( AudioDevicePtr device, File *resFile, const wchar_t *fileName);

	virtual BOOL on_loaded( int iError);
	virtual BOOL on_play_starting();
	virtual BOOL on_play_started( int iError);
	virtual void on_stopped( StopEvent::Reason reason);

	virtual void set_loading( BOOL bLoading);

	virtual BOOL is_loading();

	virtual BOOL is_loaded();

	virtual void close();

	virtual void clear();

public:
	// ������������� ���������, ��� ���� �������� ���������������
	virtual void set_play( int iPlay);

	virtual void enabled();

	virtual void disabled();

	virtual BOOL is_enabled();

	virtual void params_saver();

	virtual BOOL apply_params();

	virtual BOOL is_params_saver();

	virtual ISndFileLoaderPlayer *get_snd_file_loader_player();

#if AUDIERE_BUILD_H
	int update_file_info( int loaded_bytes, bool bLoaded);
#endif

#if AUDIERE_RELEASE_P
	virtual BOOL is_res_attached(){ return FALSE;};
	virtual ifile *get_file(){ return NULL;};
	virtual void freeze(){};
	virtual void unfreeze(){};
	virtual BOOL is_freeze(){ return FALSE;};
	virtual void prepare_for_delete(){};
	virtual BOOL is_prepare_for_delete(){return FALSE;};
#endif

protected:
	void _apply_params();

protected:
	// ������ �� �������� ������
	//CResourceFile *m_pResFile;
	// ������������� �����
	int m_iId;
	// TRUE ���� ������������
	int  m_iPlay;
	// TRUE - ���� ��������
	BOOL m_bLoadedFile;
	// TRUE - ���� ������� �������� �����
	BOOL m_bLoading;
	// TRUE - ��������� �� ����������� ��������������� �����
	BOOL m_bLoop;
	// ��� �����
	//char m_sName[ MAX_PATH];
	CWComString m_sName;
	//
	CMutex m_mutex;
	//
	CMutex m_mutexSoundNotify;
	// ������� ��������� �����
	float m_fVolume;
	// ����������� ������� ��������� ������ � ������� ������
	float m_fPanVolume;	
	// ������� ������������ �����
	DWORD m_dwPosition;
	// ����
	TSound m_sound;
	// TRUE - ���������, ��� ���� �������� ���������������� (������� ��� �������)
	BOOL m_bStartPlay;
	// TRUE - ��������� �� ��, ��� ��������� ����� ����� ������
	BOOL m_bEnabled;
	// TRUE - ��������� �� ��, ��� ��������� ����� ����� ������, �� �� ����� ����������� ��� ������� ����� audiere, 
	// ���� �� ����� ���������� �����
	BOOL m_bFreezzed;
	// ���������, ��� ���� audiere ������
	BOOL m_bASndRemoved;
	// TRUE - ���� ����������� ��������� ��������� � ��������� ��, ����� ����� ��������
	// ��������� ����� ����������� ������ ����� ����� ����� ������� ������� _apply_params
	BOOL m_bParamsSaver;
	// ������������ ����������� �������� ����� � �����������
	CSndFileLoaderPlayer *m_pSndFileLoaderPlayer;
	// ����������� � ���������� ������ ��������� ������� � ��� ������������ ��������
	BOOL m_bTerminating;
	// ����������� � ��������
	sm::smISoundNotify *m_pISoundNotify;
	//
	BOOL m_bStream;
	//
	File *m_pResFile;
#if AUDIERE_RELEASE_O
#else
	// ����������, ���������� ����� ��������� ��������������� �����
	RefPtr<CStopCallback> m_pStopCallBack;
#endif
	// �������� TRUE ���������, ��� ������������ ���� ���������� � ���������� m_pStopCallBack �������� ���� ��������
	BOOL m_bStopped;
	// ������ �� ����������� �����-����������
	AudioDevicePtr m_pDevice;

	enum{ STOP_BY_USER = 1, STOP_BY_ENDFILE};
};

class CSoundStreamPtr : public CSoundPtrBase<OutputStreamPtr>
{
public:
	CSoundStreamPtr();
	~CSoundStreamPtr();

public:
	BOOL open( AudioDevicePtr device, File *resFile, const wchar_t *fileName);
	BOOL open( AudioDevicePtr device, File *resFile, const wchar_t *fileName, bool streaming);	
	void close();
	void clear();

public:
	void release();
	DWORD get_pos( int flag);
	void set_pos( DWORD pos);
	BOOL is_play();
	BOOL is_playing();
	BOOL is_stopped();
#if AUDIERE_RELEASE_P
	void stop_and_wait();
#endif
	void set_repeat();
	BOOL is_seekable();	
	DWORD get_frequency();
	DWORD get_length_in_ms();	
	DWORD get_length();	
#if AUDIERE_BUILD_H
	DWORD get_size();
#endif
#if SOUND_MAN_RELEASE_H
	DWORD get_current_size();
#endif
	DWORD get_buffer_size();	
	LPSTR get_buffer( DWORD *size);
#if AUDIERE_BUILD_H
	int update_file_info( int loaded_bytes, bool bLoaded);
#endif
#if AUDIERE_RELEASE_P
	BOOL is_res_attached();
	ifile *get_file();
	void freeze();
	void unfreeze();
	BOOL is_freeze();
	void create_buffer();
	void release_buffer();
	void prepare_for_delete();
	BOOL is_prepare_for_delete();
	void release_sound();
#endif

public:	
	void save_params();
	void restore_params();
	BOOL is_params_saved();

private:	
	float m_fVolumePrev;
	float m_fPanVolumePrev;	
};

class CSoundEffectPtr : public CSoundPtrBase<SoundEffectPtr>
{
public:
	CSoundEffectPtr();
	~CSoundEffectPtr();

public:
	BOOL open( AudioDevicePtr device, File *resFile, const wchar_t *fileName);
	void close();

public:	
	BOOL is_play();
	BOOL is_playing();		
	DWORD get_buffer_size();	
	LPSTR get_buffer( DWORD *size);	
	void set_pitch_shift( float shift);
	float get_pitch_shift( float shift);
#if AUDIERE_BUILD_H
	int update_file_info( int loaded_bytes, bool bLoaded);
#endif
};


//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

class CSoundManager;

class CSoundPtrMap : public CObjectsMap<ISoundPtrBase *>, public CWinThread
{
public:
	CSoundPtrMap();
	virtual ~CSoundPtrMap();	

public:
	BOOL remove( int ID, CSoundManager *pSoundManager, BOOL bAsync, BOOL bRemove = FALSE);
	BOOL remove( ISoundPtrBase *soundPtr, CSoundManager *pSoundManager, BOOL bAsync, BOOL bRemove = FALSE);
	int get_new_id();
	void remove_all_objects();
	void remove_all_objects( CSoundManager *pSound);
	BOOL update_all_sound( int act, CSoundManager *pSound);
	// ���������� ������������ ���� ������
	void stop_all_sounds();

protected:
	// ��������� �������� ��������� �����
	void set_all_sounds_disabled();
	// ��������� �������� ��������� �����
	void set_all_sounds_enabled();	

protected:
	CMutex mutex;
};

#endif // !defined(AFX_SOUNDPTR_H__54B0AECF_085B_4AC2_9BEE_FD00DC0B82A4__INCLUDED_)