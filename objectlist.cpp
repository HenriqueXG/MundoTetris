#include "objectlist.h"

ObjectList::ObjectList()
{
    glGenTextures(TEX, texturas);
    this->cubo = 1.5;

    for(int i = 0; i < TEX; i++){
        this->mats.push_back(imread(imgs[i]));
    }
}

void ObjectList::tex(int i){
    glBindTexture(GL_TEXTURE_2D, texturas[i]);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, this->mats.at(i).cols, this->mats.at(i).rows, 0, GL_BGR, GL_UNSIGNED_BYTE, this->mats.at(i).ptr());
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
}

void ObjectList::desenhaLado(int texN, float ax, float ay, float az, float bx, float by, float bz, float cx, float cy, float cz, float dx, float dy, float dz){
    tex(texN);

    glEnable(GL_TEXTURE_2D);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f);glVertex3f(ax, ay, az);
            glTexCoord2f(0.0f, 1.0f);glVertex3f(bx, by, bz);
            glTexCoord2f(1.0f, 1.0f);glVertex3f(cx, cy, cz);
            glTexCoord2f(1.0f, 0.0f);glVertex3f(dx, dy, dz);
        glEnd();
    glDisable(GL_TEXTURE_2D);
}

void ObjectList::desenha(){
    vector<Cubo>::iterator it;

    for(it = this->cubos.begin(); it != this->cubos.end(); it++) {
        glPushMatrix();

            glTranslatef(it->x, it->y, it->z);

            glNormal3f(0,0,-1);
            desenhaLado(it->tex, -cubo, cubo, cubo, -cubo, -cubo, cubo, cubo, -cubo, cubo, cubo, cubo, cubo);
            glNormal3f(0,0,1);
            desenhaLado(it->tex, -cubo, cubo, -cubo, -cubo, -cubo, -cubo, cubo, -cubo, -cubo, cubo, cubo, -cubo);
            glNormal3f(-1,0,0);
            desenhaLado(it->tex, -cubo, cubo, -cubo, -cubo, cubo, cubo, -cubo, -cubo, cubo, -cubo, -cubo, -cubo);
            glNormal3f(1,0,0);
            desenhaLado(it->tex, cubo, cubo, cubo, cubo, -cubo, cubo, cubo, -cubo, -cubo, cubo, cubo, -cubo);
            glNormal3f(0,-1,0);
            desenhaLado(it->tex, -cubo, -cubo, cubo, -cubo, -cubo, -cubo, cubo, -cubo, -cubo, cubo, -cubo, cubo);
            glNormal3f(0,1,0);
            desenhaLado(it->tex, -cubo, cubo, cubo, -cubo, cubo, -cubo, cubo, cubo, -cubo, cubo, cubo, cubo);

        glPopMatrix();
    }
}

void ObjectList::add(Cubo cubo){
    this->cubos.push_back(cubo);
}
