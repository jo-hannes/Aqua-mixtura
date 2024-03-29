cmake_minimum_required(VERSION 3.5)

project(Aqua-mixtura VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(TS_FILES
    translations/Aqua-mixtura_de_DE.ts
)

set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Aqua-mixtura
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        src/mainmodel.h src/mainmodel.cpp
        src/additive/additive.h src/additive/additive.cpp
        src/additive/additivewindow.h src/additive/additivewindow.cpp
        src/common/buttons.h src/common/buttons.cpp
        src/common/global.h src/common/global.cpp
        src/common/jsonhelper.h src/common/jsonhelper.cpp
        src/common/meta.h src/common/meta.cpp
        src/icons/icons.qrc
        src/limits/limits.h src/limits/limits.cpp
        src/limits/limitswindow.h src/limits/limitswindow.cpp
        src/malt/malt.h src/malt/malt.cpp
        src/malt/maltphedit.h src/malt/maltphedit.cpp
        src/malt/malts.h src/malt/malts.cpp
        src/malt/malttabledelegate.h src/malt/malttabledelegate.cpp
        src/malt/maltwindow.h src/malt/maltwindow.cpp
        src/mixture/mixadditivewidget.h src/mixture/mixadditivewidget.cpp
        src/mixture/mixmaltwidget.h src/mixture/mixmaltwidget.cpp
        src/mixture/mixture.h src/mixture/mixture.cpp
        src/mixture/mixtures.h src/mixture/mixtures.cpp
        src/mixture/mixturewindow.h src/mixture/mixturewindow.cpp
        src/mixture/mixwaterwidget.h src/mixture/mixwaterwidget.cpp
        src/style/style.h src/style/style.cpp
        src/style/styles.h src/style/styles.cpp
        src/style/styleswindow.h src/style/styleswindow.cpp
        src/style/styletabledelegate.h src/style/styletabledelegate.cpp
        src/water/water.h src/water/water.cpp
        src/water/waterprofileedit.h src/water/waterprofileedit.cpp
        src/water/waterprofileview.h src/water/waterprofileview.cpp
        src/water/watersources.h src/water/watersources.cpp
        src/water/watersourcewindow.h src/water/watersourcewindow.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Aqua-mixtura APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(Aqua-mixtura SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Aqua-mixtura
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(Aqua-mixtura PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Aqua-mixtura)
endif()
set_target_properties(Aqua-mixtura PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_compile_options(Aqua-mixtura PRIVATE -Wall -Wextra -Wpedantic)

include(GNUInstallDirs)
install(TARGETS Aqua-mixtura
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Aqua-mixtura)
endif()
