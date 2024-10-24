#include <future>
#include <iostream>
#include <thread>

using namespace std;

namespace {

int ThreadOne() {
  cout << "This is ThreadOne." << endl;
  // Omit logic to calculate the output value.
  return 1;
}

void ThreadTwo(int input) {
  cout << "This is ThreadTwo. Input is: " << input << endl;
}

} // namespace

int main() {
  future a = async(launch::deferred, ThreadOne);
  thread t2(ThreadTwo, a.get());
  t2.join();

  return 0;
}
