cmake_minimum_required(VERSION 3.16)

project(Visualizador_3D-Gemas VERSION 1.0 LANGUAGES CXX)

if(NOT CMAKE_CONFIGURATION_TYPES)
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
    endif()
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release RelWithDebInfo MinSizeRel)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/ui)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets OpenGL OpenGLWidgets Xml PrintSupport)
find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_library(GLU_LIBRARY NAMES GLU MesaGLU)

if(NOT GLU_LIBRARY)
    message(FATAL_ERROR "Biblioteca GLU não encontrada. Instale o pacote libglu1-mesa-dev.")
endif()

add_executable(Visualizador_3D-Gemas
    include/aboutVisualizador.hpp
    include/diagram.hpp
    include/face.hpp
    include/mainwindowimpl.hpp
    include/object3d.hpp
    include/point3d.hpp
    include/stl.hpp
    include/winaboutimpl.hpp
    include/windowopengl.hpp
    src/main.cpp
    src/mainwindowimpl.cpp
    src/object3d.cpp
    src/stl.cpp
    src/winaboutimpl.cpp
    src/windowopengl.cpp
    src/aboutVisualizador.cpp
    src/diagram.cpp
    ui/mainwindow.ui
    ui/winabout.ui
    ui/aboutvisualizador.ui
    resources/resources.qrc
)

target_include_directories(Visualizador_3D-Gemas PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(Visualizador_3D-Gemas PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGL
    Qt6::OpenGLWidgets
    Qt6::Xml
    Qt6::PrintSupport
    OpenGL::GL
    ${GLU_LIBRARY}
)

set_target_properties(Visualizador_3D-Gemas PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<LOWER_CASE:$<CONFIG>>"
)

foreach(data_dir translations Gem Models)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${data_dir}")
        add_custom_command(TARGET Visualizador_3D-Gemas POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_CURRENT_SOURCE_DIR}/${data_dir}"
                "$<TARGET_FILE_DIR:Visualizador_3D-Gemas>/${data_dir}"
        )
    endif()
endforeach()
