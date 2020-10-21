#include "stdafx.h"
#include "COMMON/OglForCLI.h"
#include "COMMON/timageloader.h"
#include "DET.h"

using namespace std;

void DiscreteElement::Draw() {
    
    glPushMatrix();
    glTranslatef(pos.x(), pos.y(), 0);
    //glRotatef(90, 0, 1, 0);

    glBindTexture(GL_TEXTURE_2D, type);

    {
        //GLuint image = loadBMP_custom("./aaa.bmp");

        glEnable(GL_TEXTURE_2D);
        glBegin(GL_TRIANGLES);
        glColor3d(1, 1, 1);
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
        glDisable(GL_TEXTURE_2D);
    }
    
    glPopMatrix();
}

void DiscreteElement::Setpos(const EVec2d& _pos) {
    pos = _pos;
}


void DET::LoadTexture() {
    //å„ÅXílÇÇ∆ÇÈÇÊÇ§Ç…â¸ë¢
    
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

}

void DET::TEST() {
    int N = 3;
    int Nele = 10;

    for (int i = 0; i < Nele; ++i) {
        //elements.push_back(Abst_elements[i % 3]);
    }

}