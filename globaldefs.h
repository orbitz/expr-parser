#ifndef _GLOBALDEFS_H_
#define _GLOBALDEFS_H_

#ifndef NULL
#define NULL (void*)0
#endif

#ifndef NUL
#define NUL "\0"
#endif

#ifdef DEBUG
#define debug(format, args...) fprintf(stderr, format, args)
#else
#define debug(format, args...)
#endif

#endif /* _GLOBALDEFS_H_ */
