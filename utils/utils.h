#include <chrono>
namespace shuxin
{
#define sleep(i) this_thread::sleep_for(chrono::seconds(i))
}