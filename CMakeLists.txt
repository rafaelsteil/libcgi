#
# Copyright 2013,2016–2018 Alexander Dahl <post@lespocky.de>
#

cmake_minimum_required(VERSION 3.1)

project(cgi
	VERSION 1.1.1
	LANGUAGES C
)
message(STATUS "cgi_VERSION: ${cgi_VERSION}")

string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UC)
string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LC)

set(CGI_DESCRIPTION "A C library to build CGI applications.")
set(CGI_URL "https://github.com/rafaelsteil/libcgi")

# includes
include(CMakePackageConfigHelpers)	# cmake 2.8.8
include(FeatureSummary)
include(GNUInstallDirs)				# cmake 2.8.5

# options
option(BUILD_SHARED_LIBS
	"Global flag to cause add_library to create shared libraries if on."
	ON
)

# subdirectories
add_subdirectory("src")

# test
enable_testing()
add_subdirectory("test")

# cmake package stuff
configure_package_config_file(${PROJECT_NAME_LC}-config.cmake.in
	"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME_LC}-config.cmake"
	INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
	PATH_VARS CMAKE_INSTALL_INCLUDEDIR
	NO_CHECK_REQUIRED_COMPONENTS_MACRO
)
write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME_LC}-config-version.cmake"
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY SameMajorVersion
)
install(FILES
	"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME_LC}-config.cmake"
	"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME_LC}-config-version.cmake"
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)

# pkg-config stuff
configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/libcgi.pc.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/libcgi.pc"
	@ONLY
)
install(FILES
	"${CMAKE_CURRENT_BINARY_DIR}/libcgi.pc"
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
)

# feature summary
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
feature_summary(WHAT ALL)

# packaging
set(CPACK_PACKAGE_NAME "libcgi")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_FILE "${PROJECT_SOURCE_DIR}/README.md")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${CGI_DESCRIPTION}")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
set(CPACK_SOURCE_IGNORE_FILES "/\\\\.kdev4/;\\\\.kdev4$;/\\\\.git/;\\\\.gitignore;\\\\.swp$;\\\\.orig$")
include(CPack)
