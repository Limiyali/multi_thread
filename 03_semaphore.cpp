#include <iostream>
#include <thread>
#include <semaphore>

using namespace std;

class A
{
public:
    void wait1()
    {
        sem_.acquire();
        cout << 1 << endl;
    }

    void wait2()
    {
        sem_.acquire();
        cout << 2 << endl;
    }

    void signal() { sem_.release(1); }

private:
    // counting_semaphore<2> sem_{0};
    binary_semaphore sem_{0};
};

int main()
{
    A a;
    thread t1(&A::wait1, &a);
    thread t2(&A::wait2, &a);
    this_thread::sleep_for(chrono::seconds(1));
    thread t3(&A::signal, &a);
    t1.join();
    t2.join();
    t3.join();
}