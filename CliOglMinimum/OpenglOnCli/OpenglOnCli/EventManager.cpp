#include "stdafx.h"

#include "COMMON\tmath.h"
#include <cmath>
#include "MainForm.h"
#include "EventManager.h"

#include <vector>
#include <list>
using namespace std;

EventManager::EventManager()
{
  std::cout << "EventManager constructor\n";
  m_btn_right = m_btn_left = m_btn_middle = false;
  glEnable(GL_DEPTH_TEST);
  //glEnable(GL_BLEND);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
  //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  glShadeModel(GL_SMOOTH);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  //glEnable(GL_BLEND);
  //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

  element_textures.LoadTexture();
  element_textures.SetTexture("e0.png", element_textures.texID[0]);
  element_textures.SetTexture("e1.png", element_textures.texID[1]);
  element_textures.SetTexture("e2.png", element_textures.texID[2]);
  element_textures.SetTexture("e3.png", element_textures.texID[3]);
  element_textures.SetTexture("e4.png", element_textures.texID[4]);
  element_textures.SetTexture("e5.png", element_textures.texID[5]);
  element_textures.SetTexture("e6.png", element_textures.texID[6]);
  element_textures.SetTexture("default_sample_element.png", element_textures.texID[7]);
  tmp = element_textures.Abst_elements[0];
  element_textures.TEST(); //put loaded elements
  
}

void EventManager::keyDown1(OglForCLI* ogl) { element_textures.synthesis(); OpenglOnCli::MainForm_RedrawPanel(); cout << "key" << 1 << " pressed!!" << endl; }
void EventManager::keyDown2(OglForCLI* ogl) { cout << "key" << 2 << " pressed!!" << endl; }
void EventManager::keyDown3(OglForCLI* ogl) { cout << "key" << 3 << " pressed!!" << endl; }
void EventManager::keyDown4(OglForCLI* ogl) { cout << "key" << 4 << " pressed!!" << endl; }
void EventManager::keyDown5(OglForCLI* ogl) { cout << "key" << 5 << " pressed!!" << endl; }

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
    double ogl_x;
    double ogl_y;
    double ogl_z;
    //cout << "(x,y) = (" << x << "," << y <<")  In Ogl : "<<ogl_x<<","<<ogl_y<<","<<ogl_z<<"  btn:  ";
    /*
    int w = ogl->win_w;
    int h = ogl->win_h;
    GetWorldCoord(x, y, w, h);
    cout << "(W,H) = ( "<< w<< " , " << h <<" )" ;

    cout << "  btn:  ";
    if (m_btn_right)cout << "right";
    if (m_btn_middle)cout << "middle";
    if (m_btn_left)cout << "left";
    */
    EVec3f Rpos, Rdir , Z0pos;
    ogl->GetCursorRay(x,y,Rpos,Rdir);

    //cout << "Ray : " << Rpos << "  ->  " << Rpos + Rdir;
    double Pz = Rpos.z(),Nz = Rdir.z();
    double k = -(Pz / Nz);
    Z0pos = Rpos + Rdir * k; //************************ マウスの z = 0上の座標 

    //cout << "..." << endl;
    //cout << Z0pos << endl;

    EVec2d cur_pos(Z0pos.x(),Z0pos.y());
    element_textures.pointer.point.set_pos(cur_pos); //マウスポインタの先のゲーム内座標

    if (!m_btn_right && !m_btn_middle && !m_btn_left) {
        //return;
    }
  ogl->MouseMove( EVec2i(x,y) );
  OpenglOnCli::MainForm_RedrawPanel();
}

