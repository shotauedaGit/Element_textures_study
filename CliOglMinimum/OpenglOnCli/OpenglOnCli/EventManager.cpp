#include "stdafx.h"

#include "COMMON\tmath.h"
#include <cmath>
#include "MainForm.h"
#include "EventManager.h"

#include <vector>
#include <list>



//クラス名.h
//クラス名.cpp

//.h
class MyVec3
{
private:
  float m_data[3];
  int *m_test;
     
public :
  MyVec3(float x = 0, float y = 0, float z = 0) 
  {
    m_data[0] = x;
    m_data[1] = y;
    m_data[2] = z;
    m_test = new int[5];
  }

  ~MyVec3()
  {
    delete[] m_test;
  }

  MyVec3(const MyVec3 &src)
  { 
    this->m_data[0] = src.m_data[0];
    this->m_data[1] = src.m_data[1];
    this->m_data[2] = src.m_data[2];
    m_counter++;
  }
  
  MyVec3 operator+(const MyVec3 v)
  {
    MyVec3 p;
    p.m_data[0] = this->m_data[0] + v.m_data[0];
    p.m_data[1] = this->m_data[1] + v.m_data[1];
    p.m_data[2] = this->m_data[2] + v.m_data[2];
    return p;
  }
  
  void Trace();
  void Trace(int a)
  {
    printf("aaaakkkk");
  }

  static int m_counter;
  static void StaticFunc(){
    //m_publ = 0;
    m_counter = 0;
  }

  static void SetCountZero(){
    m_counter = 0;
  }

  void func();
  

};

// classとは
// フィールド変数(private/public)，
// フィールド関数(private/public)
// static 変数 static 関数
// constructor

int MyVec3::m_counter = 0;




//.cpp
void MyVec3::Trace(){
  printf("aaaa %f %f %f\n", m_data[0], m_data[1], m_data[2]);
}


void MyVec3::func()
{

}


void func( int a, int &b, int *c )
{
  //値渡し，参照渡し，ポインタ渡し
  a = 10;
  b = 10;
  if( c != 0) *c = 10;
}







EventManager::EventManager()
{
  std::cout << "EventManager constructor\n";
  //
  //MyVec3 v1(1,2,3), v2(3,4,5);
  //v1.Trace();
  //v2.Trace();
  //MyVec3 v3 = v1 + v2; //オペレータオーバーロード
  //v3.Trace();
  //v3.Trace(1);
  //
  ////v1.m_priv = 10; 無理
  // 
  //v1.StaticFunc();
  //MyVec3::StaticFunc();

  ////MyVec3 p(1,2,3);
  ////MyVec3 q(1,2,3);
  ////auto a = p + q;
  ////p.Trace();
  ////q.Trace();
  ////a.Trace();
  //
  ////スコープ
  //{
  //  float a = 10;
  //  MyVec3 *aaa = new MyVec3();
 
  //  delete aaa;
  //}
  //
  //std::vector<int> array_int;
  //array_int.push_back(1);
  //array_int.push_back(2);
  //array_int.push_back(3);
  //array_int.push_back(0);
  //array_int.push_back(13);
  //array_int.push_back(11);
  //array_int.push_back(10);
  //
  //std::list<int> list_int;
  //list_int.push_back(1);
  //list_int.push_back(10);
  //list_int.push_back(122);
  //list_int.push_back(133);
  //list_int.push_back(144);
  //list_int.push_back(155);
  //list_int.push_back(1);
  //
  //array_int[4];
  //list_int.pop_front();

  //int a1 =1, a2 = 1, a3 = 1;
  //func(a1,a2,&a3);
  ////a1 == 1, a2 == 10, a3 == 10 
  //  
  m_btn_right = m_btn_left = m_btn_middle = false;
}





/*




class MyVec3
{
private: 
  //ここに各変数はprivate変数になり外からは見えない
  int m_priv;
  
public: 
  //ここに各変数はpublic変数になり外から参照・修正可能
  int m_publ;

  float m_data[3];
  
  //constructrue
  MyVec3(float x = 0, float y = 0, float z = 0)
  {
    m_data[0] = x;
    m_data[1] = y;
    m_data[2] = z;
  }

  MyVec3(const MyVec3 &src)
  {
    //todo copy
    
  }

  MyVec3 &operator=(const MyVec3 &src)
  {
    //todo copy
    return *this;
  }

  //オペレーターオーバーロード
  MyVec3 operator+(const MyVec3 &v){
    MyVec3 p;
    p.m_data[0] = this->m_data[0] + v.m_data[0];
    p.m_data[1] = this->m_data[1] + v.m_data[1];
    p.m_data[2] = this->m_data[2] + v.m_data[2];
    return p;
  }

  void Trace()
  {
    std::cout << m_data[0] << m_data[1] << m_data[2];
  }
};




*/











