@echo off

l:\csclCvsTools\CVSNT\cvs -N -q -d :local:L:\csclCvs commit -R
if errorlevel 1 goto update

l:
cd L:\collaborative-nir\program\Common
L:\csclCvsTools\CVSNT\cvs -N -d :local:L:\csclCvs update

echo 
echo �������� � ��� ࠡ��ᯮᮡ���� �易���� �஥�⮢

exit

:update
l:\csclCvsTools\CVSNT\cvs -N -d :local:L:\csclCvs update
echo 
echo � �஥�� �뫨 ���ᥭ� ���������. �������� � ��� ࠡ��ᯮᮡ����