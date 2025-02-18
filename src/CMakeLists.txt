cmake_minimum_required(VERSION 3.7)

project(Wirus VERSION 0.1 LANGUAGES CXX)

set(CMAKE_PREFIX_PATH 
    "/usr/lib64/cmake/Qt6"
    "/usr/lib64/cmake/Qt6CoreTools"
    "/usr/lib64/cmake/Qt6WidgetsTools"
    ${CMAKE_PREFIX_PATH}
)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS 
    Core 
    Gui 
    Widgets 
    CoreTools 
    GuiTools 
    WidgetsTools
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBARCHIVE REQUIRED libarchive)

set(PROJECT_SOURCES
    main.cpp
    window.h window.cpp
    
    CONSTANTS.h
    core.h core.cpp
    lang.h lang.cpp
    dialogs.h dialogs.cpp
    widgetpattern.h widgetpattern.cpp
    dnd.h dnd.cpp
    patterns.h patterns.cpp
    wmml.h wmml.cpp
    settings.h settings.cpp
)

message(STATUS "QT_VERSION_MAJOR = ${QT_VERSION_MAJOR}")

if(QT_VERSION_MAJOR AND QT_VERSION_MAJOR GREATER_EQUAL 6)
    qt_add_executable(Wirus
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        variables.h
        CONSTANTS.h
        core.h core.cpp
        lang.h lang.cpp
        dialogs.h dialogs.cpp
        widgetpattern.h widgetpattern.cpp
        dnd.h dnd.cpp
        patterns.h patterns.cpp
        wmml.h wmml.cpp
        settings.h settings.cpp
    )
else()
    if(ANDROID)
        add_library(Wirus SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(Wirus
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Wirus PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_include_directories(${PROJECT_NAME} PRIVATE ${LIBARCHIVE_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBARCHIVE_LIBRARIES})
target_compile_definitions(${PROJECT_NAME} PRIVATE ${LIBARCHIVE_CFLAGS_OTHER})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Wirus)
endif()
