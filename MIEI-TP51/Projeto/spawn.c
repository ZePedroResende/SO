#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include "readline.h"

int main(int argc, char *argv[]){
	int pid;
	int r1;
	char itoa[PIPE_BUF];
	char buff2[PIPE_BUF];
	int tamanho;
	int i;
	char buff[PIPE_BUF];
	int status;
	int coluna=1;
	int flag=-1;
	int encontrei=0;
	int i2=0;
	int i3=0;

    
    while((r1=readline(0,buff,PIPE_BUF))>0){
	 
	 for(int i=1; i<argc; i++){
		for(int j=0; (argv[i])[j]!='\0'; j++){
			if((argv[i])[j]=='$'){
				encontrei=1;
				j++;
				flag=atoi(&((argv[i])[j]));
				i=0;
				while(coluna!=flag){
					if(buff[i]==':')
						coluna++;
				i++;
				}	
				coluna=1;
				i2=0;
				i3=i;
				while(buff[i3]!=':'){
					buff2[i2]=buff[i3];
					i3++;
					i2++;
				}
			}
		}
	} 
	pid=fork();
	if(pid==0){
		fclose(stderr);
		close(1);
		if(encontrei==0)
			execvp(argv[1],&argv[1]);
		else{
			printf("%s\n",buff2);
			execlp(argv[1],argv[1],buff2,NULL);
		}
			
		exit(-errno);
	}
	else{
		wait(&status);
		tamanho=sprintf(itoa,"%d", WEXITSTATUS(status));
		write(1,buff,r1-1); /*r1-1 para nao escrever o \n que leu*/
        write(1,":",1);
        write(1,itoa,tamanho);
        write(1,"\n",1);
	}
}
	return 0;
}