// Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2
//
// $RCSfile: mlRMMLFlash.h,v $
// $Revision: 1.3 $
// $State: Exp $
// $Locker:  $
//
// last change: $Date: 2009/04/22 04:54:38 $
//              $Author: tandy $
//
// Copyright (C) 2004 ���
//
// ���� ���� �������� ������ ��� (�������� ����������� �������)
//
//////////////////////////////////////////////////////////////////////

#ifndef __rmml_Flash_Included__
#define __rmml_Flash_Included__


#define RMMLTN_Flash L"flash"

namespace rmml
{
/** !!!! ����� !!!!
 *  ����� ��� ��� ������ ������������ ���� �����
 *  � JavaScript'�� ���������� �������
 *  �-�� mlRMMLFlash::InitJSClass(cx,obj);
 */

#define EVNM_onFSCommand L"onFSCommand"

  // ����� ��� ��������� Flash
  class mlRMMLFlash : 
                      public mlRMMLElement,
                      public mlJSClass,
                      public mlRMMLLoadable,
                      public mlRMMLVisible,
                      public mlRMMLContinuous,
					  public mlIFlash,
					  public moIFlash
  {
  public:
    mlRMMLFlash();
	void destroy() { MP_DELETE_THIS }
    virtual ~mlRMMLFlash();

  // ���������� mlJSClass
    MLJSCLASS_DECL
  private:
	  enum {
		  JSPROP_scaleMode = 1, // Scale mode (0=ShowAll, 1= NoBorder, 2 = ExactFit, 3 = NoScale?). This is the same as the SCALE parameter.
		  JSPROP_wMode, // Window Mode property (0 = window, 1 = opaque, 2 = transparent) [Window] movie plays in its own rectangular window on a web page. [Opaque] the movie hides everything on the page behind it . [Transparent] the background of the HTML page shows through all transparent portions of the movie, this may slow animation performance. 
		  EVID_onFSCommand,
	  };
	  int scaleMode;
	  int wMode;

	JSFUNC_DECL(setVariable)
	JSFUNC_DECL(getVariable)
	JSFUNC_DECL(callFunction)

	moIFlash* getFlash(){ return (mpMO == NULL)?NULL:mpMO->GetIFlash(); }

  // ���������� mlRMMLElement
    MLRMMLELEMENT_IMPL
    virtual mlresult CreateMedia(omsContext* amcx);
    virtual mlresult Load();
    virtual mlRMMLElement* Duplicate(mlRMMLElement* apNewParent);

  // ���������� mlILoadable
    MLILOADABLE_IMPL

  // ���������� mlIVisible
    MLIVISIBLE_IMPL

  // ���������� mlRMMLContinuous
    MLICONTINUOUS_IMPL

  // ���������� mlIFlash
	virtual void onFSCommand(const wchar_t *command, const wchar_t *args);
	virtual int GetWModeOnCreate();

  // ���������� moIFlash
	virtual void setVariable(const wchar_t *var, const wchar_t *value);

	virtual int getScaleMode();
	virtual void setScaleMode(int);

	virtual int getWMode();
	virtual void setWMode(int);

	// ���������� ����� ������
	virtual long getVariable(const wchar_t *var, mlOutString& sRetVal);
	// ���������� ����� ����������
	virtual long callFunction(const wchar_t *request, mlOutString& sRetVal);

  };
}

#endif // __rmml_Flash_Included__
