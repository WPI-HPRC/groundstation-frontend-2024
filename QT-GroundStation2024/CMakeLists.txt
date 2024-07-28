cmake_minimum_required(VERSION 3.21.1)

project(QT-GroundStation2024 VERSION 0.1 LANGUAGES CXX)



set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(PROJECT_SOURCES
        main.cpp
        Windows/mainwindow.cpp
        Windows/mainwindow.h
        Windows/mainwindow.ui
        Widgets/Indicators/connectionIndicator.cpp
        Widgets/Indicators/connectionIndicator.h
        Widgets/Indicators/connectionIndicator.cpp
        Widgets/Indicators/ServerConnectionIndicator.cpp
        Widgets/Indicators/ServerConnectionIndicator.h
        Widgets/Indicators/RocketConnectionIndicator.cpp
        Widgets/Indicators/RocketConnectionIndicator.h
        Widgets/Indicators/PayloadConnectionIndicator.cpp
        Widgets/Indicators/PayloadConnectionIndicator.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(QT-GroundStation2024
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        styles.h styles.cpp
        stylepainters.h stylepainters.cpp


        Windows/rocketwindow.h Windows/rocketwindow.cpp Windows/rocketwindow.ui

        Windows/payloadwindow.ui Windows/payloadwindow.cpp Windows/payloadwindow.h
        
        Util/speedtester.h Util/speedtester.cpp
        Util/hprcCircularBuffer.h
        Util/mousetrackinggraphicsview.h
        Util/betterqgraphicstextitem.h
        Util/hprcwebsocket.h Util/hprcwebsocket.cpp

        Widgets/hprcdisplaywidget.h Widgets/hprcdisplaywidget.cpp
        Widgets/hprcsubgraph.h Widgets/hprcsubgraph.cpp
        Widgets/hprctimeline.h
        Widgets/hprcgraph.h
        Widgets/hprcgraphicswidget.h Widgets/hprcgraphicswidget.cpp
        Util/roundedQGraphicsRectItem.h
        Util/hprcStateMap.h
        Widgets/hprctimeline.cpp
        Widgets/hprcgraph.cpp
        Resources/Resources.qrc
        Widgets/hprcgpslockindicator.h Widgets/hprcgpslockindicator.cpp
        Windows/debugwindow.h Windows/debugwindow.cpp
        Windows/debugwindow.ui
        Widgets/debugwidget.h Widgets/debugwidget.cpp
        Windows/p_debugwindow.ui
        Widgets/p_debugwidget.h Widgets/p_debugwidget.cpp
        Windows/p_debugwindow.h Windows/p_debugwindow.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET QT-GroundStation2024 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(QT-GroundStation2024 SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(QT-GroundStation2024
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(QT-GroundStation2024 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.QT-GroundStation2024)
endif()
set_target_properties(QT-GroundStation2024 PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS QT-GroundStation2024
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(QT-GroundStation2024)
endif()

find_package(Qt6 REQUIRED COMPONENTS WebSockets)
target_link_libraries(QT-GroundStation2024 PRIVATE Qt6::WebSockets)

find_package(Qt6 REQUIRED COMPONENTS 3DCore 3DExtras 3DInput 3DRender Core Gui WebEngineWidgets WebChannel)
target_link_libraries(QT-GroundStation2024 PRIVATE
    Qt::3DCore
    Qt::3DExtras
    Qt::3DInput
    Qt::3DRender
    Qt::Core
    Qt::Gui
    Qt6::WebEngineWidgets
    Qt6::WebChannel
)
