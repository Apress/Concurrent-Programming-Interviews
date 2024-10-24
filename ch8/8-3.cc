#include <cmath>
#include <condition_variable>
#include <iostream>
#include <list>
#include <thread>

using namespace std;

namespace {

const int kNumOfThreads = 10;

mutex mu;
condition_variable cv_found;
atomic<int> thread_counter;
atomic<int> result;

void Worker(const vector<int> &input, int start, int end, int target) {
  for (int i = start; i < end; i++) {
    if (i >= input.size()) {
      break;
    }
    if (input[i] == target) {
      result = i;
      break;
    }
  }

  thread_counter++;
  cv_found.notify_one();
}

} // namespace

int main() {
  thread_counter = 0;
  result = -1;

  // Create a test cases.
  vector<int> input(1000, 0);
  for (int i = 0; i < input.size(); i++) {
    input[i] = i;
  }

  list<thread> threads;
  int segment_length = ceil(input.size() / kNumOfThreads);
  for (int i = 0; i < kNumOfThreads; i++) {
    int start = i * segment_length;
    int end = (i + 1) * segment_length;
    threads.emplace_back(Worker, ref(input), start, end, 500);
  }

  unique_lock<mutex> lock(mu);
  cv_found.wait(lock, [&]() {
    // Found the result.
    if (result != -1) {
      // Give up on all other threads.
      for (list<thread>::iterator it = threads.begin(); it != threads.end();
           it++) {
        it->detach();
      }
      return true;
    }

    if (thread_counter == kNumOfThreads) {
      // Did not find the result.
      for (list<thread>::iterator it = threads.begin(); it != threads.end();
           it++) {
        it->join();
      }
      return true;
    }

    return false;
  });

  cout << result.load() << endl; // Expect 500
  return 0;
}
