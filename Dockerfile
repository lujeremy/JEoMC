# Based on 20.04 LTS
FROM ubuntu:focal

RUN apt-get -yq update && \
    apt-get -y upgrade && \
    apt-get -yq --no-install-suggests --no-install-recommends install \
    menhir \
    llvm-10 \
    llvm-10-dev \
    m4 \
    git \
    aspcud \
    ca-certificates \
    python2.7 \
    pkg-config \
    cmake \
    opam \
    ocamlbuild

# Our glfw needs to be compiled manually from source

RUN ln -s /usr/bin/lli-10 /usr/bin/lli
RUN ln -s /usr/bin/llc-10 /usr/bin/llc

RUN opam init --disable-sandboxing
RUN opam install \
    llvm.10.0.0

WORKDIR /root

ENTRYPOINT ["opam", "config", "exec", "--"]

CMD ["bash"]
