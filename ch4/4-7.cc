#include <iostream>
#include <thread>

using namespace std;

namespace {

void ThreadOne(int &output) {
  cout << "This is ThreadOne." << endl;
  output = 1;
}

void ThreadTwo(int input) {
  cout << "This is ThreadTwo. Input is: " << input << endl;
}

} // namespace

int main() {
  int a = 0;

  thread t1(ThreadOne, ref(a));
  t1.join();

  thread t2(ThreadTwo, a);
  t2.join();

  return 0;
}
