.PHONY: all clean amalg

CC = cc
AR = ar
CFLAGS = -g0 -Os -Wall -Wextra -Werror -Isrc/include $(CFLAGS_EXTRA)

LIBNEZPLUG_SRCS = src/amalg.c

LIBNEZPLUG_OBJS = $(LIBNEZPLUG_SRCS:.c=.o)

all: libnezplug.a libnezplug.so

libnezplug.a: $(LIBNEZPLUG_OBJS)
	$(AR) rcs $@ $^

libnezplug.so: $(LIBNEZPLUG_OBJS)
	$(CC) -s -shared -o $@ $^

clean:
	rm -f libnezplug.a
	rm -f libnezplug.so
	rm -f $(LIBNEZPLUG_OBJS)
