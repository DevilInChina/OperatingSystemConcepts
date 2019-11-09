#include <bits/stdc++.h>
#include <sys/time.h>
#include <zconf.h>

using namespace std;
const int maxResourceInNeed = 10;
#define Need first
#define Have second
enum ProceStatus{
    NOTINUSE,BLOCK,ALLRIGHT
};

class RngNumber {
public:
    RngNumber() {
        timeval t1;
        gettimeofday(&t1, nullptr);
        x = t1.tv_usec % 1000;
        y = t1.tv_sec % 1000;
        z = (x * x + y * y) / 1000 % 1000;
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
        return z;
    }
    unsigned _get(int n){
        return rng61()%n;
    }
private:
    unsigned x, y, z;
};

class Customer{
public:
    explicit Customer(int ids,const vector<int>&Resourses):Rng(),id(ids){
        int n = Rng._get(Resourses.size())+1;
        for(int i = 0 ; i < n ; ++i){
            int R,Q;//// resource R need Q;
            R = Rng._get(Resourses.size());
            Q = Rng._get(Resourses[R])+1;
            condition[R] = make_pair(Q,0);
        }
        cond = NOTINUSE;
    }

    Customer(const Customer&Cp):id(Cp.id),condition(Cp.condition),cond(Cp.cond) {
        Rng = Cp.Rng;
    }

    int getRng(int n){
        return Rng._get(n)+1;
    }

    bool aquire(){/// request some resources
        resNeed.clear();
        int cnt = 0;
        ///cout<<id<<" is aquir "<<condition.size()<<endl;
        for(auto it:condition) {
            int k;
            if (it.second.first > it.second.second) {
                k = it.second.first - it.second.second;
                k = Rng._get(k) + 1;

            } else {
                k = 0;
            }
            if (k) {
                resNeed.emplace_back(make_pair(it.first, k));
                ++cnt;
            }
        }
        return cnt;
    }

    int getId(){
        return id;
    }

    string showResoures(){
        string ret = "[";
        for(auto it:resNeed){
            ret.append ("[" + to_string(it.first)+","+to_string(it.second)+"],");
        }
        ret.pop_back();
        ret.push_back(']');

        return ret;
    }

private:
    typedef pair<int,int>pd;
    ProceStatus cond;
    map<int,pd>condition;
    vector<pd>resNeed;
    RngNumber Rng;
    int id;
    friend class Server;
};

class Server{
public:
    explicit Server(char **argv,ostream & fout):Rng(),Infoout(fout){
        int R = atoi(argv[0]);
        for(int i = 1 ; i <= R ; ++i){
            resourse.push_back(atoi(argv[i]));
            ResourceLock.push_back(OutstreamLocker);
            pthread_mutex_init(&ResourceLock.back(), nullptr);
        }
        pthread_mutex_init(&OutstreamLocker, nullptr);
        pthread_mutex_init(&OuterLocker, nullptr);
        printInfo("successful init");
    };

    int resAlloc(Customer&Per){
        Lock(Per);
        map<int ,int>InNeed;
        for(auto it:Per.condition){
            InNeed[it.first] = it.second.Need-it.second.Have;
        }
        bool CanAlloc = true;
        for(auto it :InNeed){
            if(resourse[it.first]<it.second){
                CanAlloc = false;
                break;
            }
        }

        if(CanAlloc){
            for(auto it :Per.resNeed){
                resourse[it.first]-=it.second;
                Per.condition[it.first].Have+=it.second;
            }
            if(Per.cond==BLOCK) {
                printInfo("user " + to_string(Per.id) + " was Blocked and successfully allocated now." + Per.showResoures());
            }else{
                printInfo("user " + to_string(Per.id) + " successfully allocated." + Per.showResoures());
            }
            Unlock(Per);

            Per.cond = ALLRIGHT;
            return true;
        }

        if(Per.cond!=BLOCK)/// only print once
            printInfo("user "+to_string(Per.id)+" FAILED to allocated."+Per.showResoures());

        Unlock(Per);
        Per.cond = BLOCK;
        return false;
    }
    int resFree(Customer&Per){
        Lock(Per);
        for(auto &it :Per.condition){
            resourse[it.first]+=it.second.Have;
            it.second.Have = 0;
        }
        printInfo("user "+to_string(Per.id)+" free all resourses.");
        Unlock(Per);
        return true;
    }
    const vector<int>&getResourses(){
        return resourse;
    }

private:

    void printInfo(const string &s){
        time_t t1;
        time(&t1);
        string ct(ctime(&t1));

        pthread_mutex_lock(&OutstreamLocker);
        Infoout<<ct<<"["<<s<<"]\n";
        Infoout<<'[';
        for(auto it:resourse){
            Infoout<<it<<' ';
        }
        Infoout<<"]\n";
        pthread_mutex_unlock(&OutstreamLocker);

    }

    bool Lock(const Customer&Per){
    //    pthread_mutex_lock(&OuterLocker);
        for(auto it : Per.condition){
            pthread_mutex_lock(&ResourceLock[it.first]);
        }
        //pthread_mutex_unlock(&OuterLocker);
        return true;
    }

    bool Unlock(const Customer&Per){
        for(auto it : Per.condition){
            pthread_mutex_unlock(&ResourceLock[it.first]);
        }
    //    pthread_mutex_unlock(&OuterLocker);
        return true;
    }

    vector<int>resourse;

    vector<pthread_mutex_t>ResourceLock;
    pthread_mutex_t OutstreamLocker,OuterLocker;

    ostream &Infoout;

    RngNumber Rng;
};


struct Parameter{
    Customer Per;
    Server&Ser;
    explicit Parameter(const Customer &per,Server &ser):Per(per),Ser(ser){};

    Parameter(const Parameter&Cp):Per(Cp.Per),Ser(Cp.Ser){};

};

void deal(Parameter *param){
    int tot = param->Per.getRng(5);///allocate 1-5 times;
    for(int i = 0 ; i < tot ; ++i) {
        if(param->Per.aquire()) {
            while(!param->Ser.resAlloc(param->Per)){}
            sleep(1);
        }else{
        }
    }

    param->Ser.resFree(param->Per);

}
int main(int argc,char *argv[]){
    int k = atoi(argv[1]);
    ofstream fou("data.out");

    Server ser(argv+2,fou);


    vector<Parameter>params;
    vector<thread>Threads;

    for(int i = 0 ; i < k ; ++i){
        params.emplace_back(Parameter(Customer(i,ser.getResourses()),ser));
    }


    for(int i = 0 ; i < k ; ++i){
        Threads.emplace_back(thread(deal,&params[i]));
    }
    for(int i = 0 ;i  < k ; ++i){
        Threads[i].join();
    }
}/*
 *
 *
 *

 */