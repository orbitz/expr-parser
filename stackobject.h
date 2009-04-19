#ifndef _STACKOBJECT_H_
#define _STACKOBJECT_H_

/* includes */
#include <unistd.h>
#include <stdlib.h>
#include <boolean.h>
#include <globaldefs.h>

/*
 * STACKOBJECT struct.  This is basically a linked list
 */
struct STACKOBJECT
{
  void *data;
  long size;					/* How many bytes this data takes up */
  struct STACKOBJECT *next;
};

/* Functions */
struct STACKOBJECT *stack_init (void);
boolean stack_push (struct STACKOBJECT **this, const void *data, const size_t size);
char *stack_pop (struct STACKOBJECT **this);
boolean stack_isEmpty (const struct STACKOBJECT **this);
void stack_destroy (struct STACKOBJECT **this);


#endif /* _STACKOBJECT_H_ */
