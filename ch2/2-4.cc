#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

namespace {
int counter = 0;
mutex mu;

void incrementByOne() {
  // Automatically locks the mutex
  lock_guard<mutex> guard(mu);
  counter += 1;
  // `guard` automatically unlocks the mutex when it goes out of scope
}

void incrementByTwo() {
  // Automatically locks the mutex
  unique_lock<mutex> lock(mu);
  counter += 2;
  // `lock` automatically unlocks the mutex when it goes out of scope
}

} // namespace

int main() {
  thread thread1(incrementByOne);
  thread thread2(incrementByTwo);

  thread1.join();
  thread2.join();

  cout << "Final counter value: " << counter << endl;
  return 0;
}
