//
//  Elemento.h
//  
//

#ifndef Elemento_hpp
#define Elemento_hpp
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

// Representa o conteudo de uma celula do piso
class Elemento {
public:
    int tipo;
    int cor;
    float altura;
    int textID;

    Elemento() {}

    Elemento(int _tipo, int _cor, float _altura) {
        int tipo = _tipo;
        int cor = _cor;
        float altura = _altura;
    }
};
#endif /* Elemento_hpp */






