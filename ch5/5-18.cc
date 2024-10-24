#include <chrono>
#include <iostream>
#include <list>
#include <thread>

using namespace std;

namespace {
const int kVectorLength = 10;

void CallRandomNumberGenerator(int64_t &output) {
  // Simulate remote API call.
  this_thread::sleep_for(chrono::microseconds(500));
  output = rand();
}
} // namespace

int main() {
  srand(time(nullptr));

  vector<int64_t> results(kVectorLength, 0);
  list<thread> threads;
  for (int i = 0; i < kVectorLength; i++) {
    threads.emplace_back(CallRandomNumberGenerator, ref(results[i]));
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
