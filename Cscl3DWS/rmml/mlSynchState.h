#pragma once
#include "BSSCCode.h"
#include <any.hpp>
#include <string>
#include <vector>
#include <float.h>
#include "UtilTypes.h"
#include "vs_wrappers.h"

namespace rmml {

	// �������� ��� muID, ������� ����������, ��� ������� �� �������� �������� ����
#define OBJ_ID_UNDEF 0xFFFFFFFF

#define MLSYNCH_ZONE_CHANGED_MASK (1 << 0)
	// 1� - ��������� msOwner (������� ���������� ���� ������������� ��� ���������� ������)
#define MLSYNCH_OWNER_CHANGED_MASK (1 << 1)
	// 2� - ����������� ����� (miFlags)
#define MLSYNCH_FLAGS_SET_MASK (1 << 2)
	// 3� - ���������� ���������� (muReality)
#define MLSYNCH_REALITY_SET_MASK (1 << 3)
	// 4� - ���������� ���������� Z
#define MLSYNCH_COORDINATE_SET_MASK (1 << 4)
	// 5� - ���������� __connected
#define MLSYNCH_CONNECTED_CHANGED_MASK (1 << 5)
	// 6� - ���������� �������� ���������
#define MLSYNCH_BIN_STATE_CHANGED_MASK (1 << 6)
	// 7� - ���������� ��������� RMML-����������
#define MLSYNCH_SUB_OBJ_CHANGED_MASK (1 << 7)
	// 8� - ���������� ��������� �����
#define MLSYNCH_SERVERTIMEUNUSED_SET_MASK (1 << 8)
	// 9� - ���������� �������
#define MLSYNCH_LOCATION_CHANGED_MASK (1 << 9)
	// 10� - ���������� ������� �����
#define MLSYNCH_PAUSE_LEVEL_CHANGED_MASK (1 << 10)
	// 11� - ���������� ���������� X � Y
#define MLSYNCH_COORDINATE_XY_SET_MASK (1 << 11)
	// 12� - ���������� ��������� �����
#define MLSYNCH_SERVERTIME_SET_MASK (1 << 12)
	// 13� - ���������� ����� ������������
#define MLSYNCH_DELETETIME_CHANGED_MASK (1 << 13)
	// 14� - ���������� ������� �������
#define MLSYNCH_COMMUNICATION_AREA_CHANGED_MASK (1 << 14)

