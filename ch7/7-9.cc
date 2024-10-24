#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

namespace {

mutex mu;
int value;

void ThreadOne() {
  unique_lock<mutex> lock(mu);
  value++;
}

void ThreadTwo() {
  unique_lock<mutex> lock(mu);
  value += 2;
}

} // namespace

int main() {

  thread t1(ThreadOne);
  thread t2(ThreadTwo);

  t1.join();
  t2.join();

  return 0;
}
