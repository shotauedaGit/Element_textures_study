#pragma once
#include "COMMON/OglForCLI.h"
#include "DET.h"

#pragma unmanaged 


class EventManager
{
private:
  EventManager();

  bool m_btn_right, m_btn_left, m_btn_middle;
 
public: 

    DET sample_element_texture;
    DiscreteElement tmp;

  static EventManager* GetInst(){
    static EventManager p;
    return &p;
  }

  void BtnDownLeft  (int x, int y, OglForCLI *ogl);
  void BtnDownMiddle(int x, int y, OglForCLI *ogl);
  void BtnDownRight (int x, int y, OglForCLI *ogl);
  void BtnUpLeft    (int x, int y, OglForCLI *ogl);
  void BtnUpMiddle  (int x, int y, OglForCLI *ogl);
  void BtnUpRight   (int x, int y, OglForCLI *ogl);
  void MouseMove    (int x, int y, OglForCLI *ogl);
  
  void DrawScene();

  EVec3d GetWorldCoord(int x, int y,int w,int h);
  //double GetDepth(int x, int y);
};



#pragma managed

