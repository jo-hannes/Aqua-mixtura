cmake_minimum_required(VERSION 3.13)

set (PROJECT_NAME Aqua-mixtura)

project(${PROJECT_NAME} VERSION 0.1.1 LANGUAGES CXX DESCRIPTION "Water treatment for brewing beer")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets LinguistTools Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools Network)

set(TS_FILES
    translations/Aqua-mixtura_de.ts
    translations/Aqua-mixtura_en.ts
)

set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainmodel.h src/mainmodel.cpp
    src/additive/additive.h src/additive/additive.cpp
    src/additive/additivesettings.h src/additive/additivesettings.cpp
    src/additive/additivewindow.h src/additive/additivewindow.cpp
    src/common/buttons.h src/common/buttons.cpp
    src/common/dialogs.h src/common/dialogs.cpp
    src/common/download.h src/common/download.cpp
    src/common/jsonhelper.h src/common/jsonhelper.cpp
    src/common/meta.h src/common/meta.cpp
    src/common/paths.h src/common/paths.cpp
    src/icons/icons.qrc
    src/malt/malt.h src/malt/malt.cpp
    src/malt/maltphedit.h src/malt/maltphedit.cpp
    src/malt/malts.h src/malt/malts.cpp
    src/malt/malttabledelegate.h src/malt/malttabledelegate.cpp
    src/malt/maltwindow.h src/malt/maltwindow.cpp
    src/mixture/mixadditivewidget.h src/mixture/mixadditivewidget.cpp
    src/mixture/mixmaltwidget.h src/mixture/mixmaltwidget.cpp
    src/mixture/mixresultwidget.h src/mixture/mixresultwidget.cpp
    src/mixture/mixture.h src/mixture/mixture.cpp
    src/mixture/mixtures.h src/mixture/mixtures.cpp
    src/mixture/mixturewindow.h src/mixture/mixturewindow.cpp
    src/mixture/mixwaterwidget.h src/mixture/mixwaterwidget.cpp
    src/mixture/resultbar.h src/mixture/resultbar.cpp
    src/settings/settings.h src/settings/settings.cpp
    src/settings/settingstabledelegate.h src/settings/settingstabledelegate.cpp
    src/settings/settingswindow.h src/settings/settingswindow.cpp
    src/style/style.h src/style/style.cpp
    src/style/styles.h src/style/styles.cpp
    src/style/styleswindow.h src/style/styleswindow.cpp
    src/style/styletabledelegate.h src/style/styletabledelegate.cpp
    src/water/water.h src/water/water.cpp
    src/water/waterprofileedit.h src/water/waterprofileedit.cpp
    src/water/watersources.h src/water/watersources.cpp
    src/water/watersourcestabledelegate.h src/water/watersourcestabledelegate.cpp
    src/water/watersourcewindow.h src/water/watersourcewindow.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )

    qt_add_translations(
        ${PROJECT_NAME}
        SOURCES ${PROJECT_SOURCES}
        TS_FILES ${TS_FILES}
    )
    # Always rebuild translations
    add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}_lupdate)
    add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}_lrelease)
else()
    message(FATAL_ERROR "At least Qt6 is needed!")
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Aqua-mixtura)
endif()

# build packs with cpack
include(pack/CMakeLists.txt)

target_sources(${PROJECT_NAME} PRIVATE ${RESOURCE_FILES})
set_source_files_properties(${RESOURCE_FILES}
  MACOSX_PACKAGE_LOCATION "Resources"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    RESOURCE "${RESOURCE_FILES}"
)

if (NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()
