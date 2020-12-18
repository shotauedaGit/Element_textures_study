#include "stdafx.h"
#include "COMMON/OglForCLI.h"
#include "COMMON/timageloader.h"
#include "DET.h"
using namespace std;

void edit_elements::start_edit(DiscreteElement _A0, DiscreteElement _B0,vector<DiscreteElement> arround) {
    isEditing = true;
    E_A0 = _A0;E_B0 = _B0;
    A0 = E_A0.point.pos;B0 = E_B0.point.pos;
    AB = B0 - A0;
    
    draw_scare = 2.0 / (AB.norm());
    if (draw_scare < 1.0)draw_scare = 1.0;
    
    EVec2d M = (A0 + B0) / 2;
    EVec2d M0B0 = B0 - M, M0A0 = A0 - M;
    E_A0.point.set_pos(M0 + M0A0 * draw_scare);
    E_B0.point.set_pos(M0 + M0B0 * draw_scare);
    
    if (arround.size() != 0) {
        neibhor.resize(arround.size());
        neibhor = arround;

        for (int i = 0; i < arround.size(); i++){
            EVec2d Ei = arround[i].point.pos, M0Ei = Ei - M;
            neibhor[i].point.set_pos(M0 + M0Ei*draw_scare);
        }
    }

    //ABt = EVec2d(-AB.y(), AB.x());

}
void edit_elements::update_input_info(DiscreteElement p, int hover, int just_cl, int last_cl, int drag) {
    pointer = p;
    hover_idx = hover;
    just_cl_idx = just_cl;
    last_cl_idx = last_cl;
    draging_idx = drag;

    pointer.DBG();
    cout << "hover :" << hover_idx << "just_cl :" << just_cl_idx
        << "last_cl :" << last_cl_idx << "draging :" << draging_idx << endl;

}
void edit_elements::redraw_all() {
    if (!isEditing)return;
    E_A0.Draw(); E_B0.Draw();
    for (int i = 0; i < neibhor.size(); i++)neibhor[i].Draw();
}
void edit_elements::step() {
    
}

void DET::prPos(EVec2d p) {
    cout << " x,y= " << p.x() << "," << p.y() << endl;
}
void DET::prPos(string txt,EVec2d p) {
    cout <<txt<<" x,y= " << p.x() << "," << p.y() << endl;
}

void DiscreteElement::Draw() {
    
    glPushMatrix();
    glTranslatef(point.pos.x() + cent.x(), point.pos.y() + cent.y(), Z_draw);
    //glRotatef(90, 0, 1, 0);

     //-1 : テクスチャはなし

    {
        //glBlendFunc(GL_SRC_ALPHA, GL_ZERO);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        //glEnable(GL_BLEND);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);

        glDisable(GL_LIGHT0); glDisable(GL_LIGHT1); glDisable(GL_LIGHT2); glDisable(GL_LIGHT3);
        glDisable(GL_DEPTH_TEST);

        if (type != -1) {
            glBindTexture(GL_TEXTURE_2D, type);
            glEnable(GL_TEXTURE_2D);
        }
        
        glBegin(GL_TRIANGLES);
        glColor4d(mycolor.x(), mycolor.y(), mycolor.z(), mycolor.w());
        // front
        //glNormal3f(0.0f, 0.0f, 1.0f); 
        glTexCoord2f( 0.0f, 1.0f);
        glVertex3f(-H * scare / 2, -W * scare / 2, 0.0);

        //glNormal3f(0.0f, 0.0f, 1.0f); 
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(H * scare / 2, -W * scare / 2, 0.0);

        //glNormal3f(0.0f, 0.0f, 1.0f); 
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-H * scare / 2, W * scare / 2, 0.0);

        // triangle 1

        //glNormal3f(0.0f, 0.0f, 1.0f); 
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(H * scare / 2, -W * scare / 2, 0.0);

        //glNormal3f(0.0f, 0.0f, 1.0f); 
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(H * scare / 2, W * scare / 2, 0.0);

        //glNormal3f(0.0f, 0.0f, 1.0f); 
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-H * scare / 2, W * scare / 2, 0.0);

        glEnd();
        
        glDisable(GL_BLEND);
        if (type != -1)glDisable(GL_TEXTURE_2D);
    }
    
    glPopMatrix();
}
/*
void DiscreteElement::Setpos(const EVec2d& _pos) {
    //pos = _pos;
}
*/
void DiscreteElement::Draw_SelectedFlame(double line_width) {
    glPushMatrix();
    glTranslatef(point.pos.x()+cent.x(), point.pos.y()+cent.y(), 0);
    //glRotatef(90, 0, 1, 0);
     //-1 : テクスチャはなし

    {
        double fH = H * 1.1;
        double fW = W * 1.1;

        glLineWidth(line_width);
        glBegin(GL_LINES);
        
        glColor4d(1, 0, 0, 1);

        glVertex3f(-fH / 2, -fW / 2, 0.0); glVertex3f(fH / 2, -fW / 2, 0.0);
        glVertex3f(fH / 2, -fW / 2, 0.0); glVertex3f(fH / 2, fW / 2, 0.0);

        glVertex3f(fH / 2, fW / 2, 0.0); glVertex3f(-fH / 2, fW / 2, 0.0);
        glVertex3f(-fH / 2, fW / 2, 0.0); glVertex3f(-fH / 2, -fW / 2, 0.0);

        glEnd();
    }
    glPopMatrix();
}


