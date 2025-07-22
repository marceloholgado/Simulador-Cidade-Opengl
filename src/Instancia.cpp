//
//  Instancia.cpp
//  OpenGLTest
//
//  Created by Márcio Sarroglia Pinho on 22/09/20.
//  Copyright © 2020 Márcio Sarroglia Pinho. All rights reserved.
//

#include "..\lib\Instancia.h"

void DesenhaCubo2();
void DesenhaPersonagem();
void DesenhaRetangulo();
void InstanciaPonto(Ponto &p, Ponto &out);
Ponto InstanciaPonto(Ponto P);

/*****************************************************************/
// Geters and Seters

Modelos Instancia::getTipo(){
    return tipo;
}
    
void Instancia::setPosicao(Ponto posicao) {
    Posicao.x = posicao.x;
    Posicao.y = posicao.y;
    Posicao.z = posicao.z;
}
Ponto Instancia::getPosicao() {
    return Posicao;
}
void Instancia::setEscala(Ponto escala) {
    Escala.x = escala.x;
    Escala.y = escala.y;
    Escala.z = escala.z;
}
Ponto Instancia::getEscala() {
    return Escala;
}
void Instancia::setDirecao(Ponto direcao) {
    Direcao.x = direcao.x;
    Direcao.y = direcao.y;
    Direcao.z = direcao.z;
}
Ponto Instancia::getDirecao() {
    return Direcao;
}
void Instancia::setVelocidade(Ponto velocidade) {
    Velocidade = velocidade;
}
Ponto Instancia::getVelocidade() {
    return Velocidade;
}
void Instancia::setRotacao(float _rotacao) {
    rotacao = _rotacao;
}
float Instancia::getRotacao() {
    return rotacao;
}
void Instancia::setMoving(bool _moving) {
    moving = _moving;
}
bool Instancia::getMoving() {
    return moving;
}
/*****************************************************************/
// Draw functions

// ***********************************************************
//  void InstanciaPonto(Ponto3D *p, Ponto3D *out)
//  Esta funcao calcula as coordenadas de um ponto no
//  sistema de referencia do universo (SRU), ou seja,
//  aplica as rotacoes, escalas e translacoes a um
//  ponto no sistema de referencia do objeto (SRO).
// ***********************************************************
void InstanciaPonto(Ponto &p, Ponto &out)
{
    GLfloat ponto_novo[4];
    GLfloat matriz_gl[4][4];
    int  i;
    
    glGetFloatv(GL_MODELVIEW_MATRIX,&matriz_gl[0][0]);
    
    for(i=0;i<4;i++)
    {
        ponto_novo[i]= matriz_gl[0][i] * p.x+
        matriz_gl[1][i] * p.y+
        matriz_gl[2][i] * p.z+
        matriz_gl[3][i];
    }
    out.x=ponto_novo[0];
    out.y=ponto_novo[1];
    out.z=ponto_novo[2];
}

Ponto InstanciaPonto(Ponto P)
{
    Ponto temp;
    InstanciaPonto(P, temp);
    return temp;
}

void DesenhaCubo2()
{
    glBegin ( GL_QUADS );
    // Front Face
    glNormal3f(0,0,1);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    // Back Face
    glNormal3f(0,0,-1);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    // Top Face
    glNormal3f(0,1,0);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    // Bottom Face
    glNormal3f(0,-1,0);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    // Right face
    glNormal3f(1,0,0);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    // Left Face
    glNormal3f(-1,0,0);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glEnd();
}