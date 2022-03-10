#include <mutex>
#include <iostream>
#include <thread>
#include <chrono>
#include <shared_mutex>
#include <condition_variable>
#include <functional>
#include <algorithm>
#include <vector>
using namespace std;

mutex m;
condition_variable cv;
bool ready = false;
int value;

void print_id(int id)
{
    unique_lock<mutex> l(m);
    // while (!ready)
    //     cv.wait(l);
    // cv.wait(l, [&]
    //         { return ready == true; });
    auto now = chrono::system_clock::now();
    while (cv.wait_for(l, chrono::seconds(2)) == cv_status::timeout)
    {
        cout << id << " time out " << ready << endl;
    }
    cout << " id: " << id << endl;
}

void go()
{
    unique_lock<mutex> l(m);
    // ready = true;
    cv.notify_all();
}

void print_value()
{
    unique_lock<mutex> l(m);
    while (cv.wait_for(l, chrono::seconds(1)) == cv_status::timeout)
    {
        cout << " time out\n";
        cout.flush();
    }
    cout << value << endl;
}

void read_value()
{
    unique_lock<mutex> l(m);
    cin >> value;
    cv.notify_one();
}

class A
{
public:
    void step1()
    {
        {
            std::lock_guard<std::mutex> l(m_);
            step1_done_ = true;
        }
        std::cout << 1;
        cv_.notify_one();
    }

    void step2()
    {
        std::unique_lock<std::mutex> l(m_);
        cv_.wait(l, [this]
                 { return step1_done_; });
        step2_done_ = true;
        std::cout << 2;
        cv_.notify_one();
    }

    void step3()
    {
        std::unique_lock<std::mutex> l(m_);
        cv_.wait(l, [this]
                 { return step2_done_; });
        std::cout << 3;
    }

private:
    std::mutex m_;
    std::condition_variable cv_;
    bool step1_done_ = false;
    bool step2_done_ = false;
};

class B
{
public:
    void step1()
    {
        unique_lock<mutex> l(m_);
        cv_.wait(l, [this]
                 { return done_; });
        std::cout << 1;
        // cv_.notify_one();
    }

    void step2()
    {
        unique_lock<mutex> l(m_);
        cv_.wait(l, [this]
                 { return done_; });
        std::cout << 2;
        // cv_.notify_one();
    }

    void step3()
    {

        unique_lock<mutex> l(m_);
        done_ = true;
        cv_.notify_one();
    }

private:
    std::mutex m_;
    std::condition_variable cv_;
    bool done_ = false;
};

int main()
{
    B a;
    std::thread t1(&B::step1, &a);
    std::thread t2(&B::step2, &a);
    std::thread t3(&B::step3, &a);
    t1.join();
    t2.join();
    t3.join();
} // 123

// int main()
// {
//     vector<thread> threads;
//     for (int i = 0; i < 10; i++)
//         threads.emplace_back(thread(print_id, i));
//     this_thread::sleep_for(chrono::seconds(8));
//     go();
//     for_each(threads.begin(), threads.end(), mem_fn(&std::thread::join));

//     // thread t2(print_value);
//     // thread t(read_value);
//     // t.join();
//     // t2.join();
//     return 0;
// }