//テクスチャの準備
void DET::LoadTexture() {
    //後々値をとるように改造
    
    glGenTextures(NumOfTexType, texID);
}
void DET::SetTexture(char *filepath,GLuint texID) {
    unsigned char *picdata;
    int w, h;
    t_loadImage(filepath, w, h, picdata);
    //tex_mp[texID] = DiscreteElement(texID, w, h);

    Abst_elements.push_back(DiscreteElement(texID, w, h));

    cout << "SetTexture : texID" << texID << " (W,H) = (" << w << "," << h <<")"<< endl;

    glBindTexture(GL_TEXTURE_2D, texID); // set texture handle
    glTexImage2D(GL_TEXTURE_2D, // 1D, 2D, or 3D
        0,          // MIPmap level
        GL_RGBA,    // internal format
        w,          // texture width
        h,          // texture height
        0,          // border width
        GL_RGBA,    // buffer format
        GL_UNSIGNED_BYTE, // buffer type
        picdata     // buffer address
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // set bilinear filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // set bilinear filtering

    //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

}


//
void DET::TEST(){
    int rep = Abst_elements.size();//一番最後はただのまるぽち
    is_handmade_element.resize(NumOfsample_elements,false);


    for (int i = 0; i < rep; ++i){
        //Abst_elements[i].SetSize_direct(0.7, 0.7);
        Abst_elements[i].point.set_pos( EVec2d(-13-(i/5) , 9-(i%5)) ); //y座標は上が正
    }

    //初期化処理を緊急避難
    //sample_tex.Init(sample_w, sample_h, NumOfsample_elements);
    //sample_tex.Randomize();

    sample_tex.nV = NumOfsample_elements;
    sample_tex.W = sample_w;
    sample_tex.H = sample_h;
    sample_tex.init();

    setup_sample_elements();
    int iter = 0;
    while (iter < IterReps) {
        iter++;
        sample_tex.IterStep();
        //cout << "DET::sample_tex(CVT) iter " << iter << " done" << endl;
    }
    update_Point_CVT2DET();
}
void DET::setup_sample_elements() {
    int tgtisx = 0;//default sample element texture;
    default_sample_texture = 0;

    DiscreteElement e_pb = Abst_elements[tgtisx];
    e_pb.SetSize_direct(0.4, 0.4);
    e_pb.cent = sample_cent;

    for (int i = 0; i < NumOfsample_elements; ++i) {
        sample_elements.push_back(e_pb);
    }
}

void DET::update_Point_CVT2DET() {
    for (int i = 0; i < NumOfsample_elements; ++i) {
        sample_elements[i].point = sample_tex.points[i];
        
    }
}
void DET::update_Point_DET2CVT() {
    for (int i = 0; i < NumOfsample_elements; ++i) {
        sample_tex.points[i] = sample_elements[i].point;
    }
}


//IO 関係
int DET::get_hover_Edit_element_Idx() {
    int ret = -1;
    if (!e_edit.isEditing)return ret;
    double d1 = (e_edit.E_A0.point.pos - pointer.point.pos).norm();
    double d2 = (e_edit.E_B0.point.pos - pointer.point.pos).norm();

    if (min(d1, d2) < d_inter.edit_hover_th) {
        if (d1 <= d2) {
            ret = 1;
        }
        else {
            ret = 2;
        }
    }

    d_inter.E_edit_hover = ret;
    return ret;
}
int DET::get_hover_Abst_element_Idx() { //ここもmodeで管理する
    int ret = -1;
    bool found = false;
    double min_dist = 1e9;

    for (int i = 0; i < Abst_elements.size(); ++i) { 
        DiscreteElement ab_e = Abst_elements[i];
        EVec2d d = pointer.point.pos - ab_e.point.pos;
        double dist = d.norm();

        if (dist > selected_threshold)continue;
        found = true;

        if (min_dist > dist) {
            min_dist = min(min_dist, dist);
            ret = i;
        }
    }

    if (ret != -1) {
        Abst_elements[ret].Draw_SelectedFlame(2);
        selected_Abst_element_Idx = ret;
    }
    else selected_Abst_element_Idx = -1;

    d_inter.E_abst_hover = ret;
    return ret;
}
int DET::get_hover_Sample_element_Idx() { //ここもmodeで管理する
    int ret = -1;
    bool found = false;
    double min_dist = 1e9;

    for (int i = 0; i < sample_elements.size(); ++i) {
        if (should_draw(i) == false)continue;
        DiscreteElement ab_e = sample_elements[i]; if (ab_e.point.exist == false)continue;
        EVec2d d = pointer.point.pos - (ab_e.point.pos + ab_e.cent);
        double dist = d.norm();

        if (dist > selected_threshold)continue;
        found = true;

        if (min_dist > dist) {
            min_dist = min(min_dist, dist);
            ret = i;
        }
    }

    if (ret != -1) {
        sample_elements[ret].Draw_SelectedFlame(2);
        selected_sample_element_Idx = ret;
    }
    else selected_sample_element_Idx = -1;

    d_inter.E_smpl_hover = ret;
    return ret;
}


void DET::DrawRectFlame(DiscreteElement& src) {

}
void DET::DrawRectFlame(int selected_Idx, int mode) {//0:abst 1:sample 2:out element's vector
    if (selected_Idx == -1) {
        cout << "[DrawRectFlame] there is no selected elements : mode =" << mode << endl;
        return;
    }

    if (mode == 0) {
        cout << " selected :" << selected_Idx << endl;
        //Abst_elements[selected_Idx];
    }
}

//texture synthesis state 1
void DET::synthesis() {
    if (is_synth_done)return;

    cout << "synthesis handmade elements : "<< handmade_element_idx.size() << endl;
    //vector<bool> is_handmade(NumOfsample_elements,false);
    //for (int i : handmade_element_idx) { is_handmade[i] = true; }


    for (int i = 0; i < NumOfsample_elements; i++)
    {
        
        if (is_handmade_element[i] == true)continue;
        
        int src_handmade_idx = handmade_element_idx[rand() % (handmade_element_idx.size())];
        sample_elements[i].Copy_without_Point(sample_elements[src_handmade_idx]);
        //cout <<"RND synth: "<< i << " <- " << src_handmade_idx <<" (type id: "<< sample_elements[src_handmade_idx].type<<" )"<< endl;

    }

    is_synth_done = true;

}
int DET::get_Another_element_if_exist(EVec2d rtv_pos_Etar_i,vector<EVec2d> rtv_pos_Wtar, vector<EVec2d> rtv_pos_Wref) {
    
    int Wref_tar_i = -1;
    double minDist = 1e9;

    for  (int i = 0;i < rtv_pos_Wref.size();i++){
        double _Dist = ((rtv_pos_Etar_i) - (rtv_pos_Wref[i])).norm();

            if (minDist > _Dist) {
                Wref_tar_i = i;
                minDist = _Dist;
            }
    }

    int maybe_Etar_i = -1;
    EVec2d rtv_pos_Wref_tar_i = rtv_pos_Wref[Wref_tar_i];
    minDist = 1e9;

    for (int i = 0; i < rtv_pos_Wtar.size(); i++) {
        double _Dist = ((rtv_pos_Wref_tar_i)-(rtv_pos_Wtar[i])).norm();

        if (minDist > _Dist) {
            maybe_Etar_i = i;
            minDist = _Dist;
        }
    }

    if (rtv_pos_Etar_i == rtv_pos_Wtar[maybe_Etar_i])return Wref_tar_i;
    else return -1;
}
double DET::calc_neibhor_similarlity(DiscreteElement Etar, DiscreteElement Eref) {
    int nPair = 0,nSimilarPair = 0;
    
    
    vector<int> Eref_nbh = sample_tex.get_1_neibhor_point_idx(Eref.point),Wref_idx;
    for (int Wref_i : Eref_nbh) {  //Wrefの整形
        if (is_handmade_element[Wref_i])Wref_idx.push_back(Wref_i);
    }
    vector<int> Wtar_idx = sample_tex.get_1_neibhor_point_idx(Etar.point);
    
    vector<EVec2d> rtv_pos_Wtar;
    for (int Wtar_i : Wtar_idx) { rtv_pos_Wtar.push_back((sample_elements[Wtar_i].point.pos) - (Etar.point.pos)); }
    vector<EVec2d> rtv_pos_Wref;
    for (int Wref_i : Wref_idx) { rtv_pos_Wref.push_back((sample_elements[Wref_i].point.pos) - (Eref.point.pos)); }
    
    bool DBG = false;
    if(DBG)cout << "(Etar,Eref) ="<< Etar.point.id() <<","<< Eref.point.id() <<endl;

    //ペアになるかどうか検査
    //    Wtarの各要素 Wtar_i に対して、相対位置が最も近いWrefの要素 Wref_tar_i を求める
    for (int Wtar_i : Wtar_idx) {
        EVec2d rtv_pos_Etar_i = (sample_elements[Wtar_i].point.pos) - (Etar.point.pos);
        int _idx = get_Another_element_if_exist(rtv_pos_Etar_i, rtv_pos_Wtar, rtv_pos_Wref);

        if (_idx != -1/*pair found*/) {
            int Wtar_i_pair = Wref_idx[  _idx  ];

            if (DBG)cout << "pair found : " << Wtar_i << "," << Wtar_i_pair;
            nPair++;

            if ((sample_elements[Wtar_i].type) == (sample_elements[Wtar_i_pair].type) /*is same type*/) {
                nSimilarPair++;
                if (DBG)cout << " --Also , they are similar";
            }
            if (DBG)cout << endl;
        }
    }


    //    Wref_tar_iに相対位置が最も近い Wtar の要素が Wtar_i ならpair成立。
    //        Wtar_i,Wref_tar_i の類似度 (bool 種類が同じか) -> if true nSimilarPair++;
    if (nPair == 0)return 0;
    else return (double)nSimilarPair / nPair;
}
int DET::select_similar_element(DiscreteElement E,double _th) {
    //bool DBG = true;
    bool DBG = false;
    int ret = -1;

    if (DBG)cout << "Similarlity of  " << E.point.id() << endl;

    double R = -1;
    
    //bool use_th = false;
    bool use_th = false;
    double th = _th;
    vector<int> over_th;

    for (int idx : handmade_element_idx) {
        double curR = -1;
        curR = calc_neibhor_similarlity(E, sample_elements[idx]);

        if (DBG)cout << "    " << E.point.id() << " vs " << idx << " : " << curR << endl;
        if (curR > R) {
            ret = idx;
            R = curR;
        }

        if (th > curR) { over_th.push_back(idx);}
    }

    if (use_th) {
        int n = over_th.size();
        if(n!=0)ret = over_th[rand()%n];
    }
    return ret;
}
void DET::synth_Iter() {
    //vector<bool> is_handmade(NumOfsample_elements, false);
    //for (int i : handmade_element_idx) { is_handmade[i] = true; }
    for (int i = 0; i < NumOfsample_elements; i++){

        if (is_handmade_element[i] == true)continue;
        int selected_similar_element = select_similar_element(sample_elements[i], min(0.8 , (0.05*SynthCount+0.4)) );
        sample_elements[i].Copy_without_Point(sample_elements[selected_similar_element]);
        //cout << "Iter:  " << i << "<-" << selected_similar_element << endl;
    }

    curState = 2;SynthCount++;
    cout << SynthCount << endl;
}
