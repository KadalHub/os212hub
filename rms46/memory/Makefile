# REV01 Sat 03 Apr 2021 11:00:00 WIB
# START Sat 03 Apr 2021 06:00:00 WIB

# INFO: With this "Makefile", just run:
# INFO:                     make

CC            = gcc
CPP           = cpp
CFLAGS        = -std=gnu99
LDFLAGS       = -Xlinker -Map=$@.map
CPPFLAGS      =
DEPFLAGS      = -MM -MT $(@:.d=.o) 
OUTPUT_OPTION = -o $@
COMPILE       = $(CC) $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS) -c
SRCS          = $(wildcard *.c)
OBJ           = $(SRCS:.c=.o)
DEP           = $(OBJ:.o=.d)
PROGS         = $(SRCS:.c=  )

P00=mymemory

EXECS= \
   $(P00) \


all:  $(EXECS)

$(EXECS): %: %.c $(DEPS)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -f ${EXECS} 
	rm -f *.map
 
.phony: clean all

