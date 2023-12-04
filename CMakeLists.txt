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
        src/icons/icons.qrc
        src/mainmodel.h src/mainmodel.cpp
        src/model/additive.h src/model/additive.cpp
        src/model/jsonhelper.h src/model/jsonhelper.cpp
        src/model/malt.h src/model/malt.cpp
        src/model/malts.h src/model/malts.cpp
        src/model/mixture.h src/model/mixture.cpp
        src/model/water.h src/model/water.cpp
        src/model/watersources.h src/model/watersources.cpp
        src/view/additivewindow.h src/view/additivewindow.cpp
        src/view/maltphedit.h src/view/maltphedit.cpp
        src/view/malttabledelegate.h src/view/malttabledelegate.cpp
        src/view/maltwindow.h src/view/maltwindow.cpp
        src/view/waterprofileedit.h src/view/waterprofileedit.cpp
        src/view/waterprofileview.h src/view/waterprofileview.cpp
        src/view/watersourcewindow.h src/view/watersourcewindow.cpp
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
