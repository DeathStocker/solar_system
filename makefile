# This makefile has been taken from a previous project "nako"
# https://github.com/FanaticalFighter/nako

CC=gcc
CFLAGS=-c -Wall
LFLAGS=-Wall

DFLAGS=-g -Wextra -Werror

RM=rm
RMFLAGS=-f

# Source files in the project. Append new files here.
SRCS=body.c vector.c

# Create object files list from source files list.
OBJS= $(SRCS:.c=.o)

all: body
all: clean-objects

# clean completely wipes directory of all executables and objects
clean: clean-objects
	$(RM) $(RMFLAGS) body

# only removes objects, not final executable
clean-objects:
	$(RM) $(RMFLAGS) *.o

clean-data:
	$(RM) $(RMFLAGS) *.dat

debug: CFLAGS+=$(DFLAGS)
debug: LFLAGS+=$(DFLAGS)
debug: all

body: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o body -lm

# the following magic code is from here:
# http://www.cs.swarthmore.edu/~newhall/unixhelp/howto_makefiles.html
#
# Use with care. This automatically builds all .c files inside the folder.
.c.o:
	$(CC) $(CFLAGS) $< -o $@
