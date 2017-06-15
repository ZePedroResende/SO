#ifndef MENU_H
#define MENU_H

#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include "comunicador.h"
#include "readline.h"

#define NIL -1
#define SIZE 1024

typedef struct matrix{
    pid_t **m;
    int N;
}*Matrix;

typedef struct pidarray{
    pid_t pids[SIZE];
    int N;
}*PidArray;

PidArray initPidArray();
void addPid(PidArray v, pid_t p);

Matrix initMatrix(int size);

Matrix resizeMatrix(Matrix m, int size);

Matrix insertConnection(Matrix m, int in, int out, pid_t connection);

void execFunc(char *args[], Matrix m, PidArray v);

void execLine(char *ln, Matrix m, PidArray v);

void execFile(int filde, Matrix m, PidArray v);
//int main(int argc, char *argv[]);
#endif
