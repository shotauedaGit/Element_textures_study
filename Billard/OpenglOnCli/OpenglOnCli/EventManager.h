#pragma once
#include "COMMON/OglForCLI.h"
#include "COMMON/timageloader.h"
#include "CVT.h"

#pragma unmanaged 


#define FLOOR_Y 0
#define FLOOR_WIDTH   80
#define FLOOR_LENGTH 145

class SolidRect
{
public:

    GLuint txHandle;

    float W;
    float H;
    //float m_mass;


    //test

    EVec3f m_position;
    //EVec3f m_velocity;

    EVec3f m_angle;

    SolidRect(EVec3f pos) { 
        m_position = pos;
        W = 3;
        H = 3;

        m_angle = EVec3f(0, 0, 0);
    }
    SolidRect(EVec3f pos, float _w,float _h) {
        m_position = pos;
        W = _w;
        H = _h;
        m_angle = EVec3f(0, 0, 0);
    }
    ~SolidRect() {}
    
    SolidRect(const SolidRect& src) { Copy(src); }
    SolidRect& operator=(const SolidRect& src) { Copy(src); return *this; }

    void Copy(const SolidRect& src)
    {
        this->W = src.W;
        this->H = src.H;

        this->m_position = src.m_position;
        //this->m_velocity = src.m_velocity;
        this->m_angle = src.m_angle;
        //this->m_anglevelo = src.m_anglevelo;
    }

    void Draw() {
        glPushMatrix();
        glTranslatef(m_position[0], m_position[1], m_position[2]);
        glRotatef(90, 0, 1, 0);

        //Draw Rect

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
    }

    void SetPos(const float x, const float y) { m_position = EVec3f(x, 0.0f, y); }

};



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

    GLuint txHdl;
    int picW,picH;

    unsigned char* picdata;




    EventManager()
    {
        std::cout << "EventManager constructor\n";

        m_btn_right = m_btn_left = m_btn_middle = false;
        //m_balls.resize(m_cvt.nV);
        float Radius = 5.5f;

        if (t_loadImage("test.bmp", picW, picH, picdata)) {
            std::cout << "Image Loaded\n";
            std::cout << "H:"<<picH<<" W:"<<picW<<'\n';
        }

        //glEnable(GL_TEXTURE_2D);
        glEnable(GL_DEPTH_TEST);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        
        glGenTextures(1, &txHdl);  // generate texture handle
        glBindTexture(GL_TEXTURE_2D, txHdl); // set texture handle

        cout << "Binded:" << txHdl << std::endl;

        
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // set bilinear filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // set bilinear filtering
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // set access unit byte

        glTexImage2D(GL_TEXTURE_2D, // 1D, 2D, or 3D
            0,          // MIPmap level
            GL_RGBA,    // internal format
            picW,  // texture width
            picH, // texture height
            0,          // border width
            GL_RGBA,    // buffer format
            GL_UNSIGNED_BYTE, // buffer type
            picdata     // buffer address
        );



        for (int i = 0; i < m_cvt.nV; ++i) {  //*************************   SolidXXXX用の配列  初期化  ***************
            //m_balls.push_back(SolidBall(EVec3f(0, 0, 0), Radius));
            m_elements.push_back(SolidRect(EVec3f(0, 0, 0), Radius, Radius));
        }
    }




  bool m_btn_right, m_btn_left, m_btn_middle;

  std::vector<SolidBall> m_balls;//Solid ball
  std::vector<SolidRect> m_elements;//SolidRect elements (長方形格納)
  
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

