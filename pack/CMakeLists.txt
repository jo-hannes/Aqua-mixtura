# common cpack settings

set(CPACK_PACKAGE_VENDOR "jo-hannes")
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${PROJECT_SOURCE_DIR}/README.md")

# per platform settings
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
  message(STATUS "TODO")

elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")

  # special assets for linux
  # desktop file
  install(FILES
    aqua-mixtura.desktop
    DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
  # icon
  install(FILES
    ${PROJECT_SOURCE_DIR}/src/icons/logo_512x512.png
    DESTINATION ${CMAKE_INSTALL_DATADIR}/aqua-mixtura/
    RENAME aqua-mixtura.png)

  # common linux settings
  set(CPACK_DEBIAN_PACKAGE_SECTION Miscellaneous)
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER "jo-hannes")

  # get disto name
  find_program(LSB_RELEASE_EXECUTABLE lsb_release)
  if(LSB_RELEASE_EXECUTABLE)
    execute_process(COMMAND ${LSB_RELEASE_EXECUTABLE} -cs
      OUTPUT_VARIABLE LSB_CODENAME
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND ${LSB_RELEASE_EXECUTABLE} -is
      OUTPUT_VARIABLE LSB_DISTRO
      OUTPUT_STRIP_TRAILING_WHITESPACE)
  endif()
  # distro specific settings
  if ("${LSB_CODENAME}" STREQUAL "bookworm") # Debian 12
    set(CPACK_GENERATOR DEB)
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt6core6, libqt6gui6, libqt6widgets6, libqt6network6")
  elseif("${LSB_CODENAME}" STREQUAL "noble")  # Ubuntu 24.04
    set(CPACK_GENERATOR DEB)
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt6core6, libqt6gui6, libqt6widgets6, libqt6network6")
  # Match RPM distros
  elseif (("${LSB_DISTRO}" STREQUAL "Fedora") OR ("${LSB_DISTRO}" STREQUAL "openSUSE"))
    set(CPACK_GENERATOR RPM)
    set(CPACK_RPM_PACKAGE_LICENSE "GPL-2.0-only")
  endif()

elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  set(CPACK_GENERATOR DragNDrop)
  # add icon(s)
  add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/icon.icns
    COMMAND ${CMAKE_SOURCE_DIR}/pack/buildMacIcon.sh
    ARGS ${CMAKE_SOURCE_DIR}/src/icons/ ${CMAKE_CURRENT_BINARY_DIR}
  )
  set(MACOSX_BUNDLE_ICON_FILE icon.icns)
  set(RESOURCE_FILES
    ${RESOURCE_FILES}
    ${CMAKE_CURRENT_BINARY_DIR}/icon.icns
  )
  # add qt lib to app bundle
  find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${_qt_bin_dir}")
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND "${MACDEPLOYQT_EXECUTABLE}"
    ARGS "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.app" -no-plugins
  )

endif ()

include(CPack)
