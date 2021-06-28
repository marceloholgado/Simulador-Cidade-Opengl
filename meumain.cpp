// **********************************************************************
// PUCRS/Escola Polit�cnica
// COMPUTA��O GR�FICA
//
// Simulador de Cidade
//
// Marcio Sarroglia Pinho
// pinho@pucrs.br
// **********************************************************************

#include <cstdlib>
#include <iostream>
#include <cmath>
#include <ctime>

using namespace std;


#ifdef WIN32
#include <windows.h>
#include <glut.h>
#else
#include <sys/time.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif


#ifdef __linux__
#include <GL/glut.h>
#endif

#include "Temporizador.h"
#include "Instancia.h"

Temporizador T, TFuel;
double AccumDeltaT=0;

#include "Ponto.h"
#include "ListaDeCoresRGB.h"
#include "Elemento.h"
#include "TextReader.h"
#include "SOIL/SOIL.h"
#include "TextureClass.h"

GLfloat AspectRatio, angulo=0;

// Controle do modo de projecao
// 0: Projecao Paralela Ortografica; 1: Projecao Perspectiva
// A funcao "PosicUser" utiliza esta variavel. O valor dela eh alterado
// pela tecla 'p'

int ModoDeProjecao = 0;
GLfloat AlturaViewportDeMensagens = 0.1; // percentual em relacao ˆ altura da tela
// Controle do modo de projecao
// 0: Wireframe; 1: Faces preenchidas
// A funcao "Init" utiliza esta variavel. O valor dela eh alterado
// pela tecla 'w'
int ModoDeExibicao = 1;

double nFrames=0;
double TempoTotal=0;

Ponto Curva1[3];
Ponto Curva2[3];
Ponto Max, Min;

// Qtd de ladrilhos do piso. Inicialzada na INIT
int QtdX;
int QtdZ;

// Tempo da animacao dos inimigos
float TempoDaAnimacao;
float TempoFuel;
int tempo = 5;

// sentido ca curva bezier 
bool way = false;

// View do observador
float obsview_x = 3;
float obsview_y = 8;
float obsview_z = 0;

// View alvo
float alvoview_x = 0;
float alvoview_y = 0;
float alvoview_z = 0;

// codigos que definem o o tipo do elemento que est� em uma c�lula
#define VAZIO 0
#define PREDIO 10
#define RUA 20
#define COMBUSTIVEL 30
#define GRAMA 40

// define quantidades 
#define NUM_AVIOES 1
#define NUM_ENEMIES 1
#define XCIDADE 25
#define ZCIDADE 25
#define MAXFUEL 3

// Matriz que armazena informacoes sobre o que existe na cidade
Elemento Cidade[100][100];

TextReader *sketchCidade = new TextReader();

// Objetos dos que representam os personagem do jogo
Instancia avioesInimigos[NUM_AVIOES];
Instancia Carrinho;
Instancia FuelPump[MAXFUEL];

float anguloPositivoHelice = 45;
float anguloNegativoHelice = -45;
bool aumentax = true, diminuix = false;
bool aumentaz = false, diminuiz = false;
int x, z;


// definicoes de algumas funcoes
void PosicUser();
void DefinesBezierCurves(bool);

// Definicoes de navegacao
Ponto AlvoAtual = Ponto(0,0,0);
Ponto PosicaoObservador, DirObservador;
float passo = 0.5;
int gasolina = 100;
Ponto Deslocamento;
Ponto DirCarro;
Ponto DirCamera;
bool OlhandoParaFrente = true;              // Se 1 personagem esta olhando para frente senao esta mexendo a camera 
bool PilotoAutomatico = false;

// Estruturas para armazenar as texturas de uma cidade
// Constantes para indexar as texturas 
enum PISOS {
    CROSS,
    DL,
    DLR,
    DR,
    LR,
    NONE,
    UD,
    UDL,
    UDR,
    UL,
    ULR,
    UR,
    COMB,
    GRASS,
    LAST_IMG
};
// Lista de nomes das texturas
string nomeTexturas[] = {
    "texturas/ruas/CROSS.png",
    "texturas/ruas/DL.png",
    "texturas/ruas/DLR.png",
    "texturas/ruas/DR.png",
    "texturas/ruas/LR.png",
    "texturas/ruas/None.png",
    "texturas/ruas/UD.png",
    "texturas/ruas/UDL.png",
    "texturas/ruas/UDR.png",
    "texturas/ruas/UL.png",
    "texturas/ruas/ULR.png",
    "texturas/ruas/UR.png",
    "texturas/fuel.jpg",
    "texturas/grama.jpg"
};
int idTexturaRua[LAST_IMG];  // vetor com os identificadores das texturas

