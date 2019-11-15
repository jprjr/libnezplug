.PHONY: all clean

CFLAGS = -Wall -Wextra -fPIC
LDFLAGS = -lm

all: make-tables

make-tables.o: make-tables.c
	$(CC) $(CFLAGS) -o $@ -c $<

logtable.o: logtable.c
	$(CC) $(CFLAGS) -o $@ -c $<

opltable.o: opltable.c
	$(CC) $(CFLAGS) -o $@ -c $<

make-tables: make-tables.o logtable.o opltable.o
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f make-tables.o logtable.o make-tables
