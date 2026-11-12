/**
 * Arquivo: aboutVisualizador.cpp
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

#include "aboutVisualizador.hpp"

//
AboutVisualizador::AboutVisualizador(QWidget* parent, Qt::WindowFlags f)
    : QDialog(parent, f)
{
    setupUi(this);
    setWindowModality(Qt::ApplicationModal);
    setWindowTitle(tr("Sobre o Visualizador 3D-Gemas"));
    setAttribute(Qt::WA_DeleteOnClose, false);
}

//
void AboutVisualizador::on_btnOk_clicked()
{
    this->close();
}
