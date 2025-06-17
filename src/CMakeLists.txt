find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

if (WIN32)
    set(LIB "${CMAKE_SOURCE_DIR}/src/lib")

    set(LIBARCHIVE_DIR "${LIB}/libarchive")
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

include(FetchContent)

FetchContent_Declare(
  HPP_ARCHIVE
  GIT_REPOSITORY https://github.com/Procionn/hpp-archive
  GIT_TAG 0cc2a95d6413d2061c631f6ff822ca7e0b3d6ae0
)
FetchContent_MakeAvailable(HPP_ARCHIVE)

FetchContent_Declare(
  wmml
  GIT_REPOSITORY https://github.com/Procionn/wmml2
  GIT_TAG 3dd9cb7538a72b14f75f9b9e4bc8d2e3fcaaf6ae
)
FetchContent_MakeAvailable(wmml)

add_library(CORE STATIC
    core.h

    core/core.cpp
    core/lang.cpp
    core/config.cpp
    core/gameConfig.cpp
    core/configurator.cpp

    methods.cpp                         methods.h
)
add_library(SETTINGS STATIC
    settings/settings.h                 settings/settings.cpp
    settings/setlang.h                  settings/setlang.cpp
    settings/setsupport.h               settings/setsupport.cpp
    settings/setsource.h                settings/setsource.cpp
    settings/setextensions.h            settings/setextensions.cpp
    settings/collections.h              settings/collections.cpp
)
add_library(PATTERNS STATIC
    patterns/CSwitchButton.h            patterns/CSwitchButton.cpp
    patterns/CToggledButton.h           patterns/CToggledButton.cpp
    patterns/CSmartList.h
    patterns/CSmartObject.h             patterns/CSmartObject.cpp
    patterns/templatelist.h             patterns/templatelist.cpp
    patterns/CLinkTumbler.h             patterns/CLinkTumbler.cpp
    patterns/CFastDialog.h              patterns/CFastDialog.cpp
    patterns/ERRORdialog.h              patterns/ERRORdialog.cpp
    patterns/CScrollWindow.h
)
target_link_libraries(CORE     PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(CORE     PRIVATE HPP_ARCHIVE)
target_link_libraries(CORE     PUBLIC  wmml)
target_link_libraries(SETTINGS PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(SETTINGS PRIVATE wmml)
target_link_libraries(PATTERNS PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(PATTERNS PRIVATE wmml)


set(PROJECT_SOURCES
    main.cpp
    window.h window.cpp

    CONSTANTS.h

    dnd.h                               dnd.cpp
    window/CObjectsButton.h             window/CObjectsButton.cpp

    window/modlist/cobjectscontainer.h  window/modlist/cobjectscontainer.cpp
    window/modlist/cobject.h            window/modlist/cobject.cpp
    window/CMenuBar.h                   window/CMenuBar.cpp
    window/mainWidgets.h                window/CObjectList.cpp
                                        window/CContentList.cpp

    dialog_window/CNewObjectDialog.h    dialog_window/CNewObjectDialog.cpp
    dialog_window/cinpad/CInpad.h       dialog_window/cinpad/CInpad.cpp
    dialog_window/cinpad/CInpadList.h   dialog_window/cinpad/CInpadList.cpp
    dialog_window/cinpad/CInpadButton.h dialog_window/cinpad/CInpadButton.cpp
)


qt_add_executable(Wirus
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

target_compile_definitions(SETTINGS PRIVATE
  VERSION="${PROJECT_VERSION}"
)

target_link_libraries(Wirus
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    wmml

    HPP_ARCHIVE
    PATTERNS
    CORE
    SETTINGS
)
if(WIN32)
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
    target_include_directories(${PROJECT_NAME} PRIVATE ${LIBARCHIVE_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBARCHIVE_LIBRARIES})
    target_compile_definitions(${PROJECT_NAME} PRIVATE ${LIBARCHIVE_CFLAGS_OTHER})
endif()

qt_finalize_executable(Wirus)
