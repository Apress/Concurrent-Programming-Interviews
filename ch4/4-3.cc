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

  // Example method
  void Bar() { cout << "This is Bar method." << endl; }
};

} // namespace

int main() {
  Foo foo;
  thread t1(&Foo::Bar, foo);
  t1.join();

  thread t2(&Foo::Bar, ref(foo));
  t2.join();

  thread t3(&Foo::Bar, &foo);
  t3.join();
  return 0;
}
