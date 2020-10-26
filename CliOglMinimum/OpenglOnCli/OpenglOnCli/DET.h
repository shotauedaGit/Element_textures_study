#pragma once

#include "COMMON/stdc++.h"
#include "COMMON/tmath.h"

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
    EVec2d pos = EVec2d(0.0,0.0);//position
    EVec3d mycolor = EVec3d(1, 1, 1);

    DiscreteElement() {};
    DiscreteElement(const DiscreteElement& src) { Copy(src); };
    DiscreteElement(GLint _texID, double _W, double _H) {
        type = _texID;
        W = _W;
        H = _H;
    }
    DiscreteElement(GLint _texID, int _w, int _h) {
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

    }

    void Draw();
    void Setpos(const EVec2d & _pos);
    void SetColor(const EVec3d& _col) { mycolor = _col; }
    void SetSize_direct(double _W, double _H) { W = _W; H = _H;}
};

struct DET {//stands Dis

    //int NumOfDElements = 0;

    EVec2d tex_cent = EVec2d(0.0,0.0);
    double tex_w = 10.0;
    double tex_h = 10.0;

    DET() { pointer = DiscreteElement(-1, 0.3, 0.3);
            pointer.SetColor(EVec3d(1, 0, 0));
            pointer.SetSize_direct(0.2, 0.2);
    }
    DET(EVec2d tex_center, double tex_width, double tex_height) {
        tex_cent = tex_center;
        tex_w = tex_width;
        tex_h = tex_height;
        
        pointer = DiscreteElement(-1, 0.3, 0.3);
        pointer.SetColor(EVec3d(1, 0, 0));
        pointer.SetSize_direct(0.2, 0.2);

    }

    DiscreteElement pointer;

    vector<DiscreteElement> Abst_elements;
    vector<DiscreteElement> elements;


    //map<GLuint, DiscreteElement> tex_mp;

    int NumOfTexType = 30;
    GLuint texID[30];

    void LoadTexture();
    void SetTexture(char *filepath,GLuint texID);

    void TEST();
    DiscreteElement& getSelectedAbst_element();
};