# Visualizador 3D-Gemas

Repositório **não oficial** do Visualizador 3D-Gemas, software desenvolvido como parte integrante do projeto **3D-Gemas**, uma parceria entre a Universidade de Passo Fundo (UPF) e a Universidade Federal do Rio Grande do Sul (UFRGS). Este fork foi atualizado para Qt 6, com algumas melhorias na interface de usuário.

> **Nota:** este README foi gerado por inteligência artificial com base na estrutura deste repositório e no artigo relacionado citado abaixo. Revise as informações antes de utilizá-las como documentação oficial.

O Visualizador permite explorar gemas digitalizadas e consultar modelos de lapidação associados aos projetos processados pelo ecossistema 3D-Gemas. Ele é o módulo de visualização: não substitui os demais componentes do projeto que digitalizam a gema ou calculam e otimizam os projetos de lapidação.

> Este repositório é mantido de forma independente e não representa, por si só, um canal oficial das instituições parceiras.

## Contexto do projeto

A lapidação transforma uma gema bruta em uma peça facetada. O projeto 3D-Gemas busca apoiar esse processo com ferramentas de digitalização 3D e projeto de lapidação. Nesse contexto, o Visualizador apresenta a gema digitalizada junto de pré-formas e modelos de lapidação disponíveis para ela.

O artigo abaixo descreve a geração de diagramas de execução para orientar o trabalho de lapidação. O Visualizador permite gerar esses diagramas em PDF a partir dos dados do projeto carregado.

## Funcionalidades

- Abrir projetos de gemas em XML.
- Visualizar a malha 3D da gema e os modelos de lapidação e pré-formas associados.
- Controlar independentemente a visualização da gema, da pré-forma e do modelo de lapidação.
- Selecionar modelos disponíveis no projeto e manipular a visualização, incluindo zoom, cores, transparência, eixos e modos sólido, aramado ou combinado.
- Gerar um diagrama de lapidação em PDF para o projeto carregado.
- Usar a interface traduzida conforme os arquivos de tradução disponíveis e a localidade do sistema.

## Requisitos

- CMake 3.16 ou superior.
- Compilador C++ com suporte a C++17.
- Qt 6 com os módulos Core, Gui, Widgets, OpenGL, OpenGLWidgets, Xml e PrintSupport.
- OpenGL e GLU.

No Debian ou Ubuntu, um conjunto inicial de dependências pode ser instalado com:

```bash
sudo apt update
sudo apt install build-essential cmake qt6-base-dev libgl1-mesa-dev libglu1-mesa-dev
```

O nome e a disponibilidade dos pacotes podem variar entre versões e distribuições. O projeto usa CMake para localizar Qt 6, OpenGL e GLU; se alguma dependência não for encontrada, instale os pacotes de desenvolvimento correspondentes à sua plataforma.

## Compilação

### Debug

Com Makefiles ou Ninja, que são geradores de configuração única:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
```

O executável será gerado em:

```text
build/debug/Visualizador_3D-Gemas
```

### Release

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
```

O executável será gerado em:

```text
build/release/Visualizador_3D-Gemas
```

Com um gerador de configuração única, `CMAKE_BUILD_TYPE` escolhe uma configuração por diretório de build. Pode-se reconfigurar o mesmo diretório para alternar entre Debug e Release, ou manter diretórios separados, por exemplo `build-debug` e `build-release`.

### Manter Debug e Release no mesmo diretório de build

Geradores multi-config, como Ninja Multi-Config, permitem compilar ambas as configurações sem reconfigurar o diretório:

```bash
cmake -G "Ninja Multi-Config" -S . -B build-multi
cmake --build build-multi --config Debug --parallel
cmake --build build-multi --config Release --parallel
```

Os executáveis ficam em `build-multi/debug/` e `build-multi/release/`.

### Recursos copiados para a pasta de saída

Após compilar o executável, o CMake copia para a pasta correspondente à configuração os diretórios de dados que existirem na origem:

- `Gem/`
- `Models/`
- `translations/`

Por exemplo, em uma compilação Debug, esses dados ficam ao lado do executável em `build/debug/`.

## Executar

Para iniciar a interface gráfica:

```bash
./build/debug/Visualizador_3D-Gemas
```

Para iniciar em Release, use `./build/release/Visualizador_3D-Gemas`.

Também é possível passar o caminho de um projeto XML como argumento. Use um caminho absoluto para evitar ambiguidades de diretório:

```bash
./build/debug/Visualizador_3D-Gemas "$PWD/Gem/Gema1.xml"
```

Na interface, use **Arquivo → Abrir Gema** para selecionar um XML. Depois de carregar o projeto, selecione um modelo na lista e marque as caixas da gema, pré-forma e modelo de lapidação conforme deseja exibi-los. Para gerar o diagrama, use **Arquivo → Gerar Diagrama** e escolha onde salvar o PDF.

## Arquivos de dados

Os projetos e modelos são dados de entrada do Visualizador:

- **`Gem/*.xml`**: projetos de gemas. O XML contém informações da gema, sua malha e referências aos modelos associados.
- **`Models/<nome-do-modelo>/`**: arquivos de cada modelo de lapidação. O Visualizador espera encontrar os arquivos referenciados pelo projeto, incluindo a malha STL do modelo (`<nome-do-modelo>.stl`) e da pré-forma (`P<nome-do-modelo>.stl`). A geração do diagrama também consulta o XML e a imagem do modelo (`<nome-do-modelo>.xml` e `<nome-do-modelo>.png`).
- **`translations/`**: traduções da interface, quando disponíveis.

Os diretórios `Gem/`, `Models/` e `translations/` são copiados para a saída de build quando existem na origem. A compilação do programa não garante que todos os dados de modelos referenciados pelos projetos estejam presentes.


## Estrutura do código

```text
.
├── CMakeLists.txt       # Configuração de compilação
├── Gem/                 # Projetos de gemas em XML
├── Imagens/             # Ícones e imagens da interface/diagramas
├── include/             # Cabeçalhos C++
├── Models/              # Dados de modelos de lapidação (quando disponíveis)
├── resources/           # Recursos Qt
├── src/                 # Implementação C++
├── translations/        # Traduções Qt
└── ui/                  # Interfaces Qt Designer
```

## Artigo relacionado

Postal, Álvaro; Zanatta, Alexandre Lazaretti; Silva, Juliano Tonezer da; Brusso, Marcos José; Hartmann, Léo A. **Geração dos diagramas de execução de modelos de lapidação de gemas**. *Revista Brasileira de Computação Aplicada*, Passo Fundo, v. 2, n. 1, p. 69–78, mar. 2010. DOI: [10.5335/rbca.2010.007](https://doi.org/10.5335/rbca.2010.007).

## Licença

O código deste projeto é distribuído sob a **GNU General Public License, versão 3 (GPL-3.0)**. Consulte [`LICENCE.TXT`](LICENCE.TXT) para o texto da licença.
