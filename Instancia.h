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

#include "Ponto.h"
#include "Poligono.h"

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
    Poligono objeto3d;

    Instancia() {

    }
    Instancia(Modelos tipo, string filename);

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
