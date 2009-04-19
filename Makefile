#Change PROGNAME for final release
PROGNAME=math

CC=cc
INCLUDES=-I./
CCFLAGS=-g -O2 -Wall -W -pedantic -ansi -Wtraditional -Wundef -Wshadow \
	-Wpointer-arith -Wbad-function-cast -Wcast-qual -Wcast-align \
	-Wwrite-strings -Wconversion -Wsign-compare -Waggregate-return \
	-Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations \
	-Wnested-externs -Winline #-Wredundant-decls
CCLIBS=
DEFINES=-DPROGNAME="\"math\"" -DVERSION="\"0.1 beta\""
SOURCE=main.c stackobject.c infix_to_postfix.c error.c
OBJECTS=$(SOURCE:.c=.o)

all: $(OBJECTS)
	$(CC) $(DEFINES) $(INCLUDES) $(CCFLAGS) -o $(PROGNAME) $(OBJECTS) $(CCLIBS) 

.c.o:
	$(CC) $(DEFINES) $(INCLUDES) $(CCFLAGS) -c -o $@ $<

$(OBJECTS):$(SOURCE)

clean:
	rm -f $(OBJECTS) $(PROGNAME)
