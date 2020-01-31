.PHONY: all clean dist debug

CC = cc
AR = ar
CFLAGS = -fPIC -O2 -Wall -Wextra -Werror -Isrc/include $(CFLAGS_EXTRA)

LIBNEZPLUG_SRCS =  \
  src/cpu/kmz80/kmdmg.c \
  src/cpu/kmz80/kmevent.c \
  src/cpu/kmz80/kmr800.c \
  src/cpu/kmz80/kmz80.c \
  src/cpu/kmz80/kmz80c.c \
  src/cpu/kmz80/kmz80t.c \
  src/cpu/kmz80/kmz80u.c \
  src/device/logtable.c \
  src/device/nes/s_apu.c \
  src/device/nes/s_fds1.c \
  src/device/nes/s_fds2.c \
  src/device/nes/s_fds3.c \
  src/device/nes/s_fds.c \
  src/device/nes/s_fme7.c \
  src/device/nes/s_mmc5.c \
  src/device/nes/s_n106.c \
  src/device/nes/s_vrc6.c \
  src/device/nes/s_vrc7.c \
  src/device/opl/s_deltat.c \
  src/device/opl/s_opl.c \
  src/device/s_dmg.c \
  src/device/s_hesad.c \
  src/device/s_hes.c \
  src/device/s_psg.c \
  src/device/s_scc.c \
  src/device/s_sng.c \
  src/format/audiosys.c \
  src/format/handler.c \
  src/format/m_gbr.c \
  src/format/m_hes.c \
  src/format/m_kss.c \
  src/format/m_nsd.c \
  src/format/m_nsf.c \
  src/format/m_sgc.c \
  src/format/m_zxay.c \
  src/format/nezplug.c \
  src/format/nsf6502.c \
  src/format/songinfo.c \
  src/format/trackinfo.c \
  src/logtable/log_table_12_7_30.c \
  src/logtable/log_table_12_8_30.c \
  src/opltable/opl_table.c

LIBNEZPLUG_OBJS = $(LIBNEZPLUG_SRCS:.c=.o)

all: libnezplug.a libnezplug.so

debug: libnezplug-debug.a libnezplug-debug.so

libnezplug-debug.a: $(LIBNEZPLUG_OBJS)
	$(AR) rcs $@ $^

libnezplug-debug.so: $(LIBNEZPLUG_OBJS)
	$(CC) -shared -o $@ $^

libnezplug.a: dist/nezplug.o
	$(AR) rcs $@ $^

libnezplug.so: dist/nezplug.o
	$(CC) -shared -o $@ $^

decode-all: aux/decode-all.o libnezplug.a
	$(CC) -o $@ $^ -lm

decode-all-amalg: aux/decode-all.o dist/nezplug.o
	$(CC) -o $@ $^ -lm

benchmark: aux/benchmark.o libnezplug.a
	$(CC) -s -o $@ $^ -lm

benchmark-amalg: aux/benchmark.o dist/nezplug.o
	$(CC) -s -o $@ $^ -lm

dist/nezplug.o: dist/nezplug.c

dist/nezplug.c: $(LIBNEZPLUG_SRCS)
	mkdir -p dist
	perl aux/amalgate.pl $(LIBNEZPLUG_SRCS) > dist/nezplug.c

clean:
	rm -f libnezplug.a
	rm -f libnezplug.so
	rm -f libnezplug-debug.a
	rm -f libnezplug-debug.so
	rm -f $(LIBNEZPLUG_OBJS)
	rm -rf dist
	rm -f aux/decode-all.o

dist: dist/nezplug.c
	cp src/include/nezplug/nezplug.h dist/nezplug.h
