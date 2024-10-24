#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

namespace {

mutex mu;
int data = 0;

void Writer() {
  unique_lock<mutex> lock(mu);
  data++;
}

void Reader(int &output) {
  unique_lock<mutex> lock(mu);
  output = data;
}

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
