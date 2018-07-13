#
# Copyright 2013,2016,2018 Alexander Dahl <post@lespocky.de>
#

configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/config.h.in"
	"${CMAKE_CURRENT_BINARY_DIR}/config.h"
)

set(CGI_SRC
	base64.c
	cgi.c
	cookie.c
	error.c
	general.c
	list.c
	md5.c
	session.c
	string.c
)

# create binary
add_library(${PROJECT_NAME} ${CGI_SRC})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES
	OUTPUT_NAME	${PROJECT_NAME}
	SOVERSION	${PROJECT_VERSION_MAJOR}
	VERSION		${PROJECT_VERSION}
	C_STANDARD	99
)

target_include_directories(${PROJECT_NAME}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# install binary
install(TARGETS ${PROJECT_NAME}
	EXPORT ${PROJECT_NAME}-targets
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)

# install cmake targets
install(EXPORT ${PROJECT_NAME}-targets
	NAMESPACE ${PROJECT_NAME}::
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)

# install headers
install(FILES
	cgi.h
	error.h
	session.h
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/lib${PROJECT_NAME}"
)
