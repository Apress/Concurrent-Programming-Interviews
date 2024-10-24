#include <chrono>
#include <iostream>
#include <mutex>
#include <semaphore>
#include <thread>
#include <vector>

using namespace std;

namespace {

void ExampleThread(counting_semaphore<> &sem, int id) {
  sem.acquire();
  cout << "ExampleThread " << id << " acquired semaphore" << endl;

  this_thread::sleep_for(chrono::milliseconds(1000));

  cout << "ExampleThread " << id << " releasing semaphore" << endl;
  sem.release();
}
} // namespace

int main() {
  counting_semaphore sem(3);
  vector<thread> threads;

  for (int i = 0; i < 5; ++i) {
    threads.emplace_back(ExampleThread, ref(sem), i);
  }

  for (thread &t : threads) {
    t.join();
  }

  return 0;
}