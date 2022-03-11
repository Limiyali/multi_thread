#include <mutex>
#include <iostream>
#include <thread>
#include <chrono>
#include <shared_mutex>
#include <condition_variable>
#include <functional>
#include <algorithm>
#include <vector>
#include <queue>
using namespace std;

template <typename T>
class ConcurrentQueue
{
public:
    ConcurrentQueue() = default;
    ConcurrentQueue(const ConcurrentQueue &rhs)
    {
        unique_lock<mutex> l(rhs.m_);
        q_ = rhs.q_;
    }
    void push(T x)
    {
        lock_guard<mutex> l(m_);
        q_.push(x);
        cv_.notify_all();
    }

    void wait_and_pop(T &res)
    {
        unique_lock<mutex> l(m_);
        cv_.wait(l, [this]
                 { return !q_.empty(); });
        res = std::move(q_.front());
        q_.pop();
    }

    shared_ptr<T> wait_and_pop()
    {
        unique_lock<mutex> l(m_);
        cv_.wait(l, [this]
                 { return !q_.empty(); });
        auto res = make_shared<T>(std::move(q_.front()));
        q_.pop();
        return res;
    }

    bool try_pop(T &res)
    {
        unique_lock<mutex> l(m_);
        if (q_.empty())
            return false;
        res = std::move(q_.front());
        q_.pop();
        return true;
    }

    shared_ptr<T> try_pop()
    {
        unique_lock<mutex> l(m_);
        if (q_.empty())
            return nullptr;
        // __throw_bad_alloc();
        auto res = make_shared<T>(std::move(q_.front()));
        q_.pop();
        return res;
    }

    bool empty() const
    {
        unique_lock<mutex> l(m_);
        return q_.empty();
    }

private:
    mutable mutex m_;
    condition_variable cv_;
    queue<T> q_;
};

ConcurrentQueue<int> q;

void push(int i)
{
    q.push(i);
}

void pop()
{
    int res;
    q.wait_and_pop(res);
    cout << " res: " << res << endl;
}

void try_pop()
{
    int res;
    if (q.try_pop(res))
        cout << " res: " << res << endl;
    else
        cout << " empty\n";
}

int main()
{
    thread t1(pop);
    thread t4(pop);
    thread t2(try_pop);
    this_thread::sleep_for(chrono::seconds(2));
    thread t(push, 1);
    thread t3(push, 2);
    t.join();
    t2.join();
    t1.join();
    t3.join();
    t4.join();
}