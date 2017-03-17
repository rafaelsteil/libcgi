#include <ctype.h>
#include <string.h>

/*
 *	Public domain implementations of in-place string trim functions
 *
 *	2010 Michael Burr <michael.burr@nth-element.com>
 *
 *	see https://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way/2452438#2452438
 */

char* ltrim(char* s)
{
    char* newstart = s;

    while (isspace( *newstart)) {
        ++newstart;
    }

    // newstart points to first non-whitespace char (which might be '\0')
    memmove( s, newstart, strlen( newstart) + 1); // don't forget to move the '\0' terminator

    return s;
}


char* rtrim( char* s)
{
    char* end = s + strlen( s);

    // find the last non-whitespace character
    while ((end != s) && isspace( *(end-1))) {
            --end;
    }

    // at this point either (end == s) and s is either empty or all whitespace
    //      so it needs to be made empty, or
    //      end points just past the last non-whitespace character (it might point
    //      at the '\0' terminator, in which case there's no problem writing
    //      another there).
    *end = '\0';

    return s;
}

char*  trim( char* s)
{
    return rtrim( ltrim( s));
}
