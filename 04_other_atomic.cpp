#include <iostream>
#include <atomic>
#include <thread>
#include <vector>
using namespace std;

class A{

};

class B{
    virtual void a();
};

int main()
{
    cout << atomic<bool>{}.is_lock_free() << endl;
    std::atomic<bool> x(true);
    x = false;
    bool y = x.load(std::memory_order_acquire); // 读取 x 值返回给y
    x.store(true);                              // x 写为 true
    y = x.exchange(false,
                   std::memory_order_acq_rel); // x 用 false 替换，并返回旧值给 y
    bool expected = false;                     // 期望值
    while (!x.compare_exchange_weak(expected, true) && !expected)
    {
        cout << " maybe\n";
    }
    int a[5] = {1, 2, 3, 4, 5};
    atomic<int *> p(a);
    cout << *p << endl;
    int *target = p.fetch_add(2);
    cout << *target << " " << *p << endl;
    p -= 1;
    cout << *p << endl;
    atomic_int i(3);
    i.fetch_and(2);
    cout << i << endl;
    cout <<"######################\n";
    cout <<is_trivially_copyable_v<A><<endl;
    cout <<is_trivially_copyable_v<vector<int>><<endl;
    cout <<is_trivially_copyable_v<B><<endl;
}