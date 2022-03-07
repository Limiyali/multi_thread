#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
#include <shared_mutex>
using namespace std;

class A
{
public:
    void lock_shared() { std::cout << "lock_shared" << std::endl; }
    void unlock_shared() { std::cout << "unlock_shared" << std::endl; }
};

class ReadWriter
{
public:
    void read() const
    {
        shared_lock l(m_);
        this_thread::sleep_for(chrono::seconds(1));
        cout << n_ << endl;
    }

    void write()
    {
        unique_lock l(m_);
        this_thread::sleep_for(chrono::seconds(1));
        n_++;
    }

private:
    int n_ = 0;
    mutable shared_mutex m_;
};

int main()
{
    ReadWriter r;
    thread t(&ReadWriter::read, &r);
    t.join();
    this_thread::sleep_for(chrono::milliseconds(500));
    thread t2(&ReadWriter::read, &r);
    t2.join();
    // this_thread::sleep_for(chrono::milliseconds(500));
    thread t3(&ReadWriter::write, &r);
    t3.join();
    this_thread::sleep_for(chrono::milliseconds(500));
    thread t4(&ReadWriter::read, &r);
    t4.join();
}