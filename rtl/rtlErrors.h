#pragma once

#define RtlError unsigned int

#define	RTLERROR_NOERROR				0
#define	RTLERROR_CONVERTERROR			1			/*������ ��� �����������*/

#define	RTLERROR_UNRECOGNIZEDERROR		0xFFFF		/*�����-�� ������*/

#define RTLERROR_FILE_ACCESS_DENIDED     2           /*������ ������� � ����� ��� ��������*/

#define RTLERROR_FOLDER_ACCESS_DENIDED   3			 /*������ ������� � ����� ��� ��������*/

#define RTLERROR_NULL_PATH				 4			/*������ ���� � �����*/