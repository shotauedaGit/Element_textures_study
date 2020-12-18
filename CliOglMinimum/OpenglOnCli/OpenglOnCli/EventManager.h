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

    DET element_textures;
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
  
  void keyDown1(OglForCLI* ogl);
  void keyDown2(OglForCLI* ogl);
  void keyDown3(OglForCLI* ogl);
  void keyDown4(OglForCLI* ogl);
  void keyDown5(OglForCLI* ogl);
  void keyDown_Space(OglForCLI* ogl);

  void report_Key_Down_to_DET(OglForCLI* ogl, int keycode);
  void report_Key_Up_to_DET(OglForCLI* ogl,int keycode);


  void DrawScene();

  EVec3d GetWorldCoord(int x, int y,int w,int h);
  //double GetDepth(int x, int y);
};



#pragma managed

