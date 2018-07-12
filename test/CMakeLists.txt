#
# Copyright 2016–2018 Alexander Dahl <post@lespocky.de>
#

# misc
add_executable(cgi-test
	cgi_test.c
	test.c
)
target_link_libraries(cgi-test
	${PROJECT_NAME}
)
add_test(NAME cgi_escape_special_chars
    COMMAND cgi-test escape_special_chars
)
add_test(NAME cgi_process_data
    COMMAND cgi-test process_data
)
add_test(NAME cgi_param_multiple
    COMMAND cgi-test param_multiple
)
add_test(NAME cgi_process_form
    COMMAND cgi-test process_form
)
add_test(NAME cgi_ltrim
    COMMAND cgi-test ltrim
)
add_test(NAME cgi_rtrim
    COMMAND cgi-test rtrim
)

# slist
add_executable(cgi-test-slist
	cgi_test.c
	test_slist.c
)
target_link_libraries(cgi-test-slist
	${PROJECT_NAME}
)
add_test(NAME cgi_slist_add
    COMMAND cgi-test-slist add
)
add_test(NAME cgi_slist_delete_zero
    COMMAND cgi-test-slist deletezero
)
add_test(NAME cgi_slist_delete_one
    COMMAND cgi-test-slist deleteone
)
add_test(NAME cgi_slist_delete_two
    COMMAND cgi-test-slist deletetwo
)
add_test(NAME cgi_slist_delete_three
    COMMAND cgi-test-slist deletethree
)
add_test(NAME cgi_slist_get_item
    COMMAND cgi-test-slist get
)
add_test(NAME cgi_slist_process_data
    COMMAND cgi-test-slist processdata
)

# session
add_executable(cgi-test-session
	cgi_test.c
	test_session.c
)
target_link_libraries(cgi-test-session
	${PROJECT_NAME}
)
add_test(NAME cgi_session_cookie_name
	COMMAND cgi-test-session cookie_name
)

# trim
add_executable(cgi-test-trim
	cgi_test.c
	test_trim.c
	trim.c
)
target_link_libraries(cgi-test-trim
	${PROJECT_NAME}
)
add_test(NAME cgi_trim_ltrim
    COMMAND cgi-test-trim ltrim
)
add_test(NAME cgi_trim_rtrim
    COMMAND cgi-test-trim rtrim
)
add_test(NAME cgi_trim_trim
    COMMAND cgi-test-trim trim
)