//************  Solid sphere *************************************

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
static void DrawRect(double W,double H) {
    {


    
        glBegin(GL_TRIANGLES);
        // front
        // triangle 0
        //

        //glNormal3f(0.0f, 0.0f, 1.0f); 
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-H / 2, -W / 2, 0.0);

        //glNormal3f(0.0f, 0.0f, 1.0f); 
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(H / 2, -W / 2, 0.0);
        
        //glNormal3f(0.0f, 0.0f, 1.0f); 
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-H / 2, W / 2, 0.0);

        // triangle 1

        //glNormal3f(0.0f, 0.0f, 1.0f); 
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(H / 2, -W / 2,0.0);
        


        //glNormal3f(0.0f, 0.0f, 1.0f); 
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(H / 2, W / 2,0.0);

        //glNormal3f(0.0f, 0.0f, 1.0f); 
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-H / 2, W / 2, 0.0);

        

        /*
        //glNormal3f(0.0f, 0.0f, 1.0f); 
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-H / 2, 0.0,-W / 2);

        //glNormal3f(0.0f, 0.0f, 1.0f); 
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(H / 2,0.0,-W / 2);
        //glNormal3f(0.0f, 0.0f, 1.0f); 
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-H / 2,0.0, W / 2);

        // triangle 1
        //glNormal3f(0.0f, 0.0f, 1.0f); 
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(H / 2,0.0, -W / 2);
        //glNormal3f(0.0f, 0.0f, 1.0f); 
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-H / 2,0.0, W / 2);

        //glNormal3f(0.0f, 0.0f, 1.0f); 
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(H / 2,0.0, W / 2);
        */


        glEnd();

    }
}
static void Draw2DRect(double x, double y,double z, double w, double h) {
    glPushMatrix();

    if(z!=0)glTranslatef(x, y, -0.01);
    else glTranslatef(x, y, 0);

    DrawRect(w, h);
    glPopMatrix();
}
/*
double EventManager::GetDepth(int x,int y) {
    float z;
    GLint viewport[4]; // ビューポート

    // デバイス座標系とウィンドウ座標系の変換

    glReadBuffer(GL_BACK);

    glGetIntegerv(GL_VIEWPORT, viewport);
    glReadPixels(x, viewport[3] - y, 1, 1,
        GL_DEPTH_COMPONENT,
        GL_FLOAT,
        &z);

    

    return z;
}
*/
EVec3d EventManager::GetWorldCoord(int x, int y,int w,int h) {
    GLdouble model[16], proj[16];

    GLint view[4] = {0,0,w,h};
    GLfloat z;
    GLdouble wx, wy, wz;

    glReadBuffer(GL_BACK);

    glGetDoublev(GL_MODELVIEW_MATRIX, model);
    glGetDoublev(GL_PROJECTION_MATRIX, proj);

    

    glReadPixels(x, view[3] - y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);
    gluUnProject(x, view[3] - y, z, model, proj, view, &wx, &wy, &wz);

    cout << "(wx,wy,wz) = ( " << wx << "," << wy << "," << wz <<" )"<<endl;
    cout << " view(0,1,2,3) = (" << view[0] << "," << view[1] << "," << view[2] << "," << view[3] << endl;
    cout << "viewport[3]:" << view[3] << " x: " << x << " y: " << y << " depth: " << z << endl;

    return EVec3d(wx, wy, wz);
}

