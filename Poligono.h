//
//  Poligono.hpp
//  OpenGLTest
//
//  Created by Márcio Sarroglia Pinho on 18/08/20.
//  Copyright © 2020 Márcio Sarroglia Pinho. All rights reserved.
//

#ifndef Poligono_hpp
#define Poligono_hpp

#include <iostream>
using namespace std;


#ifdef WIN32
#include <windows.h>
#include <glut.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <GL/glut.h>
#endif

#include "Ponto.h"
#include <vector>

class Poligono
{
    vector <Ponto> Vertices;
    Ponto Min, Max;
public:
    Poligono();
    Ponto getVertice(int);
    unsigned long getNVertices();
    void insereVertice(Ponto);
    void desenhaPoligono();
    void desenhaVertices();
    void imprime();
    void atualizaLimites(int minx, int miny, int xmax, int maxy);
    void obtemLimites(Ponto &Min, Ponto &Max);
    void removeVertice(int p);
protected:
    int GetPontosInteresec(Ponto k, Ponto l, Ponto m, Ponto n, Ponto &p1, Ponto &p2);
    int intersec2d(Ponto k, Ponto l, Ponto m, Ponto n, double &s, double &t);
    bool HaInterseccao(Ponto k, Ponto l, Ponto m, Ponto n);
    Ponto GetPontoReta(Ponto k, Ponto l, double t);
    bool verificaValoresInterseccao(double s, double t);
    bool isPonto(Ponto p);

    // todo calcular o minimo e max local 
};

#endif 
