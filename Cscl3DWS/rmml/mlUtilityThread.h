#pragma once

namespace rmml {

/*
  ����� ���������������� ������.
  ������������ ��� ����������� ������ GC.
  ��������� ��� ������ ���������� �� ��������� Update-� SceneManager-�, 
  ���� ���������� ��� �������� ���� ������.
*/
class mlUtilityThread{
	friend class mlGuardElemsToDelete;
	class mlUThreadInternal;
	std::auto_ptr<mlUThreadInternal> mpThread;

private:
	void LockElemsToDelete();
	void UnlockElemsToDelete();

public: 
	mlUtilityThread(JSRuntime* art);
	~mlUtilityThread();

	bool StartGC();
};

// �������� ������ ��������� �� �������� �� ��������� ������ �������
class mlGuardElemsToDelete
{
	mlUtilityThread* mpUtilityThread;
public:
	mlGuardElemsToDelete(mlUtilityThread* apUtilityThread){
		mpUtilityThread = apUtilityThread;
		mpUtilityThread->LockElemsToDelete();
	}
	~mlGuardElemsToDelete(){
		mpUtilityThread->UnlockElemsToDelete();
	}
};

}