void EventManager::DrawScene()
{//ここにレンダリングルーチンを書く
  glEnable(GL_DEPTH_TEST);
  /*
  glLineWidth(10.0);
  glBegin(GL_LINES );
  glColor3d(1, 0, 0); glVertex3d(-gr, 0, 0); glVertex3d(gr, 0, 0);
  glColor3d(0, 1, 0); glVertex3d(0, -gr, 0); glVertex3d(0, gr, 0);
  glColor3d(0, 0, 1); glVertex3d(0, 0, -gr); glVertex3d(0, 0, gr);
  glEnd();
  */
  {//DrawBackGround
      EVec3d mycolor(1, 1, 1);
      double H = 20;double W = 20;

      glBegin(GL_TRIANGLES);
      glColor4d(mycolor.x(), mycolor.y(), mycolor.z(), 1);

      // front
      glVertex3f(-H / 2, -W / 2, -0.2);
      glVertex3f(H / 2, -W / 2, -0.2);
      glVertex3f(-H / 2, W / 2, -0.2);

      // triangle 1
      glVertex3f(H / 2, -W / 2, -0.2);
      glVertex3f(H / 2, W / 2, -0.2);
      glVertex3f(-H / 2, W / 2, -0.2);
      glEnd();
  }
  glLineWidth(1.0);
  int gr = 10;
  /*
  for (int xi = -gr,i = 0; xi <= gr; xi++,i++) {
      for (int yi = -gr, j = 0; yi <= gr; yi++,j++){
          glBegin(GL_LINES);
          glColor3d(0, 0, 1); glVertex3d(xi, yi, -gr); glVertex3d(xi, yi, gr);
          glEnd();

          double r = (double)i / (2 * gr);
          double b = (double)j / (2 * gr);

          //glColor3d( r, 0.3, b);
          // Draw2DRect(xi, yi, 0, 0.75, 0.75);

      }
  }
  */
  double z = 0;
  for (int xi = -gr, i = 0; xi <= gr; xi++, i++) {
      glBegin(GL_LINES);
      glColor3d(0.5, 0, 0.5); glVertex3d(xi, -gr, z); glVertex3d(xi, gr, z);
      glEnd();
  }
  for (int yi = -gr, i = 0; yi <= gr; yi++, i++) {
      glBegin(GL_LINES);
      glColor3d(0, 0.5, 0); glVertex3d(-gr, yi , z); glVertex3d(gr,yi, z);
      glEnd();
  }

  //ここからの処理は、DETのInterfaceProcess()処理で閉じ込める予定、(eventmanager経由でボタンの状態をおしえれば不足はない)
  
  element_textures.getSelectedAbst_element_Idx();//いまはフレームの描画もここで
  element_textures.getSelected_Sample_element_Idx();//いまはフレームの描画もここで

  int selected_Abst_Idx = element_textures.selected_Abst_element_Idx;
  int selected_sample_Idx = element_textures.selected_sample_element_Idx;
  int cur_Abst_Idx = element_textures.current_Abst_element_Idx;
  if (selected_Abst_Idx != -1 && m_btn_left) { 
      if(cur_Abst_Idx == -1)element_textures.current_Abst_element_Idx = selected_Abst_Idx; 
  }
  else {  }
  //cout << "selected_Abst : " << selected_Abst_Idx << endl;
  //cout << "cur_Abst 1 : " << cur_Abst_Idx << endl;

  if (cur_Abst_Idx != -1 ) { 
      if (!m_btn_left) {//ボタンリリース時に
          if (selected_sample_Idx != -1) {
              
              //パレットからのコピー部分
              int txHdl = element_textures.Abst_elements[cur_Abst_Idx].type;
              double H = element_textures.Abst_elements[cur_Abst_Idx].H;
              double W  = element_textures.Abst_elements[cur_Abst_Idx].W;
              
              element_textures.sample_elements[selected_sample_Idx].type = txHdl;
              element_textures.sample_elements[selected_sample_Idx].H = H;
              element_textures.sample_elements[selected_sample_Idx].W = W;

              //手動の要素のidxを記憶
              element_textures.handmade_element_idx.push_back(selected_sample_Idx);
          }

          element_textures.current_Abst_element_Idx = -1;
      }
      element_textures.Abst_elements[cur_Abst_Idx].Draw_SelectedFlame(4);
  }

  //cout << "cur_Abst : " << cur_Abst_Idx << endl;
  //cout << "selected_sample : " << selected_sample_Idx << endl;

  //DETのelementDraw()処理
  element_textures.pointer.Draw();

  int NumOfAbst = element_textures.Abst_elements.size();
  int NumOfsample = element_textures.sample_elements.size();
  for (int i = 0; i < NumOfAbst - 1; ++i) {
      element_textures.Abst_elements[i].Draw();
  }

  for (int i = 0; i < NumOfsample; i++){
      if (i == 5) {
          float _h = element_textures.sample_elements[i].H;
          float _w = element_textures.sample_elements[i].W;
          element_textures.sample_elements[i].mycolor;
      }

      //要素の描画
      if (element_textures.should_draw(i) == false)continue;
      element_textures.sample_elements[i].Draw();
  }

  //element_textures.sample_tex.IterStep();
  //element_textures.assignPos_sample_elements();
  //cout << "Iter!!" << endl;
  /*
  const static float diffR[4] = { 1.0f, 0, 0, 1.0f };
  const static float ambiR[4] = { 1.0f, 0, 0, 1.0f };
  const static float specR[4] = { 1.0f, 0, 0, 1.0f };
  const static float shinR[1] = { 64.0f };

  const static float diffB[4] = { 0, 0, 1.0f, 1.0f };
  const static float ambiB[4] = { 0, 0, 1.0f, 1.0f };
  const static float specB[4] = { 0, 0, 1.0f, 1.0f };
  const static float shinB[1] = { 64.0f };
  */
  //glEnable(GL_LIGHTING);
  //glEnable(GL_LIGHT0);
  //glEnable(GL_LIGHT1);
  //glEnable(GL_LIGHT2);
  /*
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR  , specB);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE  , diffB);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT  , ambiB);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shinB);
  */
  //glEnable( GL_CULL_FACE );
  //glCullFace(GL_FRONT );
  /*
  glColor3d(1, 0, 1);
  Draw2DRect(1, 1,0, 1, 1);
  Draw2DRect(-1, -1, 0, 1.5, 1.5);
  glColor3d(0, 1, 0);
  Draw2DRect(0, 0,-0.01, 0.5, 0.5);
  */
  /*
  int rep = sample_element_texture.Abst_elements.size();
  cout << " sixe : " << rep << endl;
  tmp.Setpos(EVec2d(0.5, 0.3));
  //tmp.Draw();
  for (int i = 0; i < rep; ++i) {
      sample_element_texture.Abst_elements[i].Setpos(EVec2d(i, i));
      sample_element_texture.Abst_elements[i].Draw();
  }
  */
}


