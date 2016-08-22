# This makefile has been taken from a previous project "nako"
# https://github.com/FanaticalFighter/nako

CC=gcc
CFLAGS=-c -Wall
LFLAGS=-Wall

DFLAGS=-g -Wextra -Werror

RM=rm
RMFLAGS=-f

# Source files in the project. Append new files here.
SRCS=vel_ver.c calc.c vector.c

# Create object files list from source files list.
OBJS= $(SRCS:.c=.o)

all: vel_ver
all: clean-objects

# clean completely wipes directory of all executables and objects
clean: clean-objects
	$(RM) $(RMFLAGS) vel_ver

# only removes objects, not final executable
clean-objects:
	$(RM) $(RMFLAGS) *.o

debug: CFLAGS+=$(DFLAGS)
debug: LFLAGS+=$(DFLAGS)
debug: all

vel_ver: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o vel_ver -lm

# the following magic code is from here:
# http://www.cs.swarthmore.edu/~newhall/unixhelp/howto_makefiles.html
#
# Use with care. This automatically builds all .c files inside the folder.
.c.o:
	$(CC) $(CFLAGS) $< -o $@
