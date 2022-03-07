#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
using namespace std;

mutex b;

class A
{
public:
    void lock() { cout << "lock\n"; }
    void unlock() { cout << "unlock\n"; }
};

void test_unique()
{
    A a;
    {
        unique_lock m(a);
        this_thread::sleep_for(chrono::seconds(2));
    }
}

unique_lock<mutex> get_lock()
{
    extern mutex b;
    unique_lock<mutex> l(b);
    this_thread::sleep_for(chrono::seconds(1));
    cout << " prepared\n";
    return l;
}

void f()
{
    unique_lock<mutex> l(get_lock());
    this_thread::sleep_for(chrono::seconds(1));
}

void process_file_data(int index)
{
    std::unique_lock<std::mutex> l(b);
    cout << " reading " << index << endl;
    this_thread::sleep_for(chrono::seconds(1));
    l.unlock(); // 费时操作没有必要持有锁，先解锁
    cout << " processing " << index << endl;
    this_thread::sleep_for(chrono::seconds(1));
    l.lock(); // 写入数据前上锁
    cout << " complete " << index << endl;
}

int main()
{
    thread t(process_file_data, 0);
    thread t2(process_file_data, 1);
    t.join();
    t2.join();
}