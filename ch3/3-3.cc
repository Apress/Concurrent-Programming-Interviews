#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

namespace {
mutex mu;
condition_variable cv;
bool ready = false;

void WaitingThread() {
  unique_lock<mutex> lock(mu);
  cv.wait(lock, [=]() { return ready; });
  cout << "Waiting Thread." << endl;
}

void PriorityThread() {
  unique_lock<mutex> lock(mu);
  ready = true;
  cout << "Priority Thread." << endl;
  cv.notify_one();
}
} // namespace

int main() {
  thread thread1(WaitingThread);
  thread thread2(PriorityThread);
  thread1.join();
  thread2.join();
  return 0;
}