	// ��� ���� �������� ��� ������ ������� ���������, ������� �� ������ ��������
#define MLSYNCH_FULL_STATE_MASK ( 0 \
	| MLSYNCH_FLAGS_SET_MASK              /* �������� ��������� ������� ��������� */ \
	| MLSYNCH_OWNER_CHANGED_MASK          /* ���������� */ \
	| MLSYNCH_ZONE_CHANGED_MASK           /* ���� */ \
	| MLSYNCH_COORDINATE_SET_MASK         /* ���������� Z */ \
	| MLSYNCH_BIN_STATE_CHANGED_MASK      /* �������� ��������� */ \
	| MLSYNCH_SUB_OBJ_CHANGED_MASK        /* ���������� */ \
	| MLSYNCH_LOCATION_CHANGED_MASK       /* ������������� */ \
	| MLSYNCH_COORDINATE_XY_SET_MASK      /* ���������� X, Y */ \
	| MLSYNCH_COMMUNICATION_AREA_CHANGED_MASK/* ������������� */ \
	)

	//	| MLSYNCH_DELETETIME_CHANGED_MASK     /* ����� ������������ */ \

	// ��������� ��������� �������� � ������� �� ���������� - ��� �������� � �������:
	//   - MLSYNCH_REALITY_SET_MASK         - ���������� (muReality)
	//   - MLSYNCH_CONNECTED_CHANGED_MASK
	//   - MLSYNCH_PAUSE_LEVEL_CHANGED_MASK - ������� �����
	//   - MLSYNCH_SERVERTIME_SET_MASK      - ��������� �����

	enum ESysPropertyType {
		SPT_FLAGS = 0,
		SPT_OWNER,
		SPT_ZONE,
		SPT_REALITY,
		SPT_CONNECTED,
		SPT_COORDINATE_Z,
		SPT_BIN_STATE,
		SPT_SERVERTIMEUNUSED,
		SPT_LOCATION,
		SPT_PAUSELEVEL,
		SPT_COORDINATE_X,
		SPT_COORDINATE_Y,
		SPT_SERVERTIME,
		SPT_DELETETIME,
		SPT_COMMUNICATION_AREA,
		SPT_MAX
	};

	inline void AddString( std::wstring& aString, const std::wstring& anIndent, const std::wstring& anAddon)
	{
		aString += anIndent;
		aString += anAddon;
	}

	inline void AddString( std::wstring& aString, const std::wstring& anIndent, const wchar_t* anAddon)
	{
		aString += anIndent;
		aString += anAddon;
	}

	inline void AddString( std::wstring& aString, const char* anAddon)
	{
		size_t length = strlen( anAddon);
		wchar_t* wsAttrValue = MP_NEW_ARR( wchar_t, length + 1);

		va_mbstowcs( wsAttrValue, anAddon, length);
		/*#if _MSC_VER >= 1500//vs2008
		size_t convertedChars = 0;
		mbstowcs_s( &convertedChars, wsAttrValue, length, anAddon, length);
		#else
		mbstowcs(wsAttrValue, anAddon, length);
		#endif*/
		wsAttrValue[ length] = 0;

		aString += wsAttrValue;

		MP_DELETE_ARR( wsAttrValue);
	}

	inline void AddString( std::wstring& aString, const std::wstring& anIndent, const std::string& anAddon)
	{
		aString += anIndent;

		AddString( aString, anAddon.c_str());
	}

	inline void AddString( std::wstring& aString, const std::wstring& anIndent, const char* anAddon)
	{
		aString += anIndent;

		AddString( aString, anAddon);
	}

	inline void AddPair( std::wstring& aString, const std::wstring& anIndent, const wchar_t* aKey, const wchar_t* aValue)
	{
		aString += anIndent;
		aString += aKey;
		aString += L": ";
		aString += aValue;
		aString += L"\n";
	}

	inline void AddPair( std::wstring& aString, const std::wstring& anIndent, const std::string& aKey, const wchar_t* aValue)
	{
		AddString( aString, anIndent, aKey);
		aString += L": ";
		aString += aValue;
		aString += L"\n";
	}

	inline void AddPair( std::wstring& aString, const std::wstring& anIndent, const wchar_t* aKey, const std::wstring aValue)
	{
		AddPair( aString, anIndent, aKey, aValue.c_str());
	}

	inline void AddPair( std::wstring& aString, const std::wstring& anIndent, const wchar_t* aKey, const std::string aValue)
	{
		aString += anIndent;
		aString += aKey;
		aString += L": ";
		AddString( aString, aValue.c_str());
		aString += L"\n";
	}

	inline void AddPair( std::wstring& aString, const std::wstring& anIndent, const wchar_t* aKey, int aValue, int aRadix = 10)
	{
		wchar_t buffer[ 50];
		AddPair( aString, anIndent, aKey, va_itow( aValue, buffer, aRadix, va_arrsize_in_bytes(buffer)));
	}

	inline void AddPair( std::wstring& aString, const std::wstring& anIndent, const wchar_t* aKey, unsigned __int64 aValue, int aRadix = 10)
	{
		wchar_t buffer[ 50];
		AddPair( aString, anIndent, aKey, va_ui64tow( aValue, buffer, aRadix, va_arrsize_in_bytes(buffer)));
	}

	inline void AddPair( std::wstring& aString, const std::wstring& anIndent, const wchar_t* aKey, unsigned int aValue, int aRadix = 10)
	{
		wchar_t buffer[ 50];
		AddPair( aString, anIndent, aKey, va_ultow( aValue, buffer, aRadix, va_arrsize_in_bytes(buffer)));
	}

	inline void AddPair( std::wstring& aString, const std::wstring& anIndent, const wchar_t* aKey, float aValue)
	{
		wchar_t number[ 80];
#if _MSC_VER >= 1500 //vs2008
		swprintf( number, va_arrsize_in_bytes(number), L"%f", aValue);
#else
		swprintf( number, L"%f", aValue);
#endif
		AddPair( aString, anIndent, aKey, number);
	}

	// �������� ��������� ����������������� �������
	class syncObjectBinState
	{
	public:
		syncObjectBinState()
		{
			updatingType = BSSCC_CLEAR;
			version = 0;
			binStateSize = 0;
			binStateCapacity = 0;
			binState = NULL;
		}

		syncObjectBinState(const syncObjectBinState& aSrc)
		{
			updatingType = BSSCC_CLEAR;
			version = 0;
			binStateSize = 0;
			binStateCapacity = 0;
			binState = NULL;
			*this = aSrc;
		}

		virtual ~syncObjectBinState()
		{
			if (binState != NULL)
				MP_DELETE_ARR( binState);
		}

		void ApllyUpdating(BSSCCode aUpdatingType, unsigned int aVersion, const BYTE* aData, int aDataSize)
		{
			switch (aUpdatingType)
			{
			case BSSCC_REPLACE_ALL:
				{
					ReplaceAll(aVersion, aData, aDataSize);
					break;
				}
			case BSSCC_APPEND:
				{
					Append(aVersion, aData, aDataSize);
					break;
				}
			case BSSCC_CLEAR:
				{
					Clear(aVersion);
					break;
				}
			}
		}

		void ApllyUpdating(const syncObjectBinState& aUpdatingState)
		{
			BSSCCode newUpdatingType;
			unsigned int newVersion;
			BYTE* newBinState;
			int newBinStateSize;
			aUpdatingState.GetFullState(newUpdatingType, newVersion, newBinState, newBinStateSize);
			ApllyUpdating(newUpdatingType, newVersion, newBinState, newBinStateSize);
		}

		void GetFullState(BSSCCode& aUpdatingType, unsigned int& aVersion, BYTE*& aBinState, int& aBinStateSize) const
		{
			aUpdatingType = updatingType;
			aVersion = version;
			aBinState = binState;
			aBinStateSize = binStateSize;
		}

		unsigned int GetVersion() const
		{
			return version;
		}

		int GetSize() const
		{
			return binStateSize;
		}

		unsigned int GetChecksum()
		{
			unsigned int checkSum = 0;
			for (int i = 0; i < binStateSize; i++)
			{
				checkSum += binState[i];
			}
			return checkSum;
		}

		syncObjectBinState& operator=(const syncObjectBinState& aSrc)
		{
			if (this != &aSrc)
			{
				Clear(0);
				ApllyUpdating(aSrc);
			}
			return *this;
		}

	private:
		BSSCCode updatingType;
		unsigned int version;
		BYTE* binState;
		int binStateSize;
		int binStateCapacity;

		void Grow(int aNeedSize)
		{
			if (binStateCapacity < aNeedSize)
			{
				int newCapacity = aNeedSize + 3000;
				BYTE* pData = MP_NEW_ARR( BYTE, newCapacity);
				if (binState != NULL)
				{
					memcpy( pData, binState, binStateSize);
					MP_DELETE_ARR( binState);
				}
				binState = pData;
				binStateCapacity = newCapacity;
			}
		}

		void Clear(unsigned int aVersion)
		{
			updatingType = BSSCC_CLEAR;
			version = aVersion;
			binStateSize = 0;
		}

		void Append(unsigned int aVersion, const BYTE* aData, int aDataSize)
		{
			if (updatingType != BSSCC_REPLACE_ALL)
				updatingType = BSSCC_APPEND;
			version = aVersion;
			CopyBinState(aData, aDataSize);
		}

		void ReplaceAll(unsigned int aVersion, const BYTE* aData, int aDataSize)
		{
			updatingType = BSSCC_REPLACE_ALL;
			version = aVersion;
			binStateSize = 0;
			CopyBinState(aData, aDataSize);
		}

		void CopyBinState(const BYTE* aData, int aDataSize)
		{
			int newSize = binStateSize + aDataSize;
			Grow(newSize);
			memcpy(binState + binStateSize, aData, aDataSize);
			binStateSize = newSize;
		}
	};

	struct mlSysSynchProps
	{
	public:
		// ��������� �����, ������������ ����� �������� ������
		int miSet;
		//// �����
		// 0-� ���� � ���� ����������, �� ��� ������ � ����������� �������� ������������� (SOA)
#define MLSYNCH_SERVER_OWNER_ASSIGNATION_MASK 1
		// 1-� ���� � ���� ����������, �� �������� ����� ������ ������ �����������, ���� �� ���������� (���� ���, �� ����� �� ����� ������)
#define MLSYNCH_OWNER_ONLY_SYNCHR_MASK 2
		// 2-� ���� - ���� ����������, �� ������ ������ ��������� �� ���� ������ � ������������� ������� (��������, ������)
#define MLSYNCH_EXCLUSIVE_OWN_MASK 4
		int miFlags;
		// �����������MLSYNCH_OWNER_ONLY_SYNCHR_MASK 2
		MP_WSTRING msOwner;
		// ������������� ����
		int miZone; 
		// ������������� ����������
		unsigned int muReality;
		// ������� �����
		unsigned int muPauseLevel; 
		// ������ true. ���� �������� � true �� false, �� ���� ������� �� ���� ������� ����
		bool mbConnected;
		// ������������� �������
		MP_WSTRING msLocation;
		// ������������� � �������
		MP_WSTRING msCommunicationArea;

#define MLSYNCH_UDEF_X FLT_MIN
#define MLSYNCH_UDEF_Y FLT_MIN
#define MLSYNCH_UDEF_Z FLT_MIN

		// ���������� x, y, z
		float mfCoordinateX, mfCoordinateY, mfCoordinateZ;
		// ��������� �����
		unsigned __int64 serverTime;
		// �������� ���������
		syncObjectBinState mBinState;
		// ����� ������������ (������������� ���������� �������)
		unsigned __int64 mlDeleteTime;

		bool operator==( const mlSysSynchProps& anOtherSysProps) const
		{
			if (this == &anOtherSysProps)
				return true;
			if (miSet != anOtherSysProps.miSet)
				return false;
			if ((miSet & MLSYNCH_CONNECTED_CHANGED_MASK) != 0 && mbConnected != anOtherSysProps.mbConnected)
				return false;
			if ((miSet & MLSYNCH_SERVERTIME_SET_MASK) != 0 && serverTime != anOtherSysProps.serverTime)
				return false;
			if ((miSet & MLSYNCH_FLAGS_SET_MASK) != 0 && miFlags != anOtherSysProps.miFlags)
				return false;
			if ((miSet & MLSYNCH_OWNER_CHANGED_MASK) != 0 && msOwner != anOtherSysProps.msOwner)
				return false;
			if ((miSet & MLSYNCH_ZONE_CHANGED_MASK) != 0 && miZone != anOtherSysProps.miZone)
				return false;
			if ((miSet & MLSYNCH_REALITY_SET_MASK) != 0 && muReality != anOtherSysProps.muReality)
				return false;
			if ((miSet & MLSYNCH_COORDINATE_XY_SET_MASK) != 0 && mfCoordinateX != anOtherSysProps.mfCoordinateX)
				return false;
			if ((miSet & MLSYNCH_COORDINATE_XY_SET_MASK) != 0 && mfCoordinateY != anOtherSysProps.mfCoordinateY)
				return false;
			if ((miSet & MLSYNCH_COORDINATE_SET_MASK) != 0 && mfCoordinateZ != anOtherSysProps.mfCoordinateZ)
				return false;
			if ((miSet & MLSYNCH_PAUSE_LEVEL_CHANGED_MASK) != 0 && muPauseLevel != anOtherSysProps.muPauseLevel)
				return false;
			if ((miSet & MLSYNCH_DELETETIME_CHANGED_MASK) != 0 && mlDeleteTime != anOtherSysProps.mlDeleteTime)
				return false;
			if ((miSet & MLSYNCH_LOCATION_CHANGED_MASK) != 0 && msLocation != anOtherSysProps.msLocation)
				return false;
			if ((miSet & MLSYNCH_COMMUNICATION_AREA_CHANGED_MASK) != 0 && msCommunicationArea != anOtherSysProps.msCommunicationArea)
				return false;

			return true;
		}

		bool operator!=( const mlSysSynchProps& anOtherSysProps) const
		{
			return !(*this == anOtherSysProps);
		}

		mlSysSynchProps():
			MP_WSTRING_INIT(msLocation),
			MP_WSTRING_INIT(msCommunicationArea),				
			MP_WSTRING_INIT(msOwner)
		{
			Reset();
		}

		mlSysSynchProps(const mlSysSynchProps& aSrc):
			MP_WSTRING_INIT(msLocation),
			MP_WSTRING_INIT(msCommunicationArea),
			MP_WSTRING_INIT(msOwner)
		{
			*this = aSrc;
		}

		~mlSysSynchProps(){
		}

		void Reset()
		{
			miSet = 0;
			miFlags = 0;
			msOwner = L"";
			miZone = -1;
			muReality = 0;
			muPauseLevel = 0;
			mbConnected = true;
			msLocation = L"";
			mfCoordinateX = MLSYNCH_UDEF_X;
			mfCoordinateY = MLSYNCH_UDEF_Y;
			mfCoordinateZ = MLSYNCH_UDEF_Z;
			serverTime = 0;
			mBinState.ApllyUpdating(BSSCC_CLEAR, 0, NULL, 0);
			mlDeleteTime = 0;
			msCommunicationArea = L"";
		}

		void SetFlags( int aFlags)
		{
			miFlags = aFlags;
			miSet |= MLSYNCH_FLAGS_SET_MASK;
		}

		void SetOwner( std::wstring anOwner)
		{
			msOwner = anOwner;
			miSet |= MLSYNCH_OWNER_CHANGED_MASK;
		}

		void SetZone( int aZone)
		{
			miZone = aZone;
			miSet |= MLSYNCH_ZONE_CHANGED_MASK;
		}

		void SetLocation( const std::wstring& aLocation)
		{
			// ���� ������� ���������� �� �������, �� �� ���� �� ����� � �������
			/* BDima 01.07.2010. �� �������. ��� �������� ������� ����� ��������?
			if(!(miSet & MLSYNCH_LOCATION_CHANGED_MASK)){
			msLocation = aLocation;
			miSet |= MLSYNCH_LOCATION_CHANGED_MASK;
			}
			*/
			msLocation = aLocation;
			miSet |= MLSYNCH_LOCATION_CHANGED_MASK;
		}

		void SetCommunicationArea( const std::wstring& aCommunicationArea)
		{			
			msCommunicationArea = aCommunicationArea;
			miSet |= MLSYNCH_COMMUNICATION_AREA_CHANGED_MASK;
		}

		void SetReality( unsigned int aReality)
		{
			muReality = aReality;
			miSet |= MLSYNCH_REALITY_SET_MASK;
		}

		void SetConnected( unsigned char aConnected)
		{
			mbConnected = aConnected?true:false;
			miSet |= MLSYNCH_CONNECTED_CHANGED_MASK;
		}

		bool IsConnected() const
		{
			if ((miSet || MLSYNCH_CONNECTED_CHANGED_MASK) == 0)
				return true;
			if (mbConnected)
				return true;
			else
				return false;
		}

		void SetCoordinateZ( float aCoordinateZ)
		{
			mfCoordinateZ = aCoordinateZ;
			miSet |= MLSYNCH_COORDINATE_SET_MASK;
		}

		bool SetCoordinateXY( float aCoordinateX, float aCoordinateY)
		{
			bool bIsChangedXY = false;
			if (mfCoordinateX != aCoordinateX)
				bIsChangedXY = true;
			if (mfCoordinateY != aCoordinateY)
				bIsChangedXY = true;

			mfCoordinateX = aCoordinateX;
			mfCoordinateY = aCoordinateY;
			miSet |= MLSYNCH_COORDINATE_XY_SET_MASK;

			return bIsChangedXY;
		}


		void SetPauseLevel( unsigned int aPauseLevel)
		{
			muPauseLevel = aPauseLevel;
			miSet |= MLSYNCH_PAUSE_LEVEL_CHANGED_MASK;
		}

		void SetServerTime( unsigned __int64 aServerTime)
		{
			serverTime = aServerTime;
			miSet |= MLSYNCH_SERVERTIME_SET_MASK;
		}

		void SetBinState( const syncObjectBinState& aBinState)
		{
			mBinState.ApllyUpdating(aBinState);
			miSet |= MLSYNCH_BIN_STATE_CHANGED_MASK;
		}

		void SetBinState( BSSCCode aUpdatingType, unsigned int aVersion, const BYTE* aData, int aDataSize)
		{
			mBinState.ApllyUpdating(aUpdatingType, aVersion, aData, aDataSize);
			miSet |= MLSYNCH_BIN_STATE_CHANGED_MASK;
		}

		void SetDeleteTime( unsigned __int64 aDeleteTime)
		{
			mlDeleteTime = aDeleteTime;
			miSet |= MLSYNCH_DELETETIME_CHANGED_MASK;
		}

		mlSysSynchProps& operator=(const mlSysSynchProps& aSrc){
			if (this == &aSrc)
				return *this;
			miSet = aSrc.miSet;
			miFlags = aSrc.miFlags;
			msOwner = aSrc.msOwner;
			miZone = aSrc.miZone;
			muReality = aSrc.muReality;
			mbConnected = aSrc.mbConnected;
			msLocation = aSrc.msLocation;
			mfCoordinateX = aSrc.mfCoordinateX;
			mfCoordinateY = aSrc.mfCoordinateY;
			mfCoordinateZ = aSrc.mfCoordinateZ;
			serverTime = aSrc.serverTime;
			muPauseLevel = aSrc.muPauseLevel;
			mBinState = aSrc.mBinState;
			mlDeleteTime = aSrc.mlDeleteTime;
			msCommunicationArea = aSrc.msCommunicationArea;

			return *this;
		}

		bool isEqual(const wchar_t* apch1, const wchar_t* apch2){
			for(;;){
				if(*apch1!=*apch2) return false;
				if(*apch1==L'\0') return true;
				apch1++, apch2++;
			}
		}

		int update( const mlSysSynchProps& aSrc, int anAllowedSet)
		{
			int iSet = aSrc.miSet & anAllowedSet;

			if (iSet & MLSYNCH_FLAGS_SET_MASK)
				SetFlags( aSrc.miFlags);

			if (iSet & MLSYNCH_OWNER_CHANGED_MASK)
			{
				if (!isEqual(msOwner.c_str(), aSrc.msOwner.c_str()))
					SetOwner( aSrc.msOwner);
				else
					iSet &= ~MLSYNCH_OWNER_CHANGED_MASK;
			}

			if (iSet & MLSYNCH_ZONE_CHANGED_MASK)
				SetZone( aSrc.miZone);

			if (iSet & MLSYNCH_LOCATION_CHANGED_MASK)
				SetLocation( aSrc.msLocation);

			if (iSet & MLSYNCH_REALITY_SET_MASK)
				SetReality( aSrc.muReality);

			if (iSet & MLSYNCH_CONNECTED_CHANGED_MASK)
				SetConnected( aSrc.mbConnected);

			if (iSet & MLSYNCH_COORDINATE_SET_MASK)
				SetCoordinateZ( aSrc.mfCoordinateZ);

			if (iSet & MLSYNCH_COORDINATE_XY_SET_MASK)
				SetCoordinateXY( aSrc.mfCoordinateX, aSrc.mfCoordinateY);

			if (iSet & MLSYNCH_BIN_STATE_CHANGED_MASK)
				SetBinState( aSrc.mBinState);

			if (iSet & MLSYNCH_SERVERTIME_SET_MASK)
				SetServerTime( aSrc.serverTime);

			if (iSet & MLSYNCH_PAUSE_LEVEL_CHANGED_MASK)
				SetPauseLevel( aSrc.muPauseLevel);

			if (iSet & MLSYNCH_DELETETIME_CHANGED_MASK)
				SetDeleteTime( aSrc.mlDeleteTime);

			if (iSet & MLSYNCH_COMMUNICATION_AREA_CHANGED_MASK)
				SetCommunicationArea( aSrc.msCommunicationArea);

			return iSet;
		}

		unsigned char GetSysPropsCount() const
		{
			unsigned char ucSysPropCnt = 0;
			if(miSet){
				if(miSet & MLSYNCH_FLAGS_SET_MASK)
					ucSysPropCnt++;
				if(miSet & MLSYNCH_OWNER_CHANGED_MASK)
					ucSysPropCnt++;
				if(miSet & MLSYNCH_ZONE_CHANGED_MASK)
					ucSysPropCnt++;
				if(miSet & MLSYNCH_REALITY_SET_MASK)
					ucSysPropCnt++;
				if(miSet & MLSYNCH_COORDINATE_SET_MASK)
					ucSysPropCnt++;
				if (miSet & MLSYNCH_COORDINATE_XY_SET_MASK)
					ucSysPropCnt+=2;
				if(miSet& MLSYNCH_SERVERTIME_SET_MASK)
					ucSysPropCnt++;
				if (miSet & MLSYNCH_BIN_STATE_CHANGED_MASK)
					ucSysPropCnt++;
				if(miSet & MLSYNCH_LOCATION_CHANGED_MASK)
					ucSysPropCnt++;
				if(miSet & MLSYNCH_DELETETIME_CHANGED_MASK)
					ucSysPropCnt++;
				if(miSet & MLSYNCH_COMMUNICATION_AREA_CHANGED_MASK)
					ucSysPropCnt++;
			}
			return ucSysPropCnt;
		}

		void ToString( std::wstring& aString, const wchar_t* anIndent) const
		{
			std::wstring indent;
			if (anIndent != NULL)
				indent = anIndent;

			aString += L"{\n";
			indent += L"  ";

			if(miSet & MLSYNCH_FLAGS_SET_MASK)
				AddPair( aString, indent, L"flags", miFlags);
			if(miSet & MLSYNCH_OWNER_CHANGED_MASK)
				AddPair( aString, indent, L"owner", msOwner);
			if(miSet & MLSYNCH_ZONE_CHANGED_MASK)
				AddPair( aString, indent, L"zone", miZone);
			if(miSet & MLSYNCH_REALITY_SET_MASK)
				AddPair( aString, indent, L"reality", muReality);
			if(miSet & MLSYNCH_COORDINATE_SET_MASK)
				AddPair( aString, indent, L"z", mfCoordinateZ);
			if (miSet & MLSYNCH_COORDINATE_XY_SET_MASK)
			{
				AddPair( aString, indent, L"x", mfCoordinateX);
				AddPair( aString, indent, L"y", mfCoordinateY);
			}
			if(miSet& MLSYNCH_SERVERTIME_SET_MASK)
				AddPair( aString, indent, L"serverTime", serverTime);
			if (miSet & MLSYNCH_BIN_STATE_CHANGED_MASK)
				AddPair( aString, indent, L"binStateSize", mBinState.GetSize());
			if(miSet & MLSYNCH_LOCATION_CHANGED_MASK)
				AddPair( aString, indent, L"location", msLocation);
			if(miSet & MLSYNCH_DELETETIME_CHANGED_MASK)
				AddPair( aString, indent, L"deleteTime", mlDeleteTime);
			if(miSet & MLSYNCH_COMMUNICATION_AREA_CHANGED_MASK)
				AddPair( aString, indent, L"communicationArea", msCommunicationArea);

			indent.resize( indent.size() - 2);
			AddString( aString, indent, L"}");
		}
	};


	// 1� - ��������� msOwner (������� ���������� ���� ������������� ��� ���������� ������)
