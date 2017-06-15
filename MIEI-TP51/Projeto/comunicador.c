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
#include "nodo.h"
#include "menu.h"
#define NIL -1

int acaba = 1;

int aWriteB(int in , int *out, int nOut ) {
    char buf[PIPE_BUF];
    int a = 0;
    int i;

    while((a = readline(in,buf,PIPE_BUF))>0) {

        i = 0;
        while(i<nOut) {
            //            inject("ola","2");
            // printf("%s\n",buf);
            write(out[i],buf,a);
            i++;
        }
    }
    return a;
}

int comunicador(int in,int nNodos, Matrix m) {
    //printf("o meu pid é %d\n",getpid());
    int fd1,fd2;
    char buf[PIPE_BUF];
    int i= in;
    char str1[15];
    sprintf(str1, "%d", in);
    char * outmyfifo = concat("/tmp/outmyfifo",str1);
    //    printf("out -> %s\n",outmyfifo);
    fd1 = open(outmyfifo, O_RDONLY);
    int x = 0;
    while(( x= readline(fd1,buf,PIPE_BUF))>0) {

        int a = 0;
        //printf("%d  e o a %d  %d \n",x,a,i);
        while(a<=nNodos) {
            if((m->m)[i][a]!= NIL) {
                char str2[15];
                sprintf(str2, "%d", a);
                char * inmyfifo = concat("/tmp/inmyfifo",str2);
                //printf("in -> %s\n",inmyfifo);
                //                printf(("o pai %d é %d\n"),in,getppid());
                fd2 = open(inmyfifo, O_WRONLY);
                write(fd2,buf,x);
                //                close(fd2);
            }
            a++;
        }

    }

    //printf(("o pai é %d\n"),getppid());
    //    printf("1adeus mundo cruel\n");
    return (0);
}


int disconnect(char *in, char *out, Matrix m) {

    int i=0,p=0,a=0;
    for(i=0; i<(m->N); i++) {
        a = (m->m)[atoi(in)][i];
        if(a != NIL) {
            //           printf("%d\n",a);
            kill(a,SIGTERM);
            waitpid(a,NULL,0);
            break;
        }
    }
    //    printf("ta diz %d",(m->m)[3][1]);
    a = (m->m)[atoi(in)][atoi(out)] ;
    insertConnection(m, atoi(in), atoi(out), NIL);

    p = fork();
    if(!p) {

        comunicador(atoi(in),(m->N),m);
        exit(0);
    }
    else {

        for(i=0; i < (m->N); i++) {
            if((m->m)[atoi(in)][i] != NIL)
                m = insertConnection(m, atoi(in), i, p);
        }
        //wait(NULL);
        return p;
    }


}
int connect(char *in, char **out, Matrix m) {
    int i=0,p=0,a=0;
    for(i=0; i<m->N; i++) {
        a = (m->m)[atoi(in)][i];
        if(a != NIL) {
            //printf("%d\n",a);
            kill(a,SIGTERM);
            break;
        }
    }

    for(i=0; out[i]; i++) {
        a = (m->m)[atoi(in)][atoi(out[i])] ;
        insertConnection(m, atoi(in), atoi(out[i]), 1);
    }
    p = fork();
    if(!p) {

        comunicador(atoi(in),m->N,m);
        exit(0);
    }
    else {
        for(i=0; i < m->N; i++) {
            if((m->m)[atoi(in)][i] != NIL)
                insertConnection(m, atoi(in), i, p);
        }
        return p;
    }
}

int inject(char *node,char *cmd, char **args) {
    if(fork()==0) {

        int fd1=0;
        char * inmyfifo = concat("/tmp/inmyfifo",node);
        fd1 = open(inmyfifo, O_WRONLY);
        dup2(fd1,1);
        close(fd1);
        //printf("olaaa\n");
        execvp(cmd,args);
        //        printf("quanto escreveu %d\n",ola);
        _exit(0);
    }
    else
        return 0;
}

