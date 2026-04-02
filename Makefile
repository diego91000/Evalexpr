CC = gcc
CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wvla
LDFLAGS =
LDLIBS =

PROG = evalexpr

SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:.c=.o)

all : $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS)

check:
	./tests/test2.sh

clean:
	rm -f $(OBJS) $(PROG) testsuite
