#ifndef _UTILS_H
#define _UTILS_H

#include <stdlib.h>
#define handle_socket_exit(msg, fd)                                            \
  do {                                                                         \
    close(fd);                                                                 \
    perror(msg);                                                               \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

#endif
