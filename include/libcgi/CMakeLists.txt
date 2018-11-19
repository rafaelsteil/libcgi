#
# Copyright 2018 Alexander Dahl <post@lespocky.de>
#
# SPDX-License-Identifier: LGPL-2.1+
# License-Filename: LICENSES/LGPL-2.1.txt
#

configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/config.h.in"
	"${CMAKE_CURRENT_BINARY_DIR}/config.h"
)

install(FILES
	cgi.h
	cgi_types.h
	error.h
	session.h
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/libcgi"
)
