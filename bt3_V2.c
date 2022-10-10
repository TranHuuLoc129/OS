#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/errno.h>
#include <sys/stat.h>

#define FIFO1 "/tmp/ff.1"
#define FIFO2 "/tmp/ff.2"
#define PM 0666
extern int errno;
#define PIPE_BUF 4096


int main(int argc, char **argv){

	char buffer[20];
	int kq,a,b;
	char c;
	
	int childpid, readfd, writefd;
	
	if((mknod(FIFO1, S_IFIFO | PM,0) <0) && (errno != EEXIST)){
		printf("Fail to create FIFO 1. Aborted.\n");
		return -1;
	}

	if((mknod(FIFO2, S_IFIFO | PM,0) <0) && (errno != EEXIST)){
		unlink(FIFO1);
		printf("Fail to create FIFO 2. Aborted.\n");
		return -1;
	}

	childpid = fork();

	if(childpid == 0){ // con // doc fifo1, ghi fifo2
	
		if((readfd = open(FIFO1, 0)) < 0){
			perror("Child cannot open readFIFO.\n");
			
		}
	
		if((writefd = open(FIFO2, 1)) < 0){
			perror("Child cannot open writeFIFO.\n");
			
		}

		read(readfd,buffer, sizeof(buffer)); // doc doi so tu fifo1
		printf("Child read data from parent: %s\n", buffer);

		 char str1[10]="", str2[10]="",str3[10]="";
		    
		   char *p;
		   p = strtok(buffer," ");
		   strcpy(str1,p);
		   p = strtok(NULL," ");
		   strcpy(str2,p);
		   p = strtok(NULL," ");
		   strcpy(str3,p);
                   a = atoi(str1);
		
		   b = atoi(str2);

                   c  = str3[0];
           
		    switch(c){
			case '+':
				kq = a+b;
				break;
			
			case '-':
				kq = a-b;
				break;

			case 'x':
				kq = a*b;
				break;

			case '/':
				kq = a/b;
				break;
		    }

	
		write(writefd,&kq,sizeof(kq));// ghi kq vao fifo2
		close(readfd);
		close(writefd);
		return -1;		
	}
	else{
		if(childpid > 0){ // cha // ghi fifo 1, doc fifo2
			if((writefd = open(FIFO1, 1)) < 0){
				perror("Parent cannot open writeFIFO.\n");
			}
			
			if((readfd = open(FIFO2, 0)) < 0){
				perror("Parent cannot open readFIFO.\n");
			}

			char str[40]="";
			int i;
			for(i = 1; i<argc-1; i++ ){
				strcat(str,argv[i]);
				strcat(str," ");
			}

			strcat(str,argv[i]);

			
			write(writefd, str, strlen(str)); // ghi doi so vao fifo1
			int temp2;

			read(readfd, &temp2, sizeof(temp2)); // doc kq tu fifo2
			printf("Parent read data from child:	 %d\n", temp2);
			
			while(wait((int*) 0)!=childpid);
			close(readfd);
			close(writefd);

			if(unlink(FIFO1)<0){
				perror("Cannot remove FIFO1.\n");
			}
			if(unlink(FIFO2)<0){
				perror("Cannot remove FIFO2.\n");
			}

			
			
			return 1;
			
		}
		else{
			printf("Fork failed\n");
			return -1;
		}	
	}
	


	return 0;
}
