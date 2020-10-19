#include "stdafx.h"

#include "COMMON\tmath.h"
#include "COMMON\timageloader.h"

#include "COMMON\stdc++.h"
#include <cmath>
#include "MainForm.h"
#include "EventManager.h"

//todo 
//
// MouseListener にredraw --> timerでイベント起動
// Ballをクラス化 
// 移動・回転シミュレーション 
// シーンを描く TODO --> みんな
// 


//クラス名.h
//クラス名.cpp

SolidBall::SolidBall( EVec3f pos )
{
  m_radius    = 3    ; //cm
  m_mass      = 170  ; //g
  m_position  = pos  ;
  m_velocity  = EVec3f(0,0,0);
  m_angle     = EVec3f(0,0,0);
  m_anglevelo = EVec3f(0,0,0);
}

SolidBall::SolidBall(EVec3f pos , float radius)
{
    m_radius = radius; //cm
    m_mass = 170; //g
    m_position = pos;
    m_velocity = EVec3f(0, 0, 0);
    m_angle = EVec3f(0, 0, 0);
    m_anglevelo = EVec3f(0, 0, 0);
}

SolidBall::~SolidBall( )
{
}



void SolidBall::Step( float h )
{
  static EVec3f Gravity = EVec3f(0,-30,0);
  
  // a = G 
  // dv/dt = a  --> dv = a dt
  // dx/dt = v  --> dx = v dt

  /*
  if (m_position.y() < m_radius)m_velocity.y()=0.0f;
  else m_velocity += h * Gravity   ;
  */
  
  m_position += h * m_velocity; 
  
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
  


void SolidBall::Draw( )
{
  //todo 
  // m_angle --> Matrxi3x3 m

  //半径rの球を各
  glPushMatrix();
  glTranslatef( m_position[0], m_position[1], m_position[2] );
  //glMultiMat3d(m.data());
  DrawSphere(  10, 10, m_radius );
  glPopMatrix();
}


static EVec3f cursor_p, cursor_d;
void EventManager::BtnDownLeft  (int x, int y, OglForCLI *ogl)
{
    if (pastkey == 49)return;

  m_btn_left = true;
  ogl->BtnDown_Trans( EVec2i(x,y) );
  ogl->GetCursorRay( EVec2i(x,y), cursor_p, cursor_d);

  if (executeIter)executeIter = false;
  else executeIter = true;

  /*
  int L = 20;
  float mx = 3, mn = 2;
  for (int i = 0; i < 70; ++i) {
      float px = ((rand() % (2 * L * 100)) / 100.0) - 10.0;
      float pz = ((rand() % (2 * L * 100)) / 100.0) - 10.0;
      float R = ((rand() % 100) / 100.0) * (mx - mn) + mn;
      m_balls.push_back(SolidBall(EVec3f(px, 0, pz), R)); //ボールを新たに発生させる
      std::cout << "  px:" << px << "  pz:" << pz << std::endl;
  }

  */

  //if (m_cvt.isFinished == false)m_cvt.Randomize();
  //m_cvt.Randomize();
  //if (m_cvt.isFinished == false)m_cvt.IterStep();
  
  //if (m_cvt.doneDelauny < (m_cvt.nV)-1) {
  //    int tgt = m_cvt.doneDelauny;
  //   m_cvt.DBG_idx_DelTri(tgt + 1);
  //}
  //else {
  //    int tgt = m_cvt.doneVolonoi;
  //    m_cvt.DBG_idx_CentVolo(tgt + 1);
  //}
  

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

  //if (m_cvt.isFinished == false)m_cvt.Randomize();
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
}


void EventManager::KeyDown_1(OglForCLI* ogl) {
    if (pastkey == 49)return;

    m_cvt.InitTriangleVector();
    m_cvt.DelaunayTrianglaion();
    pastkey = 49;
}

void EventManager::KeyDown_2(OglForCLI* ogl) {
    if (pastkey == 50)return;

    m_cvt.VCpoints = m_cvt.CentroidVoronoi();
    m_cvt.AssignCentroid();
    pastkey = 50;
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
  const static float ambi[4] = { 1.0f, 0.2f, 0, 1.0f };
  const static float spec[4] = { 0, 0, 0, 0.3f };
  const static float shin[1] = { 64.0f };


  const static float diffG[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
  const static float ambiG[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

  const static float diffGB[4] = { 0.5f, 0.9f, 0.9f, 1.0f };
  const static float ambiGB[4] = { 0.3f, 0.8f, 0.8f, 1.0f };
  
  //*
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  //glEnable(GL_LIGHT1);
  //glEnable(GL_LIGHT2);
  //*/

  //*
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR , spec);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE  , diffG);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT  , ambiG);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shin);
  //*/

  //render floor 
  glEnable( GL_CULL_FACE );
  glDisable(GL_CULL_FACE );

  

  
  //glLineWidth(1);
  glBegin(GL_TRIANGLES);
  glNormal3f(0,1,0);

  float w = m_cvt.width;
  float h = m_cvt.height;

  /*
  glVertex3f(-w, 0, -h);
  glVertex3f(w, 0, -h);
  glVertex3f(w, 0, h);

  glVertex3f(w, 0, h);
  glVertex3f(-w, 0, h);
  glVertex3f(-w, 0, -h);
  */

  /*
  glVertex3f(-w, 0, -h);
  glVertex3f(w, 0, -h);

  glVertex3f(w, 0, -h);
  glVertex3f(w, 0, h);

  glVertex3f(w, 0, h);
  glVertex3f(-w, 0, h);

  glVertex3f(-w, 0, h);
  glVertex3f(-w, 0, -h);
  */

  /*
  glVertex3f(-FLOOR_WIDTH,0, -FLOOR_LENGTH);
  glVertex3f( FLOOR_WIDTH,0, -FLOOR_LENGTH);

  glVertex3f( FLOOR_WIDTH,0, -FLOOR_LENGTH);
  glVertex3f( FLOOR_WIDTH,0, FLOOR_LENGTH);

  glVertex3f( FLOOR_WIDTH, 0, FLOOR_LENGTH);
  glVertex3f(-FLOOR_WIDTH, 0, FLOOR_LENGTH);

  glVertex3f(-FLOOR_WIDTH, 0, FLOOR_LENGTH);
  glVertex3f(-FLOOR_WIDTH,0, -FLOOR_LENGTH);
  */

  glEnd();

  bool DBG = false; 
  bool showDelaunay = false;
  //showDelaunay = true;
  
  int lim;
  //if (m_cvt.doneDelauny <= m_cvt.nV-1)lim = m_cvt.doneDelauny;

  //* //****************************  質感設定??  *************************************
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffG);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambiG);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shin);
  //*/


  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, txHdl);

  //for (int i = 0; i < m_cvt.nV; ++i) m_balls[i].Draw();//*******************  SolidXXXX 描画完了  **********************
  for (int i = 0; i < m_cvt.nV; ++i) m_elements[i].Draw();//*******************  SolidXXXX 描画完了  **********************


  {
      /*
      glPushMatrix();
      glTranslatef(0,0,0);
      glRotatef(90,0,0,1);
      //Draw Rect
      float H = 50.0;
      float W = 50.0;

      {
          //GLuint image = loadBMP_custom("./aaa.bmp");


          glBegin(GL_TRIANGLES);
          // front
          // triangle 0
          //glNormal3f(0.0f, 0.0f, 1.0f); 
          glTexCoord2f(0.0f, 1.0f);
          glVertex3f(-H / 2, 0.0f, -W / 2);

          //glNormal3f(0.0f, 0.0f, 1.0f); 
          glTexCoord2f(0.0f, 0.0f);
          glVertex3f(H / 2, 0.0f, -W / 2);
          //glNormal3f(0.0f, 0.0f, 1.0f); 
          glTexCoord2f(1.0f, 1.0f);
          glVertex3f(-H / 2, 0.0f, W / 2);

          // triangle 1
          //glNormal3f(0.0f, 0.0f, 1.0f); 
          glTexCoord2f(0.0f, 0.0f);
          glVertex3f(H / 2, 0.0f, -W / 2);
          //glNormal3f(0.0f, 0.0f, 1.0f); 
          glTexCoord2f(1.0f, 1.0f);
          glVertex3f(-H / 2, 0.0f, W / 2);


          //glNormal3f(0.0f, 0.0f, 1.0f); 

          glTexCoord2f(1.0f, 0.0f);
          glVertex3f(H / 2, 0.0f, W / 2);

          glEnd();
      }

      //
      glPopMatrix();

      */
  
  }
  glDisable(GL_TEXTURE_2D);



  if (showDelaunay) {
      for (int i = 0; i < m_cvt.triangles.size(); ++i) {
          if (m_cvt.triangles[i].exist) {
              Triangle ti = m_cvt.triangles[i];

              if (DBG)cout << "GRAW " << i << " th triangle" << endl;

              Point tA = ti.A; EVec3f vA(tA.x, 0.1, tA.y);
              Point tB = ti.B; EVec3f vB(tB.x, 0.1, tB.y);
              Point tC = ti.C; EVec3f vC(tC.x, 0.1, tC.y);
              /*
              float cR = (float)rand() / RAND_MAX;
              float cG = (float)rand() / RAND_MAX;
              float cB = (float)rand() / RAND_MAX;

              */

              glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ti.MyCol);
              glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ti.MyCol);
              //glColor3f(cR, cG, cB);
              glLineWidth(1);
              glBegin(GL_LINES);
              glVertex3f(tA.x, 0.1, tA.y);
              glVertex3f(tB.x, 0.1, tB.y);
              glEnd();
              glBegin(GL_LINES);
              glVertex3f(tB.x, 0.1, tB.y);
              glVertex3f(tC.x, 0.1, tC.y);
              glEnd();
              glBegin(GL_LINES);
              glVertex3f(tC.x, 0.1, tC.y);
              glVertex3f(tA.x, 0.1, tA.y);
              glEnd();

          }
      }
  }

  if (DBG)cout << "********Draw*********" << endl;

  /*
  glLineWidth(10);
  glBegin(GL_LINES );
  EVec3f tmp = cursor_p + 100*cursor_d;
  glVertex3fv ( cursor_p.data() );
  glVertex3fv ( tmp.data() );
  glEnd();
  */

}




