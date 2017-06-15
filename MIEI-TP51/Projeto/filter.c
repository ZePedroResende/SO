#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

// =    return 1
#define EQ 1
// >=   return 2
#define EGT 2
// <=   return 3
#define ELT 3
// >    return 4
#define GT 4
// <    return 5
#define LT 5
// !=   return 6
#define DIF 6
// else return 0

char *getln(char *buf, ssize_t N, int *size){
    int i;
    char *r;

    for(i = 0 ; i < N && buf[i] != '\n' && buf[i] != EOF && buf[i] != '\0'; i++);

    if(buf[i] == '\n' && i < N)
        i++;

    r = strndup(buf, i);
    *size = i;

    return r;
}

ssize_t readln(int fildes, char *buf, size_t maxlen){

    int i, n;
    char ch='\0';

    i = 0;
    do{
        n = read(fildes, &ch, 1);
        buf[i]=ch;
        if(n == 1) i++;
    }while(i < maxlen && ch !='\n' && n == 1);
    buf[i] = '\0';

    return i;
}

int cmpFunc(char *str){
    int r = 0;

    if(strcmp(str, "=") == 0)
        r = EQ;
    else if(strcmp(str, ">=") == 0)
        r = EGT;
    else if(strcmp(str, "<=") == 0)
        r = ELT;
    else if(strcmp(str, "<") == 0)
        r = LT;
    else if(strcmp(str, ">") == 0)
        r = GT; 
    else if(strcmp(str, "!=") == 0)
        r = DIF;

    return r;

}

int strIsNumber(char *str){
    int i;

    for(i = 0 ; isdigit(str[i]) && str[i] != '\0' ; i++);

    return(str[i] == '\0');
}

int argCheck(char **argv){
    int r = 0; 

    if(strIsNumber(argv[1]) &&
            cmpFunc(argv[2]) &&
            strIsNumber(argv[3]) &&
            (strcmp(argv[1], "0") > 0) &&
            (strcmp(argv[3], "0") > 0))
        r = 1;

    return r;
}

int lnfill(char *ln, char **c, int N){
    int i, i1, i2, size;

    for(i = i2 = 0; i < N ; i1++, i2++){
        for(i1 = i, size = 0 ; i1 < (N-1) && ln[i1] != ':'; i1++, size++);
        c[i2] = strndup(ln + i, size);
        i = i1 + 1;
    }
    return i2;
}

int comp(char **c, int n1, int cond, int n2){
    int r = 0, l1, l2;

    if(!strIsNumber(c[n1]) && !strIsNumber(c[n2])){
        if(cond == EQ)
            r = (strcmp(c[n1] , c[n2]) == 0);
        else if(cond == GT)
            r = (strcmp(c[n1] , c[n2]) > 0);
        else if(cond == LT)
            r = (strcmp(c[n1] , c[n2]) < 0);
        else if(cond == EGT)
            r = (strcmp(c[n1] , c[n2]) >= 0);
        else if(cond == ELT)
            r = (strcmp(c[n1] , c[n2]) <= 0);
        else if(cond == DIF)
            r = (strcmp(c[n1] , c[n2]) != 0);
    }
    else if(strIsNumber(c[n1]) && strIsNumber(c[n2])){
        l1 = (int) strtol(c[n1], (char**) NULL, 10);
        l2 = (int) strtol(c[n2], (char**) NULL, 10);

        if(cond == EQ)
            r = (l1 == l2);
        else if(cond == GT)
            r = (l1 > l2);
        else if(cond == LT)
            r = (l1 < l2);
        else if(cond == EGT)
            r = (l1 >= l2);
        else if(cond == ELT)
            r = (l1 <= l2);
        else if(cond == DIF)
            r = (l1 != l2);
    }
    
    else perror("Estão a ser comparadas palavras com número"), exit(-1); 

    return r;
}

int filterln(char *ln, int N, int n1, int cond, int n2) {
    int r = 0, N1; 

    char **c = (char**) malloc(sizeof(char*) * N/2 + 2);

    N1 = lnfill(ln, c, N);

    if(n1 < N1 && n2 < N1)
        r = comp(c, n1, cond, n2);

    for(int i = 0 ; i < N1 ; i++)
        free(c[i]);
    free(c);

    return r;
}

int main(int argc, char **argv){
    ssize_t R, r;
    int n1, n2, cond, size = 0;
    char buf[PIPE_BUF], *buf2, *ln;
    int filde;

    if(argc != 4 || !argCheck(argv))
        perror("São necessários 3 argumentos: [valor >0] [condição] [valor >0]"), exit(-1);

    n1 = (int) strtol(argv[1], (char**) NULL, 10) - 1;
    n2 = (int) strtol(argv[3], (char**) NULL, 10) - 1;
    cond = cmpFunc(argv[2]);

    do{
        R = r = read(0, buf, PIPE_BUF);
        buf2 = buf;

        do{
            ln = getln(buf2, r, &size);
            buf2 = buf2 + size;
            r = r - size;
            if(filterln(ln, size, n1, cond, n2))
                write(1, ln, size);

            free(ln);
        }while(size > 0); 
    }while(R > 0);

    return 0;
}
