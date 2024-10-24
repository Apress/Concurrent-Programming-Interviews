#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

namespace utility {
// Solution is required to use this function to print to standard output.
void PrintA() { cout << "A"; }

// Solution is required to use this function to print to standard output.
void PrintB() { cout << "B"; }
} // namespace utility

namespace {
mutex mu;
condition_variable cv;
bool to_print_a_ = true;

void ThreadA() {
  for (int i = 0; i < 5; i++) {
    unique_lock<mutex> lock(mu);
    cv.wait(lock, [=]() { return to_print_a_; });
    utility::PrintA();
    to_print_a_ = false;
    cv.notify_one();
  }
}

void ThreadB() {
  for (int i = 0; i < 5; i++) {
    unique_lock<mutex> lock(mu);
    cv.wait(lock, [=]() { return !to_print_a_; });
    utility::PrintB();
    to_print_a_ = true;
    cv.notify_one();
  }
}
} // namespace

int main() {
  thread threads[2];
  threads[0] = thread(ThreadA);
  threads[1] = thread(ThreadB);
  threads[0].join();
  threads[1].join();
  return 0;
}
