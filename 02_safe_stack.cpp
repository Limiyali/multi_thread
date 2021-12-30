#include <iostream>
#include <string>
#include <stack>
#include <mutex>
#include <thread>
#include <memory>
using std::cout;
using std::endl;

struct empty_stack : public std::exception
{
    const char *what() const noexcept
    {
        return "empty stack!";
    }
};

template <typename T>
class scope_stack
{
private:
    std::stack<T> t;
    std::mutex m;

public:
    scope_stack() : t(std::stack<T>()) {}
    scope_stack(const scope_stack &rhs)
    {
        std::lock_guard g(rhs.m);
        t = rhs.t;
    }
    scope_stack &operator=(scope_stack &) = delete;
    void push(T n)
    {
        std::lock_guard g(m);
        t.push(n);
    }
    std::shared_ptr<T> pop()
    {
        std::lock_guard g(m);
        if (t.empty())
            throw empty_stack();
        const std::shared_ptr<T> res = std::make_shared<T>(std::move(t.top()));
        t.pop();
        return res;
    }
    void pop(T &n)
    {
        std::lock_guard g(m);
        if (t.empty())
            throw empty_stack();
        n = std::move(t.top());
        t.pop();
    }
    bool empty() const
    {
        std::lock_guard<std::mutex> g(m);
        return t.empty();
    }
};
scope_stack<int> a;
void test_pop()
{
    // std::this_thread::sleep_for(std::chrono::seconds(2));
    try
    {
        a.pop();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}
void test_push()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    a.push(1);
}

int main()
{
    std::thread t(test_push);
    std::thread t2(test_pop);
    t.join();
    t2.join();
    return 0;
}