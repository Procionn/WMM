cmake_minimum_required(VERSION 3.16)

project(Wirus VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

if (WIN32)
    set(LIBARCHIVE_DIR "${CMAKE_SOURCE_DIR}/lib/libarchive")
    include_directories("${LIBARCHIVE_DIR}/include")
    link_directories("${LIBARCHIVE_DIR}/lib")
elseif(UNIX)
    set(CMAKE_PREFIX_PATH
        "/usr/lib64/cmake/Qt6"
        "/usr/lib64/cmake/Qt6CoreTools"
        "/usr/lib64/cmake/Qt6WidgetsTools"
        ${CMAKE_PREFIX_PATH}
    )
endif()

set(PROJECT_SOURCES
    main.cpp
    window.h window.cpp

    CONSTANTS.h
    core.h                              core.cpp
    lang.h                              lang.cpp
    dialogs.h                           dialogs.cpp
    widgetpattern.h                     widgetpattern.cpp
    dnd.h                               dnd.cpp
    patterns.h                          patterns.cpp
    wmml.h                              wmml.cpp
    methods.cpp                         methods.h
    settings/settings.h                 settings/settings.cpp
    settings/setlang.h                  settings/setlang.cpp
    settings/setsupport.h               settings/setsupport.cpp
    settings/setsource.h                settings/setsource.cpp
    settings/setextensions.h            settings/setextensions.cpp
    window/modlist/cobjectscontainer.h  window/modlist/cobjectscontainer.cpp
    window/modlist/cobject.h            window/modlist/cobject.cpp
    patterns/CSwitchButton.h            patterns/CSwitchButton.cpp
    patterns/CToggledButton.h           patterns/CToggledButton.cpp
    patterns/CSmartList.h
    patterns/CSmartObject.h             patterns/CSmartObject.cpp
    patterns/templatelist.h             patterns/templatelist.cpp
)


qt_add_executable(Wirus
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    patterns/CLinkTumbler.h patterns/CLinkTumbler.cpp
)


target_link_libraries(Wirus PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE archive)

    set_target_properties(Wirus PROPERTIES
        ${BUNDLE_ID_OPTION}
        WIN32_EXECUTABLE TRUE
    )

    include(GNUInstallDirs)
    install(TARGETS Wirus
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
elseif(UNIX)
    target_link_libraries(Wirus PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

    target_include_directories(${PROJECT_NAME} PRIVATE ${LIBARCHIVE_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBARCHIVE_LIBRARIES})
    target_compile_definitions(${PROJECT_NAME} PRIVATE ${LIBARCHIVE_CFLAGS_OTHER})
endif()

qt_finalize_executable(Wirus)
