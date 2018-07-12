Introduction
------------
LibCGI is a simple to use yet powerful library written from scratch to assist in the making of CGI appliactions in C. It has support for string manipulation, linked lists, cookies, sessions, GET and POST methods and more.

It was my first Open Source project, somewhere back in the 2000s and somethings. Originally it was hosted at Sourceforge.

Building
--------
LibCGI is built with [CMake](https://cmake.org/). One quick way:

* create a build directory and change into it

        mkdir build
        cd build

* call cmake

        cmake ..

* build it

        make

Of course every other way CMake provides would also work, see CMake documentation.

TODO
----
Well, I am keeping this TODO list here for historical (and funny) reasons. Nowadays I don't have any plans to enhance or change anything.

- Template Engine
- Mime functions
- Encrypted session
- Memory Management functions
- Code speed ups
- Other miscelaneous functions
- Win32 port? ( who wants it? )

Changelog
---------

__Version 1.2.0__

_Thanks to Alexander Dahl, D Frost, Thomas Petazzoni_

2015-04-23

* Improvements to `ltrim()` and `rtrim()`

2016-05-12

* Introduce unit tests with CTest

2016-05-18

* Fix possible segfaults in list implementation ([#31](https://github.com/rafaelsteil/libcgi/issues/31))

2017-02-16

* Added API function to free session internals ([#25](https://github.com/rafaelsteil/libcgi/issues/25))
* Reset session initialized flag ([#33](https://github.com/rafaelsteil/libcgi/issues/33))

2017-09-03

* Fixes on CMake to build as C only project and as shared or static lib, contributed by [Buildroot](https://buildroot.org/)

2018-07-11

* More fixes on CMake build, modern approach

2018-07-12

* Fix include paths in examples

__Version 1.1__

_Thanks to Alexander Dahl, D Frost, Rafael Steil, Michael Olbrich, Robert Schwebel_

After moving the source to GitHub in 2011 this release contains numerous changes from 2012 up to 2016, while still keeping the old API. Consider this a maintenance release which still has lots of flaws. This is a short summary of changes, for more details you can now use the Git history.

* Applied bugfixes from [Debian](https://www.debian.org/) and [ptxdist](http://www.ptxdist.org/)
* Added LICENSE file
* Fixed several memory leaks
* Optimized speed
* Corrected declarations
* Removed some compiler warnings
* Fixed sessions when using redirects ([#19](https://github.com/rafaelsteil/libcgi/issues/19))
* Fixed possible segmentation faults in slist implementation ([#24](https://github.com/rafaelsteil/libcgi/issues/24))
* Switched build system to CMake ([#27](https://github.com/rafaelsteil/libcgi/issues/27))

__Version 1.0__

_Thanks to James Marjie, Mitch, Tamas Sarlos, Luis Figueiredo, Oliver Schlag and Sverre H. Huseby_

2003-06-03

* Fixed a possible memory problem in htmlentities() function

2003-08-19

* Changed md5 suport. No external libs needed anymore

2003-09-05

* Renamed `list_*` functions to `slist_*` ( 's' of 'single' )

2003-09-10

* Some quick fixes to fs functions

2003-12-05

* Moved to `va_copy()` instead direct variable reference copy
* Removed hard-coded program names from Makefile.in ( gcc and ar )

2003-12-15

* Fixed many CRLF issues ( were just LF instead CRLF )
* Some header files reorganization
* Fixed the examples ( deprecated things )

__Version 0.8.2__

_Thanks to Tonu Samuel_

2003-01-10

* Fixed some pointer manipulation bugs in list.c and cgi.c

__Version 0.8.1__

_Thanks to Oliver Schlag, Tommy, Diogo Gonzaga, Robert Csok_

2002-12-27

* Some changes in the documentation
* New source tree
* Added `make_string` function

2002-12-26

* Added md5 function
* Added recvline function

2002-12-06

* Fixed a small string check bug in explode() function

2002-10-28

* Now compiles well under Solaris
* getline() function is no longer necessary. Implemented my own way to read from session files.

__Version 0.8.0__

_Thanks to Hao Yu, Rainer Huobert, Zentara, Dennis Reichel, Tony Hansen, Jakub Skopal_

2002-10-19

* Rewritten `process_data()` function.

2002-07-15

* Added addslashes() function to cgi.c ( was missing )

2002-07-04

* Fixed Makefile to correctly include error.o when compiling libcgi.a
* Fixed `process_data()` function to prevent bad-formed `%xx` data

2002-09-21

* Fixed pointer manipulation problems in base64.c
* Added support to C++

2002-07-02

* Fixed many warnings messages

__Version 0.7.5__

2002-06-29

* Fixed a bug in `cgi_session_start()`
* Added `cgi_session_save_path()` function
* Added `cgi_session_cookie_name()` function

2002-06-14

* Fixed `cgi_cookie_add()` to correctly handle cookie expiration times

2002-06-13

* Revised session manipulation functions
* Code cleanups

2002-06-11

* Rewritten strpos() function.
* Added strnpos() function

2002-06-09

* Updated explode() function
* New examples

2002-06-08

* Updated htmlentities() function
* Added `str_base64_encode()` and `str_base64_decode()` functions

2002-06-06

* Fixed a small bug in `cgi_unescape_special_chars()`
* Fixed `process_data()` function to handle `%xx` ( hexa chars ) correctly
* New `cgi_include()` function

__Version 0.7.4__

_Thanks to Andre 'pXd'_

2002-06-05

* Release with know bugs
* Fixed a memory leak at `process_data()` function
* Fixed an incorrect parser when there was only one parameter in the URL
* Droped redundante calls to sizeof() when using malloc()

2002-05-12

* Code cleanups
* Dropped some documentation
* Examples revised and updated

__Version 0.7.3__

2002-04-14

* Code revision
* header files rewritten
* Added `cgi_send_header()` function

__Version 0.7.2__

_Special thanks to Erik Jansson and Robert Csok_

2002-03-25

* Due to a little bug, `cgi_include()` was dropped to its original version

2002-03-22

* Examples revised

2002-03-21

* Added `str_nreplace()` function
* Changed replace() to `str_replace()`
* Added stripnslashes() function
* Documentation updated

2002-03-20

* New documentation
* Added `cgi_` to many function names
* Added addnslashes() function

2002-03-13

* Added htmlentities() function

2002-03-12

* Fixed another bug at `process_data()` function that caused
more than necessary characters to be copied

2002-03-11

* Fixed a memory leak in `cgi_process_form()` function
* Changed `init_hex_table()` function to use memset() insted for() loop to fill the array

2002-03-10

* Fixed a format bug in `cgi_include()` function
* Fixed a bug in `process_data()` function that
caused some characters to be incorrectly handled
* Improved `cgi_include()` function performance

2002-03-04

* Added ltrim(), rtrim() and trim() functions

__Version 0.7.1__

2002-01-??

* A bug in `sess_file_rewrite()` that caused application to crash was fixed

__Version 0.7__

_Special thanks to ZE bennyben_

2002-01-??

* Session support major updates
* Some memory leaks and String manipulation bugs fixed
* Removed debug() funciont and #define's relacted
* Added `libcgi_error()` for internal error handling
* Source code tree modified
* Header files better written
* More examples
* Documentation updated

__Version 0.1 update 6__

_Special thanks to Marcio Kuchma_

2001-12-17

* `pt_BR` documentation, Makefile rewritten, minor improvements

__Version 0.1 update 5__

2001-11-14

* A friend told me about a paper that describes ways to crack session id's.. I know that my session support ( file session.c ) is really unstable...

2001-11-12

* Modified strpos() function. Now you can specify the number of the characters to match

__Version 0.1 update 4__

2001-11-10

* Finished the script to process XML documentation

2001-11-10

* Changed Makefile. Now it shows the make options

__Version 0.1 update 3__

2001-11-06

* Added file() function

__Version 0.1 update 2__

2001-01-11

* Finished documentation in XML format

__Version 0.1 update 1__

2001-10-??

* Makefile fixed. Now it's more simple

__Version 0.1__

2001-10-??

* First non-oficial release
