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

int giaithua(int n){
	if(n<2){
		return 1;
	}
	
	int res = 1,i;
	for(i = 1; i<= n; i++){
		res = res *i;
	}

	return res;


}

int main(int argc, char **argv){

	int buffer;
	
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

		read(readfd,&buffer, sizeof(buffer)); // doc doi so tu fifo1
		printf("Child read data from parent: %d\n", buffer);

		int kq = giaithua(buffer);
	
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

			int temp1 = atoi(argv[1]);
			
			write(writefd, &temp1, sizeof(temp1)); // ghi doi so vao fifo1
			int temp2;

			read(readfd, &temp2, sizeof(temp2)); // doc kq tu fifo2
			printf("Parent read data from child: %d\n", temp2);
			
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
