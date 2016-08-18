// FaceDesc.h: interface for the CFaceDesc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FACEDESC_H__F5B20328_7ACC_4D7B_B96D_065FF010E3B1__INCLUDED_)
#define AFX_FACEDESC_H__F5B20328_7ACC_4D7B_B96D_065FF010E3B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <stack>
#include <map>
#include <set>
#include <sstream>
#include <algorithm>
#include <D3dx8math.h>
#include "coreanimation.h"
#include "model.h"

typedef struct faceFileDesc{
	WORD	lNum;	//����� ������
	WORD	vNum;	//����� ������
	WORD	aBase[4];	//������ ������� ������� ������
	WORD	aBaseSubMeshes[4];	//������ ������� ������� ������
	WORD	dwSubmeshCount;	//����� SubMesh
	BYTE	bFileName;
	char*	pFileName;

	faceFileDesc::faceFileDesc(){
		pFileName = NULL;
		bFileName = 0;
	}
} FACEFILEDESC;

class CalSaver;
class CalCoreAnimation;

class CFaceDesc  
{
public:
	friend class CalSaver;
	void prepare();
	void AddPair(WORD , WORD, WORD );
	bool Create(LPSTR fileName);
	CFaceDesc();
	virtual ~CFaceDesc();
public:
	WORD GetSubmeshVertCount(WORD smn);
	WORD dwSubMeshCount;
	void SetSubmeshCount(WORD wSmc);
	FACEFILEDESC	header;
	//�������� ������
	std::vector<WORD>	wIndexes;	//������ ������ ��� ��������( � �������� �����)
	std::vector<D3DXVECTOR3>	wDeltas;	//������������ ���������� ������������

	//������� ������������ ������� ������
	std::vector<WORD>	aMaxIndexes;	//������������ ���������� ������������
	std::vector<WORD>	aCalIndexes;	//������������ ���������� ������������
	std::vector<WORD>	aSubMeshes;	//������ SubMesh

protected:
	WORD	wNum;	//����� ������
	WORD	vNum;	//����� ������
	WORD	w1, w2, w3, w4;	//������ ������� ������( � �������� �����)
};

//namespace nsCalFace {

class CFace : public CalCoreAnimation
{
public:
	friend class CalSaver;
	CFace();
	virtual ~CFace();
public:
	bool init(CalModel* ap_Model);
	void calcVectors();
	FACEFILEDESC* getHeader();
	FACEFILEDESC	header;

	//������� �������
	D3DXVECTOR3	base;
	D3DXVECTOR3	vi;
	D3DXVECTOR3	vj;
	D3DXVECTOR3	vk;

	//������� ������������ ������� ������
	std::vector<WORD>	aSubmeshNum;	//������ SubMesh
	std::vector<WORD>	aVertNum;		//����� ������
	std::vector<WORD*>	aVertIndexes;	//������ ������
	std::vector<D3DXVECTOR3*>	aDeltas;	//������������ ���������� ������������
};
//}

#endif // !defined(AFX_FACEDESC_H__F5B20328_7ACC_4D7B_B96D_065FF010E3B1__INCLUDED_)
