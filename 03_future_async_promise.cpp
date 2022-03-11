#include <iostream>
#include <future>
using namespace std;

class A
{
public:
    int f(int i)
    {
        this_thread::sleep_for(chrono::seconds(1));
        return i;
    }
};

void t_fu()
{
    A a;
    future<int> res = async(launch::deferred, &A::f, &a, 1);
    this_thread::sleep_for(chrono::seconds(2));
    cout << " start async\n";
    cout << res.get() << endl;
}

void t_pa()
{
    packaged_task<int(int)> task([](int i)
                                 {
                                     this_thread::sleep_for(chrono::seconds(1));
                                     return i;
                                 });
    task(1);
    this_thread::sleep_for(chrono::seconds(2));
    cout << "stop sleeping\n";
    auto res = task.get_future();
    cout << res.get() << endl;
}

void t_pr()
{
    promise<int> ps;
    ps.set_value(1);
    auto res = ps.get_future();
    cout << res.get() << endl;
}

int main()
{
    t_pr();
}