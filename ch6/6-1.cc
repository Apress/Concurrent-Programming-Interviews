#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

namespace {

int a = 0;
int b = 0;

mutex mu;

void ThreadOne() { a++; }

void ThreadTwo() {
  unique_lock<mutex> lock(mu);
  b++;
}

} // namespace

int main() {
  thread t1(ThreadOne);
  thread t2(ThreadOne);

  t1.join();
  t2.join();
  cout << "current a is: " << a << endl; // race condition

  thread t3(ThreadTwo);
  thread t4(ThreadTwo);

  t3.join();
  t4.join();
  cout << "current b is: " << b << endl; // no race condition

  return 0;
}
