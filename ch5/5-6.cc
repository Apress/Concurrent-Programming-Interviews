#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

namespace {

int foo_bar = 0;
timed_mutex mu;

void ThreadOne() {
  unique_lock<timed_mutex> lock(mu, defer_lock);
  if (!lock.try_lock_for(chrono::seconds(2))) {
    cout << "Thread one proceeded and did not acquire the lock." << endl;
  }
}

} // namespace

int main() {
  // lock `mu` and never release.
  mu.lock();

  thread t1(ThreadOne);
  t1.join();

  return 0;
}
