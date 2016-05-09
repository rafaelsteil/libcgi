#
# Copyright 2013,2016 Alexander Dahl <post@lespocky.de>
#
cmake_minimum_required(VERSION 2.8.8)

project(cgi)
string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UC)
string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LC)
set(LIBPREFIX ${PROJECT_NAME})
set(LIBPREFIX_LC ${PROJECT_NAME_LC})
set(LIBPREFIX_UC ${PROJECT_NAME_UC})

set(PACKAGE_MAJOR_VERSION "1")
set(PACKAGE_MINOR_VERSION "0")
set(PACKAGE_BUILD_VERSION "0")
set(PACKAGE_VERSION "${PACKAGE_MAJOR_VERSION}.${PACKAGE_MINOR_VERSION}.${PACKAGE_BUILD_VERSION}")

# includes
include(CMakePackageConfigHelpers)	# cmake 2.8.8
include(FeatureSummary)
include(GNUInstallDirs)				# cmake 2.8.5

# additional compiler flags
add_definitions(-Wall -Wextra -Wcast-align)

# subdirectories
add_subdirectory("src")

# cmake package stuff
configure_package_config_file(${PROJECT_NAME_LC}-config.cmake.in
	"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME_LC}-config.cmake"
	INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
	PATH_VARS CMAKE_INSTALL_INCLUDEDIR
	NO_CHECK_REQUIRED_COMPONENTS_MACRO
)
write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME_LC}-config-version.cmake"
	VERSION ${PACKAGE_VERSION}
	COMPATIBILITY AnyNewerVersion
)
install(FILES
	"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME_LC}-config.cmake"
	"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME_LC}-config-version.cmake"
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)

# feature summary
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
feature_summary(WHAT ALL)
