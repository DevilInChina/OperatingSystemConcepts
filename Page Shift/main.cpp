#include <bits/stdc++.h>
#include <sys/time.h>

using namespace std;
#define MaxPage 10
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

class adapter{
public:
    adapter(int *s){
        nextTimeInUse = s;
    }
    int *nextTimeInUse;
    virtual int shift(int time,int n) = 0;
    virtual void push(int time ,int n) = 0;
    virtual void info_change(int time, int n) = 0;
    virtual void show() = 0;
    virtual ~adapter()= default;
};

class FIFO:public adapter{
    queue<int>qq;
public:
    FIFO(int *input):adapter(input){}
    int shift(int time,int n){
        int ret = qq.front();
        qq.pop();
        push(time,n);
        return ret;
    }
    void info_change(int time, int n){}
    void push(int time,int n){
        qq.push(n);
    }
    void show(){
        cout<<qq.size()<<":FIFO"<<endl;
    }
    ~FIFO()= default;
};
class LRU :public adapter{
    set<pair<int,int> > qq;
    int LastUseTime[MaxPage];
public:
    explicit LRU(int *input):adapter(input){
        memset(LastUseTime,-1, sizeof(LastUseTime));
    }
    int shift(int time,int n){
        int ret = qq.begin()->second;
        qq.erase(qq.begin());
        push(time,n);
        return ret;
    }
    void push(int time,int n){
        LastUseTime[n] = time;
        qq.insert(make_pair(time,n));
        //cout<<qq.size()<<endl;
    }
    void info_change(int time, int n){
        auto it = qq.find(make_pair(LastUseTime[n],n));

        if(it!=qq.end()) {
            qq.erase(it);
            push(time, n);
        }
        //cout<<qq.size()<<endl;
    }
    void show() {
        cout << "LRU:" << qq.size() << ":";
        for (auto it:qq) {
            cout << it.second << ' ';
        }
        cout << endl;
    }
    ~LRU()= default;
};
class OPT :public adapter{
    set<pair<int,int> >qq;

public:

    explicit OPT(int *input):adapter(input){}

    int shift(int time,int n){
        auto it = qq.end();
        --it;
        int ret = it->second;
      //  cout<<it->first<<' '<<it->second<<endl;
        qq.erase(it);
        push(time,n);
        return ret;
    }
    void push(int time,int n){
        int i;
        qq.insert(make_pair(nextTimeInUse[time],n));
    }
    void info_change(int time, int n){
        auto it = qq.find(make_pair(time,n));

       // cout<<time<<' '<<n<<' '<<it->second<<endl;
        if(it!=qq.end()) {
            qq.erase(it);
            push(time, n);
        }else{

        }
    }
    void show(){    cout << "OPT:" << qq.size() << ":";
        for (auto it:qq) {
            cout << it.second << ' ';
        }
        cout << endl;

    }
    ~OPT()= default;
};

enum Command{
    FIFO_M,LRU_M,OPT_M
};
enum RET_INFO{
    IN_Page,ERROR,Need_Shift
};
class Shfit {
    bool inPage[MaxPage];
    int totalPage, curPage;
    adapter *s;
public:
    explicit Shfit(int n, int k, int *input) : totalPage(n) {
        memset(inPage, 0, sizeof(inPage));
        curPage = 0;
        switch (k) {
            case FIFO_M:
                s = new FIFO(input);
                break;
            case LRU_M:
                s = new LRU(input);
                break;
            case OPT_M:
                s = new OPT(input);
                break;
            default:
                break;
        }
    }

    RET_INFO push(int time, int num) {
        ///  cout<<num<<":";

        if (num >= MaxPage || num < 0)return ERROR;///don't have this page
        if (inPage[num]) {
            s->info_change(time, num);

            return IN_Page;/// is in page
        } else {
            if (curPage == totalPage) {
                int k = s->shift(time, num);
                inPage[k] = false;

                inPage[num] = true;
                return Need_Shift;
            } else {
                s->push(time, num);
                inPage[num] = true;
                ++curPage;
                return Need_Shift;
            }
        }
    }

    ~Shfit() {
        delete s;
    }
};

int main(int argc,char *argv[]){
    RngNumber s;
    int tot_time=atoi(argv[1]);
    int tot = atoi(argv[2]);
    int *input = new int[tot_time+1];
    int lastUse[MaxPage] ;
    memset(lastUse,-1, sizeof(lastUse));

    int *nextInUse = new int [tot_time+1];
    memset(nextInUse,-1, sizeof(int)*(tot_time+1));
    for(int i = 0 ; i < tot_time ; ++i){
        input[i] = s._get(MaxPage);
        //cout<<input[i]<<' ';
    }
    //cout<<endl;
    for(int i = 0 ; i < tot_time ; ++i){
        if(lastUse[input[i]]==-1)lastUse[input[i]] = i;
        else {
            nextInUse[lastUse[input[i]]] = i;
            lastUse[input[i]] = i;
        }
    }

    for(int i = 0 ; i < tot_time    ; ++i){
        if(nextInUse[i]==-1)nextInUse[i] = tot_time;
        //cout<<nextInUse[i]<<' ';
    }
    cout<<endl;
    int cnt[3] = {0};
    input[tot_time] = -1;
    string opt[3] = {"FIFO","LRU","OPT"};
    cout<<"Method \tPage fault rate"<<endl;
    for(int k = 0 ; k < 3 ; ++k) {
        Shfit a(tot, k, nextInUse);
        for (int i = 0; i < tot_time; ++i) {
            switch (a.push(i, input[i])) {
                case Need_Shift:
                    ++cnt[k];
                    break;
                case IN_Page:
                    break;
                default:
                    cout<<"ERROR"<<endl;
                    exit(0);
            }
        }
        cout<<opt[k]<<" \t"<<100.0*cnt[k]/tot_time<<"%"<<endl;
    }

    delete []nextInUse;
    delete []input;

}
