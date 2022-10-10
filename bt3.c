#include <stdio.h>
#include <unistd.h>
#include <string.h>




int main(int argc, char **argv){

	
	int fp1[2], fp2[2];
	
	char buffer[20];

	int pid;

	if(argc<4){
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
		    read(fp1[0],buffer, sizeof(buffer));

		    printf("Read data from parent: %s\n", buffer); // doc doi so tu fp1
		    close(fp1[0]);		    
			
		    int kq,a,b;
		    char tmp,c;
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

		    printf("Data send to parent: %d\n", kq);
		    close(fp2[0]); // dong doc => ghi
		    write(fp2[1], &kq, sizeof(kq)); // ghi kq vao fp2
		    close(fp2[1]);
		}
		else{
		   if(pid> 0){ // cha // ghi so va dau => doc kq tu con => xuat ra
		        close(fp1[0]); // dong doc => ghi
			char str[40]="";
			int i;
			for(i = 1; i<argc-1; i++ ){
				strcat(str,argv[i]);
				strcat(str," ");
			}

			strcat(str,argv[i]);

			printf("Data from parent: %s \n", str);
			write(fp1[1],str, strlen(str)); //ghi doi so vao fp1
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
