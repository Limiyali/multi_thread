#include <iostream>
#include <cassert>
#include <thread>
#include <semaphore>
#include <barrier>
using namespace std;
class A
{
public:
    void f()
    {
        barrier sync_point{
            3, [&]() noexcept
            { ++i_; }};
        for (auto &x : tasks_)
        {
            x = thread([&]
                       {
                           cout << 1;
                           sync_point.arrive_and_wait();
                           assert(i_ == 1);
                           cout << 2;
                           sync_point.arrive_and_wait();
                           assert(i_ == 2);
                           cout << 3;
                           sync_point.arrive_and_wait();
                           assert(i_ == 3);
                       });
        }
        thread t([&]
                 {
                     cout << 1;
                     sync_point.arrive_and_wait();
                     assert(i_ == 1);
                     cout << 2;
                     sync_point.arrive_and_drop();
                    //  assert(i_ == 2);
                    //  cout << 3;
                    //  sync_point.arrive_and_wait();
                    //  assert(i_ == 3);
                 });
        for (auto &x : tasks_)
            x.join();
        t.join();
    }

private:
    thread tasks_[2] = {};
    int i_ = 0;
};

int main()
{
    A a;
    a.f();
}