#include "server.h"
#include <netinet/in.h>

int main(void) {
  struct Server server =
      server_init(AF_INET, 8008, SOCK_STREAM, 0, 10, INADDR_ANY, server_launch);

  server.launch(&server);

  return 0;
}
