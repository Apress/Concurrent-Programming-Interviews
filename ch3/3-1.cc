#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

namespace {
mutex mu;
bool ready = false;

void WaitingThread() {
  while (true) {
    unique_lock<mutex> lock(mu);
    if (ready) {
      break;
    }
  }
  cout << "Waiting Thread." << endl;
}

void PriorityThread() {
  unique_lock<mutex> lock(mu);
  ready = true;
  cout << "Priority Thread." << endl;
}
} // namespace

int main() {
  thread thread1(WaitingThread);
  thread thread2(PriorityThread);
  thread1.join();
  thread2.join();
  return 0;
}
