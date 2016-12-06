#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>

#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <cmath>
#include <iostream>
#include <cstdlib>
#include <vector>

#include "objectlist.h"

using namespace std;
using namespace cv;

typedef struct p Posicao;
struct p{
    float x;
    float y;
    float z;
};

float cubo = 1.5;
float altura = 20;

vector<float> xs;
vector<float> ys;
vector<float> zs;

int indexCubo = 0;
int speed = 20;
float desl = 0.15;

float atualX = 0;
float atualZ = 0;

bool stop = false;
bool pause = false;
bool nextCubo = false;

int texNext = 0;
int texSky = 5;
int texFloor = 4;

vector<Posicao> posicoes;

ObjectList* obj;

GLUquadric *quad;

int camX = 0;
int camY = 10;
int camZ = 0;

int wq, hq;
bool lado = false;

float buscaPos(){
    vector<Posicao>::iterator it;

    for(it = posicoes.begin(); it != posicoes.end(); it++){
        if(it->x == obj->cubos.at(indexCubo).x && it ->z == obj->cubos.at(indexCubo).z){
            return it->y + cubo*2;
        }
    }

    return 0;
}

bool exists(float x, float y, float z){
    vector<Posicao>::iterator it;

    for(it = posicoes.begin(); it != posicoes.end(); it++){
        if(it->x == x && it ->z == z){
            if(it->y >= y-cubo){
                return true;
            }
            break;
        }
    }

    return false;
}

void addPos(){
    vector<Posicao>::iterator it;

    for(it = posicoes.begin(); it != posicoes.end(); it++){
        if(it->x == obj->cubos.at(indexCubo).x && it->z == obj->cubos.at(indexCubo).z){
            it->y = obj->cubos.at(indexCubo).y;
            return;
        }
    }

    Posicao* pos = (Posicao*) malloc(sizeof(Posicao));
    pos->x = obj->cubos.at(indexCubo).x;
    pos->y = obj->cubos.at(indexCubo).y;
    pos->z = obj->cubos.at(indexCubo).z;

    posicoes.push_back(*pos);

    delete pos;
}

void gerenciaTeclado(unsigned char key, int x, int y) {

    switch (key) {
        case 'j':
            if(desl < 0.2) desl += 0.05;
            break;
        case 'h':
            if(desl > 0.1) desl -= 0.05;
            break;
        case 's':
            stop = !stop;
            break;
        case 'p':
            pause = !pause;
            break;
        case 'z':
            if(obj->cubos.size() > 1){
                indexCubo--;

                obj->cubos.pop_back();
                obj->cubos.at(indexCubo).y = altura;

                vector<Posicao>::iterator it;
                for(it = posicoes.begin(); it != posicoes.end(); it++){
                    if(it->x == obj->cubos.at(indexCubo).x && it->z == obj->cubos.at(indexCubo).z){
                        it->y = it->y - cubo*2;
                        break;
                    }
                }

                obj->cubos.at(indexCubo).tex = texNext;
            }
            break;
        case 'a':
            nextCubo = true;
            obj->cubos.at(indexCubo).y = obj->cubos.at(indexCubo).y - fmod(obj->cubos.at(indexCubo).y, (float)cubo);

            break;
        case 'n':
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(65, (float)wq / (float)hq, 1, 200);
            if(camZ > -120 && !lado) camZ -= 10;
            if(camX < 80 && lado) camX += 10;
            if(camZ == -40 && !lado) camZ -= 10;
            if(camX == 0 && lado) camX += 10;
            gluLookAt(camX,camY,camZ, 0,0,-40, 0,1,0);
            break;
        case 'm':
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(65, (float)wq / (float)hq, 1, 200);
            if(camZ < 40 && !lado) camZ += 10;
            if(camX > -80 && lado) camX -= 10;
            if(camZ == -40 && !lado) camZ += 10;
            if(camX == 0 && lado) camX -= 10;
            gluLookAt(camX,camY,camZ, 0,0,-40, 0,1,0);
            break;
        case 'v':
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(65, (float)wq / (float)hq, 1, 200);
            if(camY > 10) camY -= 10;
            gluLookAt(camX,camY,camZ, 0,0,-40, 0,1,0);
            break;
        case 'b':
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(65, (float)wq / (float)hq, 1, 200);
            if(camY < 90) camY += 10;
            gluLookAt(camX,camY,camZ, 0,0,-40, 0,1,0);
            break;
        case 'r':
            lado = false;
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(65, (float)wq / (float)hq, 1, 200);
            camX = 0; camY = 10; camZ = 0;
            gluLookAt(camX,camY,camZ, 0,0,-40, 0,1,0);
            break;
        case 't':
            lado = true;
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(65, (float)wq / (float)hq, 1, 200);
            camX = -40; camY = 10; camZ = -40;
            gluLookAt(camX,camY,camZ, 0,0,-40, 0,1,0);
            break;
        case 'y':
            lado = true;
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(65, (float)wq / (float)hq, 1, 200);
            camX = 40; camY = 10; camZ = -40;
            gluLookAt(camX,camY,camZ, 0,0,-40, 0,1,0);
            break;
        case 'u':
            lado = false;
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(65, (float)wq / (float)hq, 1, 200);
            camX = 0; camY = 10; camZ = -80;
            gluLookAt(camX,camY,camZ, 0,0,-40, 0,1,0);
            break;
    }

    glutPostRedisplay();
}

