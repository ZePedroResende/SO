#include <unistd.h>
#include <string.h>
#include <limits.h>
#include "readline.h"

int main(int argc, char *argv[]) {
    int r1;
    char buff[PIPE_BUF];

    while((r1=readline(0,buff,PIPE_BUF))>0) {
    	if(r1>1){
        write(1,buff,r1-1); /*r1-1 para nao escrever o \n que leu*/
        write(1,":",1);
        write(1,argv[1],strlen(argv[1]));
        write(1,"\n",1);
    	}
    }
    return 0;
}
