#include <iostream>
#include <unistd.h>
#include <thread>
#include <set>
#include <cstring>
#include <mutex>
#include "3_13.cpp"
RB_Tree<int>pids;
#define MAX_PID 5000
#define MIN_PID 300
using namespace std;

#define MaxThread 100
std::mutex locker;

int cnt = 0;
int relcnt = 0;
int allocate_map(){
    pids.clear();
    for(int i = MIN_PID; i  <= MAX_PID ; ++i){
        pids.insert(i);
    }
    srand(time(nullptr));
    cnt = relcnt = 0;
    return 1;
}

int allocate_pid() {
    if (pids.empty()) {
        return -1;
    }
    int ret = -1;
    while (!locker.try_lock());
    ret = *pids.begin();
    pids.erase(ret);
    ++cnt;
    locker.unlock();
    return ret;
}
void release_pid(int pid) {

    if (pid >= MIN_PID && pid <= MAX_PID) {
        while (!locker.try_lock());
        pids.insert(pid);
        ++relcnt;
        locker.unlock();
    }

}


void process(int stat){
    int k = allocate_pid();
    if(k==-1){
        cout<<"Allocate False"<<endl;
    }else{
        cout<<"PID "<<k<<" succeed allocated\n"<<endl;
        sleep(1);
        release_pid(k);
        cout<<"PID "<<k<<" succeed released\n"<<endl;
    }
}


int main() {

    allocate_map();
    thread Thread[MaxThread];

    for(int i = 0;i < MaxThread ; ++i){
        Thread[i] = thread(process,i);
    }
    for(int i = 0;i < MaxThread ; ++i){
        Thread[i].join();
    }

    cout<<endl<<"total allocate:"<<cnt<<endl<<"total released:"<<relcnt<<endl;
}