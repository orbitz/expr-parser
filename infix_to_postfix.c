#include <stdlib.h>
#include <infix_to_postfix.h>
#include <error.h>
#include <errorcodes.h>
#include <tokens.h>
#include <actions.h>
#include <stackobject.h>

/* Strucutre used only here */
static struct token_t
{
	char    token[MAXTOKLEN];
	int     tok_type;
};

/* this variable allows us to do a look up on each operators precidence */
static const char *prec_look_up[] = { "+-", "*/", "^", "()", NULL };

/*
	 Function: get_prec(const char *token)
	 Descpriont: Returns the precidence of the given token
*/
static int get_prec (const char *token)
{
	int     c, i;

	for (c = 0; prec_look_up[c] != NULL; c++)
		for (i = 0; prec_look_up[c][i] != NULL; i++)
			if (prec_look_up[c][i] == token[0])
				return c + 1;

	return 0;
}

/*
   Function: get_token(const char *expr, struct token_t *tok, int *offset)
   Description: gets the next token, puts it into tok, and changes c to be
	              the index of the next token. Returns 1 on good, 0 on error.
								should use creteError() to create the error.
*/
static int get_token (const char *expr, struct token_t *tok, int *offset)
{
	int     c = *offset;
	int     d = 0;

	if (tok == NULL)
		{
			createError (BADVAR_E, "*tok does not point to anything");
			goto badexit;
		}

	if (expr[c] == 0)
		tok->tok_type = TOK_END;
	else if (!isdigit (expr[c]))
		{
			/*
			   This needs a fixing for an operators greater than 1 char 
			 */
			tok->token[0] = expr[c];
			tok->token[1] = 0;				/* lets not allow buffer overflows */
			c++;
			tok->tok_type = TOK_OPR;
		}
	else if (isdigit (expr[c]))
		{
			int     ddec = 0;					/* Checks for double decimals: 2.5.7 */
			for (; ((isdigit (expr[c])) && (ddec < 2)) || (expr[c] == '.'); c++)
				{
					if (expr[c] == '.')
						ddec++;

					tok->token[d] = expr[c];
					d++;
				}

			if (ddec >= 2)
				{
					createError (DECIMAL_E, "Too many decimal points in number");
					tok->tok_type = TOK_ERR;
					goto badexit;
				}
			else
				tok->tok_type = TOK_NUM;
		}
	else
		{
			createError (UNKNOWN_E, "Unknown error parsing values");
			tok->tok_type = TOK_ERR;
			goto badexit;
		}

	*offset = c;

	return 1;

badexit:
	return 0;
}

/*
   Function: get_action(struct token_t *left, struct token_t *right)
   Description: reurns the precidence of the operator
*/
static int get_action (const struct token_t *left,
											 const struct token_t *right)
{
	int     pleft, pright;

	if ((left->tok_type == TOK_OPR) && (right->tok_type == TOK_OPR))
		{
#ifdef DEBUG
			printf ("left: %s   right: %s\n", left->token, right->token);
#endif
			if ((left->token[0] == '(') && (right->token[0] == ')'))
				return ACT_IGN;

			if (left->token[0] == '(')
				return ACT_SHT;

			if (right->token[0] == ')')
				return ACT_RED;

			pleft = get_prec ((char *) &left->token);
			pright = get_prec ((char *) &right->token);

			if ((pleft > pright) || (pleft == pright))
				return ACT_RED;

			if (pleft < pright)
				return ACT_SHT;
		}
	else if ((left->tok_type == TOK_NON) && (right->tok_type == TOK_END))
		return ACT_NON;
	else if ((left->tok_type != TOK_NON) && (right->tok_type == TOK_END))
		return ACT_RED;

	return ACT_ERR;
}

/*
	 Function: infix_to_postfix(char *infix)
   Description: Takes an infix character array and returns a postfix char array
   Example: 4 + ( 6 + 10 ) * 5 outputs 4 6 10 + 5 * +
*/
char   *infix_to_postfix (const char *infix)
{
	struct STACKOBJECT *operstack = stack_init ();
	char   *ret = malloc (MAXEXPRLEN);
	/*
	   coper is a poitner so we can use it when we pop opers off the stat later 
	 */
	struct token_t *coper = malloc (sizeof (struct token_t));
	struct token_t ntok;
	int     c = 0;
	int     done = 0;
	int     action;

	memset (ret, 0, MAXEXPRLEN);
	memset ((struct token_t *) &ntok, 0, sizeof (struct token_t));

	coper->tok_type = TOK_NON;		/* Means this hodls no token right now */

	stack_push (&operstack, coper, sizeof (struct token_t));


	while (!done)
		{
			get_token (infix, coper, &c);

			if (coper->tok_type != TOK_NUM)
				done = 1;
			else
				{
					strncat (ret, coper->token, MAXEXPRLEN);
					ret[strlen (ret)] = ' ';
				}
		}

	done = 0;

	while (!done)
		{
			/*
			   should probably check return value 
			 */
			get_token (infix, (struct token_t *) &ntok, &c);

			if (!(isError ()))
				{
					if (ntok.tok_type == TOK_NUM)
						{
							strncat (ret, &ntok.token, MAXEXPRLEN);
							ret[strlen (ret)] = ' ';
						}
					else if (ntok.tok_type == TOK_END)
						{
							while (coper->tok_type != TOK_NON)
								{
									strncat (ret, coper->token, MAXEXPRLEN);
									ret[strlen (ret)] = ' ';

									coper = stack_pop ((struct STACKOBJECT *) &operstack);
								}

							done = 1;
						}

					else
						{
							action = get_action (coper, &ntok);

							if (action == ACT_RED)
								{
#ifdef DEBUG
									printf ("REDUCING\n");
#endif
									do
										{
											strncat (ret, coper->token, MAXEXPRLEN);
											ret[strlen (ret)] = ' ';

											if (!stack_isEmpty (&operstack))
												{
													free (coper);
													coper = stack_pop (&operstack);
												}
											action = get_action (coper, &ntok);
										}
									while (action == ACT_RED);

									if (action == ACT_IGN)
										coper = stack_pop (&operstack);
									else
										{
											stack_push (&operstack, coper, sizeof (struct token_t));
											memcpy (coper, &ntok, sizeof (struct token_t));
										}
								}
							else if (action == ACT_SHT)
								{
#ifdef DEBUG
									printf ("SHIFTING\n");
#endif
									stack_push ((struct STACKOBJECT *) &operstack, coper,
															sizeof (struct token_t));
									if (isError ())
										{
											setErrorOn ();
											free (ret);
											ret = NULL;
											goto badexit;
										}
									coper = malloc (sizeof (struct token_t));
									memcpy (coper, &ntok, sizeof (struct token_t));
								}
#ifdef DEBUG
							else
								{
									printf ("NOTHING\n");
								}
#endif
						}
				}
#ifdef DEBUG
			printf ("ret: %s\n", ret);
#endif
			if (coper->tok_type == TOK_NON && ntok.tok_type == TOK_END)
				done = 1;
		}

badexit:
	return ret;
}
