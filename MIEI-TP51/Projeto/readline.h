#ifndef READLINE_H
#define READLINE_H
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
size_t readline(int fildes, char * buf, size_t nbyte);
#endif
