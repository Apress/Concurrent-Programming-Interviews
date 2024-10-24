#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

namespace {
// Shared variable
int counter = 0;

// Mutex for synchronizing access to `counter`
mutex mu;

void incrementByOne() {
  mu.lock(); // Lock the mutex
  counter += 1;
  mu.unlock(); // Unlock the mutex
}

void incrementByTwo() {
  mu.lock(); // Lock the mutex
  counter += 2;
  mu.unlock(); // Unlock the mutex
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
