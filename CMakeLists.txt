cmake_minimum_required(VERSION 3.16)

add_definitions(-DROOT_PATH="${CMAKE_SOURCE_DIR}/")

set(EXECUTABLE_NAME "ExcelParser")

project(${EXECUTABLE_NAME} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 COMPONENTS Gui Quick QuickControls2 Core Concurrent REQUIRED)

add_subdirectory(themes)
add_subdirectory(controls)
add_subdirectory(texts)

include(FetchContent)
FetchContent_Declare(
    QXlsx
    GIT_REPOSITORY https://github.com/QtExcel/QXlsx.git
    GIT_TAG fbf2c37e640ddaf86845dd610c5d5f86aa9ed52d
    SOURCE_SUBDIR QXlsx
)
FetchContent_MakeAvailable(QXlsx)

qt_standard_project_setup(REQUIRES 6.5)

# Include all image references
qt_add_resources(IMAGE_RESOURCES images.qrc)

# Collect all the source files in the project and library
file(GLOB HPP_SOURCES lib/*.hpp lib/excel/*.hpp)
file(GLOB H_SOURCES lib/*.h lib/model/*.h lib/excel/*.h)
file(GLOB CPP_SOURCES lib/*.cpp lib/model/*.cpp lib/excel/*.cpp main.cpp)

# Set app icon
set (WIN32_ICON ${CMAKE_CURRENT_SOURCE_DIR}/assets/ExcelParser.rc)

qt_add_executable(${EXECUTABLE_NAME}
    ${HPP_SOURCES}
    ${H_SOURCES}
    ${IMAGE_RESOURCES}
    ${CPP_SOURCES}
    ${WIN32_ICON}
)

# Group all source files in the project
file(GLOB QML_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} qml/*.qml)
source_group("Qml Files" FILES ${QML_SOURCES})

qt_add_qml_module(appExcelMinimalApp
    URI ${EXECUTABLE_NAME}
    VERSION 1.0
    QML_FILES ${QML_SOURCES}
)

set_target_properties(${EXECUTABLE_NAME} PROPERTIES
    MACOSX_BUNDLE FALSE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(${EXECUTABLE_NAME}
    PRIVATE Qt6::Core
    PRIVATE Qt6::Gui
    PRIVATE Qt6::Quick
    PRIVATE Qt6::QuickControls2
    PRIVATE Qt6::Concurrent
    PRIVATE QXlsx::QXlsx
    PRIVATE ThemesLibplugin
    PRIVATE ControlsLibplugin
    PRIVATE TextsLibplugin
)

include(GNUInstallDirs)
install(TARGETS appExcelMinimalApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
