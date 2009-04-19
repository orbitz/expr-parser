#include <stackobject.h>

/*
 * Function - stack_init
 * Return - Pointer to NULL
 * Description - Returns a pointer to NULL
 */
struct STACKOBJECT *stack_init (void)
{
	struct STACKOBJECT *this = NULL;

	return this;
}

/*
 * Function - stack_push
 * Description - Push's data onto the stack
 */
boolean stack_push (struct STACKOBJECT ** this, const void *data,
										const size_t size)
{
	struct STACKOBJECT *curr;

	curr = (struct STACKOBJECT *) malloc (sizeof (struct STACKOBJECT));
	if (curr == NULL)
		{
			return FALSE;
		}

	curr->next = *this;
	*this = curr;
	curr->data = (char *) malloc (size);

	if (curr->data == NULL)
		{
			return FALSE;
		}

	memcpy ((void *) curr->data, (void *) data, size);
	curr->size = size;

	return TRUE;
}

/*
 * Function - stack_pop
 * Return - Pointer to data that was on the stack
 * Description - Returns a pointer to data on the stack
 */
char   *stack_pop (struct STACKOBJECT **this)
{
	struct STACKOBJECT *curr = *this;
	char   *data;

	if (curr != NULL)
		{
			data = (char *) malloc ((size_t) curr->size);
			memcpy (data, curr->data, (size_t) curr->size);
			curr = (*this)->next;
			free (*this);
			*this = curr;
		}
	else
		{
			data = NULL;
		}

	return data;
}

/*
 * Function - stack_isEmpty
 * Return - TRUE or FALSE
 * Description - Says if the function is empty or not
 */
boolean stack_isEmpty (const struct STACKOBJECT ** this)
{
	boolean retval;

	if (*this == NULL)
		{
			retval = TRUE;
		}
	else
		{
			retval = FALSE;
		}

	return retval;
}

/*
 * Function - stack_destroy
 * Description - Destroys a stack, sets the in stack to NULL when done
 */
void stack_destroy (struct STACKOBJECT **this)
{
	while (!stack_isEmpty (this))
		{
			stack_pop (this);
		}

	free (*this);
	*this = NULL;
}
