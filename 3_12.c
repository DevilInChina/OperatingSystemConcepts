
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#define N 5
int num[N] = {1,2,3,4,5};
int main(){
       	pid_t  pid = fork();
	if(pid<0){
	        fprintf(stderr,"Fork Failed");
	        return 1;
	}else if(pid){/// father
            sleep(100000);
//            wait(NULL);
        }else{}
}
