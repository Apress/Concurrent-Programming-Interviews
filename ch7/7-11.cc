#include <iostream>
#include <thread>

using namespace std;

namespace {

int data = 0;

void Writer() { data++; }

void Reader(int &output) { output = data; }

} // namespace

int main() {
  int result = 0;
  thread t1(Writer);
  thread t2(Reader, ref(result));

  t1.join();
  t2.join();
  cout << "current result is: " << result << endl;

  return 0;
}
