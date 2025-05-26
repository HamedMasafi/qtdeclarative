cmake_minimum_required(VERSION 3.16)

project(dockexample VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS
    Quick
    QuickControls2
    QuickTemplates2
)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(dockexample
    main.cpp
    filetreemodel.h filetreemodel.cpp
    rectangleshape.h rectangleshape.cpp
    basicshape.h basicshape.cpp
    commands.h commands.cpp
    factory.h factory.cpp
    ellipseshape.h ellipseshape.cpp
    paintcanvas.h paintcanvas.cpp
    propertymanager.h propertymanager.cpp
)

qt_add_qml_module(dockexample
    URI DockTest
    VERSION 1.0
    NO_RESOURCE_TARGET_PATH
    QML_FILES
        qml/Main.qml
        qml/UndoPanel.qml
        qml/DocumentsPanel.qml
        qml/ExportDockPanel.qml
        qml/TabPosition.qml
        qml/CusomTitleBar.qml
        qml/ToolsPanel.qml
        qml/OutputDockPanel.qml
        qml/PropertiesPanel.qml
    RESOURCES
        images/doc.png
        images/settings.png
        images/undo.png
        images/tool.png
        images/export.png
)


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(dockexample PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.dockexample
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(dockexample
    PUBLIC
        Qt6::Core
        Qt6::Gui
        Qt6::Quick
        Qt6::QuickControls2
)

include(GNUInstallDirs)
install(TARGETS dockexample
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
