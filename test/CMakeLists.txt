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
add_test(escape_special_chars
	cgi-test escape_special_chars
)
add_test(process_data
	cgi-test process_data
)
add_test(param_multiple
	cgi-test param_multiple
)
add_test(process_form
	cgi-test process_form
)
add_test(ltrim
	cgi-test ltrim
)
add_test(rtrim
	cgi-test rtrim
)

# slist
add_executable(cgi-test-slist
	cgi_test.c
	test_slist.c
)
target_link_libraries(cgi-test-slist
	${PROJECT_NAME}
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

# trim
add_executable(cgi-test-trim
	cgi_test.c
	test_trim.c
	trim.c
)
target_link_libraries(cgi-test-trim
	${PROJECT_NAME}
)
add_test(test_ltrim
	cgi-test-trim ltrim
)
add_test(test_rtrim
	cgi-test-trim rtrim
)
add_test(test_trim
	cgi-test-trim trim
)

# additional tests with valgrind
find_program(VALGRIND NAMES valgrind)
if(NOT VALGRIND-NOTFOUND)
	message(STATUS "VALGRIND: ${VALGRIND}")

	add_test(NAME valgrind_escape_special_chars
		COMMAND ${VALGRIND} --leak-check=full --error-exitcode=1
					$<TARGET_FILE:cgi-test> escape_special_chars
	)
	add_test(NAME valgrind_process_data
		COMMAND ${VALGRIND} --leak-check=full --error-exitcode=1
					$<TARGET_FILE:cgi-test> process_data
	)
	add_test(NAME valgrind_param_multiple
		COMMAND ${VALGRIND} --leak-check=full --error-exitcode=1
					$<TARGET_FILE:cgi-test> param_multiple
	)
	add_test(NAME valgrind_process_form
		COMMAND ${VALGRIND} --leak-check=full --error-exitcode=1
					$<TARGET_FILE:cgi-test> process_form
	)
	add_test(NAME valgrind_ltrim
		COMMAND ${VALGRIND} --leak-check=full --error-exitcode=1
					$<TARGET_FILE:cgi-test> ltrim
	)
	add_test(NAME valgrind_rtrim
		COMMAND ${VALGRIND} --leak-check=full --error-exitcode=1
					$<TARGET_FILE:cgi-test> rtrim
	)

	add_test(NAME valgrind_slist_add
		COMMAND ${VALGRIND} --leak-check=full --error-exitcode=1
					$<TARGET_FILE:cgi-test-slist> add
	)
	add_test(NAME valgrind_slist_delete_zero
		COMMAND ${VALGRIND} --leak-check=full --error-exitcode=1
					$<TARGET_FILE:cgi-test-slist> deletezero
	)
	add_test(NAME valgrind_slist_delete_one
		COMMAND ${VALGRIND} --leak-check=full --error-exitcode=1
					$<TARGET_FILE:cgi-test-slist> deleteone
	)
	add_test(NAME valgrind_slist_delete_two
		COMMAND ${VALGRIND} --leak-check=full --error-exitcode=1
					$<TARGET_FILE:cgi-test-slist> deletetwo
	)
	add_test(NAME valgrind_slist_delete_three
		COMMAND ${VALGRIND} --leak-check=full --error-exitcode=1
					$<TARGET_FILE:cgi-test-slist> deletethree
	)
	add_test(NAME valgrind_slist_get_item
		COMMAND ${VALGRIND} --leak-check=full --error-exitcode=1
					$<TARGET_FILE:cgi-test-slist> get
	)
	add_test(NAME valgrind_slist_process_data
		COMMAND ${VALGRIND} --leak-check=full --error-exitcode=1
					$<TARGET_FILE:cgi-test-slist> processdata
	)

	add_test(NAME valgrind_test_ltrim
		COMMAND ${VALGRIND} --leak-check=full --error-exitcode=1
					$<TARGET_FILE:cgi-test-trim> ltrim
	)
	add_test(NAME valgrind_test_rtrim
		COMMAND ${VALGRIND} --leak-check=full --error-exitcode=1
					$<TARGET_FILE:cgi-test-trim> rtrim
	)
	add_test(NAME valgrind_test_trim
		COMMAND ${VALGRIND} --leak-check=full --error-exitcode=1
					$<TARGET_FILE:cgi-test-trim> trim
	)
endif(NOT VALGRIND-NOTFOUND)
