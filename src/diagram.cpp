/**
 * Arquivo: diagram.cpp
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

#include "diagram.hpp"

using namespace std;

void Diagram::generate(QString& modelo, const QString& filePath, QDomElement& root, QDomNode& child, QImage& img1, QImage& img2, QImage& img3, QImage& img4)
{
    // QString printerName = "";
    QPrinter printer(QPrinter::HighResolution);

    // Gera e salvar o arquivo PDF
    QString projeto = root.attribute("Name");
    // projeto.chop(4);
    projeto.replace(" ", ""); // se houver espaço em branco ele tira e deixa sem espaço

    QString outputFilePath = filePath;
    if (!outputFilePath.toLower().endsWith(".pdf"))
        outputFilePath += ".pdf";

    QFileInfo outputFile(outputFilePath);
    QDir outputDir = outputFile.absoluteDir();
    if (!outputDir.exists()) {
        if (!outputDir.mkpath(outputDir.absolutePath())) {
            QMessageBox::critical(nullptr, "Erro", QString("Não foi possível criar a pasta de saída do diagrama: %1").arg(outputDir.absolutePath()));
            return;
        }
    }

    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(outputFilePath);

    // Pagina 1 - Diagrama Principal
    printer.newPage();

    // Set papel
    printer.setCopyCount(1);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageOrientation(QPageLayout::Portrait);
    printer.setFullPage(false);
    printer.setResolution(300);

    QPainter painter(&printer);
    if (!painter.isActive()) {
        QMessageBox::critical(nullptr, "Erro", QString("Não foi possível iniciar o gerador de PDF para %1").arg(outputFilePath));
        return;
    }

    QRect bounds;
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Copia a imagem do QUADRO(bordas e linhas)
    QPoint target1(-100, -100);
    QImage imagemquadro(":/Imagens/Diagram/Quadro.png");
    painter.drawImage(target1, imagemquadro.scaled(2480, 3510, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // Declaração de variaveis
    int ax = 1410, ax2 = 340, ax3 = 1300, bx = 2300, by = 3300; // variaveis usadas nas coordenadas
    int c1 = 1410, c2 = 1520, c3 = 1660, p1 = 340, p2 = 440, p3 = 600; // variaveis usadas nas coordenadas dentro dos cortes dos modelos

    //--ABRE O XML DO MODELO
    QDomDocument docModelo; // Abrir o arquivo xml
    QDomElement rootModelo; // Guarda o elemento

    QString endressXML = (QApplication::applicationDirPath() + "/Models/" + modelo + "/" + modelo + ".xml");
    QFile file(endressXML);

    if (!file.open(QIODevice::ReadOnly | QFile::Text)) // Abre e testa se o arquivo XMl se foi aberto
    {
        QMessageBox::critical(0, ("Critical Error"), QString("Esta Gema ainda nao possui projeto de Lapidacao\nCannot open file XML %1").arg(endressXML).toStdString().c_str());
        return; // finaliza a função
    } else {
        if (!docModelo.setContent(&file)) // copia o arquivo Xml para QDomDocument doc e testa o parser Xml(foi copiado corretamente)
        {
            QMessageBox::critical(0, ("Critical Error"), QString("Erro no Projeto de lapidação\nParsing error for file XML %1").arg(endressXML).toStdString().c_str());
            file.close(); // Fecha o arquivo
            return; // finaliza a função
        }
        file.close(); // fecha o arquivo definitivamente
        rootModelo = docModelo.documentElement(); // Ele acha o Tag pai Projeto
    }

    // Set fonte
    QFont font("Arial", 9);
    font.setPointSize(9);
    painter.setFont(font);

    // Le os cortes das facetas do XML e gera no diagrama
    QDomNode Cut = rootModelo.firstChild().firstChild().firstChild().nextSiblingElement("Cut").firstChild().nextSiblingElement("Pavilion").firstChild();
    int linha = 2215;
    while (!Cut.isNull()) {

        painter.drawText(p1, linha, bx, by, 0, Cut.toElement().attribute("vertice1"), &bounds);
        if (Cut.toElement().attribute("vertice2") == " ")
            painter.drawText(p2, linha, bx, by, 0, Cut.toElement().attribute("vertice2"), &bounds);
        else
            painter.drawText(p2, linha, bx, by, 0, Cut.toElement().attribute("vertice2") + "\260", &bounds);

        painter.drawText(p3, linha, bx, by, 0, Cut.toElement().attribute("vertice3"), &bounds);

        linha += 40;
        Cut = Cut.nextSibling();
    }

    Cut = rootModelo.firstChild().firstChild().firstChild().nextSiblingElement("Cut").firstChild().nextSiblingElement("Crown").firstChild();
    linha = 2215;
    while (!Cut.isNull()) {

        painter.drawText(c1, linha, bx, by, 0, Cut.toElement().attribute("vertice1"), &bounds);
        if (Cut.toElement().attribute("vertice2") == " ")
            painter.drawText(c2, linha, bx, by, 0, Cut.toElement().attribute("vertice2"), &bounds);
        else
            painter.drawText(c2, linha, bx, by, 0, Cut.toElement().attribute("vertice2") + "\260", &bounds);

        painter.drawText(c3, linha, bx, by, 0, Cut.toElement().attribute("vertice3"), &bounds);

        linha += 40;
        Cut = Cut = Cut.nextSibling();
    }

    // Copia a imagem do MODELO
    QPoint target2(220, 150);
    QImage imagemmodelo(QApplication::applicationDirPath() + "/Models/" + modelo + "/" + modelo + ".png");
    painter.drawImage(target2, imagemmodelo.scaled(2066, 1325, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // Copia a imagem do LOGO
    QPoint target3(1855, 20);
    QImage imagemlogo(":/Imagens/Diagram/Logo.png");
    painter.drawImage(target3, imagemlogo.scaled(380, 160, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // Mostra os dados nas coordenadas indicadas;
    font.setPointSize(10);
    painter.setFont(font);
    painter.drawText(ax2, 1650, bx, by, 0, "Projeto:  " + projeto, &bounds);
    painter.drawText(ax2, 1700, bx, by, 0, "Data:  " + root.firstChild().firstChild().nextSiblingElement("Date").toElement().text(), &bounds);
    painter.drawText(ax2, 1750, bx, by, 0, "Proprietário:  " + root.firstChild().firstChild().toElement().text(), &bounds);
    painter.drawText(ax2, 1800, bx, by, 0, "Modelo:  " + modelo, &bounds);
    painter.drawText(ax2, 1850, bx, by, 0, "Peso Estimado:  " + child.firstChild().toElement().text() + "ct", &bounds);
    painter.drawText(ax2, 1900, bx, by, 0, "Aproveitamento:  " + child.firstChild().nextSiblingElement("Volume-Yield").toElement().text() + "%", &bounds);
    painter.drawText(ax2, 1950, bx, by, 0, "Facetas:  " + rootModelo.firstChild().firstChild().firstChild().nextSiblingElement("Facets").toElement().text() + " + " + rootModelo.firstChild().firstChild().firstChild().nextSiblingElement("FacetsCintura").toElement().text() + " (cintura)", &bounds);
    painter.drawText(ax2, 2000, bx, by, 0, "Index:  " + rootModelo.firstChild().firstChild().firstChild().toElement().text() + " divisões", &bounds);
    painter.drawText(ax2, 2050, bx, by, 0, "R.I.:  " + rootModelo.firstChild().firstChild().firstChild().nextSiblingElement("RI").toElement().text(), &bounds);

    painter.drawText(ax, 1650, bx, by, 0, "Altura(H):  " + child.firstChild().nextSiblingElement("Measures").firstChild().nextSiblingElement("Total").toElement().text() + "mm", &bounds);
    painter.drawText(ax, 1700, bx, by, 0, "Coroa(C):  " + child.firstChild().nextSiblingElement("Measures").firstChild().toElement().text() + "mm", &bounds);
    painter.drawText(ax, 1750, bx, by, 0, "Cintura(R):  " + child.firstChild().nextSiblingElement("Measures").firstChild().nextSiblingElement("Girdle").toElement().text() + "mm", &bounds);
    painter.drawText(ax, 1800, bx, by, 0, "Pavilhão(P):  " + child.firstChild().nextSiblingElement("Measures").firstChild().nextSiblingElement("Pavilion").toElement().text() + "mm", &bounds);

    painter.drawText(ax, 1880, bx, by, 0, "Lado(L1):  " + child.firstChild().nextSiblingElement("Measures").firstChild().nextSiblingElement("GirdleL1").toElement().text() + "mm", &bounds);
    painter.drawText(ax, 1930, bx, by, 0, "Mesa(M1):  " + child.firstChild().nextSiblingElement("Measures").firstChild().nextSiblingElement("TableM1").toElement().text() + "mm", &bounds);

    painter.drawText(ax, 2010, bx, by, 0, "Lado(L2):  " + child.firstChild().nextSiblingElement("Measures").firstChild().nextSiblingElement("GirdleL2").toElement().text() + "mm", &bounds);
    painter.drawText(ax, 2060, bx, by, 0, "Mesa(M2):  " + child.firstChild().nextSiblingElement("Measures").firstChild().nextSiblingElement("TableM2").toElement().text() + "mm", &bounds);

    font.setPointSize(16);
    painter.setFont(font);
    painter.drawText(ax2, 1570, bx, by, 0, "PROJETO DE LAPIDAÇÃO", &bounds);
    painter.drawText(ax, 1570, bx, by, 0, "PROPORÇÕES", &bounds);
    painter.drawText(ax2, 2135, bx, by, 0, "PAVILHÃO", &bounds);
    painter.drawText(ax, 2135, bx, by, 0, "COROA", &bounds);

    font.setPointSize(8);
    painter.setFont(font);
    painter.drawText(210, 3108, bx, by, 0, "Executores:", &bounds);
    painter.drawText(1250, 3118, bx, by, 0, "Apoio Financeiro: Edital MCT/SEBRAE/FINEP - MPEs - 07/2006", &bounds);

    painter.drawText(1890, 185, bx, by, 0, "www.upf.br/3dgemas", &bounds);

    QPoint targetupf(210, 3168);
    QImage imagemupf(":/Imagens/upf.png");
    painter.drawImage(targetupf, imagemupf.scaled(160, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    QPoint targetufrgs(420, 3174);
    QImage imagemufrgs(":/Imagens/ufrgs.png");
    painter.drawImage(targetufrgs, imagemufrgs.scaled(191, 110, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    QPoint targetctp(650, 3150);
    QImage imagemctp(":/Imagens/ctp.png");
    painter.drawImage(targetctp, imagemctp.scaled(168, 143, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    painter.drawText(780, 3250, bx, by, 0, "www.upf.br/ctpedras", &bounds);

    QPoint targetsebrae(1250, 3208);
    QImage imagemsebrae(":/Imagens/sebrae.png");
    painter.drawImage(targetsebrae, imagemsebrae.scaled(160, 83, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    QPoint targetfinep(1450, 3208);
    QImage imagemfinep(":/Imagens/finep.png");
    painter.drawImage(targetfinep, imagemfinep.scaled(189, 70, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    QPoint targetMCT(1675, 3228);
    QImage imagemMCT(":/Imagens/MCT.png");
    painter.drawImage(targetMCT, imagemMCT.scaled(357, 53, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    QPoint targetcnpq(2080, 3208);
    QImage imagemcnpq(":/Imagens/cnpq.png");
    painter.drawImage(targetcnpq, imagemcnpq.scaled(171, 75, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // Pagina 2 - Diagrama Pré-Forma
    printer.newPage();

    // Copia a imagem do QUADRO(bordas e linhas)
    QPoint target10(-100, -100);
    QImage imagemquadropre(":/Imagens/Diagram/Quadro_preforma.png");
    painter.drawImage(target10, imagemquadropre.scaled(2480, 3510, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // Copia as imagens da pre-formado viewer e desenha no diagrama
    QPoint target11(200, 250);
    painter.drawImage(target11, img1.scaled(1000, 1000, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    QPoint target12(200, 1400);
    painter.drawImage(target12, img2.scaled(1000, 1000, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    QPoint target13(1200, 250);
    painter.drawImage(target13, img3.scaled(1000, 1000, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    QPoint target14(1200, 1400);
    painter.drawImage(target14, img4.scaled(1000, 1000, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // Copia a imagem do LOGO p/ pag. 2
    painter.drawImage(target3, imagemlogo.scaled(380, 160, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    font.setPointSize(10);
    painter.setFont(font);
    painter.drawText(ax2, 2580, bx, by, 0, "Projeto:  " + projeto, &bounds);
    painter.drawText(ax2, 2630, bx, by, 0, "Data:  " + root.firstChild().firstChild().nextSiblingElement("Date").toElement().text(), &bounds);
    painter.drawText(ax2, 2680, bx, by, 0, "Proprietário:  " + root.firstChild().firstChild().toElement().text(), &bounds);
    painter.drawText(ax2, 2730, bx, by, 0, "Modelo:  " + modelo, &bounds);
    painter.drawText(ax2, 2780, bx, by, 0, "Identificação:  " + root.firstChild().firstChild().nextSiblingElement("GemIdentity").toElement().text(), &bounds);
    painter.drawText(ax2, 2830, bx, by, 0, "Peso Gema Bruta:  " + root.firstChild().firstChild().nextSiblingElement("Weight").toElement().text() + " g", &bounds);
    painter.drawText(ax2, 2880, bx, by, 0, "Ângulo Coroa:  " + rootModelo.firstChild().firstChild().nextSiblingElement("MeasuresPreform").firstChild().toElement().text() + "\260", &bounds);
    painter.drawText(ax2, 2930, bx, by, 0, "Ângulo Pavilhão:  " + rootModelo.firstChild().firstChild().nextSiblingElement("MeasuresPreform").firstChild().nextSiblingElement("AnglePavilion").toElement().text() + "\260", &bounds);

    painter.drawText(ax3, 2580, bx, by, 0, "Altura(H):  " + child.firstChild().nextSiblingElement("Measures").firstChild().nextSiblingElement("Preform").firstChild().nextSiblingElement("Total").toElement().text() + "mm", &bounds);
    painter.drawText(ax3, 2630, bx, by, 0, "Coroa(C):  " + child.firstChild().nextSiblingElement("Measures").firstChild().nextSiblingElement("Preform").firstChild().toElement().text() + "mm", &bounds);
    painter.drawText(ax3, 2680, bx, by, 0, "Pavilhão(P):  " + child.firstChild().nextSiblingElement("Measures").firstChild().nextSiblingElement("Preform").firstChild().nextSiblingElement("Pavilion").toElement().text() + "mm", &bounds);
    painter.drawText(ax3, 2730, bx, by, 0, "Cintura(R):  " + child.firstChild().nextSiblingElement("Measures").firstChild().nextSiblingElement("Girdle").toElement().text() + "mm", &bounds);

    painter.drawText(ax3, 2810, bx, by, 0, "Lado(L1):  " + child.firstChild().nextSiblingElement("Measures").firstChild().nextSiblingElement("GirdleL1").toElement().text() + "mm", &bounds);
    painter.drawText(ax3, 2860, bx, by, 0, "Lado(L2):  " + child.firstChild().nextSiblingElement("Measures").firstChild().nextSiblingElement("GirdleL2").toElement().text() + "mm", &bounds);

    font.setPointSize(16);
    painter.setFont(font);
    painter.drawText(ax2, 2480, bx, by, 0, "PRÉ-FORMA", &bounds);
    painter.drawText(ax3, 2480, bx, by, 0, "PROPORÇÕES PRÉ-FORMA", &bounds);

    // Rodapé
    font.setPointSize(8);
    painter.setFont(font);
    painter.drawText(210, 3108, bx, by, 0, "Executores:", &bounds);
    painter.drawText(1250, 3118, bx, by, 0, "Apoio Financeiro: Edital MCT/SEBRAE/FINEP - MPEs - 07/2006", &bounds);

    painter.drawText(1890, 185, bx, by, 0, "www.upf.br/3dgemas", &bounds);

    painter.drawImage(targetupf, imagemupf.scaled(160, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    painter.drawImage(targetufrgs, imagemufrgs.scaled(191, 110, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    painter.drawImage(targetctp, imagemctp.scaled(168, 143, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    painter.drawText(780, 3250, bx, by, 0, "www.upf.br/ctpedras", &bounds);
    painter.drawImage(targetsebrae, imagemsebrae.scaled(160, 83, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    painter.drawImage(targetfinep, imagemfinep.scaled(189, 70, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    painter.drawImage(targetMCT, imagemMCT.scaled(357, 53, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    painter.drawImage(targetcnpq, imagemcnpq.scaled(171, 75, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    painter.end();

    // Abre o PDF
#ifdef WIN32
    ShellExecute(NULL, L"open", LPWSTR(outputFilePath.utf16()), NULL, NULL, SW_SHOWNORMAL);
#else
    QProcess process;
    process.startDetached("kfmclient exec " + outputFilePath);
    process.waitForStarted();
#endif
}
