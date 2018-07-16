#
# Copyright 2018 Alexander Dahl <post@lespocky.de>
#

configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/config.h.in"
	"${CMAKE_CURRENT_BINARY_DIR}/config.h"
)

install(FILES
	cgi.h
	error.h
	session.h
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/libcgi"
)
