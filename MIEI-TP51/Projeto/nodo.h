#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <limits.h>
#include <assert.h>
#include "readline.h"
char* concat(const char *s1, const char *s2);

int nodo(char * numero, char *cmd , char **args) ;

int controlador() ;

int main();
