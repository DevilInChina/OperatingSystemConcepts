#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>
typedef long long ll;
ll transNumber(char *str){
    ll ret = 0;
    while (*str){
        if(isdigit(*str)){
            ret = ret*10+(*str-'0');
        }else return -1;
        ++str;
    }
    return ret;
}
void print(const char *str){
    printf("%s",str);
    fflush(stdout);
}
int main(int argc,char **argv) {
    pid_t pid = fork();
    if(pid<0){

    }else if(pid==0){
        ll k = transNumber(argv[argc-1]);
       // printf("%lld\n",k);
        if(k<0) {
            print("Wrong parameter, please try again.\n");
            return 0;
        }
        print("Son process begin.\n");

        printf("%lld ",k);
        while (k!=1) {
            fflush(stdout);
            if (k & 1)k = k * 3 + 1;
            else {
                k >>= 1;
            }
            printf("%lld ", k);
        }
        print("\n");
    }else{
        wait(NULL);
        print("Son process done, return to father process\n");
    }
}
