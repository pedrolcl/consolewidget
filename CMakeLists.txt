# SPDX-License-Identifier: MIT
# Copyright (c) 2024 Pedro López-Cabanillas <plcl@users.sf.net>

cmake_minimum_required(VERSION 3.19)

project( consolewidget
    VERSION 0.0.1
    LANGUAGES CXX
)

find_package (Qt6 REQUIRED COMPONENTS Core Gui Widgets)

qt_standard_project_setup()

set(SOURCES
    ConsoleIODevice.cpp
    ConsoleIODevice.h
    ConsoleWidget.cpp
    ConsoleWidget.h
)

set(HEADERS
    ConsoleIODevice.h
    ConsoleWidget.h
)

qt_add_library( ${PROJECT_NAME} SHARED ${SOURCES})
qt_add_library( ${PROJECT_NAME}-static STATIC ${SOURCES})

target_include_directories( ${PROJECT_NAME}-static PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>
)
set_target_properties( ${PROJECT_NAME}-static PROPERTIES VERSION ${PROJECT_VERSION} )
set_target_properties( ${PROJECT_NAME}-static PROPERTIES PUBLIC_HEADER "${HEADERS}")
add_library( ConsoleWidget::ConsoleWidget-static ALIAS ${PROJECT_NAME}-static)
list( APPEND CONSOLEWIDGET_TARGETS ${PROJECT_NAME}-static )
target_link_libraries ( ${PROJECT_NAME}-static PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Widgets
)

target_include_directories( ${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>
)
set_target_properties( ${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION} )
set_target_properties( ${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR} )
set_target_properties( ${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${HEADERS}" )
add_library( ConsoleWidget::ConsoleWidget ALIAS ${PROJECT_NAME} )
list( APPEND CONSOLEWIDGET_TARGETS ${PROJECT_NAME} )
target_link_libraries ( ${PROJECT_NAME} PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Widgets
)

install (
    TARGETS ${CONSOLEWIDGET_TARGETS}
    EXPORT ConsoleWidgetTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)

install(
    EXPORT ConsoleWidgetTargets
    FILE ${PROJECT_NAME}-targets.cmake
    NAMESPACE ConsoleWidget::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

export(
    EXPORT ConsoleWidgetTargets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-targets.cmake"
    NAMESPACE ConsoleWidget::
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${PROJECT_NAME}-config-version.cmake
    VERSION ${UCHARDET_VERSION}
    COMPATIBILITY AnyNewerVersion
)

if(IS_ABSOLUTE ${CMAKE_INSTALL_LIBDIR})
    set(consolewidget_libdir "${CMAKE_INSTALL_LIBDIR}")
else()
    set(consolewidget_libdir "\${exec_prefix}/${CMAKE_INSTALL_LIBDIR}")
endif()
if(IS_ABSOLUTE ${CMAKE_INSTALL_INCLUDEDIR})
    set(consolewidget_includedir "${CMAKE_INSTALL_INCLUDEDIR}")
else()
    set(consolewidget_includedir "\${prefix}/${CMAKE_INSTALL_INCLUDEDIR}")
endif()

configure_file(
    ${PROJECT_NAME}-config.cmake.in
    ${PROJECT_NAME}-config.cmake
    @ONLY
)

configure_file(
    ${PROJECT_NAME}.pc.in
    ${PROJECT_NAME}.pc
    @ONLY
)

install ( FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

install ( FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)
