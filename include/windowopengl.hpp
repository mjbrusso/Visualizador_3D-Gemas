/**
 *
 * Arquivo:   windowopengl.hpp
 * Descricao: desenho dos objetos tridimensionais com openGL
 * Versao:    1.0
 * Autores:   Projeto 3D-Gemas
 *
 **/

// Copyright (c) 2010 UNIVERSIDADE DE PASSO FUNDO,  UNIVERSIDADE FEDERAL DO RIO GRANDE DO SUL, JONAS FERNANDO CECCON FITZ JÓIAS, LUIZ ANTÔNIO SERRANO CARNEIRO, IRACEMA SANTINO LANDO (ARTEFATOS DE PEDRA E MADEIRA), SOM ARTE DESIGN, STUDIO & PUBLICIDADE
//
//  Este arquivo é parte do programa Visualizador 3D-Gemas
//
//  Visualizador 3D-Gemas é um software livre; você pode redistribui-lo e/ou
//  modifica-lo dentro dos termos da Licença Pública Geral GNU como publicada pela Fundação do Software Livre (FSF);
//  na versão 3 da Licença.
//
//  Este programa é distribuido na esperança que possa ser  util,
//  mas SEM NENHUMA GARANTIA; sem uma garantia implicita de ADEQUAÇÂO a qualquer
//  MERCADO ou APLICAÇÃO EM PARTICULAR. Veja a
//  Licença Pública Geral GNU para maiores detalhes.
//
//  Você deve ter recebido uma cópia da Licença Pública Geral GNU
//  junto com este programa, se não, escreva para a Fundação do Software
//  Livre(FSF) Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
//  Autores: Alexandre Lazaretti Zanatta, Álvaro Postal, Evandro Luis Viapiana, Eduardo Presser de Souza, Gildomar Borges Severo, Juliano Tonezer da Silva, Léo Afraneo Hartmann, Marcos José Brusso, Pedro Luiz Juchem, Victor Billy da Silva.

#ifndef WINDOWOPENGL_H
#define WINDOWOPENGL_H

//
#include "object3d.hpp"
#include <GL/glu.h>
#include <QOpenGLWidget>
#include <QtGui>
#include <QtOpenGL>
#include <math.h>

//--Constantes
#define NUMVISIONS 2
#define PASSO 1
#define PASSO2 5
#define BEGINWIDTH 800
#define BEGINHEIGHT 600
#define LARGURA_VISOES 100
#define ALTURA_VISOES 100

//--Structs
struct typeVisions {
    GLint XBegin; // posicao x inicial da viewport
    GLint width; // largura da viewport
    GLint YBegin; // posicao y inicial da viewport
    GLint height; // altura da viewport
    GLint cam; // qual camera esta naquela visao
};

struct typeCam {
    GLint positionX; // posicao x da camera
    GLint positionY; // posicao y da camera
    GLint positionZ; // posicao z da camera
    GLint targetX; // alvo x da visualizacao
    GLint targetY; // alvo y da visualizacao
    GLint targetZ; // alvo z da visualizacao
    GLfloat begin; // inicio da area de visualizacao em profundidade
    GLfloat end; // fim da area de visualizacao em profundidade
    GLfloat angle; // abertura da 'lente' - efeito de zoom
    GLfloat aspect; // relacao entre largura e altura
};

struct typeLight {
    GLfloat position[4]; // posicao da luz
    GLfloat ambiente[4]; // cor da luz ambiente
    GLfloat difusa[4]; // cor da luz difusa
    GLfloat especular[4]; // cor da luz especular
    GLfloat especularidade[4]; // especularidade
};

class WindowOpenGL : public QOpenGLWidget {
    Q_OBJECT

public:
    WindowOpenGL(QWidget* parent);
    void zoom(int value);
    void parametersPreview(int indexCam);
    void paintGL();
    void initializeGL();
    void resizeGL(int width, int height);
    void desenha_eixos(void);
    void setChangeAxis(bool newAxis);
    bool getChangeAxis();
    void setChangeLight(GLint newChangeLight);
    GLint getChangeLight();
    void setChangePreview(GLint newChangePreview);
    bool getChangePreview();
    void setVision(typeVisions newVision);
    typeVisions getVision();
    void setTypeCam(typeCam newCam);
    typeCam getCam();
    void setTypeLight(typeLight newLight);
    typeLight getLight();
    void setStone(Object3D* newStone);
    Object3D* getStone();
    void setModel(Object3D* newModel);
    void setPreShape(Object3D* newModel);
    Object3D* getModel();
    void setPosition(int newPosition);
    // void setCurrentObject(Object3D* newObject);
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void setObject3D(bool flag);
    void resetCamera();
    void definitionLighting();

private:
    bool changeAxis;
    int position;
    GLint changeLight, changePreview;
    GLfloat xRotCam, yRotCam, zRotCam;
    typeVisions vision[NUMVISIONS];
    typeCam cam[NUMVISIONS];
    typeLight light;
    Object3D* stone;
    Object3D* model;
    Object3D* preShape;
    QPoint lastPos;
};
#endif
