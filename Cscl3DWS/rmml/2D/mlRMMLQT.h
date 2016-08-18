// Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2
//
// $RCSfile: mlRMMLQT.h,v $
// $Revision: 1.4 $
// $State: Exp $
// $Locker:  $
//
// last change: $Date: 2009/07/16 13:32:57 $
//              $Author: linda $
//
// Copyright (C) 2004 ���
//
// ���� ���� �������� ������ ��� (�������� ����������� �������)
//
//////////////////////////////////////////////////////////////////////

#ifndef __rmml_QuickTime_Included__
#define __rmml_QuickTime_Included__

#include "mlRMMLIQT.h"
#include <string>

namespace rmml
{
  struct moIQT;

#define EVNM_enterHotSpot    L"enterHotSpot"
#define EVNM_leaveHotSpot    L"leaveHotSpot"
#define EVNM_triggerHotSpot  L"triggerHotSpot"

#define EVNM_enterNode       L"enterNode"
#define EVNM_leaveNode       L"leaveNode"

  /** ����� ��� ��������� Flash */
  class mlRMMLQT : 
                    public mlRMMLElement,
                    public mlJSClass,
                    public mlRMMLLoadable,
                    public mlRMMLVisible,
                    public mlRMMLContinuous,
                    public mlIQT
  {
  public:
    mlRMMLQT();
	void destroy() { MP_DELETE_THIS }
    virtual ~mlRMMLQT();

    mlRMMLElement* GetElem_mlRMMLQT();

  // ���������� mlJSClass
    MLJSCLASS_DECL

  // ���������� mlMedia
    virtual void* GetInterface(long iid);

  // ���������� mlRMMLElement
    MLRMMLELEMENT_IMPL
	virtual EventSpec* GetEventSpec2(){ return _EventSpec; } 
    virtual mlresult CreateMedia(omsContext* amcx);
    virtual mlresult Load();
    virtual mlRMMLElement* Duplicate(mlRMMLElement* apNewParent);

  // ���������� mlILoadable
    MLILOADABLE_IMPL

  // ���������� mlIVisible
    MLIVISIBLE_IMPL

  // ����������  mlRMMLContinuous
    MLICONTINUOUS_IMPL

  protected:
    enum {
      JSPROP_nodeID,
      JSPROP_hotSpotID,
      JSPROP_nodeType,
      JSPROP_panAngle,
      JSPROP_tiltAngle,
      JSPROP_fieldOfView,

      EVID_enterHotSpot,
      EVID_leaveHotSpot,
      EVID_triggerHotSpot,

      EVID_enterNode,
      EVID_leaveNode,
	  EVID_updateTime
    };

  public:
    std::wstring getHotType(hotspot hotSpotID = -1);
//    void triggerHotSport(hotsport hotSportID);
  protected:
    bool change_Node(nodeid nodeID_);
    void change_panAngle(double panAngle_);
    void change_tiltAngle(double tiltAngle_);
    void change_fieldOfView(double fieldOfView_);

// ����� ��������������� ������� ���������� �����
    virtual void enterHotSport(hotspot hotSportID_);
    virtual void leaveHotSport(hotspot hotSportID_);
    virtual bool triggerHotSport(hotspot hotSportID_);

    virtual void enterNode(nodeid nodeID_);
    virtual void leaveNode(nodeid nodeID_);


// ������� ������� ���������� �� mme, ���-�� ���������� � ���������� �����
// ���������� �������� �������, ����������� �� ���������� mlIQT
    virtual void set_Node(nodeid nodeID_);
    virtual void set_hotSpot(hotspot hotSpotID_);
    virtual void set_panAngle(double panAngle_);
    virtual void set_tiltAngle(double tiltAngle_);
    virtual void set_fieldOfView(double fieldOfView_);

  protected:
    moIQT *getQuickTime();

  protected:
    nodeid
      nodeID;       // ������������� �������� ����
    MP_WSTRING
      nodeType;     // ��� �������� ����
    hotspot
      hotSpotID;    // �������������, �������� ����
                    // ��� ������� ������ ���������� ������
    jsdouble
      panAngle,     // ������� �� ����������� � ��������
      tiltAngle,    // ������� �� ��������� � ��������
      fieldOfView;  // ����������� � "�����"
  };
}

#endif // __rmml_QuickTime_Included__