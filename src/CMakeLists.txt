#
# Copyright 2013,2016 Alexander Dahl <post@lespocky.de>
#
configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/config.h"
)

include_directories(
	"${CMAKE_CURRENT_BINARY_DIR}"
)

set(CGI_SRC
	base64.c
	cgi.c
	cookie.c
	error.c
	general.c
	list.c
	#	md5.c
	session.c
	string.c
)

# create binary
add_library(${PROJECT_NAME}-shared SHARED ${CGI_SRC})
set_target_properties(${PROJECT_NAME}-shared PROPERTIES
	OUTPUT_NAME	${PROJECT_NAME}
	SOVERSION	${PROJECT_VERSION_MAJOR}
	VERSION		${PROJECT_VERSION}
)

# install binary
install(TARGETS ${PROJECT_NAME}-shared
	EXPORT ${PROJECT_NAME}-targets
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)

# install cmake targets
install(EXPORT ${PROJECT_NAME}-targets
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)

# install headers
install(FILES
	cgi.h
	error.h
	session.h
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/lib${PROJECT_NAME}"
)
