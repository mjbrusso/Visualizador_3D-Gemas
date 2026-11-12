/**
 * Arquivo: windowopengl.cpp
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
// Autores: Alexandre Lazaretti Zanatta, Álvaro Postal, Evandro Luis Viapiana, Eduardo Presser de Souza, Gildomar Borges Severo, Juliano Tonezer da Silva, Léo Afraneo Hartmann, Marcos José Brusso, Pedro Luiz Juchem, Victor Billy da Silva.

#include "windowopengl.hpp"
#include "mainwindowimpl.hpp"
#include <iostream>

WindowOpenGL::WindowOpenGL(QWidget* parent)
    : QOpenGLWidget(parent)
{
}

void WindowOpenGL::setStone(Object3D* newStone)
{
    stone = newStone;
}

Object3D* WindowOpenGL::getStone()
{
    return stone;
}

void WindowOpenGL::setModel(Object3D* newModel)
{
    model = newModel;
}

void WindowOpenGL::setPreShape(Object3D* newModel)
{
    preShape = newModel;
}

Object3D* WindowOpenGL::getModel()
{
    return model;
}

void WindowOpenGL::parametersPreview(int indexCam)
{
    // seleciona o tipo de matriz para a projeï¿½ï¿½o
    glMatrixMode(GL_PROJECTION);

    // limpa (zera) as matrizes
    glLoadIdentity();

    // Especifica e configura a projeï¿½ï¿½o perspectiva
    gluPerspective(cam[indexCam].angle, cam[indexCam].aspect, cam[indexCam].begin, cam[indexCam].end);

    // Especifica sistema de coordenadas do modelo
    glMatrixMode(GL_MODELVIEW);

    // Inicializa sistema de coordenadas do modelo
    glLoadIdentity();

    glTranslatef(-cam[indexCam].positionX, -cam[indexCam].positionY, -cam[indexCam].positionZ);
    glRotatef(xRotCam, 1.0, 0.0, 0.0);
    glRotatef(yRotCam, 0.0, 1.0, 0.0);
    glRotatef(zRotCam, 0.0, 0.0, 1.0);
    glTranslatef(cam[indexCam].positionX, cam[indexCam].positionY, cam[indexCam].positionZ); // faz a camera voltar para posição de visualização

    // Especifica posiï¿½ï¿½o da camera (o observador) e do alvo
    if ((position != 5 && position != 6 && position != 3)) //   caso seja a visï¿½o superior e inferior
        gluLookAt(cam[indexCam].positionX, cam[indexCam].positionY, cam[indexCam].positionZ, cam[indexCam].targetX, cam[indexCam].targetY, cam[indexCam].targetZ, 0, 1, 0);
    else if (position == 3)
        gluLookAt(cam[indexCam].positionX, cam[indexCam].positionY, cam[indexCam].positionZ, cam[indexCam].targetX, cam[indexCam].targetY, cam[indexCam].targetZ, 0, 0, -1);
    else
        gluLookAt(cam[indexCam].positionX, cam[indexCam].positionY, cam[indexCam].positionZ, cam[indexCam].targetX, cam[indexCam].targetY, cam[indexCam].targetZ, 0, 0, 1);
}

void WindowOpenGL::paintGL()
{
    GLUquadricObj* eixos;

    eixos = gluNewQuadric();
    gluQuadricDrawStyle(eixos, GLU_FILL);

    glClearColor(1, 1, 1, 1);
    // Limpa a janela de visualizaï¿½ï¿½o com a cor de fundo especificada
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // mostra o objteo em todas as viewports
    glEnable(GL_LIGHTING);
    glViewport(vision[0].XBegin, vision[0].YBegin, vision[0].width, vision[0].height);
    // configura a perspectiva e posiciona a camer
    parametersPreview(vision[0].cam);

    static bool once = true;
    if (once) {
        once = false;
        Face f;
        f.normal.setX(0);
        f.normal.setY(0);
        f.normal.setZ(1);
        f.vertices[0].setX(-5);
        f.vertices[0].setY(-5);
        f.vertices[0].setZ(0);
        f.vertices[1].setX(5);
        f.vertices[1].setY(-5);
        f.vertices[1].setZ(0);
        f.vertices[2].setX(0);
        f.vertices[2].setY(5);
        f.vertices[2].setZ(0);
        Object3D debugObj;
        debugObj.setNumFaces(1);
        debugObj.setFormFaces(&f);
        debugObj.setVisible(true);
        debugObj.setTypePreview(solid);
        debugObj.setColor(QColor::fromRgbF(0.0, 1.0, 0.0, 1.0));
        debugObj.draw();
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Composite transparent objects back-to-front without depth writes.
    glDepthMask(GL_FALSE);
    if (model && model->getNumFaces() > 0)
        model->draw();
    if (preShape && preShape->getNumFaces() > 0)
        preShape->draw();
    if (stone && stone->getNumFaces() > 0)
        stone->draw();
    glDepthMask(GL_TRUE);

    glPushMatrix(); // TUDO
    glViewport(0, 0, 100, 100);
    parametersPreview(1);

    if (changeAxis) {
        glEnable(GL_LINE_SMOOTH); // antialising
        glLineWidth(1);
        glPushMatrix();
        glColor4f(0.3, 0.3, 0.0, 1.0);
        gluSphere(eixos, 1, 8, 8);
        glPopMatrix();
        // eixo y
        glPushMatrix();
        glBegin(GL_LINES);
        glColor4f(0.0, 0.0, 1.0, 1.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 11.0, 0.0);
        glEnd();
        glBegin(GL_LINES);
        glVertex3f(0.0, 11.0, 0.0);
        glVertex3f(-1.0, 7.5, 0.0);
        glEnd();
        glBegin(GL_LINES);
        glVertex3f(0.0, 11.0, 0.0);
        glVertex3f(1.0, 7.5, 0.0);
        glEnd();
        // letra-------------------------Y------------------------
        glBegin(GL_LINES);
        glVertex3f(-1.0, 14.0, 0.0);
        glVertex3f(0.0, 13.0, 0.0);
        glEnd();
        glBegin(GL_LINES);
        glVertex3f(1, 14.0, 0.0);
        glVertex3f(0, 13.0, 0.0);
        glEnd();
        glBegin(GL_LINES);
        glVertex3f(0.0, 13.0, 0.0);
        glVertex3f(0.0, 12.0, 0.0);
        glEnd();

        glPopMatrix();
        // eixo x
        glPushMatrix();
        glBegin(GL_LINES);
        glColor4f(0.0, 1.0, 0.0, 1.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(11.0, 0.0, 0.0);
        glEnd();
        glBegin(GL_LINES);
        glVertex3f(11.0, 0.0, 0.0);
        glVertex3f(7.5, 1.0, 0.0);
        glEnd();
        glBegin(GL_LINES);
        glVertex3f(11.0, 0.0, 0.0);
        glVertex3f(7.5, -1.0, 0.0);
        glEnd();
        // letra-------------------X-------------------------------
        glBegin(GL_LINES);
        glVertex3f(13.0, 1.0, 0.0);
        glVertex3f(15.0, -1.0, 0.0);
        glEnd();
        glBegin(GL_LINES);
        glVertex3f(15.0, 1.0, 0.0);
        glVertex3f(13.0, -1.0, 0.0);
        glEnd();
        glPopMatrix();
        // eixo Z
        glPushMatrix();
        glBegin(GL_LINES);
        glColor4f(1.0, 0.0, 0.0, 1.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 0.0, 10.0);
        glEnd();
        glBegin(GL_LINES);
        glVertex3f(0.0, 0.0, 11.0);
        glVertex3f(0.0, 1.0, 7.5);
        glEnd();
        glBegin(GL_LINES);
        glVertex3f(0.0, 0.0, 11.0);
        glVertex3f(0.0, -1.0, 7.5);
        glEnd();
        // letra--------------------------z----------------------------------------------
        glBegin(GL_LINE_STRIP);
        glVertex3f(0.0, 1.0, 13.0);
        glVertex3f(0.0, 1.0, 15.0);
        glVertex3f(0.0, 1.0, 13.0);
        glVertex3f(0.0, -1.0, 15.0);
        glVertex3f(0.0, -1.0, 13.0);
        glVertex3f(0.0, -1.0, 15.0);
        glEnd();
        glPopMatrix();
    }
    glPopMatrix(); // fim de tudo
}

void WindowOpenGL::initializeGL()
{
    // inicialização das variáveis de rotação de camera
    xRotCam = 0;
    yRotCam = 0;
    zRotCam = 0;

    // cor de fundo da janela (RGBA)

    switch (position) {
    case 1: // frente
        cam[0].positionX = 0;
        cam[0].positionY = 0;
        cam[0].positionZ = 80;
        cam[0].targetX = 0;
        cam[0].targetY = 0;
        cam[0].targetZ = 0;
        cam[0].begin = 0.1;
        cam[0].end = 5000.0;
        cam[0].angle = 25;
        cam[0].aspect = BEGINWIDTH / BEGINHEIGHT;
        // posicionamento da camera 1 ( eixos )
        cam[1].positionX = 0;
        cam[1].positionY = 0;
        cam[1].positionZ = 80;
        cam[1].targetX = 0;
        cam[1].targetY = 0;
        cam[1].targetZ = 0;
        cam[1].begin = 0.1;
        cam[1].end = 5000.0;
        cam[1].angle = 25;
        cam[1].aspect = LARGURA_VISOES / ALTURA_VISOES;
        break;

    case 2: // traseira
        cam[0].positionX = 0;
        cam[0].positionY = 0;
        cam[0].positionZ = -80;
        cam[0].targetX = 0;
        cam[0].targetY = 0;
        cam[0].targetZ = 0;
        cam[0].begin = 0.1;
        cam[0].end = 5000.0;
        cam[0].angle = 25;
        cam[0].aspect = BEGINWIDTH / BEGINHEIGHT;
        // posicionamento da camera 1 ( eixos )
        cam[1].positionX = 0;
        cam[1].positionY = 0;
        cam[1].positionZ = -80;
        cam[1].targetX = 0;
        cam[1].targetY = 0;
        cam[1].targetZ = 0;
        cam[1].begin = 0.1;
        cam[1].end = 5000.0;
        cam[1].angle = 25;
        cam[1].aspect = LARGURA_VISOES / ALTURA_VISOES;
        break;

    case 3: // esquerdo
        cam[0].positionX = -80;
        cam[0].positionY = 0;
        cam[0].positionZ = 0;
        cam[0].targetX = 0;
        cam[0].targetY = 0;
        cam[0].targetZ = 0;
        cam[0].begin = 0.1;
        cam[0].end = 5000.0;
        cam[0].angle = 25;
        cam[0].aspect = BEGINWIDTH / BEGINHEIGHT;
        // posicionamento da camera 1 ( eixos )
        cam[1].positionX = -80;
        cam[1].positionY = 0;
        cam[1].positionZ = 0;
        cam[1].targetX = 0;
        cam[1].targetY = 0;
        cam[1].targetZ = 0;
        cam[1].begin = 0.1;
        cam[1].end = 5000.0;
        cam[1].angle = 25;
        cam[1].aspect = LARGURA_VISOES / ALTURA_VISOES;
        break;

    case 4: // direita
        cam[0].positionX = 80;
        cam[0].positionY = 0;
        cam[0].positionZ = 0;
        cam[0].targetX = 0;
        cam[0].targetY = 0;
        cam[0].targetZ = 0;
        cam[0].begin = 0.1;
        cam[0].end = 5000.0;
        cam[0].angle = 25;
        cam[0].aspect = BEGINWIDTH / BEGINHEIGHT;
        // posicionamento da camera 1 ( eixos )
        cam[1].positionX = 80;
        cam[1].positionY = 0;
        cam[1].positionZ = 0;
        cam[1].targetX = 0;
        cam[1].targetY = 0;
        cam[1].targetZ = 0;
        cam[1].begin = 0.1;
        cam[1].end = 5000.0;
        cam[1].angle = 25;
        cam[1].aspect = LARGURA_VISOES / ALTURA_VISOES;
        break;

    case 5: // Superior
        cam[0].positionX = 0;
        cam[0].positionY = 80;
        cam[0].positionZ = 0;
        cam[0].targetX = 0;
        cam[0].targetY = 0;
        cam[0].targetZ = 0;
        cam[0].begin = 0.1;
        cam[0].end = 5000.0;
        cam[0].angle = 25;
        cam[0].aspect = BEGINWIDTH / BEGINHEIGHT;
        // posicionamento da camera 1 ( eixos )
        cam[1].positionX = 0;
        cam[1].positionY = 80;
        cam[1].positionZ = 0;
        cam[1].targetX = 0;
        cam[1].targetY = 0;
        cam[1].targetZ = 0;
        cam[1].begin = 0.1;
        cam[1].end = 5000.0;
        cam[1].angle = 25;
        cam[1].aspect = LARGURA_VISOES / ALTURA_VISOES;
        break;

    case 6: // inferior
        cam[0].positionX = 0;
        cam[0].positionY = -80;
        cam[0].positionZ = 0;
        cam[0].targetX = 0;
        cam[0].targetY = 0;
        cam[0].targetZ = 0;
        cam[0].begin = 0.1;
        cam[0].end = 5000.0;
        cam[0].angle = 25;
        cam[0].aspect = BEGINWIDTH / BEGINHEIGHT;
        // posicionamento da camera 1 ( eixos )
        cam[1].positionX = 0;
        cam[1].positionY = -80;
        cam[1].positionZ = 0;
        cam[1].targetX = 0;
        cam[1].targetY = 0;
        cam[1].targetZ = 0;
        cam[1].begin = 0.1;
        cam[1].end = 5000.0;
        cam[1].angle = 25;
        cam[1].aspect = LARGURA_VISOES / ALTURA_VISOES;
        break;

    case 7: // principal
        cam[0].positionX = 0;
        cam[0].positionY = 0;
        cam[0].positionZ = 80;
        cam[0].targetX = 0;
        cam[0].targetY = 0;
        cam[0].targetZ = 0;
        cam[0].begin = 0.1;
        cam[0].end = 5000.0;
        cam[0].angle = 25;
        cam[0].aspect = BEGINWIDTH / BEGINHEIGHT;
        // posicionamento da camera 1 ( eixos )
        cam[1].positionX = 0;
        cam[1].positionY = 0;
        cam[1].positionZ = 80;
        cam[1].targetX = 0;
        cam[1].targetY = 0;
        cam[1].targetZ = 0;
        cam[1].begin = 0.1;
        cam[1].end = 5000.0;
        cam[1].angle = 25;
        cam[1].aspect = LARGURA_VISOES / ALTURA_VISOES;
        break;
    }

    // configura as visï¿½es
    vision[0].XBegin = 0;
    vision[0].width = BEGINWIDTH;
    vision[0].YBegin = 0;
    vision[0].height = BEGINHEIGHT;
    vision[0].cam = 0;

    vision[1].XBegin = 0;
    vision[1].width = 100;
    vision[1].YBegin = 0;
    vision[1].height = 100;
    vision[1].cam = 1;
    // posiï¿½ï¿½o da fonte de luz
    light.position[0] = 0.0;
    light.position[1] = 0.0;
    light.position[2] = 400.0;
    light.position[3] = 1.0;

    // cor e intensidade da luz ambiente
    light.ambiente[0] = 0.3;
    light.ambiente[1] = 0.3;
    light.ambiente[2] = 0.3;
    light.ambiente[3] = 0.3;

    // cor e intensidade da luz difusa
    light.difusa[0] = 0.7;
    light.difusa[1] = 0.7;
    light.difusa[2] = 0.7;
    light.difusa[3] = 1.0;

    // cor e intensidade da luz especular
    light.especular[0] = 0.7;
    light.especular[1] = 0.7;
    light.especular[2] = 0.7;
    light.especular[3] = 1.0;

    // cor e intensidade da especularidade
    light.especularidade[0] = 1.0;
    light.especularidade[1] = 1.0;
    light.especularidade[2] = 1.0;
    light.especularidade[3] = 1.0;

    if (stone && stone->getFormFaces() == nullptr && stone->getNumFaces() == 0)
        stone->initialize();
    if (preShape && preShape->getFormFaces() == nullptr && preShape->getNumFaces() == 0)
        preShape->initialize();
    if (model && model->getFormFaces() == nullptr && model->getNumFaces() == 0)
        model->initialize();

    // Habilita o depth-buffering
    glEnable(GL_DEPTH_TEST);
    // ativa a remocao das faces ocultas
    glEnable(GL_CULL_FACE);
    // Habilita o modelo de colorizacao de Gouraud
    glShadeModel(GL_SMOOTH);
    // habilita calculo transparencia
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    definitionLighting();
}

void WindowOpenGL::resizeGL(int width, int height)
{
    glClearColor(0.9, 0.9, 0.9, 1.0);
    glViewport(0, 0, width, height);
    cam[0].aspect = (float)width / (float)height;
    vision[0].width = width;
    vision[0].height = height;
}

void WindowOpenGL::mousePressEvent(QMouseEvent* event)
{
    lastPos = event->pos();
}

void WindowOpenGL::mouseMoveEvent(QMouseEvent* event)
{
    const QPoint currentPos = event->position().toPoint();

    if (event->buttons() & Qt::LeftButton) {
        if (currentPos.x() > lastPos.x()) {
            yRotCam += PASSO2;
            if (yRotCam > 360)
                yRotCam = 0;
        }
        if (currentPos.x() < lastPos.x()) {
            yRotCam -= PASSO2;
            if (yRotCam < -360)
                yRotCam = 0;
        }
        if (currentPos.y() > lastPos.y()) {
            xRotCam += PASSO2;
            if (xRotCam > 360)
                xRotCam = 0;
        }
        if (currentPos.y() < lastPos.y()) {
            xRotCam -= PASSO2;
            if (xRotCam < -360)
                xRotCam = 0;
        }
    } else if (event->buttons() & Qt::RightButton) {
        if (currentPos.x() < lastPos.x()) {
            zRotCam -= PASSO2;
            if (zRotCam < -360)
                zRotCam = 0;
        }
        if (currentPos.x() > lastPos.x()) {
            zRotCam += PASSO2;
            if (zRotCam > 360)
                zRotCam = 0;
        }
    }

    update();
    lastPos = currentPos;
}

void WindowOpenGL::zoom(int value)
{
    if (value == -1 && cam[0].angle <= 5)
        return;
    if (value == 1 && cam[0].angle >= 100)
        return;
    else
        cam[0].angle += (value * PASSO);
    update();
}

void WindowOpenGL::setObject3D(bool flag)
{
    stone->setVisible(flag);
    model->setVisible(flag);
    preShape->setVisible(flag);
}

void WindowOpenGL::resetCamera()
{
    xRotCam = 0;
    yRotCam = 0;
    zRotCam = 0;
    cam[0].angle = 25;
    update();
}

void WindowOpenGL::setChangeAxis(bool newAxis)
{
    changeAxis = newAxis;
    update();
}

bool WindowOpenGL::getChangeAxis()
{
    return changeAxis;
}

void WindowOpenGL::setPosition(int newPosition)
{
    position = newPosition;
}

void WindowOpenGL::definitionLighting()
{
    // habilita iluminacao
    glEnable(GL_LIGHTING);

    // Ativa o uso da luz ambiente
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light.ambiente);

    // habilita e configura a luz 0 (podem ter 8. de 0 a 7)
    // posicao da luz no universo
    glLightfv(GL_LIGHT0, GL_POSITION, light.position);

    // configura a luz ambiente
    glLightfv(GL_LIGHT0, GL_AMBIENT, light.ambiente);
    // configura a luz difusa
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light.difusa);
    // configura a luz especular
    glLightfv(GL_LIGHT0, GL_SPECULAR, light.especular);

    // habilita a luz 0
    glEnable(GL_LIGHT0);

    // habilita a cor do material no calculo das cores do objeto
    glEnable(GL_COLOR_MATERIAL);

    // Define a reflexao do material
    glMaterialfv(GL_FRONT, GL_SPECULAR, light.especularidade);

    // define o brilho do material
    glMateriali(GL_FRONT, GL_SHININESS, 3);
}
