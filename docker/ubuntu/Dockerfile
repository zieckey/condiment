FROM ubuntu
MAINTAINER zieckey@codeg.cn
RUN apt-get update
RUN mv /etc/apt/sources.list /etc/apt/sources.list.backup
ADD https://raw.githubusercontent.com/zieckey/tools/master/ubuntu/sources.list /etc/apt/sources.list
EXPOSE 80
