#include <vector>
#include <iostream>
#include <thread>
#include "TS_List.h"

using namespace std;

class A{};

int main(){
    TS_List<A> list;

    vector<thread> vv;

    for(int i = 0; i < 100; ++i){
        vv.push_back(thread([&] () mutable {
            list.push_back(A());
            cout<<list.size();
        }));
    }

    for(auto &t : vv){
        t.join();
    }

}