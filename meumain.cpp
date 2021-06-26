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

Temporizador T;
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

// define quantidades 
#define NUM_AVIOES 10
#define NUM_ENEMIES 5
#define XCIDADE 15
#define ZCIDADE 16

// Matriz que armazena informacoes sobre o que existe na cidade
Elemento Cidade[100][100];

TextReader *sketchCidade = new TextReader();

// Objetos dos que representam os personagem do jogo
Instancia avioesInimigos[NUM_AVIOES];
Instancia Carrinho;
float anguloPositivoHelice = 45;
float anguloNegativoHelice = -45;

// definicoes de algumas funcoes
void PosicUser();
void DefinesBezierCurves(bool);

// Definicoes de navegacao
Ponto AlvoAtual = Ponto(0,0,0);
Ponto PosicaoObservador, DirObservador;
float passo = 0.5;
Ponto Deslocamento;
Ponto DirCarro;
Ponto DirCamera;
int OlhandoParaFrente = 1;              // Se 1 personagem esta olhando para frente senao esta mexendo a camera 

// Estruturas para armazenar as texturas de uma cidade
// Constantes para indexar as texturas 
enum PISOS {
    CROSS,
    CURVA1,
    CURVA2,
    CURVA3,
    CURVA4,
    ENDLINE1,
    ENDLINE2,
    ENDLINE3,
    ENDLINE4,
    LINE1,
    LINE2,
    LAST_IMG
};
// Lista de nomes das texturas
string nomeTexturas[] = {
    "texturas/ruas/cruzamento.png",
    "texturas/ruas/curva1.png",
    "texturas/ruas/curva2.png",
    "texturas/ruas/curva3.png",
    "texturas/ruas/curva4.png",
    "texturas/ruas/fimrua1.png",
    "texturas/ruas/fimrua2.png",
    "texturas/ruas/fimrua3.png",
    "texturas/ruas/fimrua4.png",
    "texturas/ruas/linha1.png",
    "texturas/ruas/linha2.png",
    "texturas/ruas/rua.png"
};
int idTexturaRua[LAST_IMG];  // vetor com os identificadores das texturas

// **********************************************************************
// void CarregaTexturas()
// **********************************************************************
void CarregaTexturas() {
    for(int i = 0; i < LAST_IMG; i++)
        idTexturaRua[i] = LoadTexture(nomeTexturas[i].c_str());
}

int EscolheTexturaRua(int i, int j) {
    if (i == 3 && j == 2 || i == 3 && j == 6 || i == 3 && j == 10 || i == 3 && j == 14) {
        return CROSS;
    }
    if (i > 0 && j > 0) {
        if (Cidade[i-1][j].tipo == RUA && Cidade[i-1][j].tipo == RUA && Cidade[i][j+1].tipo == PREDIO && Cidade[i][j-1].tipo == PREDIO) {
            return LINE1;
        }
        if ((i == 0 || i == XCIDADE-1) && (Cidade[i-1][j].tipo == PREDIO || Cidade[i-1][j].tipo == VAZIO) || (Cidade[i+1][j].tipo == PREDIO || Cidade[i+1][j].tipo == VAZIO)) {
            return LINE2;
        }
    } else {
        return LINE2;
    }
}
// **********************************************************************
void IncializaCidade(int QtdX, int QtdZ, int &posx, int &posz) {
    cout << "Incializa cidade" << endl;
    int i, j;
    for (int k = 1; k <= 10; k++) {
        for (i = 0; i < QtdX; i++) {
            for (j = 0; j < QtdZ; j++) {
                cout << " VAZIO " << k << " " << sketchCidade->vetInfo[k].tipo << endl;
                if (sketchCidade->matrix[i][j] == k) {
                    if (sketchCidade->vetInfo[k].tipo == VAZIO) {    
                        Cidade[i][j].tipo = VAZIO;
                        Cidade[i][j].cor = sketchCidade->vetInfo[k].cor;
                        Cidade[i][j].altura = sketchCidade->vetInfo[k].altura;
                    } 
                    else if (sketchCidade->vetInfo[k].tipo == PREDIO) {
                        Cidade[i][j].tipo = PREDIO;
                        Cidade[i][j].cor = sketchCidade->vetInfo[k].cor;
                        Cidade[i][j].altura = sketchCidade->vetInfo[k].altura;
                    }
                    else if (sketchCidade->vetInfo[k].tipo == RUA) {
                        Cidade[i][j].tipo = RUA;
                        Cidade[i][j].cor = sketchCidade->vetInfo[k].cor;
                        Cidade[i][j].altura = sketchCidade->vetInfo[k].altura;
                        Cidade[i][j].textID = EscolheTexturaRua(i, j);
                    }
                }
            }
        } 
    }
    posx = i;
    posz = j;
}

