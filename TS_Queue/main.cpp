#include <iostream>
#include <thread>

#include "TS_Queue.h"

using namespace std;

class A{};

int main(){
    TS_Queue<A> q;

    vector<thread> vv;

    for(int i = 0; i < 100; ++i){
        vv.push_back(thread([&] () mutable {
            q.push(A());
            cout<<q.size();
        }));
    }

    for(auto &t : vv){
        t.join();
    }

}