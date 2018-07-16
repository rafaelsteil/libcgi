#ifndef _ERROR_H
#define _ERROR_H	1

#ifdef __cplusplus
extern "C" {
#endif


#define E_WARNING	0
#define E_FATAL		1
#define E_CAUTION 	2
#define E_INFORMATION 	3
#define E_MEMORY	4

extern void libcgi_error(int error_code, const char *msg, ...);
extern const char *libcgi_error_type[];

#ifdef __cplusplus
}
#endif

#endif
