#ifndef _ERROR_H_
#define _ERROR_H_

#include <boolean.h>

struct error_t
{
	char   *msg;
	unsigned int code;
	boolean state;
};

extern struct error_t __error__;

const char   *getErrorMsg (void);
unsigned int getErrorCode (void);
void    createError (const unsigned int code, const char *msg);
boolean isError (void);
void setErrorOn(void);

#endif /* _ERROR_H_ */