void EventManager::BtnDownLeft  (int x, int y, OglForCLI *ogl)
{
  m_btn_left = true;
  ogl->BtnDown_Trans( EVec2i(x,y) );
} 

void EventManager::BtnDownMiddle(int x, int y, OglForCLI *ogl)
{
  m_btn_middle = true;
  ogl->BtnDown_Zoom( EVec2i(x,y) );
}

void EventManager::BtnDownRight (int x, int y, OglForCLI *ogl)
{
  m_btn_right = true;
  ogl->BtnDown_Rot( EVec2i(x,y) );
}



void EventManager::BtnUpLeft  (int x, int y, OglForCLI *ogl)
{
  m_btn_left = false;
  ogl->BtnUp();
}

void EventManager::BtnUpMiddle(int x, int y, OglForCLI *ogl)
{
  m_btn_middle = false;
  ogl->BtnUp();
}

void EventManager::BtnUpRight (int x, int y, OglForCLI *ogl)
{
  m_btn_right = false;
  ogl->BtnUp();
}

void EventManager::MouseMove    (int x, int y, OglForCLI *ogl)
{
  if ( !m_btn_right && !m_btn_middle && !m_btn_left) return;

  ogl->MouseMove( EVec2i(x,y) );
  OpenglOnCli::MainForm_RedrawPanel();
}






static EVec3f GetPosOnSphere( const float &phi, const float &theta)
{
  return EVec3f( std::cos(phi) * std::cos(theta), 
                 std::sin(phi),
                -std::cos(phi) * std::sin(theta) );
}


static void DrawSphere(int reso_i, int reso_j, float radius)
{
  EVec3f *norms = new EVec3f[reso_i * reso_j];
  EVec3f *verts = new EVec3f[reso_i * reso_j];

  for ( int i = 0; i < reso_i; ++i ) 
  {
    for ( int j = 0; j < reso_j; ++j )
    {

      norms[j + i * reso_j ] = GetPosOnSphere( M_PI * i / (reso_i - 1.0f) - M_PI / 2.0f,    
                                           2 * M_PI * j / (reso_j - 1.0f) );
      verts[j + i * reso_j] = radius * norms[ j + i * reso_j ];
    }
  }

  glBegin(GL_TRIANGLES);
  for ( int i = 0; i < reso_i-1; ++i ) 
  {
    for ( int j = 0; j < reso_j-1; ++j )
    { 
      int idx0 = reso_j*  i   + j, idx1 = reso_j*  i  + j+1;
      int idx2 = reso_j*(i+1) + j, idx3 = reso_j*(i+1)+ j+1;
      glNormal3fv( norms[ idx0 ].data() ); glVertex3fv( verts[ idx0 ].data() );
      glNormal3fv( norms[ idx2 ].data() ); glVertex3fv( verts[ idx2 ].data() );
      glNormal3fv( norms[ idx3 ].data() ); glVertex3fv( verts[ idx3 ].data() );

      glNormal3fv( norms[ idx0 ].data() ); glVertex3fv( verts[ idx0 ].data() );
      glNormal3fv( norms[ idx3 ].data() ); glVertex3fv( verts[ idx3 ].data() );
      glNormal3fv( norms[ idx1 ].data() ); glVertex3fv( verts[ idx1 ].data() );
    }
  }
  glEnd();
     

  delete[] verts;
  delete[] norms;
}
  



void EventManager::DrawScene()
{
  //ここにレンダリングルーチンを書く
  glBegin(GL_LINES );
  glColor3d(1,0,0); glVertex3d(0,0,0); glVertex3d(10,0,0);
  glColor3d(0,1,0); glVertex3d(0,0,0); glVertex3d(0,10,0);
  glColor3d(0,0,1); glVertex3d(0,0,0); glVertex3d(0,0,10);
  glEnd();

  
  const static float diff[4] = { 1.0f, 0.2f, 0, 0.3f };
  const static float ambi[4] = { 1.0f, 0.2f, 0, 0.3f };
  const static float spec[4] = { 1,1,1,0.3f };
  const static float shin[1] = { 64.0f };

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_LIGHT2);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR  , spec);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE  , diff);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT  , ambi);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shin);
  

  glEnable( GL_CULL_FACE );
  glCullFace(GL_FRONT );

  glBegin(GL_TRIANGLES );
  glNormal3f(0,0,1);
  glVertex3f(0,0,0);
  glVertex3f(1,0,0);
  glVertex3f(1,1,0);

  glNormal3f(0,1,0);
  glVertex3f(0,0,0);
  glVertex3f(1,0,0);
  glVertex3f(1,0,1);
  glEnd();

  DrawSphere(20,20,3.0f);
 
}


