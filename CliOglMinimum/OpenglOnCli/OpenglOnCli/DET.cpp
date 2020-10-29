#include "stdafx.h"
#include "COMMON/OglForCLI.h"
#include "COMMON/timageloader.h"
#include "DET.h"

using namespace std;

void DiscreteElement::Draw() {
    
    glPushMatrix();
    glTranslatef(pos.x(), pos.y(), 0);
    //glRotatef(90, 0, 1, 0);

     //-1 : テクスチャはなし

    {
        if (type != -1) {
            glBindTexture(GL_TEXTURE_2D, type);
            glEnable(GL_TEXTURE_2D);
        }
        
        glBegin(GL_TRIANGLES);
        glColor4d(mycolor.x(), mycolor.y(), mycolor.z(),0);
        // front
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
        glVertex3f(H / 2, -W / 2, 0.0);

        //glNormal3f(0.0f, 0.0f, 1.0f); 
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(H / 2, W / 2, 0.0);

        //glNormal3f(0.0f, 0.0f, 1.0f); 
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-H / 2, W / 2, 0.0);

        glEnd();
        if (type != -1)glDisable(GL_TEXTURE_2D);
    }
    
    glPopMatrix();
}

void DiscreteElement::Setpos(const EVec2d& _pos) {
    pos = _pos;
}

void DiscreteElement::Draw_SelectedFlame(double line_width) {
    glPushMatrix();
    glTranslatef(pos.x(), pos.y(), 0);
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

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

}

void DET::TEST() {
    int rep = Abst_elements.size() - 1;//一番最後はただのまるぽち

    for (int i = 0; i < rep; ++i){
        Abst_elements[i].Setpos(EVec2d(-8,8-i)); //y座標は上が正
    }


    //初期化処理を緊急避難
    sample_tex.Init(sample_w, sample_h, NumOfsample_elements);
    sample_tex.Randomize();

    setup_sample_elements();
    int iter = 0;
    while (sample_tex.isFinished == false) {
        iter++;
        sample_tex.IterStep();
        cout << "DET::sample_tex(CVT) iter " << iter << " done" << endl;
    }

    assignPos_sample_elements();

}

void DET::setup_sample_elements() {
    int tgtisx = Abst_elements.size() - 1;
    DiscreteElement e_pb = Abst_elements[tgtisx];
    e_pb.SetSize_direct(0.4, 0.4);

    for (int i = 0; i < NumOfsample_elements; ++i) {
        sample_elements.push_back(e_pb);
    }
}
void DET::assignPos_sample_elements() {
    for (int i = 0; i < NumOfsample_elements; ++i) {
        double _x = sample_tex.points[i].x;
        double _y = sample_tex.points[i].y;

        sample_elements[i].pos = (EVec2d(_x,_y) + sample_cent);
        
    }
}


int DET::getSelectedAbst_element_Idx() { //ここもmodeで管理する
    int ret = -1;
    bool found = false;
    double min_dist = 1e9;

    for (int i = 0; i < Abst_elements.size(); ++i) { 
        DiscreteElement ab_e = Abst_elements[i];
        EVec2d d = pointer.pos - ab_e.pos;
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

    return ret;
}

int DET::getSelected_Sample_element_Idx() { //ここもmodeで管理する
    int ret = -1;
    bool found = false;
    double min_dist = 1e9;

    for (int i = 0; i < sample_elements.size(); ++i) {
        DiscreteElement ab_e = sample_elements[i];
        EVec2d d = pointer.pos - ab_e.pos;
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