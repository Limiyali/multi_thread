#include <chrono>
#include <iostream>
#include <future>
#include "utils/utils.h"
using namespace std;

class A
{
public:
    void signal()
    {
        cout << 1;
        this_thread::sleep_for(chrono::seconds(2));
        ps_.set_value_at_thread_exit();
    }
    void wait()
    {
        auto res = ps_.get_future();
        cout << " get future\n";
        res.wait();
        cout << 2;
    }

private:
    promise<void> ps_;
};

void test_A()
{
    A a;
    thread t(&A::signal, &a);
    thread t2(&A::wait, &a);
    t.join();
    t2.join();
}

class B
{
public:
    void task() { cout << 1; }
    void wait_for_task()
    {
        ps_.get_future().wait();
        task();
    }
    void signal() { ps_.set_value(); }

private:
    promise<void> ps_;
};

int test_B()
{
    B b;
    thread t(&B::wait_for_task, &b);
    sleep(1);
    b.signal();
    t.join();

    future<void> res = async([]
                             { throw logic_error("error"); });
    try
    {
        res.get();
    }
    catch (const exception &e)
    {
        cout << e.what() << endl;
    }
    return 0;
}

int main()
{
    test_B();
    promise<int> ps;
    try
    {
        ps.set_value(1);
        throw logic_error("error");
    }
    catch (const std::exception &e)
    {
        // ps.set_exception(current_exception());
    }
    auto res = ps.get_future();
    try
    {
        cout<<res.get()<<endl;
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
}