cmake_minimum_required(VERSION 3.1)
project(switcher VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_definitions(-DNAME="${PROJECT_NAME}")
add_definitions(-DVERSION="${PROJECT_VERSION}")

find_package(Qt5 COMPONENTS Core Network Widgets REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(PkgConfig)
pkg_check_modules(GSETTINGS REQUIRED gsettings-qt)
include_directories(${GSETTINGS_INCLUDE_DIRS})

include(GNUInstallDirs)

########################
# executable
add_executable(${PROJECT_NAME}
    res/icon.qrc
    src/main.cpp
    src/settings.cpp
    src/settings.hpp
    src/switcher.cpp
    src/switcher.hpp
)
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Network Qt5::Widgets ${GSETTINGS_LIBRARIES})

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES LICENSE.md README.md DESTINATION ${CMAKE_INSTALL_DOCDIR})
install(FILES systemd/switcher.service DESTINATION ${CMAKE_INSTALL_LIBDIR}/systemd/user/)

########################
# package
set(CPACK_STRIP_FILES ON)
set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "...")
set(CPACK_PACKAGE_DESCRIPTION "...")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Dimitry Ishenko <dimitry (dot) ishenko (at) (gee) mail (dot) com>")

set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_DEBIAN_PACKAGE_DEPENDS )
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)
