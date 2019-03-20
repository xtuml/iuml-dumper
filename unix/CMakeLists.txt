#
# UK Crown Copyright (c) 2016. All Rights Reserved
#
cmake_minimum_required(VERSION 3.2.1)

project(iUML VERSION 1.0.0)

set(UML_ROOT            $ENV{UML_HOME}              CACHE PATH "Root directory of iUML installation")

set(INSTALL_LIB_DIR     lib                         CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR     bin                         CACHE PATH "Installation directory for executables")
set(INSTALL_ETC_DIR     etc                         CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include                     CACHE PATH "Installation directory for header files")
set(INSTALL_CMAKE_DIR   lib/cmake/${PROJECT_NAME}   CACHE PATH "Installation directory for CMake files")

# Symbol conflict for clog between libC and libm means we need to force libC first when building executables
set(UML_LIB_HACK -L${UML_ROOT}/lib C)

add_subdirectory(apigen)
add_subdirectory(iumlapi)
add_subdirectory(maslgen)
add_subdirectory(utils)

install( 
  FILES       setupenv.sh
              iuml.patch
  DESTINATION ${INSTALL_ETC_DIR}
)

install (
  EXPORT      iUML 
  DESTINATION ${INSTALL_CMAKE_DIR}
  NAMESPACE   iUML:: )

set(CPACK_GENERATOR TGZ)
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
include(CPack)
