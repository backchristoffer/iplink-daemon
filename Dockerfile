FROM centos/devtoolset-7-toolchain-centos7
WORKDIR /app
COPY ./src/iplink_src/. .
USER root
RUN yum -y install net-tools iproute
RUN chown default .
USER default
RUN gcc -pthread -o iplink iplink.c
CMD ["./iplink"]