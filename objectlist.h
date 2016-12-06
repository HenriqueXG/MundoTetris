#ifndef OBJECTLIST_H
#define OBJECTLIST_H

#include "cubo.h"

#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>

#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <vector>
#include <iostream>

#define TEX 9

using namespace std;
using namespace cv;

class ObjectList
{
public:
    ObjectList();

    void desenha();
    void add(Cubo cubo);
    void tex(int i);
    void desenhaLado(int texN, float ax, float ay, float az, float bx, float by, float bz, float cx, float cy, float cz, float dx, float dy, float dz);

    vector<Cubo> cubos;

    float cubo;
    GLuint texturas[TEX];
    vector<Mat> mats;
    String imgs[TEX] = {"madeira.jpg", "pedra.jpg", "grama.jpg", "borg.jpg", "tech.jpg", "universo.jpg", "agua.jpg", "nuvem.jpg", "terra.jpg"};
};

#endif // OBJECTLIST_H
