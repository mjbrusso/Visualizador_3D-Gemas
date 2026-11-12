/**
 * Arquivo: main.cpp
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
#include <QApplication>
#include <QFileInfo>
#include <QSplashScreen>
#include <QTranslator>


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    const QString appDir = QApplication::applicationDirPath();
    const QString projectDir = QDir(appDir).absoluteFilePath("..");

    QDir::setCurrent(appDir);

    // const int splashDelayMs = 1000;
    // QPixmap pixmap(":/Imagens/Icones/Splash.png");
    // QSplashScreen splash(pixmap);
    // splash.show();

    const QString localeName = QLocale::system().name();
    const QString qtTranslationPath = QLibraryInfo::path(QLibraryInfo::TranslationsPath);
    const QString qtTranslationFile = QStringLiteral("%1/qt_%2.qm").arg(qtTranslationPath, localeName);
    QTranslator qtTranslator;
    if (QFileInfo::exists(qtTranslationFile)) {
        const bool qtLoaded = qtTranslator.load(localeName, qtTranslationPath);
        if (qtLoaded) {
            app.installTranslator(&qtTranslator);
        }
    }

    QTranslator translator;
    const QString appTranslationsDir = QStringLiteral("%1/translations").arg(appDir);
    const QString buildTranslationsDir = QStringLiteral("%1/translations").arg(projectDir);
    const QString sourceRootDir = QDir(appDir).absoluteFilePath("../..");
    const QString sourceTranslationsDir = QStringLiteral("%1/translations").arg(sourceRootDir);
    const QString appTranslationFile = QStringLiteral("%1/cadGemas_%2.qm").arg(appTranslationsDir, localeName);
    const QString buildTranslationFile = QStringLiteral("%1/cadGemas_%2.qm").arg(buildTranslationsDir, localeName);
    const QString sourceTranslationFile = QStringLiteral("%1/cadGemas_%2.qm").arg(sourceTranslationsDir, localeName);

    if (QFileInfo::exists(appTranslationFile)) {
        const bool appTranslationLoaded = translator.load(localeName, appTranslationsDir);
        if (appTranslationLoaded) {
            app.installTranslator(&translator);
        }
    } else if (QFileInfo::exists(buildTranslationFile)) {
        const bool buildTranslationLoaded = translator.load(localeName, buildTranslationsDir);
        if (buildTranslationLoaded) {
            app.installTranslator(&translator);
        }
    } else if (QFileInfo::exists(sourceTranslationFile)) {
        const bool sourceTranslationLoaded = translator.load(localeName, sourceTranslationsDir);
        if (sourceTranslationLoaded) {
            app.installTranslator(&translator);
        }
    }
    MainWindowImpl win;
    if (argc > 1) {
        win.loadXml(argv[1]);
    }

    win.showMaximized();

    // QTimer::singleShot(splashDelayMs, [&]() {
    //     win.showMaximized();
    //     splash.finish(&win);
    // });

    app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));

    return app.exec();
}
