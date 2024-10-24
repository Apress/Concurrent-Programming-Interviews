#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

namespace {
mutex mu;
condition_variable cv;
bool ready = false;

void worker_thread() {
  unique_lock<mutex> lock(mu);
  if (cv.wait_for(lock, chrono::seconds(5), [] { return ready; })) {
    cout << "Condition satisfied within 5 seconds" << endl;
    return;
  }
  cout << "Timed out waiting for the condition" << endl;
}
} // namespace

int main() {
  thread t(worker_thread);

  // Simulate delay for other work
  this_thread::sleep_for(chrono::seconds(2));
  {
    lock_guard<mutex> lock(mu);
    ready = true;
    cv.notify_one();
  }

  t.join();
  return 0;
}
