#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
#include <shared_mutex>
using namespace std;

class A
{
public:
    void f()
    {
        m_.lock();
        m_.unlock();
    }

    void g()
    {
        m_.lock();
        f();
        m_.unlock();
    }

private:
    recursive_mutex m_;
};

int main()
{
    A{}.g(); // OK
}
