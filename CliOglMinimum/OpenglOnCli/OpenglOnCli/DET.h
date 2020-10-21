#pragma once

#include "COMMON/stdc++.h"
#include "COMMON/tmath.h"

using namespace std;


struct DiscreteElement {



    GLuint type = 0;
    int pic_w = -1;
    int pic_h = -1;

    double scare = 1.0;
    double H = 1.0;
    double W = 1.0;

    double rot = 0.0; //Rotation angle
    EVec2d pos = EVec2d(0.0,0.0);//position

    DiscreteElement() {};
    DiscreteElement(const DiscreteElement& src) { Copy(src); };
    DiscreteElement(GLuint _texID, int _w, int _h) {
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
    void Setpos(const EVec2d &_pos);
};

struct DET {//stands Dis
    EVec2d tex_cent = EVec2d(0.0,0.0);
    
    double tex_w = 10.0;
    double tex_h = 10.0;

    vector<DiscreteElement> Abst_elements;
    vector<DiscreteElement> elements;
    //map<GLuint, DiscreteElement> tex_mp;

    int NumOfTexType = 30;
    GLuint texID[30];

    void LoadTexture();
    void SetTexture(char *filepath,GLuint texID);

    void TEST();
};