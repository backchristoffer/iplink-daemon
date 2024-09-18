FROM fedora:40
WORKDIR /app
COPY . .
USER root
RUN dnf -y install net-tools iproute
RUN chown default .
USER default
RUN gcc -pthread -o iplinkd iplink.c
CMD ["./iplinkd"]