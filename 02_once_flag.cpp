#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
#include <shared_mutex>
using namespace std;

class A
{
public:
    void delay()
    {
        init = true;
        this_thread::sleep_for(chrono::seconds(1));
        flag = true;
    }
    void f()
    {
        if (flag)
            cout << " A::f()\n";
        else
            cout << " error\n";
    }
    bool init = false;
    bool flag = false;
};

A a;
shared_ptr<A> p;
once_flag flag;
mutex m;

void init()
{
    call_once(flag, [&]
              { a.delay(); });
    a.f();
}

void error_init()
{
    if (!a.init)
    { // 未上锁，其他线程可能在执行 #1，则此时 p 不为空
        std::lock_guard<std::mutex> l(m);
        if (!a.init)
        {
            a.delay();
        }
    }
    a.f(); // p 可能指向一块还未构造实例的内存，从而崩溃
}

int main()
{
    thread t(init);
    thread t2(init);
    t.join();
    t2.join();
}