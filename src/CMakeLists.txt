find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network)

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
  GIT_TAG 86f721ea666bc9fe2da4669e21fb3cb0070946c4
)
FetchContent_MakeAvailable(HPP_ARCHIVE)

FetchContent_Declare(
  wmml
  GIT_REPOSITORY https://github.com/Procionn/wmml2
  GIT_TAG 5367decfaa97cf0ba3de9bde0e5bd15b7d6df211
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
    core/importer.cpp                   importer.h
    ModManager/ModManager.cpp           ModManager.h
    ModManager/ModList.cpp
    ModManager/objects.cpp

    dialog_window/unificator.cpp        dialog_window/unificator.h
    core/IGameConfig.cpp                core/IGameConfig.h
    core/InterprocessData.h
)
add_library(SETTINGS STATIC
    settings/settings.h                     settings/settings.cpp
    settings/setlang.h                      settings/setlang.cpp
    settings/setsupport.h                   settings/setsupport.cpp
    settings/setsource.h                    settings/setsource.cpp
    settings/setextensions.h                settings/setextensions.cpp
    settings/collections.h                  settings/collections.cpp
    settings/ModStorage.h                   settings/ModStorage.cpp
    settings/ModStorage/ModStorageList.h    settings/ModStorage/ModStorageList.cpp
    settings/ModStorage/ModStorageObject.h  settings/ModStorage/ModStorageObject.cpp
    settings/ModStorage/ModVersionList.h    settings/ModStorage/ModVersionList.cpp
)
add_library(PATTERNS STATIC
    patterns/CSwitchButton.h            patterns/CSwitchButton.cpp
    patterns/CToggledButton.h           patterns/CToggledButton.cpp
    patterns/CSmartList.h
    patterns/CSmartObject.h             patterns/CSmartObject.cpp
    patterns/templatelist.h             patterns/templatelist.cpp
    patterns/CBaseSmartObject.h         patterns/CBaseSmartObject.cpp
    patterns/CLinkTumbler.h             patterns/CLinkTumbler.cpp
    patterns/CFastDialog.h              patterns/CFastDialog.cpp
    patterns/ERRORdialog.h              patterns/ERRORdialog.cpp
    patterns/RenameDialog.h             patterns/RrenameDialog.cpp
    patterns/CScrollWindow.h
    patterns/WaitingWindow.h
)
target_link_libraries(CORE PRIVATE
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Network
  HPP_ARCHIVE
  wmml_static
)
target_link_libraries(SETTINGS PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(PATTERNS PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_compile_definitions(CORE     PUBLIC LOG)
target_compile_definitions(PATTERNS PUBLIC LOG)


set(PROJECT_SOURCES
    main.cpp
    cli.h cli.cpp
    window.h window.cpp

    CONSTANTS.h

    dnd.h                               dnd.cpp
    window/CObjectsButton.h             window/CObjectsButton.cpp

    window/modlist/cobjectscontainer.h  window/modlist/cobjectscontainer.cpp
    window/modlist/cobject.h            window/modlist/cobject.cpp
    window/CMenuBar.h                   window/CMenuBar.cpp
    window/CSubInfoFrame.h              window/CSubInfoFrame.cpp
    window/CSortedFrame.h               window/CSortedFrame.cpp
    window/CSearchWidget.h              window/CSearchWidget.cpp
    window/mainWidgets.h                window/CObjectList.cpp
                                        window/CContentList.cpp

    dialog_window/CNewObjectDialog.h    dialog_window/CNewObjectDialog.cpp
    dialog_window/cinpad/CInpad.h       dialog_window/cinpad/CInpad.cpp
    dialog_window/cinpad/CInpadList.h   dialog_window/cinpad/CInpadList.cpp
    dialog_window/cinpad/CInpadButton.h dialog_window/cinpad/CInpadButton.cpp
    dialog_window/CProperties.h dialog_window/CProperties.cpp
)


qt_add_executable(Wirus
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

target_compile_definitions(SETTINGS PRIVATE
  VERSION="${PROJECT_VERSION}"
)
target_compile_definitions(Wirus PRIVATE
  VERSION="${PROJECT_VERSION}"
)

target_link_libraries(Wirus PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    wmml_static

    PATTERNS
    CORE
    SETTINGS
)
if(WIN32)
    add_executable(WinRoot
      winmodule/winrootmain.cpp
      core/InterprocessData.h
      winmodule/winrootbackend.h winmodule/winrootbackend.cpp
      methods.cpp                methods.h
    )
    target_link_libraries(WinRoot PRIVATE
      Qt${QT_VERSION_MAJOR}::Network
      Qt${QT_VERSION_MAJOR}::Widgets
      wmml_static
    )
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
    add_custom_command(
        OUTPUT resources.o
        COMMAND ${CMAKE_RC_COMPILER} ${CMAKE_SOURCE_DIR}/winRes/resources.rc -O coff -o resources.o
        DEPENDS ${CMAKE_SOURCE_DIR}/winRes/resources.rc
                ${CMAKE_SOURCE_DIR}/winRes/rule.manifest
                ${CMAKE_SOURCE_DIR}/winRes/logo.ico
    )
    target_sources(Wirus PRIVATE resources.o)
    add_custom_command(
        OUTPUT root.o
        COMMAND ${CMAKE_RC_COMPILER} ${CMAKE_SOURCE_DIR}/winRes/root.rc -O coff -o root.o
        DEPENDS ${CMAKE_SOURCE_DIR}/winRes/root.rc
                ${CMAKE_SOURCE_DIR}/winRes/root.manifest
    )
    target_sources(WinRoot PRIVATE root.o)
elseif(UNIX)
    target_include_directories(${PROJECT_NAME} PRIVATE ${LIBARCHIVE_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBARCHIVE_LIBRARIES})
    target_compile_definitions(${PROJECT_NAME} PRIVATE ${LIBARCHIVE_CFLAGS_OTHER})
endif()

qt_finalize_executable(Wirus)
