cmake_minimum_required(VERSION 3.1)
project(switcher VERSION 0.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_definitions(-DNAME="${PROJECT_NAME}")
add_definitions(-DVERSION="${PROJECT_VERSION}")

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(PkgConfig)
pkg_check_modules(GSETTINGS REQUIRED gsettings-qt)
include_directories(${GSETTINGS_INCLUDE_DIRS})

include(GNUInstallDirs)

########################
# executable
set(CLIENT_NAME ${PROJECT_NAME})
set(DAEMON_NAME ${PROJECT_NAME}d)

add_executable(${CLIENT_NAME}
    src/profile.cpp
    src/profile.hpp
    src/proxy.hpp
    src/switcher.cpp
)
target_link_libraries(${CLIENT_NAME} Qt5::Core ${GSETTINGS_LIBRARIES})

add_executable(${DAEMON_NAME}
    res/icon.qrc
    src/profile.cpp
    src/profile.hpp
    src/proxy.hpp
    src/settings.cpp
    src/settings.hpp
    src/switcherd.cpp
)
target_link_libraries(${DAEMON_NAME} Qt5::Core Qt5::Widgets ${GSETTINGS_LIBRARIES})

install(TARGETS ${CLIENT_NAME} ${DAEMON_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES LICENSE.md README.md DESTINATION ${CMAKE_INSTALL_DOCDIR})
install(FILES systemd/switcherd.service DESTINATION ${CMAKE_INSTALL_LIBDIR}/systemd/user/)

########################
# package
set(CPACK_STRIP_FILES ON)
set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Proxy switcher")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Dimitry Ishenko <dimitry (dot) ishenko (at) (gee) mail (dot) com>")

set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_DEBIAN_PACKAGE_DEPENDS )
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)
