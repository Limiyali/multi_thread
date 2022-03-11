#include <future>
#include <iostream>
using namespace std;

int main()
{
    promise<void> ps;
    // future<void> ft = ps.get_future();
    // shared_future<void> sf(move(ft));
    auto sf = ps.get_future().share();
    ps.set_value();
    sf.get();
    sf.get();
}