#pragma once

class C3DSoundFactor
{
public:
	C3DSoundFactor();
	virtual ~C3DSoundFactor();

public:
	void calc( double adFactorDist, double adMinDist);

public:
	// ����������� ������� ������ ��������� �����
	float _attenutedCoef;
	// ����������� ������� ������ ��������� �����
	float _leftAttenutedCoef;
	// ����������� ������� ������ ��������� �����
	float _rightAttenutedCoef;
	// ����������� ����������, �� 0 �� minDist ��������� ����� �� ��������
	float _minDist;
	// ����������
	float _maxDist;
	// ����������, �� ������� ��������� ����� ������ � ��� ����
	float _attenunedDistCoef;
	// ����������� ������� ������ ��������� �����, ���� ������� ����������� ������ ���� �����
	float _avDir2youCoef;
	// ������� ����������� ��� �������� �������� ����� ����� "������" ����������
	float _angle_coef;
};
