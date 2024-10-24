#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

using namespace std;

namespace {

void ThreadImpl(atomic<int> &data) {
  for (int i = 0; i < 10000000; i++) {
    data++;
  }
}

} // namespace

int main() {
  chrono::time_point t0 = chrono::high_resolution_clock::now();
  atomic<int> foo = 0, bar = 0;
  thread t1(ThreadImpl, ref(foo));
  thread t2(ThreadImpl, ref(bar));

  t1.join();
  t2.join();

  // Expect to take more than 300 ms.
  cout << "Program takes "
       << chrono::duration_cast<chrono::milliseconds>(
              chrono::high_resolution_clock::now() - t0)
              .count()
       << " milliseconds." << endl;

  return 0;
}
