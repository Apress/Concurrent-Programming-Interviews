#include <chrono>
#include <iostream>
#include <list>
#include <thread>

using namespace std;

namespace {
const int kVectorLength = 10;
const int kMaxConcurrentCalls = 3;

int CallRandomNumberGenerator() {
  // Simulate remote API call.
  this_thread::sleep_for(chrono::microseconds(500));
  return rand();
}

void Worker(int start, int segment_size, vector<int64_t> &results) {
  for (int i = 0; i < segment_size; i++) {
    if (start + i >= results.size()) {
      break;
    }
    results[start + i] = CallRandomNumberGenerator();
  }
}

} // namespace

int main() {
  srand(time(nullptr));

  vector<int64_t> results(kVectorLength, 0);
  list<thread> threads;
  int segment_size = kVectorLength / kMaxConcurrentCalls + 1;
  for (int i = 0; i < kMaxConcurrentCalls; i++) {
    threads.emplace_back(Worker, i * segment_size, segment_size, ref(results));
  }
  for (list<thread>::iterator it = threads.begin(); it != threads.end(); it++) {
    it->join();
  }

  for (int i = 0; i < kVectorLength; i++) {
    cout << results[i] << " ";
  }
  cout << endl;
  return 0;
}
