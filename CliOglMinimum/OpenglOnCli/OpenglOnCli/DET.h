#pragma once

#include "COMMON/stdc++.h"
#include "COMMON/tmath.h"
//#include "CVT.h"
#include "CVT_v2.h"
using namespace std;

/*Interface memo
Point neib

CVT_v2::



*/


struct DiscreteElement {
    int type_id = 0;
    int id = 0;//いる！？！？
    GLint type = -1;
    int pic_w = -1;
    int pic_h = -1;

    double scare = 1.0;
    double H = 1.0;
    double W = 1.0;

    double rot = 0.0; //Rotation angle
    
    Point point;
    EVec2d cent;
    EVec2d ab_pos = EVec2d(-10.0,10.0);//position
    EVec3d mycolor = EVec3d(1, 1, 1);

    DiscreteElement() { cent = EVec2d(0, 0); };
    DiscreteElement(const DiscreteElement& src) { Copy(src); };
    DiscreteElement(GLint _texID, int _w, int _h) {
        cent = EVec2d(0, 0);
        type = _texID;
        W = (double)_w / max(_w, _h);
        H = (double)_h / max(_w, _h);

        pic_w = _w;
        pic_h = _h;
    }
    DiscreteElement(GLint _texID,int type_id,int id, int _w, int _h) { //_w = picture width
        cent = EVec2d(0, 0);
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
        this->cent = src.cent;
        this->point = src.point;
        this->pic_w = src.pic_w;
        this->pic_h = src.pic_h;

        this->scare = src.scare;
        this->ab_pos = src.ab_pos;
        this->rot = src.rot;
        
        this->H = src.H;
        this->W = src.W;

    }
    void Draw();
    void Draw_SelectedFlame(double line_width);
    //void Setpos(const EVec2d & _pos);
    void SetColor(const EVec3d& _col) { mycolor = _col; }
    void SetSize_direct(double _W, double _H) { W = _W; H = _H;}
    void SetTexture(DiscreteElement src) {
        type = src.type;
        type_id = src.type_id;
        
        H = src.H; W = src.W;
    }
};

struct DET {//stands Dis

    //int NumOfDElements = 0;
    EVec2d sample_cent = EVec2d(1.0,0.0);

    double src_w = 5.0;
    double src_h = 5.0;

    double sample_w = 17.0;
    double sample_h = 17.0;

    EVec2d out_cent = EVec2d(4.0, 0.0);
    double out_w = 7.0;
    double out_h = 7.0;
    double selected_threshold = 1.0;

    bool hover_synth_button = false;
    bool is_synth_done = false;

    int default_sample_texture;

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

    //CVT sample_tex;
    CVT_v2 sample_tex;


    DET() { pointer = DiscreteElement(-1, 0.3, 0.3);
            pointer.SetColor(EVec3d(1, 0, 0));
            pointer.SetSize_direct(0.1, 0.1);
            pointer.point.set_pos(EVec2d(0, 0));
            pointer.cent = EVec2d(0, 0);
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
    //DiscreteElement synth_button;

    vector<DiscreteElement> Abst_elements;
    int NumOfsample_elements = 200;

    int NumOfout_elements = 300;
    vector<DiscreteElement> out_elements;
    
    
    vector<DiscreteElement> sample_elements;
    vector<int> handmade_element_idx;


    int NumOfTexType = 30;
    GLuint texID[30];

    void LoadTexture();
    void SetTexture(char *filepath,GLuint texID);
    void setup_sample_elements(); //outももろともしてしまいたい

    bool should_draw(int sample_element_idx) {
        if (is_synth_done) {
            return true;
        }
        else
        {
            DiscreteElement sm_e = sample_elements[sample_element_idx];
            double x = sm_e.point.pos.x();
            double y = sm_e.point.pos.y();
            if ((fabs(x) <= src_w / 2 && fabs(y) <= src_h / 2))return true;
            else return false;
        }
    }

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

    void synthesis();
    void synth_Iter();

    int select_similar_element(DiscreteElement E);

    //vector<int> get_neighbor_element_idx(int sample_elements_idx,bool onlyHandmade) {
        
    //}
};