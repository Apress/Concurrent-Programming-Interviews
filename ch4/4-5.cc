#include <iostream>
#include <thread>

using namespace std;

namespace {

void ThreadOne(int &input) {
  cout << "This is ThreadOne." << endl;
  input += 1;
}

} // namespace

int main() {
  int a = 0;

  thread t1(ThreadOne, ref(a));
  t1.join();
  cout << "Value of a is: " << a << endl;

  return 0;
}
