#include <iostream>
#include <string>
#include <stack>
#include <mutex>
#include <thread>
#include <memory>
using std::cout;
using std::endl;
using std::ref;

class A
{
public:
    explicit A(int r) : x(r) {}
    int x;
    std::mutex m;
};

void f(A &from, A &to)
{
    // std::scoped_lock l(from.m,to.m);
    std::lock(from.m, to.m);
    std::lock_guard l1(from.m, std::adopt_lock);
    // std::lock(from.m);
    cout << &from.m << " to:" << &to.m << endl;
    cout << " lock " << from.x << endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::lock_guard l2(to.m, std::adopt_lock);
    cout << " lock " << to.x << endl;
    from.x--;
    to.x++;
}

int main()
{
    A x(30);
    A y(40);
    std::thread t1(f, ref(x), ref(y));
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::thread t2(f, ref(y), ref(x));
    t1.join();
    t2.join();
}