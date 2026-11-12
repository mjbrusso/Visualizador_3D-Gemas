/**
 *
 * Arquivo:   object3d.hpp
 * Descricao: objeto tridimensional bem como alguns metodos para sua manipulacao
 * Versao:    1.0
 * Autores: Projeto 3D-Gemas
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

#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "face.hpp"
#include "point3d.hpp"
#include <QColor>
#include <QOpenGLWidget>
#include <QtOpenGL>
#include <stdio.h>

enum TypePreview {
    solid,
    wireframe,
    point,
    solidAndWire
};

class Object3D {

public:
    Object3D();

    void draw();
    void rotate(int value, char newAngle);
    void changeScale(int value);
    void changeColor(int value);
    void initialize();
    void setAlpha(float value);

    void setAngle(Point3D newAngle)
    {
        angle = newAngle;
    }

    [[nodiscard]] Point3D getAngle() const
    {
        return angle;
    }

    void setScale(Point3D newScale)
    {
        scale = newScale;
    }

    [[nodiscard]] Point3D getScale() const
    {
        return scale;
    }

    void setColor(QColor newColor)
    {
        color = newColor;
    }

    [[nodiscard]] QColor getColor() const
    {
        return color;
    }

    void setDimension(Point3D newDimension)
    {
        dimension = newDimension;
        draw();
    }

    [[nodiscard]] Point3D getDimension() const
    {
        return dimension;
    }

    void setFormFaces(Face* newFormFaces)
    {
        formFaces = newFormFaces;
    }

    [[nodiscard]] Face* getFormFaces()
    {
        return formFaces;
    }

    void setTypePreview(TypePreview newTypePreview)
    {
        typePreview = newTypePreview;
    }

    [[nodiscard]] TypePreview getTypePreview() const
    {
        return typePreview;
    }

    void setVisible(bool newVisible)
    {
        visible = newVisible;
    }

    [[nodiscard]] bool getVisible() const
    {
        return visible;
    }

    void setNumFaces(GLint newNumFaces)
    {
        numFaces = newNumFaces;
    }

    [[nodiscard]] GLint getNumFaces() const
    {
        return numFaces;
    }

    void setSigma(double value)
    {
        sigma = value;
    }

private:
    QColor color;
    Point3D angle;
    Point3D scale;
    Point3D dimension;
    Face* formFaces;
    TypePreview typePreview;
    bool visible;
    float alpha;
    GLint numFaces;
    double sigma;
};
#endif
