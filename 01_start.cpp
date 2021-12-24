#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

void hello()
{
    cout << " hello world\n";
}

class A
{
public:
    void operator()() const
    {
        cout << " hello A\n";
    }
};

struct Tt
{
    int &i;
    Tt(int &tmp) : i(tmp) {}
    void operator()() const
    {
        this_thread::sleep_for(chrono::seconds(3));
        cout << " do start\n";
        cout << i << endl;
    }
};

int main()
{
    thread t(hello);
    t.join();
    A a;
    thread t1(a);
    thread t2{A()};
    thread t3((A()));
    thread t4{[]
              { cout << "lamda\n"; }};
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    int x=4;
    thread t5{Tt(x)};
    t5.join();

}