#define SYSP_OWNER_CHANGED_MASK (1 << 1)
	// 5� - ���������� __connected
#define SYSP_CONNECTED_CHANGED_MASK (1 << 5)
	// 7� - ���������� ��������� RMML-����������
#define SYSP_SUB_OBJ_CHANGED_MASK (1 << 7)
	// 8� - ���������� ��������� �����
#define SYSP_SERVERTIME_SET_MASK (1 << 8)


	enum syncObjValueType
	{
		SYVT_UNDEFINED = 0, // �������������� �������� (void)
		SYVT_NULL,     // ������ �������� (null)
		SYVT_INT,      // 31������ ����� ����� (int)
		SYVT_DOUBLE,   // ������������ ����� (double)
		SYVT_STRING,   // ������ � UNICODE (std::wstring)
		SYVT_OBJECT,   // ��������� ���� syncObjectState
		SYVT_OBJLINK,  // ��������� ���� Link (std::string)
		SYVT_BIN,      // �������� ������ 
	};

	struct syncObjectState;

	// ���������������� �������� 
	struct syncObjectProp
	{
		// ��������� ������������� �������� � dot-�������
		MP_STRING name;

		// �������� ��������
		CByteVector binValue;

		syncObjectProp():
			MP_VECTOR_INIT(binValue),
			MP_STRING_INIT(name)
		{
		}
		syncObjectProp(const std::string& name_):
			MP_VECTOR_INIT(binValue),
			MP_STRING_INIT(name)
		{
			this->name = name_;
		}

		bool operator==( const syncObjectProp& aProperty) const
		{
			if (this == &aProperty)
				return true;
			if (name != aProperty.name)
				return false;
			if (binValue != aProperty.binValue)
				return false;
			return true;
		}

		bool operator!=( const syncObjectProp& aProperty) const
		{
			return !(*this == aProperty);
		}

		void ToString( std::wstring& aString, const wchar_t* anIndent) const
		{
			std::wstring indent;
			if (anIndent != NULL)
				indent = anIndent;

			AddPair( aString, indent, name, L"{...}");
		}
	};

	// ������ ���������������� �������
	typedef MP_VECTOR<syncObjectProp> syncObjectPropVector;

	// ������ ��������� �����������
	typedef MP_VECTOR<syncObjectState> syncObjectStateVector;

	// ��������� ��������� ���������������� ������� � ��������, ���������� � �������
	enum ESynchStateError
	{
		SSE_NO_ERRORS,
		SSE_WRONG_STATE,
		SSE_OBJECT_NOT_FOUND,
		SSE_NOT_SYNCRONIZED,
		SSE_DELETE_WRONG_REALITY_AVATAR,
		SSE_DELETE_DISCONECTED,
		SSE_DELETE_CHANGED_REALITY_AVATAR,
		SSE_DELETE_NOT_FOUND,
		SSE_REJECTED,
		SSE_HASHERROR,
		SSE_OBSOLETE	// ���������� �����. �������� �������� �������� ������ ���������, ������� � ���� ��� ����
	};

	// ���������, ���������� ������ � ��������� ������� ��������, ���������� �������������
	struct syncObjectState
	{
		enum StateType
		{
			ST_FULL = 0,		// 0 - ������ ��������� ������� (creation_info ������ ���� �� ������). ������ ��������� ����������� � �������������� � ��������� clientVersion, servertVersion
			ST_UPDATE,			// 1 - ����� ����������
			ST_NOTFOUND,		// 2 - ������ �� ������� �� ������
			ST_CONFIRMATION,	// 3 - ����� �� ����������, ��������� � ������� �� ������
			ST_FORCE			// 4 - ������ ��������� �������, ���������� ����� ����������� ��������� �������
		};

		StateType type;	

		// ������ ������� (�������� � ������� � ������� �������������)
#define MLSYNCH_UDEF_VER 0xFFFF
		// ��������� ������ ��������� ������� 
		unsigned short mwServerVersion;
		// ����������� ������ ��������� ������� 
		unsigned short mwClientVersion;

		MP_STRING string_id;	// ��������� ������������� (���)������� � dot-�������

		unsigned int uiID;
		unsigned int uiBornRealityID;
		unsigned int uiRealityID;

		MP_WSTRING creation_info;// ����������, ����������� ��� �������� ������� � JSON-�������

		// ��������� ��������
		mlSysSynchProps sys_props;

		// ���������������� ��������
		syncObjectPropVector props;

		// ����������
		syncObjectStateVector sub_objects;

		unsigned int uiHashObj;

		//-
		syncObjectState()
#ifdef MEMORY_PROFILING
			: MP_VECTOR_INIT(props),
			MP_VECTOR_INIT(sub_objects),
			MP_WSTRING_INIT(creation_info),
			MP_STRING_INIT(string_id)
#endif
		{
			uiID = OBJ_ID_UNDEF;
			type = ST_UPDATE;
		}

		syncObjectState(const syncObjectState& other):
			MP_VECTOR_INIT(props),
			MP_VECTOR_INIT(sub_objects),
			MP_WSTRING_INIT(creation_info),
			MP_STRING_INIT(string_id)
		{
			type = other.type;	
			mwServerVersion = other.mwServerVersion;
			mwClientVersion = other.mwClientVersion;
			string_id = other.string_id;
			uiID = other.uiID;
			uiBornRealityID = other.uiBornRealityID;
			uiRealityID = other.uiRealityID;
			creation_info = other.creation_info;
			sys_props = other.sys_props;
			props = other.props;
			sub_objects = other.sub_objects;
			uiHashObj = other.uiHashObj;
		}

		void SetFull(){ type = ST_FULL; }

		// �������� �� ��������� ������?
		bool IsFullState() const
		{
			if (type == ST_FULL || type == ST_FORCE)
				return true;
			else
				return false;
		}

		// �������� �� ��������� �����������
		bool IsForceState() const
		{
			if (type == ST_FORCE)
				return true;
			else
				return false;
		}

		void SetNotFound(){ type = ST_NOTFOUND; }

		// �������� �� ��� ���������� ������� � ���, 
		// ��� ������� ���� �� ������� �� ������� ���?
		bool IsNotFound() const { return (type == ST_NOTFOUND ? true : false); }

		bool operator==( const syncObjectState& aState) const
		{
			if (this == &aState) // ���������� ��� ��������� - ����� ��� ���� � ��� �� ������
				return true;

			if (mwServerVersion != aState.mwServerVersion)
				return false;
			if (mwClientVersion != aState.mwClientVersion)
				return false;
			if (type != aState.type)
				return false;
			if (string_id != aState.string_id)
				return false;
			if (uiID != aState.uiID)
				return false;
			if (uiBornRealityID != aState.uiBornRealityID)
				return false;
			if (uiRealityID != aState.uiRealityID)
				return false;
			if( uiHashObj != aState.uiHashObj)
				return false;
			if (sys_props != aState.sys_props)
				return false;
			if (props != aState.props)
				return false;
			if (sub_objects != aState.sub_objects)
				return false;
			return true;
		}

		bool operator!=( const syncObjectState& aState) const
		{
			return !(*this == aState);
		}

		void ToString( std::wstring& aString, const wchar_t* anIndent) const
		{
			std::wstring indent;
			if (anIndent != NULL)
				indent = anIndent;

			aString += L"{\n";
			indent += L"  ";

			if (uiID != OBJ_ID_UNDEF)
			{
				wchar_t tmpString[ 100];
#if _MSC_VER >= 1500 //vs2008
				swprintf( tmpString, va_arrsize_in_bytes(tmpString), L"objectID: %u, bornRealityID: %u, realityID: %u\n",
					uiID, uiBornRealityID, uiRealityID);
#else
				swprintf( tmpString, L"objectID: %u, bornRealityID: %u, realityID: %u\n",
					uiID, uiBornRealityID, uiRealityID);
#endif
				AddString( aString, indent, tmpString);
			}
			else
				AddPair( aString, indent, L"name", string_id);

			wchar_t tmpString1[ 20];
#if _MSC_VER >= 1500 //vs2008
			swprintf( tmpString1, va_arrsize_in_bytes(tmpString1), L"hashObj: %u\n", uiHashObj);
#else
			swprintf( tmpString1, L"hashObj: %u\n", uiHashObj);
#endif	
			AddString( aString, indent, tmpString1);

			if (sys_props.GetSysPropsCount() > 0)
			{
				AddString( aString, indent, L"sysProperties: ");
				sys_props.ToString( aString, indent.c_str());
				AddString( aString, indent, L"\n");
			}

			if (props.size() > 0)
			{
				AddString( aString, indent, L"sysProperties: {\n");
				indent += L"  ";
				syncObjectPropVector::const_iterator it = props.begin();
				for (;  it != props.end();  it++)
					it->ToString( aString, indent.c_str());
				indent.resize( indent.size() - 2);
				AddString( aString, indent, L"}\n");
			}

			if (sub_objects.size() > 0)
			{
				AddString( aString, indent, L"subobjects: {\n");
				indent += L"  ";
				syncObjectStateVector::const_iterator it = sub_objects.begin();
				for (;  it != sub_objects.end();  it++)
					it->ToString( aString, indent.c_str());
				indent.resize( indent.size() - 2);
				AddString( aString, indent, L"}\n");
			}

			indent.resize( indent.size() - 2);
			AddString( aString, indent, L"}");
		}
	};


}