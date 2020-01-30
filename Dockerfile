FROM alpine:latest

RUN apk add make

COPY . /src/nezplug
WORKDIR /src/nezplug
RUN make clean

FROM gcc:6
COPY --from=0 /src/nezplug /src/nezplug
WORKDIR /src/nezplug
RUN make debug && make dist/nezplug.o && make libnezplug.so
RUN ls -lh dist libnezplug.so


FROM gcc:7
COPY --from=0 /src/nezplug /src/nezplug
WORKDIR /src/nezplug
RUN make debug && make dist/nezplug.o && make libnezplug.so
RUN ls -lh dist libnezplug.so


FROM gcc:8
COPY --from=0 /src/nezplug /src/nezplug
WORKDIR /src/nezplug
RUN make debug && make dist/nezplug.o && make libnezplug.so
RUN ls -lh dist libnezplug.so

FROM gcc:9
COPY --from=0 /src/nezplug /src/nezplug
WORKDIR /src/nezplug
RUN make debug && make dist/nezplug.o && make libnezplug.so
RUN ls -lh dist libnezplug.so

FROM debian:10
RUN apt-get update && \
    apt-get install -y curl gpg
RUN echo "deb http://apt.llvm.org/buster/ llvm-toolchain-buster-8 main" > /etc/apt/sources.list.d/llvm.list && \
    echo "deb-src http://apt.llvm.org/buster/ llvm-toolchain-buster-8 main" >> /etc/apt/sources.list.d/llvm.list && \
    echo "deb http://apt.llvm.org/buster/ llvm-toolchain-buster-9 main" >> /etc/apt/sources.list.d/llvm.list && \
    echo "deb-src http://apt.llvm.org/buster/ llvm-toolchain-buster-9 main" >> /etc/apt/sources.list.d/llvm.list && \
    echo "deb http://apt.llvm.org/buster/ llvm-toolchain-buster main" >> /etc/apt/sources.list.d/llvm.list && \
    echo "deb-src http://apt.llvm.org/buster/ llvm-toolchain-buster main" >> /etc/apt/sources.list.d/llvm.list && \
    curl -SsL https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - && \
    apt-get update && \
    apt-get install -y \
      clang \
      clang-tools \
      clang-9 \
      clang-tools-9 \
      clang-8 \
      clang-tools-8 \
      perl \
      make
COPY --from=0 /src/nezplug /src/nezplug
WORKDIR /src/nezplug
RUN make CC=clang-8 debug && make CC=clang-8 dist/nezplug.o && make CC=clang-8 libnezplug.so
RUN ls -lh dist libnezplug.so
RUN make clean && make CC=clang-9 debug && make CC=clang-9 dist/nezplug.o && make CC=clang-9 libnezplug.so
RUN ls -lh dist libnezplug.so
RUN make clean && make CC=clang debug make CC=clang dist/nezplug.o && make CC=clang libnezplug.so
RUN ls -lh dist libnezplug.so
RUN make clean && scan-build --use-cc clang make CC=clang debug