/*
   int main() {

   int** x;
   int dimension = 100;
   x = malloc(dimension* sizeof(int*));
   for (int i = 0; i <= dimension; i++) {
   x[i] = malloc(dimension* sizeof(int));
   for(int a = 0; a <= dimension; a++)
   x[i][a] = NIL;
   }



   int pid[1024];
   int files[1024];

   char *cmd = "const";
   char *args[] = {"const","l0",NULL};
   char *args1[] = {"const","50",NULL};
   char *args2[] = {"const","40",NULL};

   nodo("1",cmd,args);
   nodo("2",cmd,args1);
   nodo("3",cmd,args2);
   nodo("4",cmd,args2);
   char *out[]= {"2",NULL};
   char *out1[]= {"3",NULL};
   char *out2[]= {"2",NULL};
   connect("1",out ,x,100);
   printf("2 1 %d\n",x[1][2]);
   connect("1",out1 ,x,100);
   printf("3 1 %d\n",x[1][3]);
   printf("2 1 %d\n",x[1][2]);
   connect("4",out2,x,100);
   char * inmyfifo = "/tmp/inmyfifo1";
   char * inmyfifo4 = "/tmp/inmyfifo4";
   char * outmyfif1 = "/tmp/outmyfifo2";
   char * outmyfif2 = "/tmp/outmyfifo3";
   int fd1[2];
   int fd2[2];
   int stdin[1];
   int stdout[1];
   fd1[0] = open(inmyfifo, O_WRONLY);
   fd1[1] = open(inmyfifo4, O_WRONLY);
   fd2[0] = open(outmyfif1, O_RDONLY);
   fd2[1] = open(outmyfif2, O_RDONLY);
   printf("%d  %d %d\n",fd1[0],fd2[0],fd2[1]);
   stdin[0]= 0;
   stdout[0] = 1;
   if(fork()==0) {
   aWriteB(fd2[0],stdout,1);
   }
   else {
   if(fork()==0) {
   aWriteB(fd2[1],stdout,1);

   }

   else {
   aWriteB(stdin[0],fd1,2);
   }


   }


   return 0;
   }
   */
/*
   int connect(char *in, char **out,int **matrix,int tamanho) {
   int i=0,p=0;
   for(i=0; out[i]; i++) {
   int a = matrix[atoi(in)][atoi(out[i])] ;
//        if(a!= NIL)
//            kill(a,SIGKILL);
matrix[atoi(in)][atoi(out[i])]= 1;
}
p = fork();
if(!p) {

comunicador(atoi(in),tamanho,matrix);
exit(0);
}
else {

for(i=0; out[i]; i++) {
matrix[atoi(in)][atoi(out[i])] = p;
}
return 0;
}


}
*/

int teste() {

    Matrix x = initMatrix(100);
    /*
    int dimension = 100;
    x = malloc(dimension* sizeof(int*));
    for (int i = 0; i <= dimension; i++) {
        x[i] = malloc(dimension* sizeof(int));
        for(int a = 0; a <= dimension; a++)
            x[i][a] = NIL;
    }
    */

    /*    if(nodo > dimension) {

          x = realloc(x,2*dimension* sizeof(int*));
          for (int i = 0; i < dimension; i++) {
          x[i] = realloc(x[i],2*dimension* sizeof(int));
          }
          }
          */
    int pid[1024];

    char *cmd = "const";
    char *cmd1 = "/usr/bin/tee";
    char *args[] = {"tee","log.txt",NULL};
    char *args1[] = {"const","20",NULL};
    char *args2[] = {"const","30",NULL};
    char *args3[] = {"const","40",NULL};

    char *args4[] = {"cat","1.txt",NULL};
    char *args5[] = {"cat","2.txt",NULL};

    int i = 0;

    pid[i] = nodo("1",cmd1,args);
    i++;
    pid[i] = nodo("2",cmd,args1);
    i++;
    pid[i] = nodo("3",cmd,args2);
    i++;
    nodo("4",cmd,args3);
    char *out[]= {"1",NULL};
    char *out2[]= {"4",NULL};
    pid[i] = connect("3",out ,x);
    i++;
    pid[i] = connect("2",out ,x);
    i++;
    //   connect("2",out2,x,100);
    //    connect("3",out ,x,100);
    //    disconnect("3",out ,x,100);
    //    connect("3",out ,x,100);
    //    printf("asdasd\n");
    char * inmyfifo = "/tmp/outmyfifo1";
    char * inmyfifo1 = "/tmp/outmyfifo4";
    char * outmyfif1 = "/tmp/inmyfifo2";
    char * outmyfif2 = "/tmp/inmyfifo3";
    int fd1[2];
    int fd2[2];
    //int fd3[1];
    int stdi[1];
    int stdou[1];
    fd1[0] = open(inmyfifo, O_RDONLY);
    fd1[1] = open(inmyfifo1, O_RDONLY);
    fd2[1] = open(outmyfif1, O_WRONLY);
    fd2[0] = open(outmyfif2, O_WRONLY);
    stdi[0]= 0;
    stdou[0] = 1;
    int p;
    p = fork();
    /*    if(!p) {
            aWriteB(fd1[0],stdou,1);
        }
        */
    /*    else {


            if(fork()==0) {
                aWriteB(fd1[1],stdou,1);
            }
            else {
    */
    pid[i] = p;
    i++;
    //inject("sdadsa1\n","2");
    inject("3",args4[0],args4);
    connect("2",out2,x);
    inject("2",args5[0],args5);
    //      inject("sdadsa2\n","2");
    //        inject("ggggggg\n","3");
    //        inject("ola\n","2");
    aWriteB(stdi[0],fd2,2);
    //        wait(NULL);
    int q;
    /*
       while(1)
       pause();
       */
    for (q=0 ; q<i; q++)
        waitpid(pid[q], NULL, 0);

//}

//}

    return 0;
}
