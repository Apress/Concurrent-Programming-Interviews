#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

using namespace std;

namespace {

class SimpleSemaphore {
private:
  mutex mu_;
  condition_variable cv_;
  int count_;

public:
  SimpleSemaphore(int max_count) : count_(max_count) {}

  void Acquire() {
    unique_lock<mutex> lock(mu_);
    cv_.wait(lock, [this] { return count_ > 0; });
    count_--;
  }

  void Release() {
    unique_lock<mutex> lock(mu_);
    count_++;
    cv_.notify_one();
  }
};

void ExampleThread(SimpleSemaphore &sem, int id) {
  sem.Acquire();
  cout << "ExampleThread " << id << " acquired semaphore" << endl;

  this_thread::sleep_for(chrono::milliseconds(1000));

  cout << "ExampleThread " << id << " releasing semaphore" << endl;
  sem.Release();
}
} // namespace

int main() {
  SimpleSemaphore sem(3);
  vector<thread> threads;

  for (int i = 0; i < 5; ++i) {
    threads.emplace_back(ExampleThread, ref(sem), i);
  }

  for (thread &t : threads) {
    t.join();
  }

  return 0;
}