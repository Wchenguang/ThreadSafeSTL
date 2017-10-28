#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

#include "TS_Stack.h"

class A{};

using namespace std;

int main()
{

    TS_Stack<A> ss;
    std::mutex m;
    vector<thread> threads;
    for (int i = 0; i < 100; ++i)
    {
        threads.push_back(thread([&]()
             {
                 ss.push(A());
                 ss.pop();
                 m.lock();
                 std::cout<<"pushed"<<std::endl;
                 std::cout<<"size ";
                 m.unlock();
                 std::cout<<ss.size();
             }));
    }

    for (auto &thread : threads)
    {
        thread.join();
    }


    return 0;
}
