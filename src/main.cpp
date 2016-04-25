#include <iostream>
// #define CXX_XOR_PROJECT_USE_UINTPTR_T 1
#define CXX_XOR_PROJECT_USE_REINTERPRET_CAST 1
#include "dllist.hxx"
#include <vector>

int main(int argc, char* argv[])
{
  int a = 4;
  int b = 3;
  int* c = &a;
  int* d = &b;
  std::size_t n = 6;

  /**
   * create instances declared in the header files to test whether or not
   * the header definitions compile
   */
  auto x = xorptr_traits<int>::create(&a, &b);
  std::cout << "address a: " << &a << "\n";
  std::cout << "address b: " << &b << "\n";
  std::cout << "address xor: x ^ a " << xorptr_traits<int>::extract(x, &a) << '\n';
  std::cout << "address xor: x ^ b " << xorptr_traits<int>::extract(x, &b) << '\n';

  auto y = xorptr<int>(&a, &b);
  std::cout << "\n\nUsing xorptr class\n";
  std::cout << "address a: " << &a << "\n";
  std::cout << "address b: " << &b << "\n";
  std::cout << "address y ^ c: " << (y ^ c) << "\n";
  std::cout << "address y ^ d: " << (y ^ d) << "\n";


  dllist<int> blah({ 1,5,3,1,4 });
  dllist<int> something(n, 10);
  // use copy constructor
  dllist<int> j(something);
  for (auto const& l : j) {
    std::cout << l << ' ';
  }

  std::cout << '\n';

  auto beg = blah.begin();
  blah.erase(beg);
  for (auto const& l : blah) {
    std::cout << l << ' ';
  }

  std::cout << '\n';
  std::cout << blah.size() << '\n';


  return 0;
}
