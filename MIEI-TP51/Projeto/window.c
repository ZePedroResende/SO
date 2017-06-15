#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include "readline.h"


void write_stdout(int fildes,char *buff1,void *buff2, int nbytes1,int nbytes2){
	write(fildes,buff1,nbytes1); /*r1-1 para nao escrever o \n que leu*/
    write(fildes,":",1);
    write(fildes,buff2,nbytes2);
    write(fildes,"\n",1);
}

int main(int argc, char *argv[]){
    int arg3=atoi(argv[3]);/*qts elementos preciso para calcular*/
	int arg1=atoi(argv[1]);/*coluna que contem a informação*/
	int res[2];/*posições em que vai ser guardadas as informações para calcular o avg,max,min,sum*/
	int r1;
    char buff[PIPE_BUF];
    int coluna=1;/*numero de colunas que encontrei*/
    int i=0;/*percorrer o buff*/
    int j=0;/*para preencher o array*/
    int j_antigo=0;
    int soma=0;
    int media=0;
    int max=0;
    int min=0;
    int flag=0;
    char itoa[PIPE_BUF];
    int tamanho=0;
    

    while((r1=readline(0,buff,PIPE_BUF))>0){
		while(coluna!=arg1){/*ao sair deste while, buff[i] está na posição que me interessa*/
			if(buff[i]==':')
				coluna++;
			i++;
		}
		coluna=1;
		if(j==arg3 || flag==1){ /*já tenho os argumentos que preciso para calcular*/
			if(strcmp(argv[2],"avg")==0){
				for(j=0;j<arg3;j++){
					soma+=res[j];
				}
				media=soma/arg3;
				tamanho=sprintf(itoa,"%d", media);
				write_stdout(1,buff,itoa,r1-1,tamanho);
				j=j_antigo;
				res[j]=atoi(&buff[i]);
				j++;
				if(j_antigo==arg3-1)
					j_antigo=0;
				else
					j_antigo ++;
				flag=1;
				soma=0;
			}
			else if(strcmp(argv[2],"max")==0){
				for(j=0;j<arg3;j++){
					if(max<res[j])
						max=res[j];
				}
				tamanho=sprintf(itoa,"%d", max);
				write_stdout(1,buff,itoa,r1-1,tamanho);
				j=j_antigo;
				res[j]=atoi(&buff[i]);
				j++;
				if(j_antigo==arg3-1)
					j_antigo=0;
				else
					j_antigo ++;
				flag=1;
				max=0;
			}
			else if(strcmp(argv[2],"min")==0){
				min=res[0];
				for(j=1;j<arg3;j++){
					if(min>res[j])
						min=res[j];
				}
				tamanho=sprintf(itoa,"%d", min);
				write_stdout(1,buff,itoa,r1-1,tamanho);
				j=j_antigo;
				res[j]=atoi(&buff[i]);
				j++;
				if(j_antigo==arg3-1)
					j_antigo=0;
				else
					j_antigo ++;
				flag=1;
				min=0;
			}

			else if(strcmp(argv[2],"sum")==0){
				for(j=0;j<arg3;j++){
					soma+=res[j];
				}
				tamanho=sprintf(itoa,"%d", soma);
				write_stdout(1,buff,itoa,r1-1,tamanho);
				j=j_antigo;
				res[j]=atoi(&buff[i]);
				j++;
				if(j_antigo==arg3-1)
					j_antigo=0;
				else
					j_antigo ++;
				flag=1;
				soma=0;
			}
		}
		else{
		res[j]=atoi(&buff[i]);
		tamanho=sprintf(itoa,"%d", res[j-1]);
		write_stdout(1,buff,itoa,r1-1,tamanho);
		j++;
		}
	i=0;
	}
}