void EventManager::Step()
{
  //todo処理
  //std::cout << "step";
  /*

  for ( auto &it : m_balls ) 
  {
    it.Step( 0.01 );
    //移動計算 OK
    //回転も TODO 井尻
  }

  //球の重なり具合から、球の速度を求める
  float eps = 0.3f;
  for (auto &ball : m_balls)ball.SetVelo(EVec3f(0, 0, 0));
  for ( int i=0; i < (int)m_balls.size(); ++i )
  {
    for ( int j=0; j < (int)m_balls.size(); ++j )
    {
      if (j == i)continue;
      EVec3f f = m_balls[i].GetPos() - m_balls[j].GetPos();
      
      float ri = m_balls[i].GetR();
      float rj = m_balls[i].GetR();

      float Dist_i2j = f.norm();
      float k = (ri + rj) - Dist_i2j;
      
      EVec3f nvi = m_balls[i].GetVel();
      EVec3f nvj = m_balls[j].GetVel();

      if (k > 0) {
          nvi += f*k;
          nvj -= f*k;
      }
      
      m_balls[i].SetVelo(nvi);
      m_balls[j].SetVelo(nvj);
    }

    //std::min()
  }
  for (auto& ball : m_balls) {
      //if (ball.GetVel().norm() < eps)ball.SetVelo(EVec3f(0, 0, 0));
      float n2 = ball.GetVel().norm()/(0.6f);
      //ball.SetVelo(ball.GetVel());
  }
  */






  //m_cvt.IterStep();
  OpenglOnCli::MainForm_RedrawPanel();
  bool DBG = false;

  if (executeIter) {
      if (m_cvt.isFinished == false)m_cvt.IterStep();
  }

  for (int i = 0; i < m_cvt.nV; ++i) {
      //m_balls[i].SetPos(m_cvt.points[i].x, m_cvt.points[i].y); //******************************  要素の位置更新  **********************
      m_elements[i].SetPos(m_cvt.points[i].x, m_cvt.points[i].y);
  }

  if (DBG)cout << "********Step*********" << endl;
}
