//
//  Instancia.h
//  OpenGLTest
//
//  Created by Márcio Sarroglia Pinho on 22/09/20.
//  Copyright © 2020 Márcio Sarroglia Pinho. All rights reserved.
//

#ifndef Instancia_hpp
#define Instancia_hpp
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;
#ifdef WIN32
#include <windows.h>
#include <gl/glut.h>
#else
#include <sys/time.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif


#ifdef __linux__
#include <GL/glut.h>
#endif

#include "Ponto.h"

enum Modelos {
    CARRO,
    AVIAO,
    BUILD,
    VAZIO,
    BULLET
};

class Instancia {
    //Poligono *modelo;
protected:    
    Modelos tipo;
    Ponto Posicao, Escala, Direcao, Velocidade;
    float rotacao;
    bool moving = false;
public:
    Ponto pontosBezier[3];

    Instancia() {

    }

    Modelos getTipo();
    
    void setPosicao(Ponto);
    Ponto getPosicao();
    void setEscala(Ponto);
    Ponto getEscala();
    void setDirecao(Ponto);
    Ponto getDirecao();
    void setVelocidade(Ponto);
    Ponto getVelocidade();
    void setRotacao(float);
    float getRotacao();
    void setMoving(bool);
    bool getMoving();

    void desenhaNoPonto(Ponto pos);
    void desenha();
};


#endif /* Instancia_hpp */
