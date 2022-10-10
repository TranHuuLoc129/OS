#include <stdio.h>
#include <unistd.h>
#include <string.h>


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

	
	int fp1[2], fp2[2];
	
	int buffer;

	int pid;

	if(argc<2){
	    printf("Doi so thieu.\n");
	    return -1;
	}
	
	if(pipe(fp1) == 0 && pipe(fp2) == 0 ){
	   
	    pid = fork();
	    
	    if(pid<0){
		printf("Fork failed.");
		return -1;
	    }
	    else{
		if(pid == 0){ // con // lay so => tinh giai thua => ghi
		    close(fp1[1]); // dong ghi => doc
		    read(fp1[0], &buffer, sizeof(buffer));
		    printf("Read data from parent: %d\n",buffer); // doc doi so tu fp1
		    close(fp1[0]);		    
			
		    int kq = giaithua(buffer);
		    printf("Data send to parent: %d\n", kq);
		    close(fp2[0]); // dong doc => ghi
		    write(fp2[1], &kq, sizeof(kq)); // ghi kq vao fp2
		    close(fp2[1]);
		}
		else{
		   if(pid> 0){ // cha // ghi doi so => doc kq tu con => xuat ra
		        close(fp1[0]); // dong doc => ghi
		        printf("Data from parent: %s\n", argv[1]);
		        int temp;
			temp = atoi(argv[1]);
			write(fp1[1], &temp, sizeof(temp)); //ghi doi so vao fp1
			close(fp1[1]);

			int temp2;
			close(fp2[1]); // dong ghi => doc
			read(fp2[0], &temp2, sizeof(temp2)); // doc kq tu fp2
			printf("Data get from child: %d\n", temp2);
			close(fp2[0]);
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
