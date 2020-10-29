#pragma once

#include "COMMON/stdc++.h"
#include "COMMON/tmath.h"

#include "CVT.h"

using namespace std;


struct DiscreteElement {


    int type_id = 0;
    int id = 0;

    GLint type = -1;
    int pic_w = -1;
    int pic_h = -1;

    double scare = 1.0;
    double H = 1.0;
    double W = 1.0;

    double rot = 0.0; //Rotation angle
    EVec2d pos = EVec2d(-10.0,10.0);//position
    EVec3d mycolor = EVec3d(1, 1, 1);

    DiscreteElement() {};
    DiscreteElement(const DiscreteElement& src) { Copy(src); };
    DiscreteElement(GLint _texID, int _w, int _h) {
        type = _texID;
        W = (double)_w / max(_w, _h);
        H = (double)_h / max(_w, _h);

        pic_w = _w;
        pic_h = _h;
    }
    DiscreteElement(GLint _texID,int type_id,int id, int _w, int _h) { //_w = picture width
        type = _texID;
        W = (double)_w / max(_w, _h);
        H = (double)_h / max(_w, _h);

        pic_w = _w;
        pic_h = _h;
    };

    ~DiscreteElement() {};

    DiscreteElement& operator=(const DiscreteElement& src) { Copy(src); return *this; }

    void Copy(const DiscreteElement &src) {
        this->type = src.type;
        this->pic_w = src.pic_w;
        this->pic_h = src.pic_h;

        this->scare = src.scare;
        this->pos = src.pos;
        this->rot = src.rot;
        
        this->H = src.H;
        this->W = src.W;

    }

    void Draw();
    void Draw_SelectedFlame(double line_width);
    void Setpos(const EVec2d & _pos);
    void SetColor(const EVec3d& _col) { mycolor = _col; }
    void SetSize_direct(double _W, double _H) { W = _W; H = _H;}
};

struct DET {//stands Dis

    //int NumOfDElements = 0;

    EVec2d sample_cent = EVec2d(-6.0,-3.0);
    double sample_w = 3.0;
    double sample_h = 3.0;


    EVec2d out_cent = EVec2d(4.0, 0.0);
    double out_w = 7.0;
    double out_h = 7.0;

    double selected_threshold = 1.0;


    //配列の番号でアクセスしたい
    int next_Abst_element_Idx = 0;
    int next_out_elements_Idx = 0;
    int next_sample_elements_Idx = 0;

    int selected_Abst_element_Idx = -1;
    int current_Abst_element_Idx = -1;
    
    int selected_sample_element_Idx = -1;
    int selected_Out_element_Idx = -1;

    int current_Out_element_Idx_1 = -1; //2つ選択してからことが始まるので
    int current_Out_element_Idx_2 = -1;

    CVT sample_tex;
    //CVT out_tex;


    DET() { pointer = DiscreteElement(-1, 0.3, 0.3);
            pointer.SetColor(EVec3d(1, 0, 0));
            pointer.SetSize_direct(0.1, 0.1);
            pointer.pos = EVec2d(0, 0);

    }
    /*
    DET(EVec2d tex_center, double tex_width, double tex_height) {
        tex_cent = tex_center;
        tex_w = tex_width;
        tex_h = tex_height;
        
        pointer = DiscreteElement(-1, 0.3, 0.3);
        pointer.SetColor(EVec3d(1, 0, 0));
        pointer.SetSize_direct(0.1, 0.1);
    }
    */

    DiscreteElement pointer;
    DiscreteElement Flame_Selected;

    vector<DiscreteElement> Abst_elements;

    int NumOfsample_elements = 20;
    int NumOfout_elements = 300;

    vector<DiscreteElement> out_elements;
    vector<DiscreteElement> sample_elements;

    //map<GLuint, DiscreteElement> tex_mp;

    int NumOfTexType = 30;
    GLuint texID[30];

    void LoadTexture();
    void SetTexture(char *filepath,GLuint texID);

    void setup_sample_elements(); //outももろともしてしまいたい

    void TEST();
    int getSelectedAbst_element_Idx();
    int getSelected_Sample_element_Idx();

    void assignPos_sample_elements();
    //void assignPos_out_elements();


    void DrawRectFlame(DiscreteElement& src);
    void DrawRectFlame(int selected_Idx,int mode);//0:abst 1:sample 2:out element's vector
    
    int nextIdx_process(int mode) {//0:abst 1:sample 2:out element's vector
        if (mode == 0) { next_Abst_element_Idx++; return next_Abst_element_Idx; }
        else if (mode == 1) { next_sample_elements_Idx++; return next_sample_elements_Idx; }
        else if(mode ==2){ next_out_elements_Idx++; return next_out_elements_Idx;
        }else {
            cout << "nextIdx_process input error input ::" << mode << endl;
            return -1;
        }
    }
};