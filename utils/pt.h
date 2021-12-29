#include <iostream>
#include <ostream>
#include <type_traits>

namespace shuxin
{

  template <typename T>
  class AddSpace
  {
  private:
    T const &ref; // refer to argument passed in constructor
  public:
    AddSpace(T const &r) : ref(r) {}
    friend std::ostream &operator<<(std::ostream &os, AddSpace<T> s)
    {
      return os << s.ref << " "; // output passed argument and a space
    }
  };

  template <typename T>
  void pt(const T &rhs)
  {
    for (auto &it : rhs)
    {
      std::cout << it << " ";
    }
    std::cout << std::endl;
  }

} // namespace shuxin