#pragma once

// ������ ������ ����� � ���������
int Start_Recording();
// ���������� ������ ����� � ���������
int Stop_Recording( LPWSTR *fileName, unsigned int aSize);
// ���������� ������� ��������� ������������� ����� � ���������
float Get_CurrentMicInputLevelNormalized();
