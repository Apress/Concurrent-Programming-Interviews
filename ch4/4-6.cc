#include <functional>
#include <iostream>
#include <thread>

using namespace std;

namespace {

void ThreadOne(int a, int b) {
  cout << "a = " << a << endl;
  cout << "b = " << b << endl;
}

} // namespace

int main() {
  auto boundFunc = bind(ThreadOne, 1, placeholders::_1);

  thread t1(boundFunc, 2);
  t1.join();

  return 0;
}
