#pragma once
#include "COMMON/OglForCLI.h"

#pragma unmanaged 


#define FLOOR_Y 0
#define FLOOR_WIDTH   80
#define FLOOR_LENGTH 145



class SolidBall
{
private:
  float m_radius;
  float m_mass  ;
  
  EVec3f m_position;
  EVec3f m_velocity;

  EVec3f m_angle    ;
  EVec3f m_anglevelo;

public: 
  SolidBall( EVec3f pos );
  ~SolidBall();
  SolidBall( const SolidBall &src) { Copy(src); }
  SolidBall &operator=(const SolidBall &src){ Copy(src); return *this;}
  
  void Copy(const SolidBall &src) 
  {
    this->m_radius    = src.m_radius;
    this->m_mass      = src.m_mass;
    this->m_position  = src.m_position;
    this->m_velocity  = src.m_velocity;
    this->m_angle     = src.m_angle;
    this->m_anglevelo = src.m_anglevelo;  
  }
  
  void Draw();
  void Step( float h );
  EVec3f GetPos(){ return m_position; }
  void SetVelo(const EVec3f &velo){ m_velocity = velo; }
};







class EventManager
{
private:
  EventManager();

  bool m_btn_right, m_btn_left, m_btn_middle;
  
  std::vector<SolidBall> m_balls;
public: 
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

  void Step();
};



#pragma managed

