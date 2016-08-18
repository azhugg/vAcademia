#ifndef OBJECTSTATEQUEUE_H__
#define OBJECTSTATEQUEUE_H__

#include <list>
#include <map>

namespace rmml {
struct syncObjectState;
}
using namespace rmml;

class CObjectStateQueue
{
public:
	CObjectStateQueue();
	virtual ~CObjectStateQueue();

	// ������� (������� ������ - ������ �����) ��������� ��������
	typedef std::vector<syncObjectState*> CObjectStatesVec;
	bool NextObjectState( unsigned int aObjectID, unsigned int aBornRealityID, unsigned int aRealityID, syncObjectState* &aObjectsStates);
	unsigned int Push( syncObjectState* aObjectState);
	bool Pop( syncObjectState*& aObjectState);
	bool IsEmpty();
	bool IsLeaked();

	syncObjectState* NewSyncObjectState();
	void AttachSyncObjectState(syncObjectState* aSyncObjectState);
	void DetachSyncObjectState(syncObjectState* aSyncObjectState);
	void DeleteObjectState( syncObjectState*& aSyncObjectState);
	void DeleteAllObjectStates( unsigned int aObjectID, unsigned int aBornRealityID, unsigned int aRealityID);

	void DeleteObjectsExceptReality( unsigned int auRealityID);

	void SetLogWriter( ILogWriter* aILogWriter)
	{
		m_ILogWriter = aILogWriter;
	}

private:
	void DeleteObjectStateImpl( syncObjectState*& aSyncObjectState);

private:
	// ����������� ������ �� �������� ���������� ����������� �������
	CRITICAL_SECTION locker;

	// ������� (������� ������ - ������ �����) ��������� ��������
	typedef MP_LIST<syncObjectState*> CQueueObjectStates;
	CQueueObjectStates queueObjectStates;

	// ������� (������� ������ - ������ �����) ��������� ��������
	typedef MP_MAP<syncObjectState*, syncObjectState*> CObjectStatesMap;
	CObjectStatesMap objectStateHeap;

	ILogWriter* m_ILogWriter;
};

#endif // OBJECTSTATEQUEUE_H__