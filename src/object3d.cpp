/**
 * Arquivo: object3d.cpp
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

#include "object3d.hpp"
#include <iostream>

Object3D::Object3D()
{
    initialize();
}

void Object3D::initialize()
{
    setTypePreview(solidAndWire);
    angle.setX(0);
    angle.setY(0);
    angle.setZ(0);
    dimension.setX(0.0); // 0
    dimension.setY(0.0);
    dimension.setZ(0.0);
    scale.setX(1.0); // 1
    scale.setY(1.0);
    scale.setZ(1.0);
    setNumFaces(0);
    alpha = 1.0;
    setVisible(true);
}

void Object3D::draw()
{
    int i;
    if (!visible || numFaces == 0) {
        return; // Não desenha os objetos, não exibe a gema e as pré-formas
    }
    glPushMatrix();

    glRotatef(sigma, 0, 1, 0);

    glRotatef(angle.getX(), 1, 0, 0);
    glRotatef(angle.getY(), 0, 1, 0);
    glRotatef(angle.getZ(), 0, 0, 1);

    // a translação de ser negative - Bily - gildo
    glTranslatef(-dimension.getX(), -dimension.getY(), -dimension.getZ());

    glScalef(scale.getX(), scale.getY(), scale.getZ());

    glColor4f((float)color.redF(), (float)color.greenF(), (float)color.blueF(), alpha);

    glLineWidth(1);
    // desenha todas as faces da gema
    for (i = 0; i < numFaces; i++) {
        // vetor normal da faces - para onde a face estï¿½ apontando, virada
        glNormal3f(formFaces[i].normal.getX(), formFaces[i].normal.getY(), formFaces[i].normal.getZ());
        // escolhe o tipo de visualizaï¿½ï¿½o : sï¿½lida, wireframe, sï¿½lida/wireframe ou pontos
        switch (typePreview) {
            // solida e solida com wireframe
        case solid:

        case solidAndWire:
            glBegin(GL_TRIANGLES);
            glVertex3f(formFaces[i].vertices[0].getX(), formFaces[i].vertices[0].getY(), formFaces[i].vertices[0].getZ());
            glVertex3f(formFaces[i].vertices[1].getX(), formFaces[i].vertices[1].getY(), formFaces[i].vertices[1].getZ());
            glVertex3f(formFaces[i].vertices[2].getX(), formFaces[i].vertices[2].getY(), formFaces[i].vertices[2].getZ());
            glEnd();
            break;

        case wireframe: // wireframe
            glLineWidth(1);
            glBegin(GL_LINE_LOOP);
            glVertex3f(formFaces[i].vertices[0].getX(), formFaces[i].vertices[0].getY(), formFaces[i].vertices[0].getZ());
            glVertex3f(formFaces[i].vertices[1].getX(), formFaces[i].vertices[1].getY(), formFaces[i].vertices[1].getZ());

            glVertex3f(formFaces[i].vertices[1].getX(), formFaces[i].vertices[1].getY(), formFaces[i].vertices[1].getZ());
            glVertex3f(formFaces[i].vertices[2].getX(), formFaces[i].vertices[2].getY(), formFaces[i].vertices[2].getZ());
            glEnd();
            break;

        case point: // pontos
            glPointSize(2);
            glBegin(GL_POINTS);
            glVertex3f(formFaces[i].vertices[0].getX(), formFaces[i].vertices[0].getY(), formFaces[i].vertices[0].getZ());
            glVertex3f(formFaces[i].vertices[1].getX(), formFaces[i].vertices[1].getY(), formFaces[i].vertices[1].getZ());
            glVertex3f(formFaces[i].vertices[2].getX(), formFaces[i].vertices[2].getY(), formFaces[i].vertices[2].getZ());
            glEnd();
            break;
        }
    }
    // se for solida com wireframe desenha as linhas sobre o solido jah desenhado
    if (typePreview == solidAndWire) {
        glEnable(GL_LINE_SMOOTH); // antialising
        glLineWidth(1);
        for (i = 0; i < numFaces; i++) {
            glBegin(GL_LINE_LOOP);
            glVertex3f(formFaces[i].vertices[0].getX(), formFaces[i].vertices[0].getY(), formFaces[i].vertices[0].getZ());
            glVertex3f(formFaces[i].vertices[1].getX(), formFaces[i].vertices[1].getY(), formFaces[i].vertices[1].getZ());

            glVertex3f(formFaces[i].vertices[1].getX(), formFaces[i].vertices[1].getY(), formFaces[i].vertices[1].getZ());
            glVertex3f(formFaces[i].vertices[2].getX(), formFaces[i].vertices[2].getY(), formFaces[i].vertices[2].getZ());
            glEnd();
        }
    }
    glPopMatrix(); // fim de tudo

}

void Object3D::rotate(int value, char newAngle)
{
    switch (newAngle) {
    case 'x':
        angle.setX(angle.getX() + 1 * value);
        // angle.setX(value);
        break;
    case 'y':
        angle.setY(angle.getY() + 1 * value);
        // angle.setY(value);
        break;
    case 'z':
        angle.setZ(value);
        break;
    }
    draw();
}

void Object3D::changeScale(int value)
{

    if (value == 1) {
        scale.setY(scale.getY() + 0.1 * value);
        scale.setX(scale.getX() + 0.1 * value);
        scale.setZ(scale.getZ() + 0.1 * value);
    }
    if (value == -1 && scale.getX() > 0.1) {
        scale.setX(scale.getX() - 0.1);
        scale.setY(scale.getY() - 0.1);
        scale.setZ(scale.getZ() - 0.1);
    }
    draw();
}

void Object3D::changeColor(int value)
{
    setColor(value);
    draw();
}

void Object3D::setAlpha(float value)
{
    alpha = value / 100;
    draw();
}
