#
# Copyright 2016 Alexander Dahl <post@lespocky.de>
#

include_directories(
	"${PROJECT_SOURCE_DIR}/src"
)

add_executable(cgi-test-slist
	cgi_test.c
	test_slist.c
)
target_link_libraries(cgi-test-slist
	"${PROJECT_NAME}-shared"
)
add_test(slist_add
	cgi-test-slist add
)
add_test(slist_delete_zero
	cgi-test-slist deletezero
)
add_test(slist_delete_one
	cgi-test-slist deleteone
)
add_test(slist_delete_two
	cgi-test-slist deletetwo
)
add_test(slist_delete_three
	cgi-test-slist deletethree
)
add_test(slist_get_item
	cgi-test-slist get
)
add_test(slist_process_data
	cgi-test-slist processdata
)
