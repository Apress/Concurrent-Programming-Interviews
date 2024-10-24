#include <iostream>
#include <thread>

using namespace std;

namespace {
void foo() { cout << "This is thread foo!" << endl; }

void bar() { cout << "This is thread bar!" << endl; }
} // namespace

int main() {
  thread t_foo(foo);
  thread t_bar(bar);

  t_foo.join();
  t_bar.join();

  return 0;
}
