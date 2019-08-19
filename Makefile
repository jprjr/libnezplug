.PHONY: all clean amalg

CC = cc
AR = ar
CFLAGS = -g -Os -Wthread-safety -Wall -Wextra -Werror -Isrc/include $(CFLAGS_EXTRA)

LIBNEZPLUG_SRCS = src/amalg.c

LIBNEZPLUG_OBJS = $(LIBNEZPLUG_SRCS:.c=.o)

all: libnezplug.a libnezplug.so

libnezplug.a: $(LIBNEZPLUG_OBJS)
	$(AR) rcs $@ $^

libnezplug.so: $(LIBNEZPLUG_OBJS)
	$(CC) -shared -o $@ $^

clean:
	rm -f libnezplug.a
	rm -f libnezplug.so
	rm -f $(LIBNEZPLUG_OBJS)

amalg:
	mkdir -p dist
	perl aux/amalgate.pl src/amalg.c > dist/nezplug.c
	cp src/include/nezplug/nezplug.h dist/nezplug.h
	cp src/include/nezplug/pstdint.h dist/pstdint.h
