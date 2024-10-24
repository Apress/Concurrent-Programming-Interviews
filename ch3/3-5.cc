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
  auto timeout_time = chrono::system_clock::now() + chrono::seconds(10);
  if (cv.wait_until(lock, timeout_time, [] { return ready; })) {
    cout << "Condition satisfied before the timeout" << endl;
    return;
  }
  cout << "Waited until timeout" << endl;
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
