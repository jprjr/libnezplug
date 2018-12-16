.PHONY: all clean

CC = cc
AR = ar
CFLAGS = -g -O0 -Wall -Wextra -Werror -Isrc/include $(CFLAGS_EXTRA)

LIBNEZPLUG_SRCS = \
  src/cpu/kmz80.c \
  src/device/nes/logtable.c \
  src/device/nes/s_apu.c \
  src/device/nes/s_fds.c \
  src/device/nes/s_fds1.c \
  src/device/nes/s_fds2.c \
  src/device/nes/s_fds3.c \
  src/device/nes/s_fme7.c \
  src/device/nes/s_mmc5.c \
  src/device/nes/s_n106.c \
  src/device/nes/s_vrc6.c \
  src/device/nes/s_vrc7.c \
  src/device/opl/s_deltat.c \
  src/device/opl/s_opl.c \
  src/device/opl/s_opltbl.c \
  src/device/s_dmg.c \
  src/device/s_hes.c \
  src/device/s_hesad.c \
  src/device/s_logtbl.c \
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
  src/format/songinfo.c

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