void gerenciaTecladoSetas(int key, int x, int y){

    switch(key)
    {
        case GLUT_KEY_UP:
            if(!exists(obj->cubos.at(indexCubo).x, obj->cubos.at(indexCubo).y, obj->cubos.at(indexCubo).z-(cubo*2))) obj->cubos.at(indexCubo).z -= (cubo*2);
            break;
        case GLUT_KEY_DOWN:
            if(!exists(obj->cubos.at(indexCubo).x, obj->cubos.at(indexCubo).y, obj->cubos.at(indexCubo).z+(cubo*2))) obj->cubos.at(indexCubo).z += (cubo*2);
            break;
        case GLUT_KEY_LEFT:
            if(!exists(obj->cubos.at(indexCubo).x-(cubo*2), obj->cubos.at(indexCubo).y, obj->cubos.at(indexCubo).z)) obj->cubos.at(indexCubo).x -= (cubo*2);
            break;
        case GLUT_KEY_RIGHT:
            if(!exists(obj->cubos.at(indexCubo).x+(cubo*2), obj->cubos.at(indexCubo).y, obj->cubos.at(indexCubo).z)) obj->cubos.at(indexCubo).x += (cubo*2);
            break;
    }

    glutPostRedisplay();
}

void Floor(){
    float TAM = 125.0;

    glNormal3f(0,1,0);

    obj->tex(texFloor);

    glEnable(GL_TEXTURE_2D);

        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f);glVertex3f(-TAM, 0, TAM);
            glTexCoord2f(0.0f, 1.0f);glVertex3f(-TAM, 0, -TAM);
            glTexCoord2f(1.0f, 1.0f);glVertex3f(TAM, 0, -TAM);
            glTexCoord2f(1.0f, 0.0f);glVertex3f(TAM, 0, TAM);
        glEnd();

    glDisable(GL_TEXTURE_2D);
}

void Ilumina(){
    glMatrixMode(GL_MODELVIEW);

    GLfloat luzAmbiente[4] = {0.2,0.2,0.2,1.0};
    GLfloat luzDifusa[4] = {0.7,0.7,0.7,1.0};
    GLfloat posicaoLuz[4] = {20.0,20.0,-40.0,1.0};

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT,luzAmbiente);
    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz);
}

void MenuTex(int op){

    switch(op) {
        case 0:
            texNext = 0;
            break;
        case 1:
            texNext = 1;
            break;
        case 2:
            texNext = 2;
            break;
        case 3:
            texNext = 3;
            break;
        case 4:
            texNext = 8;
            break;
    }
    glutPostRedisplay();
}

void MenuAmb(int op){

    switch(op) {
        case 0:
            texFloor = 4;
            texSky = 5;
            break;
        case 1:
            texFloor = 6;
            texSky = 7;
            break;
    }
    glutPostRedisplay();
}

void MenuPrincipal(int op){
}

void CriaMenu(){
    int menu,submenu1,submenu2;
    submenu1 = glutCreateMenu(MenuTex);
    glutAddMenuEntry("Madeira",0);
    glutAddMenuEntry("Pedra",1);
    glutAddMenuEntry("Grama",2);
    glutAddMenuEntry("Terra",4);
    glutAddMenuEntry("Borg Cube",3);
    submenu2 = glutCreateMenu(MenuAmb);
    glutAddMenuEntry("Tech",0);
    glutAddMenuEntry("Clean",1);
    menu = glutCreateMenu(MenuPrincipal);
    glutAddSubMenu("Textura",submenu1);
    glutAddSubMenu("Ambiente",submenu2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void Ambiente(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -40.0f);

    Ilumina();

    if(obj->cubos.size()-1 < indexCubo){
        Cubo cubo(atualX, altura, atualZ);
        cubo.tex = texNext;
        obj->add(cubo);
    }

    glPushMatrix();
        obj->desenha();
    glPopMatrix();

    glPushMatrix();
        glEnable(GL_TEXTURE_2D);
            obj->tex(texSky);
            gluQuadricTexture(quad,1);
            glRotatef(-90, 0, 0, 1);
            gluSphere(quad,120,40,40);
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
        Floor();
    glPopMatrix();

    glutSwapBuffers();
}

void anima(int value){

    if(obj->cubos.size()-1 < indexCubo){
        glutPostRedisplay();
        glutTimerFunc(speed, anima, 0);
    }
    else if(obj->cubos.at(indexCubo).y >= cubo && obj->cubos.at(indexCubo).y > buscaPos() && !pause && !nextCubo){
        obj->cubos.at(indexCubo).y -= desl;
    }
    else if(!stop && !pause || nextCubo){
        atualX = obj->cubos.at(indexCubo).x;
        atualZ = obj->cubos.at(indexCubo).z;

        if(obj->cubos.at(indexCubo).y >= altura && altura <= 60){
            altura *= 2;
        }

        nextCubo = false;

        addPos();
        indexCubo++;
    }

    glutPostRedisplay();
    glutTimerFunc(speed, anima, 0);

}

void Reshape(int w, int h){
    wq = w, hq = h;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65, (float)w / (float)h, 1, 250);
    gluLookAt(camX,camY,camZ, 0,0,-40, 0,1,0);
}

void Inicializa(){
    glClearColor(0.0, 1.0, 1.0, 1.0);

    quad = gluNewQuadric();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    CriaMenu();
}

int main(int argc, char** argv) {
    obj = new ObjectList();
    obj->cubo = cubo;
    Cubo cubo(0,altura,0);
    cubo.tex = 0;
    obj->add(cubo);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);

    glutCreateWindow("PROJETO DE COMPUTACAO GRAFICA");
    Inicializa();

    glutDisplayFunc(Ambiente);

    glutKeyboardFunc(gerenciaTeclado);
    glutSpecialFunc(gerenciaTecladoSetas);

    glutReshapeFunc(Reshape);

    glutTimerFunc(speed, anima, 0);

    glutMainLoop();

    return 0;

}
