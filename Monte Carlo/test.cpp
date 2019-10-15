#include <bits/stdc++.h>
using namespace std;
int main(int argc,char *argv[]){

    istringstream is(argv[1]);
    int k;
    is>>k;
    string s = "./main ";
    s+=string(argv[2]);
    long long The,times;
    istringstream ith(argv[2]);
    ith>>The;
    istringstream itime(argv[3]);
    itime>>times;
    s+=' ';
    s+=string(argv[3]);
    s+=">>";
    s+=string(argv[4]);

    for(int i = 0 ; i < k ; ++i){
      //  cerr<<i<<'\n';
        system(s.c_str());////调用程序计算Pi

    }
    freopen(argv[4],"r",stdin);
    vector<double>a,t;

    double ss,sb,sum = 0,avs = 0,avt =0,ts = 0;
    while (cin>>ss>>sb){
        a.push_back(ss);
        t.push_back(sb);
        sum+=ss;
        avt+=sb;
    }
    sum/=k;
    avt/=k;
    for(auto c:a){
        avs+=(c-sum)*(c-sum);
    }
    for(auto c:t){
        ts+=(c-avt)*(c-avt);
    }
    avs = sqrt(avs/k);
    ts = sqrt(ts/k);
    printf("Pi:%.9f s:%.5f Time:%.5f Ts:%.5f Pms:%.5f\n",sum,avs,avt,ts,The/avt*times);
}