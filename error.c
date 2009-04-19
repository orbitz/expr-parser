/*
 * NOTE: NOTE THREAD SAFE NOT NOT NOT NOT EVIL BAD EVIL
 */
#include <error.h>

struct error_t __error__;

/*
 * Function - geterrormsg(void)
 * Description - gets the emssage from the error global structure
 */
const char *getErrorMsg (void)
{
	return __error__.msg;
}

/*
 * Function - geterrorcode(void)
 * Description - gets the error code form the global structure
 */
unsigned int getErrorCode (void)
{
	return __error__.code;
}

/*
 * Function - createerror(unsigned int code, char *code)
 * Descrition - Creates an error to be checekd later
 */
void createError (const unsigned int code, const char *msg)
{
	__error__.code = code;
	__error__.msg = (char *) msg;
	__error__.state = TRUE;
}

/*
 * Function isError(void)
 * Description - Checks for the existence of an error, if an error exists,
 *           the sate of the erorr is chagned to non-existent after this
 *           but code and msg are sitll accessable
 */
boolean isError (void)
{
	boolean ret;

	ret = __error__.state;
	__error__.state = FALSE;

	return ret;
}

/*
 * Function - setErrorOn(void)
 * Description - turns the error state back on, good if you use isError()
 *        to check if there is an error, but want the state to be on again
 */
void setErrorOn (void)
{
	__error__.state = TRUE;
}
