#ifndef COMUNICADOR_H
#define COMUNICADOR_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <limits.h>
#include <signal.h>
#include <assert.h>
#include "readline.h"
#include "menu.h"
#include "nodo.h"

typedef struct matrix *Matrix;

int aWriteB(int in , int *out, int nOut ) ;
int comunicador(int in,int nNodos, Matrix m);
int disconnect(char *in, char *out, Matrix m);
int connect(char *in, char **out, Matrix m);
int inject(char *node,char *cmd, char **args) ;


#endif