// **********************************************************************
// void CarregaTexturas()
// **********************************************************************
void CarregaTexturas() {
    for(int i = 0; i < LAST_IMG; i++)
        idTexturaRua[i] = LoadTexture(nomeTexturas[i].c_str());
}

int EscolheTextura(string nome) {

    if (nome == "CROSS") {
        return idTexturaRua[CROSS];
    }
    else if (nome == "DL") {
        return idTexturaRua[DL];
    }
    else if (nome == "DLR") {
        return idTexturaRua[DLR];
    }
    else if (nome == "DR") {
        return idTexturaRua[DR];
    }
    else if (nome == "LR") {
        return idTexturaRua[LR];
    }
    else if (nome == "UD") {
        return idTexturaRua[UD];
    }
    else if (nome == "UDL") {
        return idTexturaRua[UDL];
    }
    else if (nome == "UDR") {
        return idTexturaRua[UDR];
    }
    else if (nome == "UL") {
        return idTexturaRua[UL];
    }
    else if (nome == "ULR") {
        return idTexturaRua[ULR];
    }
    else if (nome == "UR") {
        return idTexturaRua[UR];
    }
    else if (nome == "10") {
        return idTexturaRua[GRASS];
    }
    else {
        return idTexturaRua[NONE];
    }
}
int VerificaTipo(string nome) {

    if (nome == "CROSS") {
        return RUA;
    }
    else if (nome == "DL") {
        return RUA;
    }
    else if (nome == "DLR") {
       return RUA;
    }
    else if (nome == "DR") {
        return RUA;
    }
    else if (nome == "LR") {
        return RUA;
    }
    else if (nome == "NONE") {
        return RUA;
    }
    else if (nome == "UD") {
        return RUA;
    }
    else if (nome == "UDL") {
        return RUA;
    }
    else if (nome == "UDR") {
        return RUA;
    }
    else if (nome == "UL") {
        return RUA;
    }
    else if (nome == "ULR") {
        return RUA;
    }
    else if (nome == "UR") {
        return RUA;
    }
    else if (nome == "10") {
        return GRAMA;
    } else {
        return PREDIO;
    }
}

// **********************************************************************
void IncializaCidade(int QtdX, int QtdZ, int &posx, int &posz) {
    cout << "Incializa cidade" << endl;
    int i, j, val, index;
    for (i = 0; i < QtdX; i++) {
        for (j = 0; j < QtdZ; j++) {
            
            val = VerificaTipo(sketchCidade->matrix[i][j]);
            
            if (val  == PREDIO) {
                index = stoi(sketchCidade->matrix[i][j]);
                Cidade[i][j].tipo = PREDIO;
                Cidade[i][j].cor = sketchCidade->vetInfo[index].cor;
                Cidade[i][j].altura = sketchCidade->vetInfo[index].altura;
            } else if (val == RUA) {
                Cidade[i][j].tipo = RUA;
                Cidade[i][j].cor = 0;
                Cidade[i][j].altura = 0;
                Cidade[i][j].textID = EscolheTextura(sketchCidade->matrix[i][j]);
            } 
            else if (val == GRAMA) {
                Cidade[i][j].tipo = GRAMA;
                Cidade[i][j].cor = 0;
                Cidade[i][j].altura = 0;
                Cidade[i][j].textID = EscolheTextura(sketchCidade->matrix[i][j]);
            }
        }
    } 
    posx = i;
    posz = j;
}

