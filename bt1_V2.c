#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/errno.h>
#include <sys/stat.h>

#define FIFO1 "/tmp/ff.1"
#define FIFO2 "/tmp/ff.1"
#define PM 0666
extern int errno;
#define PIPE_BUF 4096

int main(int argc, char **argv){

	char s1[PIPE_BUF];
	
	int childpid, readfd, writefd;
	
	if((mknod(FIFO1, S_IFIFO | PM,0) <0) && (errno != EEXIST)){
		printf("Fail to create FIFO 1. Aborted.\n");
		return -1;
	}

	childpid = fork();

	if(childpid == 0){ // con

		//mo dau doc fifo1
		if((readfd = open(FIFO1, 0)) < 0){
			perror("Children cannot open read FIFO.\n");
		}

		 while(read(readfd, s1, sizeof(s1))>0){
			printf("Read from parents: %s\n", s1);
		 }
		close(readfd);
		exit(0);
	}
	else{
		if(childpid > 0){ // cha // mo dau ghi fifo1
			if((writefd = open(FIFO1, 1)) < 0){
				perror("Parent cannot open write FIFO.\n");
			}

			int i;
			for(i = 1; i<argc; i++){
			        write(writefd, argv[i], strlen(argv[i]));
			        printf("Data from parents: %s\n", argv[i]);

		        }
			
			close(writefd);
			if(unlink(FIFO1)<0){
				perror("Cannot remove FIFO1.\n");
			}
			
			wait(NULL);
			return 1;
		}
		else{
			printf("Fork failed\n");
			return -1;
		}	
	}
	


	return 0;
}
