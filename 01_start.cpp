#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <functional>
#include <algorithm>

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
    void f(int i)
    {
        cout << i << endl;
    }
};

struct Tt
{
    int &i;
    Tt(int &tmp) : i(tmp) {}
    void operator()() const
    {
        this_thread::sleep_for(chrono::seconds(1));
        cout << " do start\n";
        cout << i << endl;
    }
};

class thread_guard
{
    thread &t;

public:
    explicit thread_guard(thread &x) : t(x) {}
    ~thread_guard()
    {
        if (t.joinable())
            t.join();
    }
    thread_guard(const thread_guard &) = delete;
    thread_guard &operator=(const thread_guard &) = delete;
};

void f(int &i)
{
    cout << (++i) << endl;
}

void f2(int i)
{
    cout << (i) << endl;
}

void ft(thread t)
{
    t.join();
}

class scoped_thread
{
    thread t;

public:
    explicit scoped_thread(thread x) : t(move(x))
    {
        if (!t.joinable())
            cerr << "error\n";
    }
    ~scoped_thread()
    {
        if (t.joinable())
            t.join();
    }
    scoped_thread(const scoped_thread &) = delete;
    scoped_thread &operator=(const scoped_thread &) = delete;
};

class mythread
{
    thread t;

public:
    mythread() noexcept = default;
    template <typename T, typename... Ts>
    explicit mythread(T &&f, Ts &&...args) : t(forward<T>(f), forward<Ts>(args)...) {}
    explicit mythread(thread x) : t(move(x)) {}
    mythread(mythread &&rhs) : t(move(rhs.t)) {}
    mythread &operator=(mythread &&rhs) noexcept
    {
        if (joinable())
            join();
        t = move(rhs.t);
        return *this;
    }
    ~mythread() noexcept
    {
        if (joinable())
            join();
    }

    void swap(mythread &&rhs) noexcept { t.swap(rhs.t); }
    std::thread::id get_id() const noexcept { return t.get_id(); }
    bool joinable() const noexcept { return t.joinable(); }
    void join() { t.join(); }
    void detach() { t.detach(); }
    std::thread &as_thread() noexcept { return t; }
    const std::thread &as_thread() const noexcept { return t; }
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
    int x = 4;
    thread t5{Tt(x)};
    thread_guard g(t5);
    int i = 5;
    thread t6(f, ref(i));
    thread_guard g2(t6);
    cout << i << endl;
    thread t7(&A::f, &a, 7);
    thread_guard g3(t7);
    thread t8(f2, 8);
    ft(move(t8));
    scoped_thread s(thread(f2, 9));
    scoped_thread s2(move(t3));
    vector<thread> v;
    for (auto i = 0; i < 10; i++)
    {
        v.emplace_back(thread(f2, i + 100));
    }
    for_each(v.begin(), v.end(), mem_fn(&std::thread::join));
    mythread(f2, 1000);
    cout<<" hard:"<<thread::hardware_concurrency()<<endl;
}