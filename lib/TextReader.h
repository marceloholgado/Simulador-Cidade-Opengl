
#ifndef TextReader_hpp
#define TextReader_hpp
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

#include "Elemento.h"

using namespace std;


class TextReader {
public:
    string matrix[1000][1000];
    Elemento vetInfo[10];
    TextReader() {}
    void readSketch(string filename) {
        ifstream input;
        string line;
        int totLines, lineNumber = 0, row, col;
        int indextipo;
        input.open(filename, ios::in);
    
        if (!input) {
            cout << "Erro ao abrir " << filename << endl;
            exit(0);
        }
        while (!input.eof()) {
            
            getline(input, line);

            if (line == "#TIPOS") {
                input >> totLines;
                for (int i = 1; i <= totLines; i++) {
                    input >> indextipo >> vetInfo[i].tipo >> vetInfo[i].cor >> vetInfo[i].altura;
                }
            }
            if (line == "#OBJETO") {
                input >> row >> col;
                cout << "Lendo arquivo " << filename << " linhas " << row << " colunas " << col << endl;
                for (int i = 0; i < row; i++) {
                    for (int j = 0; j < col; j++) {
                        input >> matrix[i][j];
                    }
                }
            }
        }
        input.close();
    }
};
#endif /* Cidade_hpp */