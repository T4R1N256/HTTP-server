#include "server.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct Server server_init(int domain, int port, int service, int protocol,
                          int backlog, u_long interface,
                          void (*launch)(struct Server *server)) {
  struct Server server;

  server.domain = domain;
  server.port = port;
  server.service = service;
  server.protocol = protocol;
  server.backlog = backlog;

  server.address.sin_family = domain;
  server.address.sin_port = htons(port);             // Correct bit order
  server.address.sin_addr.s_addr = htonl(interface); //

  server.socket_fd = socket(domain, service, protocol);

  if (server.socket_fd == -1) {
    perror("[Server] Failed to connect to socket...");
    exit(EXIT_FAILURE);
  }

  int opt = 1;

  if (setsockopt(server.socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt,
                 sizeof(opt)) == -1) {
    handle_socket_exit("[Server] Failed to set socket options...",
                       server.socket_fd);
  }

  if (bind(server.socket_fd, (struct sockaddr *)&server.address,
           sizeof(server.address)) == -1) {
    handle_socket_exit("[Server] Failed to bind socket...", server.socket_fd);
  }

  if (listen(server.socket_fd, server.backlog) == -1) {
    handle_socket_exit("[Server] Failed to start listening...",
                       server.socket_fd);
  }

  server.launch = launch;
  return server;
}

void server_launch(struct Server *server) {
  char buf[BUFFER_SIZE];

  while (1) {
    puts("==== WAITING FOR CONNECTION ===");
    int addrlen = sizeof(server->address);
    int new_socket =
        accept(server->socket_fd, (struct sockaddr *)&server->address,
               (socklen_t *)&addrlen);

    ssize_t bytes_read = read(new_socket, buf, BUFFER_SIZE - 1);

    if (bytes_read >= 0) {
      buf[bytes_read] = '\0';
      puts(buf);
    } else {
      perror("[Server] Error reading buffer...");
    }

    char *response = "HTTP/1.1 200 OK\r\n"
                     "Content-Type: text/html; charset=UTF-8\r\n\r\n"
                     "<!DOCTYPE html>\r\n"
                     "<html>\r\n"
                     "<head>\r\n"
                     "<title>Testing Basic HTTP-SERVER</title>\r\n"
                     "</head>\r\n"
                     "<body>\r\n"
                     ":-)\r\n"
                     "</body>\r\n"
                     "</html>\r\n";
    write(new_socket, response, strlen(response));
    close(new_socket);
  }
}
