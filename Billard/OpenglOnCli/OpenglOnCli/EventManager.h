#pragma once
#include "COMMON/OglForCLI.h"
#include "CVT.h"

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

  

  SolidBall(EVec3f pos);
  SolidBall( EVec3f pos , float radius);

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
  EVec3f GetPos() { return m_position; }
  EVec3f GetVel() { return m_velocity; }
  float GetR(){ return m_radius; }
  void SetVelo(const EVec3f &velo){ m_velocity = velo; }
  void SetPos(const float x,const float y) { m_position = EVec3f(x, 0.0f, y); }
};




//Event Managerはここから


class EventManager
{
private:
  EventManager();

  bool m_btn_right, m_btn_left, m_btn_middle;
  std::vector<SolidBall> m_balls;//Solid ball
  CVT m_cvt;

public: 

  bool executeIter = false;
  int pastkey = 50;

  static EventManager* GetInst(){
    static EventManager p;
    return &p;
  }


  void BtnDownLeft  (int x, int y, OglForCLI *ogl);
  void BtnDownMiddle(int x, int y, OglForCLI *ogl);
  void BtnDownRight (int x, int y, OglForCLI *ogl);
  void BtnUpLeft    (int x, int y, OglForCLI *ogl);//押上の時に表示させたいなぁ.....（CVTに収束判定を聞いて、収束していたら再びランダムでやり始める）
  void BtnUpMiddle  (int x, int y, OglForCLI *ogl);
  void BtnUpRight   (int x, int y, OglForCLI *ogl);
  void MouseMove    (int x, int y, OglForCLI *ogl);

  void KeyDown_1(OglForCLI* ogl);
  void KeyDown_2(OglForCLI* ogl);

  void DrawScene();//再描画、収束するまで描画しないっていう指定もしたい
  void Step();//１フレームごとの処理(収束するまではイテレーション、そうでないときは何もしない)



};



#pragma managed

