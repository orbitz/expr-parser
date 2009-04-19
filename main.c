/*
 * This is concept material at the moment
 * The basic idea is as follows.  Take 1 + 2 * 3.  You have 4 variables: 
 * lval, oper, rval, noper.  First read produces: lval = 1, oper = + 
 * rval = 2, noper = *.  Look at noper, on * / ^, push rval and oper onto
 * a stack, then turn lval = 2, oper = *, rval = 3, noper = NULL. noper being
 * NULL means we can do the math out now, so we get rval = 6, pop the stack
 * getting lval = 1, oper = +, rval = 6, noper = NULL. now do this, and 
 * spit out the answer.
 */
#include <stdio.h>
#include <string.h>
#include <infix_to_postfix.h>


/* Token related #defines */
#define MAXTOKLEN	20

int main (int argc, char **argv)
{

	if (argc > 1)
		printf ("%s\n", infix_to_postfix (argv[1]));

	return 0;
}
