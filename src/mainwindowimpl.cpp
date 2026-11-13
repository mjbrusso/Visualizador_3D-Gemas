/**
 * Arquivo: mainwindowimpl.cpp
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

#include "mainwindowimpl.hpp"
#include <QShortcut>

using namespace std;
//

MainWindowImpl::MainWindowImpl(QWidget* parent, Qt::WindowFlags f)
    : QMainWindow(parent, f)
{
    setupUi(this);
    propertiesTabWidget->tabBar()->hide();
    propertiesTabWidget->setCurrentIndex(1);
    // lblNovoLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    // lblNovoLabel->raise();

    QObject::connect(sliderAlpha, &QSlider::valueChanged,
        lcdNumber, static_cast<void (QLCDNumber::*)(int)>(&QLCDNumber::display));

    auto* increaseTransparency = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Up), this);
    increaseTransparency->setContext(Qt::ApplicationShortcut);
    QObject::connect(increaseTransparency, &QShortcut::activated, this, [this]() {
        if (sliderAlpha->isEnabled())
            sliderAlpha->setValue(qMin(sliderAlpha->maximum(), sliderAlpha->value() + 5));
    });

    auto* decreaseTransparency = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Down), this);
    decreaseTransparency->setContext(Qt::ApplicationShortcut);
    QObject::connect(decreaseTransparency, &QShortcut::activated, this, [this]() {
        if (sliderAlpha->isEnabled())
            sliderAlpha->setValue(qMax(sliderAlpha->minimum(), sliderAlpha->value() - 5));
    });

    treeWidgetModels->setColumnWidth(0, 200);
    windowGL->setPosition(7);
    windowGL->setStone(&stone);
    windowGL->setPreShape(&preShape);
    windowGL->setModel(&model);
    windowGL->setChangeAxis(true);

    camera1->setPosition(1);
    camera1->setStone(&stone);
    camera1->setPreShape(&preShape);
    camera1->setModel(&model);
    camera1->setChangeAxis(false);

    camera2->setPosition(5);
    camera2->setStone(&stone);
    camera2->setPreShape(&preShape);
    camera2->setModel(&model);
    camera2->setChangeAxis(false);

    camera3->setPosition(6);
    camera3->setStone(&stone);
    camera3->setPreShape(&preShape);
    camera3->setModel(&model);
    camera3->setChangeAxis(false);

    camera4->setPosition(3);
    camera4->setStone(&stone);
    camera4->setPreShape(&preShape);
    camera4->setModel(&model);
    camera4->setChangeAxis(false);

    open = false;
    checkBoxGema->setChecked(true);
    checkBoxPreForma->setChecked(true);
    checkBoxModelo->setChecked(false);
    EnableComponent(false);
}

void MainWindowImpl::chooseFile(QString filter)
{
    // GLint numFaces=0;
    QString auxXml;

    auxXml = QFileDialog::getOpenFileName(this, tr("Abrir Gema"), QApplication::applicationDirPath() + "/Gem/", "" + filter);
    if (!auxXml.isNull()) {
        // Sempre limpar o estado anterior antes de abrir uma nova gema.
        // A primeira abertura estava pulando este reset porque open == false,
        // o que forçava a necessidade de abrir o arquivo uma segunda vez.
        stone.setFormFaces(0);
        stone.setNumFaces(0);
        stone.initialize();
        stone.setVisible(true);

        preShape.setNumFaces(0);
        preShape.setFormFaces(0);
        preShape.initialize();
        preShape.setVisible(true);

        model.setNumFaces(0);
        model.setFormFaces(0);
        model.initialize();
        model.setVisible(true);

        root.clear();
        doc.clear();
        child.clear();

        preForma.clear(); // limpa o vector com as pre-formas
        modeloLap.clear(); // limpa o vector com os modelos de lapidação

        treeWidgetModels->clear(); // limpa a lista que contém as pre-formas.

        windowGL->resetCamera();
        camera1->resetCamera();
        camera2->resetCamera();
        camera3->resetCamera();
        camera4->resetCamera();

        setPositionStoneInicial();
        setAngleStoneInicial();

        EnableComponent(false);
        checkBoxGema->setChecked(true);
        checkBoxPreForma->setChecked(true);
        checkBoxModelo->setChecked(false);
        sliderAlpha->setValue(50);
        windowGL->setObject3D(true);

        // chama a função loadXml para carregar os dados da Gema
        loadXml(auxXml);
    }
}

void MainWindowImpl::on_actionAbrir_Gema_triggered()
{
    chooseFile("xml (*.xml)");
}

void MainWindowImpl::loadXml(QString path)
{
    index = 0;
    GLint numFaces = 0;

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) // Abre e testa se o arquivo XMl foi aberto
    {
        QMessageBox::critical(0, tr("Critical Error"), tr("Esta Gema ainda nao possui projeto de Lapidacao\nCannot open file XML %1").arg(path));
        return; // finaliza a função
    } else {
        if (!doc.setContent(&file)) // copia o arquivo Xml para QDomDocument doc e testa o parser Xml(foi copiado corretamente)
        {
            QMessageBox::critical(0, tr("Critical Error"), tr("Erro no Projeto de lapidação\nParsing error for file XML %1").arg(path));
            file.close(); // Fecha o arquivo
            return; // finaliza a função
        }
        file.close(); // fecha o arquivo definitivamente

        root = doc.documentElement(); // Ele acha o Tag pai Projeto

        child = root.firstChild(); // Aponta child para o nodo filho Gema

        // lê as propriedades da gema dá preforma e mostras mas labels
        if (!root.isNull()) {
            this->labelFileGem->setText(root.attribute("Name"));
            this->labelOwner->setText(child.firstChild().toElement().text());
            this->labelId->setText(child.firstChild().nextSiblingElement("GemIdentity").toElement().text());
            this->labelDate->setText(child.firstChild().nextSiblingElement("Date").toElement().text());
        } else {
            this->labelFileGem->setText("Não existe Projeto");
        }
        // aponta child para o filho da Gema-> Models -> ModelType
        child = root.firstChild().firstChild().nextSiblingElement("Models").firstChild();
        // adiciona os nomes das PréForm nas treeWidgetModels
        // int countPreforma = 0;
        QTreeWidgetItem *typePreShape = nullptr, *firstShape = nullptr;
        while (!child.isNull()) {
            if (child.isElement()) {
                typePreShape = new QTreeWidgetItem(treeWidgetModels);
                if (firstShape == NULL)
                    firstShape = typePreShape;
                typePreShape->setText(0, child.toElement().attribute("Name"));

                //---------------------------------------------------------------------------------------------------------------------------
                preShape.setFormFaces(STL::loadFaces(QApplication::applicationDirPath() + "/Models/" + child.toElement().attribute("Name") + "/" + "P" + child.toElement().attribute("Name") + ".stl", numFaces));
                preShape.setNumFaces(numFaces);
                preShape.setTypePreview(solid);
                preShape.setColor(QColor::fromRgbF(0.4, 0.4, 0.0, 1.0)); // Abre o modelo com amarela
                preShape.setAlpha(90);
                //-------------------------------------------------------------------------------------------------------
                Point3D newPoint;
                newPoint.setX(child.firstChild().nextSiblingElement("Scale-Model").toElement().text().toFloat());
                newPoint.setY(child.firstChild().nextSiblingElement("Scale-Model").toElement().text().toFloat());
                newPoint.setZ(child.firstChild().nextSiblingElement("Scale-Model").toElement().text().toFloat());
                preShape.setScale(newPoint); // pegar escalado xml-> tem qe ser implementado
                //-----------------------------------------------------------------------------------------------------
                preForma.push_back(preShape);
                numFaces = 0;
                //---------------------------------------------------------------------------------------------------------------------------
                //---------------------------------------------------------------------------------------------------------------------------
                model.setFormFaces(STL::loadFaces(QApplication::applicationDirPath() + "/Models/" + child.toElement().attribute("Name") + "/" + child.toElement().attribute("Name") + ".stl", numFaces));
                model.setNumFaces(numFaces);
                model.setTypePreview(solid);
                model.setColor(QColor::fromRgbF(0.6, 0.6, 0.0, 1.0)); // Abre o modelo com amarela
                model.setAlpha(90);
                //-------------------------------------------------------------------------------------------------------
                newPoint.setX(child.firstChild().nextSiblingElement("Scale-Model").toElement().text().toFloat());
                newPoint.setY(child.firstChild().nextSiblingElement("Scale-Model").toElement().text().toFloat());
                newPoint.setZ(child.firstChild().nextSiblingElement("Scale-Model").toElement().text().toFloat());
                model.setScale(newPoint); // pegar escalado xml-> tem qe ser implementado
                //--------------------------------------------------------------------------------------------------------------
                modeloLap.push_back(model);
                numFaces = 0;
                //---------------------------------------------------------------------------------------------------------------------------
            }
            child = child.nextSiblingElement("ModelType");
        }
        if (firstShape != NULL) {
            index = 0;
            typePreShape = firstShape;
            treeWidgetModels->setCurrentItem(typePreShape, 0);
            treeWidgetModels->setCurrentIndex(treeWidgetModels->model()->index(0, 0));
            on_treeWidgetModels_itemSelectionChanged();
        }

        model = modeloLap[0];
        preShape = preForma[0];

        stone.setVisible(true);
        preShape.setVisible(true);
        model.setVisible(true);
        checkBoxGema->setChecked(true);
        checkBoxPreForma->setChecked(true);
        checkBoxModelo->setChecked(false);
        trocarObjeto();

        // aponta child para o filho da Gema-> Models -> ModelType
        child = root.firstChild().firstChild().nextSiblingElement("Models").firstChild();
        modelo = child.toElement().attribute("Name");

        ShowProperties(child);
        translateStone();
        rotateStone();
        updateCam();
    }

    //-----------------------------------------------------Leitura da Gema do xml---------------------------------------------------------------------------
    GLint sizeVert = 0, count = 0;
    Face* faceXml = 0;

    QDomNode meshData = root.firstChild().firstChild().nextSiblingElement("meshData").firstChild(); // aponta para o nodo vertices
    sizeVert = meshData.toElement().attribute("sizeVertices").toInt(); // pega a quantidade de vertices que a gema possui
    numFaces = meshData.nextSiblingElement("faces").toElement().attribute("sizeFaces").toInt(); // pega a quantidade de faces que a gema possui

    if (faceXml)
        delete[] faceXml;

    faceXml = new Face[numFaces]; // faces lidas do Xml
    Point3D vertice[sizeVert]; // Vertices lidos do Xml

    meshData = meshData.firstChild(); // aponta para no nodo point filho do vertices

    while (!meshData.isNull()) {
        if (meshData.isElement()) {
            vertice[count].setX(meshData.toElement().attribute("x").toFloat());
            vertice[count].setY(meshData.toElement().attribute("y").toFloat());
            vertice[count].setZ(meshData.toElement().attribute("z").toFloat());
        }
        meshData = meshData.nextSibling();
        count++;
    }

    count = 0;
    meshData = root.firstChild().firstChild().nextSiblingElement("meshData").firstChild().nextSiblingElement("faces").firstChild(); // aponta para index filho do nodo Faces

    while (!meshData.isNull()) {
        if (meshData.isElement()) {
            faceXml[count].normal.setX(meshData.toElement().attribute("normalX").toFloat());
            faceXml[count].normal.setY(meshData.toElement().attribute("normalY").toFloat());
            faceXml[count].normal.setZ(meshData.toElement().attribute("normalZ").toFloat());

            faceXml[count].vertices[0].setX(vertice[meshData.toElement().attribute("index1").toInt()].getX());
            faceXml[count].vertices[0].setY(vertice[meshData.toElement().attribute("index1").toInt()].getY());
            faceXml[count].vertices[0].setZ(vertice[meshData.toElement().attribute("index1").toInt()].getZ());

            faceXml[count].vertices[1].setX(vertice[meshData.toElement().attribute("index2").toInt()].getX());
            faceXml[count].vertices[1].setY(vertice[meshData.toElement().attribute("index2").toInt()].getY());
            faceXml[count].vertices[1].setZ(vertice[meshData.toElement().attribute("index2").toInt()].getZ());

            faceXml[count].vertices[2].setX(vertice[meshData.toElement().attribute("index3").toInt()].getX());
            faceXml[count].vertices[2].setY(vertice[meshData.toElement().attribute("index3").toInt()].getY());
            faceXml[count].vertices[2].setZ(vertice[meshData.toElement().attribute("index3").toInt()].getZ());
        }
        meshData = meshData.nextSibling();
        count++;
    }
    stone.setFormFaces(faceXml);
    stone.setNumFaces(numFaces);
    stone.setVisible(true);
    stone.setColor(QColor::fromRgbF(0.0, 0.0, 1.0, 1.0)); // seta a pedra para a cor azul
    stone.setTypePreview(solid); // seta para modo de visualzação
    stone.setAlpha(100 - sliderAlpha->value());
    stone.setVisible(true);
    on_actionSolid_triggered();
    actionSolid->setChecked(true); // selecina botão do sólido
    getPositionStoneInicial(); // guarda a posição (dimension) inicial da Gema(Stone);
    getAngleStoneInicial(); // guarda os angulos iniciais da gema
    checkBoxGema->setChecked(true);
    checkBoxPreForma->setChecked(true);
    checkBoxModelo->setChecked(false);
    trocarObjeto();
    updateCam();

    // Atualiza o desenho nas tab para que não ocorra distorção durante a criação do diagrama
    this->viewerTabWidget->setCurrentIndex(1);
    this->viewerTabWidget->setCurrentIndex(0);

    //--------------------------------------------------------------------------------------------------------------------------------------
    open = true;
    stone.setVisible(true);
    preShape.setVisible(true);
    model.setVisible(true);
    windowGL->setObject3D(true);
    EnableComponent(true); // habilita os componentes
    trocarObjeto();
    updateCam();
}

void MainWindowImpl::ShowProperties(QDomNode& node)
{
    this->labelCrown->setText(node.firstChild().nextSiblingElement("Measures").firstChild().nextSiblingElement("Preform").firstChild().toElement().text()); // altura do coroa da preforma
    this->labelGirdle->setText(node.firstChild().nextSiblingElement("Measures").firstChild().nextSiblingElement("Girdle").toElement().text()); // Altura da cintura
    this->labelPavilion->setText(node.firstChild().nextSiblingElement("Measures").firstChild().nextSiblingElement("Preform").firstChild().nextSiblingElement("Pavilion").toElement().text()); // Altura da pavilhao da preforma
    this->labelHeightTotal->setText(node.firstChild().nextSiblingElement("Measures").firstChild().nextSiblingElement("Preform").firstChild().nextSiblingElement("Total").toElement().text()); // altura total da pre-Forma

    this->labelTableM1->setText(node.firstChild().nextSiblingElement("Measures").firstChild().nextSiblingElement("TableM1").toElement().text()); // tamanho da mesa
    this->labelTableM2->setText(node.firstChild().nextSiblingElement("Measures").firstChild().nextSiblingElement("TableM2").toElement().text());
    this->labelGirdleL1->setText(node.firstChild().nextSiblingElement("Measures").firstChild().nextSiblingElement("GirdleL1").toElement().text());
    this->labelGirdleL2->setText(node.firstChild().nextSiblingElement("Measures").firstChild().nextSiblingElement("GirdleL2").toElement().text());

    this->labelHarnessingPre->setText(node.firstChild().nextSiblingElement("Volume-Yield").toElement().text() + " %"); // aproveitamento para gema nesta pre-forma
    updateCam();
}

void MainWindowImpl::on_treeWidgetModels_itemSelectionChanged()
{
    QTreeWidgetItem* typePreShape;
    typePreShape = treeWidgetModels->currentItem();
    // Guarda na várial index o indice do modelo selecionado
    index = treeWidgetModels->indexOfTopLevelItem(typePreShape);

    // Guarda nos objetos modelo e preShape  um cópia do modelolap[index] e preforma[index], utiliznado os mesmo para serem visualizados pelo usuário
    model = modeloLap[index];
    preShape = preForma[index];

    // guarda o nome do modelo q esta selecionado pelo usuário
    modelo = typePreShape->text(0);

    trocarObjeto();

    // aponta node child para o nodo filho de Models -> ModelsType
    child = root.firstChild().firstChild().nextSiblingElement("Models").firstChild();

    while (!child.isNull()) {
        if (child.toElement().attribute("Name") == typePreShape->text(0)) {
            ShowProperties(child);
            translateStone();
            rotateStone();
            updateCam();
            return; // finaliza a função
        } else
            child = child.nextSibling();
    }

    updateCam();
}

void MainWindowImpl::wheelEvent(QWheelEvent* event)
{
    int delta = event->angleDelta().y();
    if (delta < 0) { // se o valor for menor e que zero-> aumenta o zoom
        windowGL->zoom(-1);
        camera1->zoom(-1);
        camera2->zoom(-1);
        camera3->zoom(-1);
        camera4->zoom(-1);
    } else {
        windowGL->zoom(1);
        camera1->zoom(1);
        camera2->zoom(1);
        camera3->zoom(1);
        camera4->zoom(1);
    }
    // senão -> diminui o zoom
    event->accept();
    updateCam();
}

void MainWindowImpl::on_actionSair_triggered()
{
    this->close();
}

void MainWindowImpl::on_actionFechar_Gema_triggered()
{
    int resposta;
    resposta = QMessageBox::question(this, tr("CAD-Gemas Viewer"), tr("Você tem certeza que deseja fechar este arquivo?"), QMessageBox::Yes | QMessageBox::No);
    if (resposta == QMessageBox::Yes) {
        treeWidgetModels->clear();
        windowGL->setObject3D(false);
        EnableComponent(false);
        stone.setFormFaces(0);
        stone.setNumFaces(0);
        model.setFormFaces(0);
        model.setNumFaces(0);
        open = true;
    } else
        return;
}

void MainWindowImpl::EnableComponent(bool flag)
{
    if (!flag) {
        this->labelFileGem->setText("");
        this->labelOwner->setText("");
        this->labelId->setText("");
        this->labelDate->setText("");
        this->labelCrown->setText("");
        this->labelGirdle->setText("");
        this->labelPavilion->setText("");
        this->labelHeightTotal->setText("");
        this->labelHarnessingPre->setText("");
        this->labelTableM1->setText("");
        this->labelTableM2->setText("");
        this->labelGirdleL1->setText("");
        this->labelGirdleL2->setText("");
    }
    windowGL->setEnabled(flag);
    propertiesTabWidget->setEnabled(flag);
    lblNovoLabel->setVisible(flag);
    actionMostrar_Eixos->setEnabled(flag);
    actionFechar_Gema->setEnabled(flag);
    actionZoom->setEnabled(flag);
    actionZoomMenos->setEnabled(flag);
    actionSolid->setEnabled(flag);
    actionWireframe->setEnabled(flag);
    actionSolida_Wiframe->setEnabled(flag);
    actionPoint->setEnabled(flag);
    actionRestaurar->setEnabled(flag);
    actionExibirCameras->setEnabled(flag);
    geraDiagrama->setEnabled(flag);
    viewerTabWidget->setEnabled(flag);
    actionColorPreshape->setEnabled(flag);
    actionColorGem->setEnabled(flag);
    actionColorModelo->setEnabled(flag);
}

void MainWindowImpl::on_actionZoom_triggered()
{
    windowGL->zoom(-1);
    camera1->zoom(-1);
    camera2->zoom(-1);
    camera3->zoom(-1);
    camera4->zoom(-1);
}

void MainWindowImpl::on_actionZoomMenos_triggered()
{
    windowGL->zoom(1);
    camera1->zoom(1);
    camera2->zoom(1);
    camera3->zoom(1);
    camera4->zoom(1);
}

void MainWindowImpl::on_actionSolid_triggered()
{
    stone.setTypePreview(solid);
    actionSolid->setCheckable(true);
    actionSolid->setChecked(true);
    actionWireframe->setCheckable(false);
    actionWireframe->setChecked(false);
    actionSolida_Wiframe->setCheckable(false);
    actionSolida_Wiframe->setChecked(false);
    actionPoint->setCheckable(false);
    actionPoint->setChecked(false);
    updateCam();
}

void MainWindowImpl::on_actionWireframe_triggered()
{
    stone.setTypePreview(wireframe);
    actionSolid->setCheckable(false);
    actionSolid->setChecked(false);
    actionWireframe->setCheckable(true);
    actionWireframe->setChecked(true);
    actionSolida_Wiframe->setCheckable(false);
    actionSolida_Wiframe->setChecked(false);
    actionPoint->setCheckable(false);
    actionPoint->setChecked(false);
    updateCam();
}

void MainWindowImpl::on_actionSolida_Wiframe_triggered()
{
    stone.setTypePreview(solidAndWire);
    actionSolid->setCheckable(false);
    actionSolid->setChecked(false);
    actionWireframe->setCheckable(false);
    actionWireframe->setChecked(false);
    actionSolida_Wiframe->setCheckable(true);
    actionSolida_Wiframe->setChecked(true);
    actionPoint->setCheckable(false);
    actionPoint->setChecked(false);
    updateCam();
}

void MainWindowImpl::on_actionPoint_triggered()
{
    stone.setTypePreview(point);
    actionSolid->setCheckable(false);
    actionSolid->setChecked(false);
    actionWireframe->setCheckable(false);
    actionWireframe->setChecked(false);
    actionSolida_Wiframe->setCheckable(false);
    actionSolida_Wiframe->setChecked(false);
    actionPoint->setCheckable(true);
    actionPoint->setChecked(true);
    updateCam();
}

void MainWindowImpl::on_actionRestaurar_triggered()
{
    windowGL->resetCamera();
    camera1->resetCamera();
    camera2->resetCamera();
    camera3->resetCamera();
    camera4->resetCamera();

    sliderAlpha->setValue(50);
    stone.setAlpha(100 - sliderAlpha->value());

    actionMostrar_Eixos->setChecked(true);
    windowGL->setChangeAxis(true);
    on_actionSolid_triggered();
    //-----------------------------------------------------------------------------------------
    stone.setColor(QColor::fromRgbF(0.0, 0.0, 1.0, 1.0));

    for (vector<Object3D>::iterator it = preForma.begin(); it != preForma.end(); it++)
        it->setColor(QColor::fromRgbF(0.4, 0.4, 0.0, 1.0));
    preShape = preForma[index];
    for (vector<Object3D>::iterator it = modeloLap.begin(); it != modeloLap.end(); it++)
        it->setColor(QColor::fromRgbF(0.6, 0.6, 0.0, 1.0));
    model = modeloLap[index];
    //--------------------------------------------------------------------------------------------
    checkBoxPreForma->setChecked(true);
    checkBoxModelo->setChecked(false);
    trocarObjeto();
}

void MainWindowImpl::on_actionMostrar_Eixos_triggered()
{
    windowGL->setChangeAxis(!windowGL->getChangeAxis());
}

void MainWindowImpl::updateCam()
{
    windowGL->update();
    camera1->update();
    camera2->update();
    camera3->update();
    camera4->update();
}

void MainWindowImpl::getPositionStoneInicial() // guarda a posição inicial da gema
{
    positionInicialStone = stone.getDimension();
}

void MainWindowImpl::setPositionStoneInicial() // seta a gema para posição inicial
{
    Point3D newPoint;
    newPoint.setX(positionInicialStone.getX());
    newPoint.setY(positionInicialStone.getY());
    newPoint.setZ(positionInicialStone.getZ());
    stone.setDimension(newPoint);
    updateCam();
}

void MainWindowImpl::getAngleStoneInicial() // guarda os angulos iniciais da gema
{
    angleInicialStone = stone.getAngle();
}

void MainWindowImpl::setAngleStoneInicial() // seta para os angulos iniciais da gema
{
    Point3D newPoint;
    newPoint.setX(angleInicialStone.getX());
    newPoint.setY(angleInicialStone.getY());
    newPoint.setZ(angleInicialStone.getZ());
    stone.setAngle(newPoint);
}

void MainWindowImpl::rotateStone() // rotaciona a gema atraves do XML
{
    setAngleStoneInicial(); // eh necessário fazer  a gema Voltar ao angulo inicial para fazer as rotações corretamente
    Point3D newPoint;
    QDomElement gemCentering = child.firstChild().nextSiblingElement("GemCentering").toElement();
    QDomElement rotate = gemCentering.firstChildElement("Rotate");
    newPoint.setX(rotate.firstChildElement("x").text().toFloat());
    newPoint.setY(rotate.firstChildElement("y").text().toFloat());
    newPoint.setZ(rotate.firstChildElement("z").text().toFloat());
    stone.setAngle(newPoint);
    stone.setSigma(gemCentering.firstChildElement("Sigma").text().toDouble());
}

void MainWindowImpl::translateStone() // translata a gema atraves do XML
{
    Point3D newPoint;
    QDomElement gemCentering = child.firstChild().nextSiblingElement("GemCentering").toElement();
    QDomElement translate = gemCentering.firstChildElement("Translate");
    newPoint.setX(translate.firstChildElement("x").text().toFloat());
    newPoint.setY(translate.firstChildElement("y").text().toFloat());
    newPoint.setZ(translate.firstChildElement("z").text().toFloat());
    stone.setDimension(newPoint);
}

void MainWindowImpl::on_sliderAlpha_valueChanged(int value)
{
    stone.setAlpha(100 - value);
    updateCam();
}

//--DIAGRAMA
QString MainWindowImpl::getDiagramSaveFilePath(bool& openAfterSave)
{
    QFileDialog dialog(this, tr("Salvar diagrama"));
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix("pdf");
    dialog.setNameFilter(tr("Arquivo PDF (*.pdf)"));
    dialog.setDirectory(QDir::homePath());

    QString defaultFileName = root.attribute("Name").replace(" ", "") + ".pdf";
    dialog.selectFile(defaultFileName);

    if (dialog.exec() != QDialog::Accepted)
        return QString();

    openAfterSave = QMessageBox::question(
        this,
        tr("Salvar diagrama"),
        tr("Deseja abrir o diagrama após salvar?"),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::Yes) == QMessageBox::Yes;

    QString filePath = dialog.selectedFiles().first();

    if (!filePath.toLower().endsWith(".pdf"))
        filePath += ".pdf";

    return filePath;
}

void MainWindowImpl::on_geraDiagrama_triggered()
{
    if (!checkBoxPreForma->isChecked() || checkBoxModelo->isChecked()) {
        int resposta;
        resposta = QMessageBox::question(this, tr("Visualizador 3D-Gemas"), tr("Para gerar o Diagrama de Lapidação, deve ser exibida somente a pré-forma. Deseja selecionar somente a pré-forma?"), QMessageBox::Yes | QMessageBox::No);
        if (resposta == QMessageBox::Yes) {
            checkBoxPreForma->setChecked(true);
            checkBoxModelo->setChecked(false);
            trocarObjeto();
            on_geraDiagrama_triggered();
        } else
            return;
    } else {
        // seta modelo solid
        stone.setTypePreview(solid);
        this->viewerTabWidget->setCurrentIndex(1);
        updateCam();
        this->viewerTabWidget->setCurrentIndex(0);
        this->viewerTabWidget->setCurrentIndex(1);
        updateCam();

        bool openAfterSave = true;
        QString filePath = getDiagramSaveFilePath(openAfterSave);

        if (filePath.isEmpty())
            return;

        QImage img1 = camera1->grabFramebuffer();
        QImage img2 = camera2->grabFramebuffer();
        QImage img3 = camera3->grabFramebuffer();
        QImage img4 = camera4->grabFramebuffer();

        Diagram::generate(modelo, filePath, root, child, img1, img2, img3, img4);

        if (openAfterSave) {
            QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
        }
    }
}

void MainWindowImpl::on_viewerTabWidget_currentChanged(int index)
{
    Q_UNUSED(index);
    if (this->viewerTabWidget->currentIndex() == 1)
        actionMostrar_Eixos->setEnabled(false);
    else
        actionMostrar_Eixos->setEnabled(true);
}

void MainWindowImpl::on_actionAboutQT_triggered()
{
    QApplication::aboutQt();
}

void MainWindowImpl::on_actionAbout_triggered()
{
    sobre.setWindowModality(Qt::ApplicationModal);
    sobre.show();
    sobre.raise();
    sobre.activateWindow();
}

void MainWindowImpl::on_actionAboutVisaulizador_triggered()
{
    sobreVisaulizador.setWindowModality(Qt::ApplicationModal);
    sobreVisaulizador.show();
    sobreVisaulizador.raise();
    sobreVisaulizador.activateWindow();
}

void MainWindowImpl::on_actionColorGem_triggered()
{
    QColor color = QColorDialog::getColor(Qt::blue, this);
    if (color.isValid())
        stone.setColor(color);

    updateCam();
}

void MainWindowImpl::on_actionColorPreshape_triggered()
{
    QColor color = QColorDialog::getColor(Qt::yellow, this);
    if (color.isValid()) {
        for (vector<Object3D>::iterator it = preForma.begin(); it != preForma.end(); it++)
            it->setColor(color);
    }
    preShape = preForma[index];
    updateCam();
}

void MainWindowImpl::on_actionColorModelo_triggered()
{
    QColor color = QColorDialog::getColor(Qt::yellow, this);
    if (color.isValid()) {
        for (vector<Object3D>::iterator it = modeloLap.begin(); it != modeloLap.end(); it++)
            it->setColor(color);
    }

    model = modeloLap[index];
    model.setVisible(true);
    updateCam();
}

void MainWindowImpl::on_checkBoxPreForma_clicked()
{
    model.setTypePreview(solid);
    trocarObjeto();
}

void MainWindowImpl::on_checkBoxGema_clicked()
{
    model.setTypePreview(solid);
    trocarObjeto();
}

void MainWindowImpl::on_checkBoxModelo_clicked()
{
    model.setTypePreview(solid);
    trocarObjeto();
}

void MainWindowImpl::trocarObjeto()
{
    const bool showGem = checkBoxGema->isChecked();
    stone.setVisible(showGem);
    stone.setTypePreview(solid);

    const bool showPreShape = checkBoxPreForma->isChecked();
    const bool showModel = checkBoxModelo->isChecked();
    preShape.setVisible(showPreShape);
    model.setVisible(showModel);
    actionColorPreshape->setEnabled(showPreShape);
    actionColorGem->setEnabled(showGem);
    actionColorModelo->setEnabled(showModel);

    if (showPreShape && showModel) {
        lblVisaoPrincipal->setText(tr("Pré-Forma e Modelo de Lapidação - ") + modelo);
        lblOutrasVisoes->setText(tr("Pré-Forma e Modelo de Lapidação - ") + modelo);
    } else if (showPreShape) {
        lblVisaoPrincipal->setText(tr("Pré-Forma - ") + modelo);
        lblOutrasVisoes->setText(tr("Pré-Forma - ") + modelo);
    } else if (showModel) {
        lblVisaoPrincipal->setText(tr("Modelo de Lapidação - ") + modelo);
        lblOutrasVisoes->setText(tr("Modelo de Lapidação - ") + modelo);
    } else if (showGem) {
        lblVisaoPrincipal->setText(tr("Somente gema - ") + modelo);
        lblOutrasVisoes->setText(tr("Somente gema - ") + modelo);
    } else {
        lblVisaoPrincipal->setText(tr("Nenhum objeto - ") + modelo);
        lblOutrasVisoes->setText(tr("Nenhum objeto - ") + modelo);
    }

    updateCam();
}