void InitializeCharacters(int posx, int posz) {
    int i, cont = 0;
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
    }

    Carrinho = Instancia();
    Carrinho.setRotacao(0);
    Carrinho.setEscala(Ponto(0.2, 0.2, 1));
    Carrinho.setMoving(true);
    Carrinho.setPosicao(Ponto(4, 0, posz));
    Carrinho.setDirecao(Ponto(0, 0, -1));

    PosicaoObservador = Carrinho.getPosicao();
    DirObservador = Carrinho.getDirecao();

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
    
    Curva1[0] = Ponto (1,3,10);
    Curva1[1] = Ponto (1,3,4);
    Curva1[2] = Ponto (1,3,-10);

    Curva2[0] = Ponto (1,3,-10);
    Curva2[1] = Ponto (1,3,4);
    Curva2[2] = Ponto (1,3,10);
    
    srand((unsigned int)time(NULL));
    
    QtdX = XCIDADE;
    QtdZ = ZCIDADE;
    
    sketchCidade->readSketch("cidade.txt");
    cout << sketchCidade->vetInfo[1].tipo << endl;
    cout << " SAIU DA READ" << endl;

    

    int i, j;
    CarregaTexturas();
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
    int cont = 0;
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
        if (way) {
            avioesInimigos[i].setPosicao(CalculaBezier3(Curva1,t));
        } else {
            avioesInimigos[i].setPosicao(CalculaBezier3(Curva2,t));
        }
    }
}
// **********************************************************************
//  Funcoes de animacao
//
void AnimateAndUpdateCharacters(double dt) {
    AvancaComBezier();
}

