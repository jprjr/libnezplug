FROM alpine:latest

RUN apk add gcc make musl-dev perl

COPY . /src/nezplug
WORKDIR /src/nezplug
RUN make clean && make
RUN strip libnezplug.so
RUN make amalg
RUN cc -Wall -Wextra -g0 -Os -c -o dist/nezplug.o dist/nezplug.c
RUN ls -lh
RUN ls -lh dist
