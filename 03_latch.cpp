#include <latch>
#include <string>
#include <iostream>
#include <thread>
using namespace std;

class A
{
public:
    void f()
    {
        for (auto &x : data_)
        {
            x.t = jthread([&]
                          {
                              this_thread::sleep_for(chrono::seconds(1));
                              x.s += x.s;
                              done_.count_down();
                          });
        }
        done_.wait();
        cout << done_.try_wait() << endl;
        for (auto &x : data_)
            cout << x.s << endl;
    }

private:
    struct
    {
        string s;
        jthread t;
    } data_[3] = {{"a"}, {"b"}, {"c"}};
    latch done_{3};
};

int main()
{
    A a;
    a.f();
}