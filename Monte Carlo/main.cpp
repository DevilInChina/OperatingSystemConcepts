#include <bits/stdc++.h>
#include <sstream>
#include <omp.h>
#include <sys/time.h>

using namespace std;
#define SIZE 10000000
int mod = 100000;
unsigned x , y , z;

unsigned rng61() {
    unsigned t;
    x ^= x << 16;
    x ^= x >> 5;
    x ^= x << 1;
    t = x;
    x = y;
    y = z;
    z = t ^ x ^ y;
    return z;
}


#define Grand(n) (((rng61()%(n))))

typedef pair<int,int> pd;
double pi;
bool fdone = false;
inline bool check(double a,double b){
    return a*a+b*b<1.0;
}
struct parameter{
    long long *cnt;
    int tot;
    bool done;
    unsigned x,y,z;
    parameter(int tots,long long *c, unsigned time){
        tot = tots;
        cnt = c;
        *cnt = 0;
        done = false;
        x = time%100;
        y = time/100%100;
        z = (x*x+y*y)/2;
    }
    unsigned rng61() {
        unsigned t;
        x ^= x << 16;
        x ^= x >> 5;
        x ^= x << 1;
        t = x;
        x = y;
        y = z;
        z = t ^ x ^ y;
        return z%mod;
    }
};
std::mutex locker;
void getPi(parameter *p){
    srand(time(nullptr));
    for(int i = 0 ; i < p->tot ; ++i){
        double a,b;
       // locker.lock();
       /*
       a = Grand(mod)*1.0/mod;
       b = Grand(mod)*1.0/mod;
        */
        a = p->rng61()*1.0/mod;
        b = p->rng61()*1.0/mod;
     //   locker.unlock();
        if(check(a,b)){
            ++*p->cnt;
        }
    }
    p->done = true;
}

bool check(parameter**p,int The){

    for(int i = 0 ; i < The ; ++i){
        if(!p[i]->done)return false;
    }
    return true;
}
int main(int argc,char *argv[]) {
    long long tot;
    int The;
    srand(time(nullptr));
    if (argc != 3) {
        cout << "Need two parameter.(Two numbers)";
        return 0;
    }
    istringstream is(argv[1]);
    istringstream ss(argv[2]);
    is>>The;
    ss>>tot;
    mod = tot/2;
    auto *cnt = new long long[The];
    auto res = new parameter*[The];
    auto p = new thread*[The];
    timeval bef,enp;
    timeval temp;
    gettimeofday(&bef, nullptr);
    for(int i = 0 ;i  < The ; ++i){

        gettimeofday(&temp, nullptr);
        res[i] = new parameter(tot,cnt+i,temp.tv_usec);
        p[i]=new thread(getPi,res[i]);
    }

    x = bef.tv_usec%100;
    y = bef.tv_usec/100%100;
    z = bef.tv_usec%100+7;
    for(int i = 0 ;i  < The ; ++i){
        p[i]->join();
    }

    while (!check(res,The));

    gettimeofday(&enp, nullptr);
    long long k = tot*The;
    long long cnts = 0;
    for(int i = 0 ;i  < The ; ++i){
        cnts+=cnt[i];
    }
    for(int i = 0 ; i < The ; ++i){
        delete res[i];
        delete p[i];
    }
    delete []p;
    delete []res;
    delete []cnt;
    printf("%.9f %.5f\n",4.0*cnts/k,1000.0*(enp.tv_sec-bef.tv_sec)+(enp.tv_usec-bef.tv_usec)/1000.0);
}