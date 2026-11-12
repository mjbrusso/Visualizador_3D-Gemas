/**
 *
 * Arquivo:   mainwindowimpl.hpp
 * Descricao: Janela Principal
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

#ifndef MAINWINDOWIMPL_H
#define MAINWINDOWIMPL_H

//
#include "object3d.hpp"
#include "point3d.hpp"
#include "stl.hpp"
#include "ui_mainwindow.h"
#include "windowopengl.hpp"
#include <QApplication>
#include <QCheckBox>
#include <QDesktopServices>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QFileDialog>
#include <QGridLayout>
#include <QListWidget>
#include <QMainWindow>
#include <QMessageBox>
#include <QProcess>
#include <QString>
#include <QStringList>
#include <QWheelEvent>
#include <iostream>
#include <qevent.h>
#include <qprinter.h>
#include <stdio.h>
#include <vector>

//-----------------------------------------
#include "aboutVisualizador.hpp"
#include "diagram.hpp"
#include "winaboutimpl.hpp"
#include <QColor>
#include <QColorDialog>
#ifdef WIN32
#include <Windows.h>
#endif

enum TypeArchive {
    archiveStone,
    archiveModel
};

class MainWindowImpl : public QMainWindow, public Ui::MainWindow {
    Q_OBJECT

public:
    MainWindowImpl(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    void loadXml(QString path);

private:
    Object3D stone;
    Object3D model;
    Object3D preShape;
    std::vector<Object3D> preForma;
    std::vector<Object3D> modeloLap;

    Point3D positionInicialStone, angleInicialStone;
    winAboutImpl sobre;
    AboutVisualizador sobreVisaulizador;

    QDomDocument doc;
    QDomElement root;
    QDomNode child;
    QString modelo;
    int index;
    bool open;

    void EnableComponent(bool flag);
    void wheelEvent(QWheelEvent* event);

    void ShowProperties(QDomNode& node);
    void setModel(Object3D newModel);
    void chooseFile(QString filter);
    QString getDiagramSaveFilePath(bool& openAfterSave);
    Object3D getStone();
    Object3D getModel();
    void updateCam();

    void getPositionStoneInicial();
    void setPositionStoneInicial();
    void getAngleStoneInicial();
    void setAngleStoneInicial();
    void rotateStone();
    void translateStone();
    void trocarObjeto();

private slots:
    void on_actionAbrir_Gema_triggered();
    void on_actionMostrar_Eixos_triggered();
    void on_treeWidgetModels_itemSelectionChanged();
    void on_actionSair_triggered();
    void on_actionZoom_triggered();
    void on_actionPoint_triggered();
    void on_actionSolida_Wiframe_triggered();
    void on_actionWireframe_triggered();
    void on_actionSolid_triggered();
    void on_actionRestaurar_triggered();
    void on_sliderAlpha_valueChanged(int value);
    void on_viewerTabWidget_currentChanged(int index);
    void on_geraDiagrama_triggered();
    void on_actionAbout_triggered();
    void on_actionAboutQT_triggered();
    void on_actionColorGem_triggered();
    void on_actionColorPreshape_triggered();
    void on_radioBtnPreForma_clicked();
    void on_radioBtnModelo_clicked();
    void on_actionColorModelo_triggered();
    void on_radioBtnPM_clicked();
    void on_actionAboutVisaulizador_triggered();
    void on_actionZoomMenos_triggered();
    void on_actionFechar_Gema_triggered();
};
#endif
