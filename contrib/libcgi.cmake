set(CTEST_PROJECT_NAME "libcgi")
set(CTEST_NIGHTLY_START_TIME "01:00:00 UTC")

set(CTEST_SITE "tiffy")
set(CTEST_SOURCE_DIRECTORY "/home/alex/src/libcgi")
set(CTEST_BINARY_DIRECTORY "build/${CTEST_PROJECT_NAME}")

set(ENV{CFLAGS} "--coverage")
set(CTEST_CMAKE_GENERATOR "Unix Makefiles")
set(CTEST_USE_LAUNCHERS 1)

set(CTEST_COVERAGE_COMMAND "gcov")
set(CTEST_MEMORYCHECK_COMMAND "/usr/bin/valgrind")

#set(CTEST_MEMORYCHECK_TYPE "ThreadSanitizer")

set(CTEST_DROP_METHOD "http")
set(CTEST_DROP_SITE "localhost")
set(CTEST_DROP_LOCATION "/cdash/submit.php?project=${CTEST_PROJECT_NAME}")
set(CTEST_DROP_SITE_CDASH TRUE)

#ctest_start("Continuous")
ctest_start("Experimental")
ctest_configure()
ctest_build()
ctest_test()
ctest_coverage()
ctest_memcheck()
#ctest_submit()
