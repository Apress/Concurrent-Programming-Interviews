#include <iostream>
#include <thread>

using namespace std;

namespace {
void foo() { cout << "This is thread foo!" << endl; }
} // namespace

int main() {
  thread t(foo);
  t.join();

  cout << "Hello, World!" << endl;
  return 0;
}
