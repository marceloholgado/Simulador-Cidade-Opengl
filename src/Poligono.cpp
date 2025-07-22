//
//  Poligono.cpp
//  OpenGLTest
//
//  Created by Márcio Sarroglia Pinho on 18/08/20.
//  Copyright © 2020 Márcio Sarroglia Pinho. All rights reserved.
//

#include "..\lib\Poligono.h"

Poligono::Poligono()
{
    
}

void Poligono::insereVertice(Ponto p)
{
    if (!isPonto(p)) {
        Vertices.push_back(p);
    }
}

Ponto Poligono::getVertice(int i)
{
    return Vertices[i];
}
void Poligono::desenhaPoligono()
{
    glBegin(GL_LINE_LOOP);
    for (int i=0; i<Vertices.size(); i++)
        glVertex3f(Vertices[i].x,Vertices[i].y,Vertices[i].z);
    glEnd();
}
void Poligono::desenhaVertices()
{
    glBegin(GL_POINTS);
    for (int i=0; i<Vertices.size(); i++)
        glVertex3f(Vertices[i].x,Vertices[i].y,Vertices[i].z);
    glEnd();
}
void Poligono::imprime()
{
    for (int i=0; i<Vertices.size(); i++)
        Vertices[i].imprime();
}

unsigned long Poligono::getNVertices()
{
    return Vertices.size();
}

//void Poligono::atualizaLimites(int minx, int miny, int maxx, int maxy, int minz, int maxz) {
//    Vertices.clear();
//    Vertices.push_back(Ponto(minx, miny, minz));
//    Vertices.push_back(Ponto(minx, miny, maxz));
//    Vertices.push_back(Ponto(maxx, miny, minz));
//    Vertices.push_back(Ponto(maxx, miny, maxz));
//}
void Poligono::atualizaLimites(int minx, int miny, int xmax, int maxy) {
    Vertices.clear();
    Vertices.push_back(Ponto(minx,miny));
    Vertices.push_back(Ponto(minx,maxy));
    Vertices.push_back(Ponto(xmax,maxy));
    Vertices.push_back(Ponto(xmax,miny));
}
void Poligono::removeVertice(int p)
{
    Vertices.erase(Vertices.begin()+p);
}

void Poligono::obtemLimites(Ponto &Min, Ponto &Max)
{
    Max = Min = Vertices[0];
    
    for (int i=0; i<Vertices.size(); i++)
    {
        Min = ObtemMinimo (Vertices[i], Min);
        Max = ObtemMaximo (Vertices[i], Max);
    }
}
int Poligono::GetPontosInteresec(Ponto k, Ponto l, Ponto m, Ponto n, Ponto &p1, Ponto &p2)
{
    double det, s, t;

    det = (n.x - m.x) * (l.y - k.y)  -  (n.y - m.y) * (l.x - k.x);

    if (det == 0.0) {
        return 0 ; // nao ha interseccao
    }
    s = ((n.x - m.x) * (m.y - k.y) - (n.y - m.y) * (m.x - k.x))/ det ;
    t = ((l.x - k.x) * (m.y - k.y) - (l.y - k.y) * (m.x - k.x))/ det ;

    if (verificaValoresInterseccao(s,t)) {
        // Calcula o ponto na reta
        p1 = GetPontoReta(k, l, s);
        p2 = GetPontoReta(m, n, t);
        return 1;
    }
    return 0;    
}

Ponto Poligono::GetPontoReta(Ponto k, Ponto l, double t) {
    Ponto a, b, r;
    a.x = k.x * (1-t);
    a.y = k.y * (1-t);
    b.x = l.x * t;
    b.y = l.y * t;

    r.x = a.x + b.x;
    r.y = a.y + b.y;

    return r;
}

// **********************************************************************
bool Poligono::HaInterseccao(Ponto k, Ponto l, Ponto m, Ponto n)
{
    int ret;
    double s,t;
    ret = intersec2d( k,  l,  m,  n, s, t);
    if (!ret) {
       return false;
    }
    return verificaValoresInterseccao(s,t);
}

bool Poligono::verificaValoresInterseccao(double s, double t) {
    if (s>=0.0 && s <=1.0 && t>=0.0 && t<=1.0) {
        return true;
    } 
    return false;
}
int Poligono::intersec2d(Ponto k, Ponto l, Ponto m, Ponto n, double &s, double &t)
{
    double det;

    det = (n.x - m.x) * (l.y - k.y)  -  (n.y - m.y) * (l.x - k.x);

    if (det == 0.0)
        return 0 ; // nao ha interseccao

    s = ((n.x - m.x) * (m.y - k.y) - (n.y - m.y) * (m.x - k.x))/ det ;
    t = ((l.x - k.x) * (m.y - k.y) - (l.y - k.y) * (m.x - k.x))/ det ;

    return 1; // ha interseccao
}
bool Poligono::isPonto(Ponto p) {
    bool test = false;
    for (int i=0; i<Vertices.size(); i++) {
        if (p.x  == Vertices[i].x && p.y == Vertices[i].y) {
            test = true;
        }
    }
    return test;
}
