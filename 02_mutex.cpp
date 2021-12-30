#include <iostream>
#include <list>
#include <mutex>
#include <chrono>
#include <thread>
#include "utils/pt.h"
using namespace std;

list<int> l;
mutex m;

int write(int rhs){
    lock_guard<mutex> g(m);
    this_thread::sleep_for(chrono::seconds(2));
    l.emplace_back(rhs);
    return 0;
}

int read(){
    lock_guard<mutex> g(m);
    this_thread::sleep_for(chrono::seconds(1));
    shuxin::pt(l);
    return 0;
}

int main(){
    thread t(write,2);
    thread t2(read);
    t.join();
    t2.join();
}