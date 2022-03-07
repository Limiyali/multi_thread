#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
#include <shared_mutex>
using namespace std;

class A
{
public:
    f() { cout << " A::f()\n"; }
};

shared_ptr<A> p;
once_flag flag;