#if _MSC_VER > 1000
#pragma once
#endif

#ifndef _mlRMMLAudio_H_
#define _mlRMMLAudio_H_

namespace rmml {

/**
 * ����� ����� RMML
 */

class mlRMMLAudio :	
					public mlRMMLElement,
					public mlJSClass,
					public mlRMMLLoadable,
					public mlRMMLContinuous,
					public mlIAudible,
					public moIAudible
{
public:
	mlRMMLAudio(void);
	void destroy() { MP_DELETE_THIS }
	~mlRMMLAudio(void);
MLJSCLASS_DECL

private:
	enum{
		JSPROP_emitterType = 1, // ��� ���������
		JSPROP_splineSrc, //	���� � xml-�����, ����������� ������
		JSPROP_volume, //	��������� �����
		JSPROP_location //	�������, � ������� �������� ���� (��� 3D-��������)
	};

	JSString *m_emitterType;
	JSString *m_splineSrc;
	int m_volume;
	JSString *m_locationID;
	MP_WSTRING msSplineSrcFull;

public:

// ���������� mlRMMLElement
MLRMMLELEMENT_IMPL
	mlresult CreateMedia(omsContext* amcx);
	mlresult Load();
	mlRMMLElement* Duplicate(mlRMMLElement* apNewParent);
	bool Freeze();
	bool Unfreeze();
	
// ����������  mlILoadable
MLILOADABLE_IMPL

// ����������  mlIContinuous
MLICONTINUOUS_IMPL
	unsigned long GetCurPos();
	unsigned long GetRMCurPos();
	void PosChanged();

// ����������  mlIAudible
MLIAUDIBLE_IMPL
	bool EmitterTypeChanged();
	bool SplineSrcChanged();
	bool VolumeChanged();
	bool LocationChanged();
	const wchar_t* GetEmitterType();
	const wchar_t* GetSplineSrc();
	const int GetVolume();
	const wchar_t* GetLocation();
};

}

#endif
