#include <iostream>
#include <atomic>
using namespace std;

struct A
{
    int a[100];
};

int main()
{
    cout << atomic<A>{}.is_lock_free() << endl;
    cout << atomic<int>{}.is_lock_free() << endl;
    cout << atomic<int>{}.is_always_lock_free << endl;
    cout << ATOMIC_BOOL_LOCK_FREE << endl;
    atomic_int a = 3, b = 5;
    int c = 3;
    a.store(5);
    cout << a << endl;
    cout << a.exchange(3) << endl;
    cout << a.load() << endl;
    cout << a.compare_exchange_strong(c, 5) << endl;
    cout << a<<endl;
    cout << "###########################\n";
    atomic_flag x = ATOMIC_FLAG_INIT;
    x.clear(memory_order_release);
    cout << x.test_and_set()<<endl;
    cout << x.test_and_set() <<endl;
}