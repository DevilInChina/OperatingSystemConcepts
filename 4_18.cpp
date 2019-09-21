#include <bits/stdc++.h>
#include <omp.h>
using namespace std;
#define SIZE 10000000
const int mod = 10007;
int Grand(int n){///+-n
    return (rand()%(n<<1))-(n);
}
typedef pair<int,int> pd;
bool done = false;
pd ret[SIZE];
void randP(){
#pragma omp parallel for
    for(int i = 0 ; i < SIZE ; ++i){
        ret[i].first = Grand(mod);
        ret[i].second=Grand(mod);
    }
    done = true;
}
double pi;
bool fdone = false;
inline bool check(double a,double b){
    //cerr<<a<<' '<<b<<'\n';
    return a*a+b*b<1.0;
}
void getPi(double *pi){
    *pi = 0;

    srand(time(nullptr));
    thread p(randP);
    p.join();
    while (!done);///wait for it
    int cnt = 0;

    double a,b;
    for(auto it:ret){
        a = 1.0*it.first/mod;
        b = 1.0*it.second/mod;
        if(check(a,b))++cnt;
    }
    *pi = 1.0*cnt/SIZE;
    *pi*=4;
    fdone = true;
}
int main(){
    double res;
    thread p(getPi,&res);
    p.join();

    while (!fdone);
    printf("%.5f\n",res);
}