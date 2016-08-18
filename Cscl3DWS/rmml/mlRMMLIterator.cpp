#include "mlRMML.h"
#include "config/oms_config.h"
#include "config/prolog.h"

namespace rmml{

mlRMMLIterator::mlRMMLIterator(JSContext* acx, mlRMMLElement* apElStart, bool abChildrenFirst, bool abInclStart):
	MP_STACK_INIT(mState)
{
	cx = acx;
	mbChildrenFirst = abChildrenFirst;
	mbInclStart = abInclStart;
	Reset(apElStart);
}

void mlRMMLIterator::Reset(mlRMMLElement* apElStart){
	ClearState();
	if(apElStart == NULL) return;
	if(!mbChildrenFirst && !mbInclStart)
		if(apElStart->mpChildren == NULL) return;
	mState.push(Composition(apElStart));
//mpaState = mState.begin();
}

mlRMMLIterator::~mlRMMLIterator(){
	ClearState();
	//mState.clear();
}

void mlRMMLIterator::ClearState(){
	//mState.clear();
	while(!mState.empty()) mState.pop();
	//mState.reserve(100);
}

mlRMMLElement* mlRMMLIterator::GetNext(){
	if(mState.size() == 0) return NULL;
	// ����� ������� � ������� �����
	Composition* pComp = &(mState.top());
	mlRMMLElement* pMLComp = pComp->pComposition;
	if(mbChildrenFirst){
		// ������� ��������� �������
		if(pMLComp->mpChildren==NULL){
			mState.pop();
//mpaState = mState.begin();
			return pMLComp;
		}
		while(1){
			// ���� ������ ��� ��� �������� ��������
			if((++(pComp->iCurChild)) >= (int)(pMLComp->mpChildren->size())){
				// �� ������� ������� �� �����
				mState.pop();
				// � ���������� ���
				return pMLComp;
			}
			// ����� ������� �������� �������
			mlRMMLElement* pMLEl = (*(pMLComp->mpChildren))[pComp->iCurChild];
			// ���� � ���� ��� �����, �� ��� � ����������
			if(pMLEl->mpChildren == NULL) return pMLEl;
			// ���� ����, �� ����� ��� �� ����
			mState.push(Composition(pMLEl));
//mpaState = mState.begin();
			pComp = &(mState.top());
			// � ��������� ���������
			pMLComp = pComp->pComposition;
		}
		
	}
	// ���� ������ ��� ��� �������� ��������
	while((++(pComp->iCurChild)) >= (int)(pMLComp->mpChildren->size())){
		// �� ������� ������� �� �����
		mlRMMLElement* pMLEl = pComp->pComposition;
		mState.pop();
//mpaState = mState.begin();
		// ���� ���� ����, �� ����� ��������
		if(mState.size() == 0){
			if(mbInclStart)
				return pMLEl;
			return NULL;
		}
		// ��������� ��������� ������� � ������ �����
		pComp = &(mState.top());
		pMLComp = pComp->pComposition;
	}
	// ����� ������� �������� �������
	mlRMMLElement* pMLEl = (*(pMLComp->mpChildren))[pComp->iCurChild];
	// ���� � ���� ��� �����, �� ��� � ����������
	if(pMLEl->mpChildren == NULL) return pMLEl;
	// ���� ����, �� ����� ��� �� ���� � ����������
	mState.push(Composition(pMLEl));
//mpaState = mState.begin();
	return pMLEl;
}

}