void InitializeCharacters(int posx, int posz) {
    int i, cont = 0, newposx, newposz;
    for (i = 0; i < NUM_AVIOES; i++) {
        avioesInimigos[i] = Instancia();
        if (i < NUM_ENEMIES) {
            avioesInimigos[i].setMoving(true);                     // Inicializa 5 avioes
        } else {
            avioesInimigos[i].setMoving(false);                    
        }
        avioesInimigos[i].setRotacao(180);
        avioesInimigos[i].setEscala(Ponto(0.4, 0.4, 0.4));
        avioesInimigos[i].setVelocidade(Ponto((Max.x - Min.x)/tempo,(Max.y - Min.y)/tempo,(Max.y - Min.y)/tempo));

        avioesInimigos[i].setPosicao(Ponto(1+i, 4+i, 1+i));

        avioesInimigos[i].pontosBezier[0] = Ponto (0, 4+i, 0);
        avioesInimigos[i].pontosBezier[1] = Ponto (0, 4+i, 0);
        avioesInimigos[i].pontosBezier[2] = Ponto (0, 4+i, 0);
    }
    i = 0;
    while (i < MAXFUEL) {
        newposx = rand() % XCIDADE;
        newposz = rand() % ZCIDADE;
        if (Cidade[newposx][newposz].tipo == RUA) {
            FuelPump[i].setPosicao(Ponto(newposx, 0, newposz));
            i++;
        }
    }

    Carrinho = Instancia();
    Carrinho.setRotacao(0);
    Carrinho.setEscala(Ponto(0.2, 0.2, 1));
    Carrinho.setMoving(true);
    Carrinho.setPosicao(Ponto(4, 0, posz));
    Carrinho.setDirecao(Ponto(0, 0, -1));

    PosicaoObservador = Carrinho.getPosicao();
    DirObservador = Carrinho.getDirecao();
    DirCamera = Carrinho.getDirecao();

    DefinesBezierCurves(true);
}
//  void init(void)
//    Inicializa os parametros globais de OpenGL
// **********************************************************************
void init(void)
{
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // Fundo de tela azul
   
    glShadeModel(GL_SMOOTH);
    //glShadeModel(GL_FLAT);
    glColorMaterial ( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
    if (ModoDeExibicao) // Faces Preenchidas??
    {
        glEnable(GL_DEPTH_TEST);
        glEnable (GL_CULL_FACE );
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else
    {
        glEnable(GL_DEPTH_TEST);
        glEnable (GL_CULL_FACE );
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    glEnable(GL_NORMALIZE);
    
    srand((unsigned int)time(NULL));
    
    QtdX = XCIDADE;
    QtdZ = ZCIDADE;
    CarregaTexturas();
   
    sketchCidade->readSketch("cidade2.txt");

    int i, j;

    IncializaCidade(QtdX, QtdZ, i, j);
    
    InitializeCharacters(i, j);
}
// Calculos Bezier
// **********************************************************************
Ponto CalculaBezier3(Ponto PC[], double t)
{
    Ponto P;
    double UmMenosT = 1-t;
    
    P =  PC[0] * UmMenosT * UmMenosT + PC[1] * 2 * UmMenosT * t + PC[2] * t*t;
    //P.z = 5;
    return P;
}
// **********************************************************************
void TracaBezier3Pontos()
{
    double t=0.0;
    double DeltaT = 1.0/10;
    Ponto P;
    //cout << "DeltaT: " << DeltaT << endl;
    glBegin(GL_LINE_STRIP);
    
    while(t<1.0)
    {
        P = CalculaBezier3(Curva1, t);
        glVertex3f(P.x, P.y, P.z);
        t += DeltaT;
       // P.imprime(); cout << endl;
    }
    P = CalculaBezier3(Curva1, 1.0); // faz o fechamento da curva
    glVertex3f(P.x, P.y, P.z);
    glEnd();
}
// **********************************************************************
void DefinesBezierCurves(bool curveDireciton) {

    for (int i = 0; i < NUM_AVIOES; i++) {
        if (curveDireciton) {
            avioesInimigos[i].pontosBezier[0] = Ponto (avioesInimigos[i].getPosicao().x, avioesInimigos[i].getPosicao().y, avioesInimigos[i].getPosicao().z);
            avioesInimigos[i].pontosBezier[1] = Ponto (avioesInimigos[i].getPosicao().x + XCIDADE, avioesInimigos[i].getPosicao().y, avioesInimigos[i].getPosicao().z);
            avioesInimigos[i].pontosBezier[2] = Ponto (avioesInimigos[i].getPosicao().x, avioesInimigos[i].getPosicao().y, avioesInimigos[i].getPosicao().z+ZCIDADE);
        } else {
            avioesInimigos[i].pontosBezier[0] = Ponto (avioesInimigos[i].getPosicao().x, avioesInimigos[i].getPosicao().y, avioesInimigos[i].getPosicao().z);
            avioesInimigos[i].pontosBezier[1] = Ponto (avioesInimigos[i].getPosicao().x - XCIDADE, avioesInimigos[i].getPosicao().y, avioesInimigos[i].getPosicao().z);
            avioesInimigos[i].pontosBezier[2] = Ponto (avioesInimigos[i].getPosicao().x, avioesInimigos[i].getPosicao().y, avioesInimigos[i].getPosicao().z-ZCIDADE);
            
         }
    }
}
// **********************************************************************
void AvancaComBezier()
{
    double t;
    t = TempoDaAnimacao/tempo;
    Ponto pontosBz[3];
    if (t > 1.0) {
        DefinesBezierCurves(way);
        TempoDaAnimacao = 0;
        way = !way;
    }

    for (int i = 0; i < NUM_AVIOES; i++) {
        avioesInimigos[i].setPosicao(CalculaBezier3(avioesInimigos[i].pontosBezier,t));
    }
}
// **********************************************************************
//  Funcoes de animacao
//
void AnimateAndUpdateCharacters(double dt) {
    AvancaComBezier();
}

bool Colisao(float Ax, float Az, float Acomp, float Aalt, float Bx, float Bz, float Bcomp, float Balt) {
    if (Az + Aalt < Bz) return false;
    else if (Az > Bz+Balt) return false;
    else if (Ax+Acomp < Bx) return false;
    else if (Ax > Bx+Bcomp) return false;

    return true;
}


void DetectaColisaoFuel() {
    float newposx, newposz;
    for (int i = 0; i < MAXFUEL; i++) {
        if (Colisao(Carrinho.getPosicao().x,Carrinho.getPosicao().z-1, 0.2, 0.2,FuelPump[i].getPosicao().x,FuelPump[i].getPosicao().z, 0.1, 0.1)) {
            newposx = rand() % XCIDADE;
            newposz = rand() % ZCIDADE;
            FuelPump[i].setPosicao(Ponto(newposx, 0, newposz));
            gasolina++;
        }
    }
}
// **********************************************************************
void animate()
{
    double dt;
    dt = T.getDeltaT();
    AccumDeltaT += dt;
    TempoTotal += dt;
    nFrames++;
    int i, j;

    if (AccumDeltaT > 1.0/60) // fixa a atualiza��o da tela em 30
    {
        AccumDeltaT = 0;
        angulo+= 1;
        glutPostRedisplay();
    }
    AnimateAndUpdateCharacters(dt);
    DetectaColisaoFuel();

    if (PilotoAutomatico && gasolina > 0) {
        Deslocamento.x = dt * Carrinho.getDirecao().x * passo;
        Deslocamento.y = dt * Carrinho.getDirecao().y * passo;
        Deslocamento.z = dt * Carrinho.getDirecao().z * passo;
        Carrinho.setPosicao(Carrinho.getPosicao() + Deslocamento);
    }
    if (TempoTotal > 5) {
        gasolina --;
        TempoTotal = 0;
    }
    TempoDaAnimacao += dt;
}
// **********************************************************************
//  void DesenhaCubo()
//
//
// **********************************************************************
void DesenhaCubo()
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
// **********************************************************************
//  Desenha um pr�dio no meio de uam c�lula
// **********************************************************************
void DesenhaPredio(float altura)
{
    glPushMatrix();
       // glTranslatef(0, -1, 0);
        glScalef(0.2, altura, 0.2);
        glTranslatef(0, 1, 0);
        DesenhaCubo();
    glPopMatrix();
    
}
// **********************************************************************
// void DesenhaLadrilho(int corBorda, int corDentro)
// Desenha uma c�lula do piso.
// Eh possivel definir a cor da borda e do interior do piso
// O ladrilho tem largula 1, centro no (0,0,0) e est� sobre o plano XZ
// **********************************************************************
void DesenhaLadrilho(int corBorda, int corDentro)
{
    defineCor(corBorda); // desenha QUAD preenchido
    glBegin ( GL_QUADS );
        glNormal3f(0,1,0);
        glVertex3f(-0.5f,  0.0f, -0.5f);
        glVertex3f(-0.5f,  0.0f,  0.5f);
        glVertex3f( 0.5f,  0.0f,  0.5f);
        glVertex3f( 0.5f,  0.0f, -0.5f);
    glEnd();
    
    //defineCor(corDentro);
    //glBegin ( GL_LINE_STRIP ); // desenha borda do ladrilho
    //    glNormal3f(0,1,0);
    //    glVertex3f(-0.5f,  0.0f, -0.5f);
    //    glVertex3f(-0.5f,  0.0f,  0.5f);
    //    glVertex3f( 0.5f,  0.0f,  0.5f);
    //    glVertex3f( 0.5f,  0.0f, -0.5f);
    //glEnd();
}

void drawTextCord() {
    glBegin ( GL_QUADS );
        glNormal3f(0,1,0);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f,  0.0f, -0.5f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f,  0.0f,  0.5f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  0.0f,  0.5f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f,  0.0f, -0.5f);
    glEnd();
}

void drawTextCordCubo() {
    glBegin ( GL_QUADS );
    // Front Face
    glNormal3f(0,0,1);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    // Back Face
    glNormal3f(0,0,-1);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
     // Top Face
    glNormal3f(0,1,0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
    // Bottom Face
    glNormal3f(0,-1,0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    // Right face
    glNormal3f(1,0,0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    // Left Face
    glNormal3f(-1,0,0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glEnd();
}

void drawFuelPumps() {
    float x, z;
    for (int i = 0; i < MAXFUEL; i++) {
        x = FuelPump[i].getPosicao().x;
        z = FuelPump[i].getPosicao().z;
        if (Cidade[int(x)][int(z)].tipo == RUA) {
            glPushMatrix();
                glTranslatef(x, 0.1, z);
                glScalef(0.1, 0.1, 0.1);
                glRotatef(angulo, 0, 1, 0);
                defineCor(White);
                glBindTexture(GL_TEXTURE_2D, idTexturaRua[COMB]);
                drawTextCordCubo();
                glBindTexture(GL_TEXTURE_2D, 0);
            glPopMatrix();
        }
    }
}

// **********************************************************************
// DesenhaCidade()
// XCIDADE: nro de c�lulas em X
// ZCIDADE: nro de c�lulas em Z
// Desenha elementos que compiem a cidade
// **********************************************************************
void DesenhaCidade()
{
    glMatrixMode(GL_MODELVIEW);

    for (int i = 0; i < XCIDADE; i++) {
        for (int j = 0; j < ZCIDADE; j++) {
            if (Cidade[i][j].tipo == RUA) {
                glPushMatrix();
                    glTranslatef(i, 0, j);
                    defineCor(White);
                    glBindTexture(GL_TEXTURE_2D, Cidade[i][j].textID);
                    drawTextCord();
                    glBindTexture(GL_TEXTURE_2D, 0);
                glPopMatrix();
            } 
            else if (Cidade[i][j].tipo == PREDIO) {
                glPushMatrix();
                    glTranslatef(i, 0, j);
                    DesenhaLadrilho(BlueViolet, Black);
                    defineCor(Cidade[i][j].cor);
                    DesenhaPredio(Cidade[i][j].altura);
                glPopMatrix();
            }
            else if (Cidade[i][j].tipo == GRAMA) {
                glPushMatrix();
                    glTranslatef(i, 0, j);
                    defineCor(White);
                    glBindTexture(GL_TEXTURE_2D, Cidade[i][j].textID);
                    drawTextCord();
                    glBindTexture(GL_TEXTURE_2D, 0);
                glPopMatrix();
            } else {
                glPushMatrix();
                    glTranslatef(i, 0, j);
                    DesenhaLadrilho(BlueViolet, Black);
                glPopMatrix();
            }
        }
    }
}

void DesenhaCarrinho() {
    glTranslatef ( Carrinho.getPosicao().x, Carrinho.getPosicao().y, Carrinho.getPosicao().z -1 );
    glRotatef(Carrinho.getRotacao(), 0, 1, 0);
    glScalef(0.2, 0.1,0.2);
    glTranslatef(0, 1, 0);
    defineCor(Red);
    DesenhaCubo();
}

void DesenhaAvioesInimigos () 
{
   
    for (int i = 0; i < NUM_AVIOES; i++) {   
        glPushMatrix(); 
        {
            // Desenha corpo do aviao
            glTranslatef ( avioesInimigos[i].getPosicao().x, avioesInimigos[i].getPosicao().y, avioesInimigos[i].getPosicao().z);
            glScalef(0.2, 0.1,0.6);
            glTranslatef(0, 1, 0);
            defineCor(Gray);
            DesenhaCubo();
        }
        glPopMatrix();
       
        glPushMatrix(); 
        {
            // Desenha asa 1
            glTranslatef ( avioesInimigos[i].getPosicao().x+0.5, avioesInimigos[i].getPosicao().y, avioesInimigos[i].getPosicao().z+0.3);
            glScalef(0.4, 0.05,0.2);
            glTranslatef(0, 1, 0);
            defineCor(Gray);
            DesenhaCubo();
        }
        glPopMatrix();
      
        glPushMatrix();
        {
            // Desenha asa 2
            glTranslatef ( avioesInimigos[i].getPosicao().x-0.5, avioesInimigos[i].getPosicao().y, avioesInimigos[i].getPosicao().z+0.3);
            glScalef(0.4, 0.05,0.2);
            glTranslatef(0, 1, 0);
            defineCor(Gray);
            DesenhaCubo();
        }
        glPopMatrix();
        
        glPushMatrix();
        {
            // Desenha helice 1 motor
            glTranslatef ( avioesInimigos[i].getPosicao().x, avioesInimigos[i].getPosicao().y, avioesInimigos[i].getPosicao().z+0.6);
            glScalef(0.04, 0.2, 0.02);
            glRotatef(anguloPositivoHelice++, 0, 0, 1);
            glTranslatef(0, 0.6, 0);
            defineCor(Yellow);
            DesenhaCubo();
        }
        glPopMatrix();

        glPushMatrix();
        {
            // Desenha helice 2 motor
            glTranslatef ( avioesInimigos[i].getPosicao().x, avioesInimigos[i].getPosicao().y, avioesInimigos[i].getPosicao().z+0.6);
            glScalef(0.04, 0.2, 0.02);
            glRotatef(anguloNegativoHelice++, 0, 0, 1);
            glTranslatef(0, 0.6, 0);
            defineCor(Yellow);
            DesenhaCubo();
        }
        glPopMatrix();
    }
} 

// **********************************************************************
//  void DefineLuz(void)
// **********************************************************************
void DefineLuz(void)
{
  // Define cores para um objeto dourado
  GLfloat LuzAmbiente[]   = {0.4, 0.4, 0.4f } ;
  GLfloat LuzDifusa[]   = {0.7, 0.7, 0.7};
  GLfloat LuzEspecular[] = {0.9f, 0.9f, 0.9 };
  GLfloat PosicaoLuz0[]  = {0.0f, 3.0f, 5.0f };  // Posi��o da Luz
  GLfloat Especularidade[] = {1.0f, 1.0f, 1.0f};

   // ****************  Fonte de Luz 0

  glEnable ( GL_COLOR_MATERIAL );

   // Habilita o uso de ilumina��o
  glEnable(GL_LIGHTING);

  // Ativa o uso da luz ambiente
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LuzAmbiente);
  // Define os parametros da luz n�mero Zero
  glLightfv(GL_LIGHT0, GL_AMBIENT, LuzAmbiente);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, LuzDifusa  );
  glLightfv(GL_LIGHT0, GL_SPECULAR, LuzEspecular  );
  glLightfv(GL_LIGHT0, GL_POSITION, PosicaoLuz0 );
  glEnable(GL_LIGHT0);

  // Ativa o "Color Tracking"
  glEnable(GL_COLOR_MATERIAL);

  // Define a reflectancia do material
  glMaterialfv(GL_FRONT,GL_SPECULAR, Especularidade);

  // Define a concentra��oo do brilho.
  // Quanto maior o valor do Segundo parametro, mais
  // concentrado ser� o brilho. (Valores v�lidos: de 0 a 128)
  glMateriali(GL_FRONT,GL_SHININESS,51);

}
void SetAlvoObsPrimeiraPessoa() {

    // Observador 1 pessoa
    PosicaoObservador.x = Carrinho.getPosicao().x;
    PosicaoObservador.y = 0.4;
    PosicaoObservador.z = Carrinho.getPosicao().z-1;

    // Alvo
    AlvoAtual = PosicaoObservador + DirCamera * 2;
}

void SetAlvoObsTerceiraPessoa() {
    // Alvo
    AlvoAtual.x = Carrinho.getPosicao().x;
    AlvoAtual.y = Carrinho.getPosicao().y;
    AlvoAtual.z = Carrinho.getPosicao().z;

    // Obeservador 3 pesssoa
    PosicaoObservador.x = 0;
    PosicaoObservador.y = 10;
    PosicaoObservador.z = 0;
}

// **********************************************************************
//  void PosicUser()
//
//
// **********************************************************************
void PosicUser()
{
    // Define os par�metros da proje��o Perspectiva
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Define o volume de visualiza��o sempre a partir da posicao do
    // observador

    gluPerspective(90,AspectRatio,0.01,1500); // Projecao perspectiva
    
    if (ModoDeProjecao == 0) {
        SetAlvoObsPrimeiraPessoa();
    }
    else {
        SetAlvoObsTerceiraPessoa();
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(PosicaoObservador.x, PosicaoObservador.y, PosicaoObservador.z,   // Posic do Observador
              AlvoAtual.x, 0, AlvoAtual.z,     // Posic do Alvo
              0.0f,1.0f,0.0f); // UP
    

    //cout << "Posicao do obs: " << PosicaoObservador.x << " " << PosicaoObservador.y << " " << PosicaoObservador.z << endl;
    //cout << "Posicao do Alvo: " << AlvoAtual.x << " " << AlvoAtual.y << " " << AlvoAtual.z << endl;
}
// **********************************************************************
//  void reshape( int w, int h )
//		trata o redimensionamento da janela OpenGL
//
// **********************************************************************
void reshape( int w, int h )
{

	// Evita divis�o por zero, no caso de uam janela com largura 0.
	if(h == 0) h = 1;
    // Ajusta a rela��o entre largura e altura para evitar distor��o na imagem.
    // Veja fun��o "PosicUser".
	AspectRatio = 1.0f * w / h;
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Seta a viewport para ocupar toda a janela
    glViewport(0, 0, w, h);
    //cout << "Largura" << w << endl;

	PosicUser();

}

void printString(string s, int posX, int posY, int cor)
{
    defineCor(cor);
    
    glRasterPos3i(posX, posY, 0); //define posicao na tela
    for (int i = 0; i < s.length(); i++)
    {
//GLUT_BITMAP_HELVETICA_10,
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s[i]);
    }
    
}

// **********************************************************************
//
// **********************************************************************
void DesenhaEm2D()
{
    int ativarLuz = false;
    if (glIsEnabled(GL_LIGHTING))
    {
        glDisable(GL_LIGHTING);
        ativarLuz = true;
    }
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // Salva o tamanho da janela
    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);
    
    // Define a area a ser ocupada pela area OpenGL dentro da Janela
    glViewport(0, 0, w, h*AlturaViewportDeMensagens); // a janela de mensagens fica na parte de baixo da janela

    // Define os limites logicos da area OpenGL dentro da Janela
    glOrtho(0,10, 0,10, 0,1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Desenha linha que Divide as ‡reas 2D e 3D
    defineCor(White);
    glLineWidth(5);
    glBegin(GL_LINES);
        glVertex2f(0,10);
        glVertex2f(10,10);
    glEnd();
    
    setprecision(2);
    
    string gasolina_restante = "GASOLINA " + to_string(gasolina);

    printString(gasolina_restante, 0, 0, Yellow);

    // Resataura os par‰metro que foram alterados
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, h*AlturaViewportDeMensagens, w, h-h*AlturaViewportDeMensagens);

    if (ativarLuz)
        glEnable(GL_LIGHTING);

}
// **********************************************************************
//  void display( void )
//
//
// **********************************************************************
void display( void )
{
    double dt = 0;
    int i;
    int j;
    dt = TFuel.getDeltaT();
    
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	DefineLuz();

	PosicUser();
    glLineWidth(2);

	glMatrixMode(GL_MODELVIEW);
   
    glPushMatrix();
        DesenhaCidade();
    glPopMatrix();
    //TracaBezier3Pontos();
    glPushMatrix(); 
    {
        DesenhaCarrinho();
    }
    glPopMatrix();

    glPushMatrix(); 
    {
        defineCor(Gray);
        DesenhaAvioesInimigos();
    }
    glPopMatrix();

    glPushMatrix(); 
    {
        drawFuelPumps();
    }
    glPopMatrix();  

    DesenhaEm2D();
	glutSwapBuffers();
}


// **********************************************************************
//  void keyboard ( unsigned char key, int x, int y )
//
//
// **********************************************************************
        
void keyboard ( unsigned char key, int x, int y )
{
	switch ( key ) 
	{
    case 27:        // Termina o programa qdo
      exit ( 0 );   // a tecla ESC for pressionada
      break;        
    case 'p':
        ModoDeProjecao = !ModoDeProjecao;
        glutPostRedisplay();
        break;
    case 'e':
        ModoDeExibicao = !ModoDeExibicao;
        init();
        glutPostRedisplay();
        break;
    case 'w':
        Deslocamento = Carrinho.getDirecao() * passo;
        Carrinho.setPosicao(Carrinho.getPosicao() + Deslocamento);
        break;
    case 'a':
        Carrinho.setRotacao(Carrinho.getRotacao()+1);
        DirCarro = Carrinho.getDirecao();
        DirCarro.rotacionaY(1);
        Carrinho.setDirecao(DirCarro);
        if (OlhandoParaFrente)
            DirCamera = Carrinho.getDirecao();
        break;
    case 's':
        Deslocamento = Carrinho.getDirecao() * (-passo); 
        Carrinho.setPosicao(Carrinho.getPosicao() + Deslocamento);
        break;
    case 'd':
        Carrinho.setRotacao(Carrinho.getRotacao()-1);
        DirCarro = Carrinho.getDirecao();
        DirCarro.rotacionaY(-1);
        Carrinho.setDirecao(DirCarro);
        if (OlhandoParaFrente)
            DirCamera = Carrinho.getDirecao();
        break;
    case 'u':
        OlhandoParaFrente = false;
        DirCamera.rotacionaX(1);
        break;
    case 'j':
        OlhandoParaFrente = false;
        DirCamera.rotacionaX(-1);
        break;
    case 'k':
        OlhandoParaFrente = false;
        DirCamera.rotacionaY(-1);
        break;
    case 'h':
        OlhandoParaFrente = false;
        DirCamera.rotacionaY(1);
        break;
    case 'c': 
        OlhandoParaFrente = true;
        DirCamera = Ponto(0, 0, -1);
    case 32:
        PilotoAutomatico = !PilotoAutomatico;
    default:
        cout << key;
    break;
  }
}

// **********************************************************************
//  void arrow_keys ( int a_keys, int x, int y )  
//
//
// **********************************************************************
void arrow_keys ( int a_keys, int x, int y )  
{
	switch ( a_keys ) 
	{
		case GLUT_KEY_UP:       // When Up Arrow Is Pressed...
			glutFullScreen ( ); // Go Into Full Screen Mode
			break;
	    case GLUT_KEY_DOWN:     // When Down Arrow Is Pressed...
			glutInitWindowSize  ( 700, 500 ); 
			break;
		default:
			break;
	}
}

// **********************************************************************
//  void main ( int argc, char** argv )
//
//
// **********************************************************************
int main ( int argc, char** argv )   
{
	glutInit            ( &argc, argv ); 
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
	glutInitWindowPosition (0,0);
	glutInitWindowSize  ( 700, 700 );
	glutCreateWindow    ( "Computacao Grafica - Exemplo Basico 3D" ); 
		
	init ();
    //system("pwd");
	
	glutDisplayFunc ( display );  
	glutReshapeFunc ( reshape );
	glutKeyboardFunc ( keyboard );
	glutSpecialFunc ( arrow_keys );
	glutIdleFunc ( animate );

	glutMainLoop ( );          
	return 0; 
}



