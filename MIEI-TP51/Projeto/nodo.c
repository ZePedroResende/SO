#include "nodo.h"
char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

int nodo(char *numero, char *cmd , char **args) {

    char * inmyfifo = concat("/tmp/inmyfifo",numero);
    mkfifo(inmyfifo, 0666);
    char * outmyfifo = concat("/tmp/outmyfifo",numero);
    mkfifo(outmyfifo, 0666);
    if(fork()== 0) {

        char buf[PIPE_BUF];
        pid_t pids[3];
        pid_t p;
        int in[2];
        int out[2];
        int i = 0;
        pipe(in);
        pipe(out);

        for(i = 0; i<3 ; i++) {
            p= fork();
            if(p == 0) break;
            pids[i] = p;
        }

        if(!p) {
            if(i == 0) {
                close(in[0]);
                close(out[0]);
                close(out[1]);
                int fd1;
                fd1 = open(inmyfifo, O_RDONLY);
                int a=0;
                while((a = readline(fd1,buf,PIPE_BUF)) > 0)
                    write(in[1],buf,a);
            }
            if(i == 1) {
                close(out[1]);
                close(in[0]);
                close(in[1]);
                int fd2;
                fd2 = open(outmyfifo, O_WRONLY);
                int a=0;
                while((a = readline(out[0],buf,PIPE_BUF))>0)
                    write(fd2,buf,a);
            }
            if(i == 2) {
                close(in[1]);
                close(out[0]);
                dup2(in[0],0);
                dup2(out[1],1);
                close(in[0]);
                close(out[1]);
                execv(cmd,args);
//                int a=0;
//                while((a = readline(0,buf,PIPE_BUF))>0)
//                    assert(0);
//                write(1,buf,a);
            }
        }
        wait(NULL);
        exit(0);
    }
    else     return(getpid());
}


int controlador() {
    int pid[1024];
    int files[1024];

    char *cmd = "const";
    char *args[] = {"const","l0",NULL};

    int p = fork();
    if(p) {
        int fd1,fd2;
        char * inmyfifo = "/tmp/inmyfifo1";

        char * outmyfifo = "/tmp/outmyfifo1";
        fd1 = open(inmyfifo, O_WRONLY);
        fd2 = open(outmyfifo, O_RDONLY);
        char buf[PIPE_BUF];
        int a=0;
        while((a = readline(0,buf,PIPE_BUF))>0) {
            write(fd1,buf,a);
            while((a = readline(fd2,buf,PIPE_BUF))>0)
                write(1,buf,a);
        }
    } else nodo("1",cmd, args);
    return 0;
}
