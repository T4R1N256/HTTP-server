#ifndef _HTTP_SERVER_H
#define _HTTP_SERVER_H

#include <netinet/in.h>

#define BUFFER_SIZE 4096

struct Server {
  int domain;
  int port;
  int service;
  int protocol;
  int backlog;
  u_long interface;

  int socket_fd;
  struct sockaddr_in address;

  void (*launch)(struct Server *server);
};

struct Server server_init(int domain, int port, int service, int protocol,
                          int backlog, u_long interface,
                          void (*launch)(struct Server *server));

void server_launch(struct Server *server);

#endif
