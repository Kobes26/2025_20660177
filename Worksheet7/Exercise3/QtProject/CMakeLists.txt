cmake_minimum_required(VERSION 3.19)
project(QtProject LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

find_package(VTK REQUIRED)

qt_standard_project_setup()

qt_add_executable(QtProject
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    Model/ModelPart.cpp
    Model/ModelPart.h
    Model/ModelPartList.cpp
    Model/ModelPartList.h
    icons.qrc
    optiondialog.h
    optiondialog.cpp
    optiondialog.ui
)

target_link_libraries(QtProject PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${VTK_LIBRARIES} )

include(GNUInstallDirs)

install(TARGETS QtProject
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET QtProject
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
add_custom_target(VRBindings)
add_custom_command(TARGET VRBindings PRE_BUILD
COMMAND ${CMAKE_COMMAND} -E
	copy_directory ${CMAKE_SOURCE_DIR}/vrbindings ${CMAKE_BINARY_DIR}/)
