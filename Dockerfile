FROM fedora:40
WORKDIR /app
COPY . .
USER root
RUN dnf -y install net-tools iproute gcc
RUN useradd -m default
RUN chown -R default:default /app
USER default
RUN gcc -pthread -o iplinkd iplink.c
CMD ["./iplinkd"]