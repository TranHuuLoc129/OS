#include <stdio.h>
#include <unistd.h>
#include <string.h>



int main(int argc, char **argv){

	
	int fp1[2];
	
	char buffer[256];

	int pid;

	if(argc<2){
	    printf("Doi so thieu.\n");
	    return -1;
	}
	
	if(pipe(fp1) == 0){
	   
	    pid = fork();
	    
	    if(pid<0){
			printf("Fork failed.");
			return -1;
	    }
	    else{
			if(pid == 0){ // con
				close(fp1[1]);
				while(read(fp1[0], buffer, sizeof(buffer))>0){
					printf("Read from parents: %s\n", buffer);
				}
				close(fp1[0]);
			}
			else{
				if(pid> 0){ // cha
					int i;
					for(i = 1; i<argc; i++){
						close(fp1[0]);
						write(fp1[1], argv[i], strlen(argv[i]));
						printf("Data fromm parents: %s\n", argv[i]);
					}
					close(fp1[1]);
					wait(NULL);
				}
			}

	    }
		    
	}
 	else{
	     printf("Pipe failed\n");
	     return -2;
	}

	return 0;
}
