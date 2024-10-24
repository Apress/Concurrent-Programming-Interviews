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

// Solution is required to use this function to print to standard output.
void PrintC() { cout << "C"; }
} // namespace utility

namespace {
mutex mu;
condition_variable cv;
int counter_a_ = 0;
int counter_b_ = 0;
int counter_c_ = 0;

void ThreadA() {
  for (int i = 0; i < 3; i++) {
    unique_lock<mutex> lock(mu);
    cv.wait(lock, [=]() { return counter_a_ > 0; });

    for (int j = 0; j < counter_a_; j++) {
      utility::PrintA();
    }

    counter_b_ = counter_a_ + 1;
    counter_a_ = 0;
    cv.notify_all();
  }
}

void ThreadB() {
  for (int i = 0; i < 3; i++) {
    unique_lock<mutex> lock(mu);
    cv.wait(lock, [=]() { return counter_b_ > 0; });

    for (int j = 0; j < counter_b_; j++) {
      utility::PrintB();
    }

    counter_c_ = counter_b_ + 1;
    counter_b_ = 0;
    cv.notify_all();
  }
}

void ThreadC() {
  for (int i = 0; i < 3; i++) {
    unique_lock<mutex> lock(mu);
    cv.wait(lock, [=]() { return counter_c_ > 0; });

    for (int j = 0; j < counter_c_; j++) {
      utility::PrintC();
    }

    counter_a_ = counter_c_ + 1;
    counter_c_ = 0;
    cv.notify_all();
  }
}
} // namespace

int main() {
  // Set the initial value of counter_a_.
  counter_a_ = 1;

  thread threads[3];
  threads[0] = thread(ThreadA);
  threads[1] = thread(ThreadB);
  threads[2] = thread(ThreadC);
  for (int i = 0; i < 3; i++) {
    threads[i].join();
  }
  return 0;
}
