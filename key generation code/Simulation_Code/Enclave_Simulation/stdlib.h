/* stdlib.h */
#ifndef __STDLIB_H
#define __STDLIB_H

#include <stdlib.h>

#ifndef _INC_FCNTL
#define _INC_FCNTL

#endif

#ifdef __cplusplus
extern "C" {
#endif

	extern int stdin, stdout, stderr;

	int system(char const* _Command);

#ifdef __cplusplus
}
#endif

#endif