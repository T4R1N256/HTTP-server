#include "server.h"
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (argc > 1)
    exit(0);

  strcpy(argv[0], "HTTP Server");

  struct Server server =
      server_init(AF_INET, 8008, SOCK_STREAM, 0, 10, INADDR_ANY, server_launch);

  server.launch(&server);

  return 0;
}
