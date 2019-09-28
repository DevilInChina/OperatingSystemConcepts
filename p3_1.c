#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <wait.h>

#define MAX_LEN 80
#define BUFFER_SIZE 256

char *args[MAX_LEN/2+1];
char buff[BUFFER_SIZE];
char cmds[BUFFER_SIZE];
#define MALLOC(T,size)\
(T*)malloc(sizeof(T)*(size))
#define MODBUFF(x)\
((x)+BUFFER_SIZE)%BUFFER_SIZE
struct {
    int low,top;
    int full;
    char buff[BUFFER_SIZE][BUFFER_SIZE];
}HisStack;
#define EMPTY_ERR \
printf("Not much history");\
fflush(stdout)
void init(){
    HisStack.low = HisStack.top = HisStack.full = 0;
}
void push(){
    ++HisStack.top;
    if(HisStack.full){
        ++HisStack.low;
    }
    HisStack.top%=BUFFER_SIZE;
    HisStack.low%=BUFFER_SIZE;
    if(HisStack.top == HisStack.low){
        HisStack.full = 1;
    }
}
int empty(){
    return !HisStack.full && HisStack.low==HisStack.top;
}
void pop(){
    if(HisStack.full){
        HisStack.full = 0;
    }
    --HisStack.top;
    HisStack.top = MODBUFF(HisStack.top);
}
int StackSize(){
    if(empty())return 0;
    else{
        return MODBUFF(HisStack.top - HisStack.low-1)+1;
    }
}
char *lastOrder(int k){
    if(StackSize()<k)return NULL;
    else{
        return HisStack.buff[MODBUFF(HisStack.top-k)];
    }
}

int readBuff(char *Buff){

    scanf("%[^\n]",Buff);
    getchar();
    if(Buff[0]=='!'){
        if(buff[1]=='!'){
            if(empty()){
                EMPTY_ERR;
            }else{
                strcpy(Buff,lastOrder(1));
                return  0;
            }
        }else {
            int t = strtol((Buff + 1),NULL, 10);
            char *s =NULL;
            if(t)s = lastOrder(t);
            if (s == NULL) {
                EMPTY_ERR;
            } else {
                strcpy(buff, s);
                return  0;
            }
        }
    }
    return 1;
}

int main(){
    int should_run = 1;
    init();

    FILE*fp = fopen("history.txt","r");
    if(fp==NULL){
        fp = fopen("history.txt","w");
        fclose(fp);
    }else{
        while ((EOF!=fscanf(fp,"%[^\n]",HisStack.buff[HisStack.top]))){
            fgetc(fp);
            push();
        }
    }

    while (should_run){
        printf("osh>");
        fflush(stdout);

        int Push = readBuff(buff);
       // puts(buff);
        int cnt = 0;
        int size = 0;
        do {
            while (buff[size] && sscanf(buff + size, "%s", cmds)) {
                int len = strlen(cmds);
                args[cnt] = MALLOC(char, len + 1);
                size += len;
                strcpy(args[cnt], cmds);
                ++cnt;
                while (isblank(buff[size]))++size;
            }
        }while(cnt==0);///separte cmds
        int check = (args[cnt-1][0]=='&');
        if(Push) {///not begin with !
            strcpy(HisStack.buff[HisStack.top], buff);
            push();
        }
        if(strcmp(buff,"history")==0){
            int k = StackSize();
            while (k){
                printf("%d %s\n",k,lastOrder(k));
                --k;
            }
        }else if(strcmp("exit",buff)==0){
            should_run = 0;
        }else {
            if (check) {
                free(args[cnt - 1]);
                args[cnt - 1] = NULL;
                --cnt;
            }
            pid_t pids = fork();

            if (pids == 0) {///sons
                execvp(args[0], args);
                return 0;
            } else if (pids > 0) {///fa
                int ks = -1;
                if (!check)
                    while (ks == -1) {
                        wait(&ks);
                    }

            } else {
                puts("error when fork();");
                fflush(stdout);
                exit(1);
            }
        }
        for(int i =0 ; i < cnt ; ++i)free(args[i]);
    }
    fp = fopen("history.txt","w");
    int k = StackSize();
    while (k){
        fprintf(fp,"%s\n",lastOrder(k));
        --k;
    }
}
