#include <iostream>
#include <thread>

using namespace std;

namespace {

class Foo {
public:
  // Default constructor
  Foo() {}

  // Copy constructor
  Foo(Foo &f) { cout << "Copy constructor is called." << endl; }

  // Implement operator ()
  void operator()() { cout << "Object used as a functor." << endl; }
};

} // namespace

int main() {
  Foo foo;
  thread t1(foo);
  t1.join();

  thread t2(ref(foo));
  t2.join();
  return 0;
}
