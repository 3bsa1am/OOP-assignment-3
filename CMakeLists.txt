cmake_minimum_required(VERSION 3.16)

project(GameCollection VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(GameCollection
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )

    target_link_libraries(GameCollection PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

    set_target_properties(GameCollection PROPERTIES
        WIN32_EXECUTABLE TRUE
    )

    qt_finalize_executable(GameCollection)
else()
    add_executable(GameCollection
        ${PROJECT_SOURCES}
    )

    target_link_libraries(GameCollection PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

    set_target_properties(GameCollection PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()
