#include <iostream>
#include <atomic>
#include <vector>
#include <thread>
#include <algorithm>
#include <functional>
using namespace std;

class spinlock
{
private:
    atomic_flag flag = ATOMIC_FLAG_INIT;

public:
    void lock()
    {
        while (flag.test_and_set(memory_order_acquire))
        {
        }
    }
    void unlock()
    {
        flag.clear(memory_order_release);
    }
};
spinlock m;
void f(int n)
{
    for (int i = 0; i < 10; ++i)
    {
        m.lock();
        std::cout << "Output from thread " << n << '\n';
        m.unlock();
    }
}

int main()
{
    vector<thread> v;
    for (int i = 0; i < 10; i++)
    {
        v.emplace_back(f, i);
    }
    for_each(v.begin(), v.end(), mem_fn(&thread::join));
}