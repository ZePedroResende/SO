#include "menu.h"

PidArray initPidArray() {
    PidArray r = (PidArray) malloc(sizeof(struct pidarray));

    if(r != NULL)
        r->N = 0;

    return r;
}

void addPid(PidArray v, pid_t p) {
    if(!(v->N < SIZE))
        perror("NÃO SE PODEM ADICIONAR MAIS PIDS\n"), _exit(-1);

    (v->pids)[v->N] = p;
    v->N++;
}

Matrix initMatrix(int size) {
    Matrix r = (Matrix) malloc(sizeof(struct matrix));
    int c;

    if(r != NULL) {
        r->N = size;
        r->m = (pid_t**) malloc(sizeof(pid_t*) * size);
        if(r->m != NULL) {
            for(int l = 0; l < size; l++) {
                (r->m)[l] = (pid_t*) malloc(sizeof(pid_t) * size);
                for(c = 0 ; (r->m)[l] != NULL && c < size ; c++)
                    (r->m)[l][c] = NIL;
            }
        }
    }
    return r;
}


Matrix resizeMatrix(Matrix m, int size) {
    int initSize = m->N;
    int l, c;

    if(size < m->N)
        perror("A REDIMENSIONAR PARA MENOR\n"), _exit(-1);

    if(m != NULL) {
        m->N = size;
        m->m = (pid_t**) realloc(m->m, sizeof(pid_t*) * size);
        if(m->m != NULL) {
            for(l = 0; l < initSize; l++) {
                (m->m)[l] = (pid_t*) realloc((m->m)[l], sizeof(pid_t) * size);
                for(c = initSize ; c < size && (m->m)[l] != NULL ; c++)
                    (m->m)[l][c] = NIL;
            }
            for(l = initSize ; l < size ; l++) {
                (m->m)[l] = (pid_t*) malloc(sizeof(pid_t) * size);
                for(c = 0 ; c < size && (m->m)[l] != NULL ; c++)
                    (m->m)[l][c] = NIL;
            }
        }
    }
    return m;
}

Matrix insertConnection(Matrix m, int in, int out, pid_t connection) {
    int major = out;
    if(in > out)
        major = in;

    if(major >= m->N)
        m = resizeMatrix(m, major+1);

    (m->m)[in][out] = connection;

    return m;
}

void execFunc(char *args[], Matrix m, PidArray v) {
    int p = 0;
    if(strcmp(args[0], "node") == 0)
        p = nodo(args[1],args[2],args+2);
    else if(strcmp(args[0], "connect") == 0)
        p = connect(args[1],args+2, m);
    else if(strcmp(args[0], "disconnect") == 0)
        p = disconnect(args[1], args[2], m);
    else if(strcmp(args[0],"inject") == 0)
        p = inject(args[1], args[2], args+2);
    else
        dprintf(2,"O comando '%s' não é válido\n",args[0]);

    addPid(v, p);
}

/*\Executa sobre uma linha/um comando.
 * @param ln Linha.
 */
void execLine(char *ln, Matrix m, PidArray v) {
    char delim[3] = " \n";
    char **args = NULL;
    char *tmp = NULL;
    int size = 2;
    int i;

    args = (char**) malloc(sizeof(char*) * size);
    if(args != NULL) {
        i = 0;
        while((tmp = strsep(&ln, delim)) != NULL) {
            if(*tmp != '\0') {
                if(i >= size-1)
                    size*=2, args = (char**) realloc(args, sizeof(char*) * size);
                args[i] = tmp;
                //printf("arg%d: ->%s<-\n",i, args[i]);
                i++;
            }
        }
        args[i] = NULL;
        if (args[0] != NULL)
            execFunc(args, m, v);
        free(args);
    }
}

/*
void execChunk(char *buf, int N){
    char delim[2] = "\n", *token;
    int n;

    while((token = strsep(&buf, delim)) != NULL && *token != '\0' && (n = strlen(token)) < N){
        //printf("%s\n",token);
        N-=n;
        execLine(token);
    }
}
*/

/*\Executa o programa sobre um ficheiro com todos os comandos
 * @param filde Identificador do ficheiro.
 */
void execFile(int filde, Matrix m, PidArray v) {
    char buf[PIPE_BUF];
    int n;

    while((n = readline(filde, buf, PIPE_BUF)) > 0) {
        if(n <= PIPE_BUF) {

            execLine(buf, m, v);
        }
        else
            dprintf(2, "ERRO: Comando de dimensão demasiado grande. (> %d)\n", PIPE_BUF);
    }
}

int main(int argc, char *argv[]) {
    char buf[PIPE_BUF];
    int i = 0, n;
    int filde;
    Matrix m = initMatrix(10);
    PidArray v = initPidArray();

    /*
    addPid(v, 3);
    addPid(v, 1);
    addPid(v, 9);
    addPid(v, 3);
    for(int i = 0 ; i < v->N ; i++)
        printf("->%d\n",(v->pids)[i]);
    */
    /*
    m = insertConnection(m,14,21,55);

    int c;
    for(int l = 0 ; l < m->N ; l++)
        for(c = 0 ; c < m->N ; c++)
            printf("L: %d; C: %d -> %d\n",l,c, (m->m)[l][c]);
    */
    /*    int p;
        p = fork();
        if(!p) {

            char * inmyfifo = "/tmp/outmyfifo1";
            int fd1[2];
            int fd2[2];
            //int fd3[1];
            int stdi[1];
            int stdou[1];
            fd1[0] = open(inmyfifo, O_RDONLY);
            stdi[0]= 0;
            stdou[0] = 1;
            aWriteB(fd1[0],stdou,1);
        }
        else {
    */
    for(i = 1 ; i < argc ; i++) {
        filde = open(argv[i], O_RDONLY);
        if(filde != -1) {
            execFile(filde, m, v);
            close(filde);
        }
        else
            dprintf(2, "ERRO: O ficheiro %s não existe.\n", argv[i]);
    }
    while((n = readline(0, buf, PIPE_BUF)) >= 0) {
        if(n <= PIPE_BUF)
            execLine(buf, m, v);
        else
            dprintf(2, "ERRO: Comando de dimensão demasiado grande. (> %d)\n", PIPE_BUF);
    }
//}

    return 0;
}
