#include <thread>
#include <iostream>
#include <algorithm>
#include <functional>
#include <numeric>
#include <vector>
using namespace std;

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

template <typename Iter, typename T>
struct accu_block
{
    void operator()(Iter start, Iter end, T &res)
    {
        res = accumulate(start, end, res);
    }
};

template <typename Iterator, typename T>
T parallel_accumulate(Iterator start, Iterator end, T init)
{
    const unsigned long len = distance(start, end);
    if (!len)
        return init;
    int min_per_thread = 25;
    int max_thread = (len + min_per_thread - 1) / min_per_thread;
    int hard_thread = thread::hardware_concurrency();
    int num_thread = min(hard_thread == 0 ? hard_thread : 2, max_thread);
    int block_size = len / num_thread;
    vector<int> res(num_thread);
    vector<thread> threads(num_thread - 1);
    Iterator start_block = start;
    Iterator end_block = start;
    for (int i = 0; i < num_thread - 1; ++i)
    {
        advance(end_block, block_size);
        mythread(accu_block<Iterator, T>{}, start_block, end_block, ref(res[i]));
        // threads[i] = thread(accu_block<Iterator, T>{}, start_block, end_block, ref(res[i]));
        start_block = end_block;
    }
    // for_each(threads.begin(),threads.end(),mem_fn(&thread::join));
    accu_block<Iterator,T>{}(start_block, end, ref(res[num_thread - 1]));
    return accumulate(res.begin(), res.end(), init);
}

int main()
{
    vector<int> a;
    for (auto i = 1; i <= 100; i++)
        a.push_back(i);
    cout << parallel_accumulate(a.begin(), a.end(), 0) << endl;
}