// **********************************************************************
void animate()
{
    double dt;
    dt = T.getDeltaT();
    AccumDeltaT += dt;
    TempoTotal += dt;
    nFrames++;

    if (AccumDeltaT > 1.0/30) // fixa a atualiza��o da tela em 30
    {
        AccumDeltaT = 0;
        angulo+= 1;
        glutPostRedisplay();
    }
    AnimateAndUpdateCharacters(dt);
    if (TempoTotal > 5.0)
    {
        cout << "Tempo Acumulado: "  << TempoTotal << " segundos. " ;
        cout << "Nros de Frames sem desenho: " << nFrames << endl;
        cout << "FPS(sem desenho): " << nFrames/TempoTotal << endl;
        TempoTotal = 0;
        nFrames = 0;
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
                    glTranslatef(i-5, 0, j);
                    defineCor(White);
                    glBindTexture(GL_TEXTURE_2D, idTexturaRua[Cidade[i][j].textID]);
                    drawTextCord();
                    glBindTexture(GL_TEXTURE_2D, 0);
                glPopMatrix();
            } else if (Cidade[i][j].tipo == PREDIO) {
                glPushMatrix();
                    glTranslatef(i-5, 0, j);
                    DesenhaLadrilho(BlueViolet, Black);
                    defineCor(Cidade[i][j].cor);
                    DesenhaPredio(Cidade[i][j].altura);
                glPopMatrix();
            } else {
                glPushMatrix();
                    glTranslatef(i-5, 0, j);
                    DesenhaLadrilho(BlueViolet, Black);
                glPopMatrix();
            }
        }
    }
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
            glTranslatef ( avioesInimigos[i].getPosicao().x+0.5, avioesInimigos[i].getPosicao().y, avioesInimigos[i].getPosicao().z);
            glScalef(0.4, 0.05,0.2);
            glTranslatef(0, 1, 0);
            defineCor(Gray);
            DesenhaCubo();
        }
        glPopMatrix();
      
        glPushMatrix();
        {
            // Desenha asa 2
            glTranslatef ( avioesInimigos[i].getPosicao().x-0.5, avioesInimigos[i].getPosicao().y, avioesInimigos[i].getPosicao().z);
            glScalef(0.4, 0.05,0.2);
            glTranslatef(0, 1, 0);
            defineCor(Gray);
            DesenhaCubo();
        }
        glPopMatrix();
        
        glPushMatrix();
        {
            // Desenha helice 1 motor
            glTranslatef ( avioesInimigos[i].getPosicao().x, avioesInimigos[i].getPosicao().y, avioesInimigos[i].getPosicao().z+0.7);
            glScalef(0.04, 0.2, 0.04);
            glRotatef(anguloPositivoHelice++, 0, 0, 1);
            glTranslatef(0.04, 0.6, 0);
            defineCor(Yellow);
            DesenhaCubo();
        }
        glPopMatrix();

        glPushMatrix();
        {
            // Desenha helice 2 motor
            glTranslatef ( avioesInimigos[i].getPosicao().x, avioesInimigos[i].getPosicao().y, avioesInimigos[i].getPosicao().z+0.7);
            glScalef(0.04, 0.2, 0.04);
            glRotatef(anguloNegativoHelice--, 0, 0, 1);
            glTranslatef(0.04, 0.6, 0);
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
    if (OlhandoParaFrente == 1) {
        DirObservador = Carrinho.getDirecao();
        AlvoAtual = PosicaoObservador + DirObservador * 2;
    } else {
        AlvoAtual = PosicaoObservador + DirCamera * 2;
    }
}

void SetAlvoObsTerceiraPessoa() {
    // Alvo
    AlvoAtual.x = Carrinho.getPosicao().x;
    AlvoAtual.y = Carrinho.getPosicao().y;
    AlvoAtual.z = Carrinho.getPosicao().z;

    // Obeservador 3 pesssoa
    PosicaoObservador.x = 0;
    PosicaoObservador.y = 20;
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

// **********************************************************************
//  void display( void )
//
//
// **********************************************************************
void display( void )
{

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
        glTranslatef ( Carrinho.getPosicao().x, Carrinho.getPosicao().y, Carrinho.getPosicao().z-1 );
        glRotatef(Carrinho.getRotacao(), 0, 1, 0);
        glScalef(0.2, 0.1,0.2);
        glTranslatef(0, 1, 0);
        defineCor(Red);
        DesenhaCubo();
    }
    glPopMatrix();
    glPushMatrix(); 
    {
        defineCor(Gray);
        DesenhaAvioesInimigos();
    }
    glPopMatrix();
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

        cout << "Direcao do carro  " << endl;
        DirCarro.imprime(); cout << endl;
        cout << "\nAngulo do carro  " <<  Carrinho.getRotacao() << endl;
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

        cout << "Direcao do carro  " << endl;
        DirCarro.imprime(); 
        cout << "\nAngulo do carro  " <<  Carrinho.getRotacao() << endl;
        break;
    case 'u':
        DirCamera = Carrinho.getDirecao();
        DirCamera.rotacionaZ(1);
        break;
    case 'j':
        DirCamera = Carrinho.getDirecao();
        DirCamera.rotacionaZ(-1);
        break;
    case 'h':
        DirCamera = Carrinho.getDirecao();
        DirCamera.rotacionaY(-1);
        break;
    case 'k':
        DirCamera = Carrinho.getDirecao();
        DirCamera.rotacionaY(1);
        break;
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



