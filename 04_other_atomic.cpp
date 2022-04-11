#include <iostream>
#include <atomic>
#include <thread>
using namespace std;

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
    while(!x.compare_exchange_weak(expected,true) && !expected){
        cout<<" maybe\n";
    }
}