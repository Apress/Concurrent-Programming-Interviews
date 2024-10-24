#include <iostream>
#include <list>
#include <shared_mutex>
#include <thread>

using namespace std;

namespace {

int foo_bar = 0;
shared_mutex mu;

void ReaderThread(int &output) {
  // Simulate business logic in the thread.
  this_thread::sleep_for(chrono::milliseconds(100));

  shared_lock<shared_mutex> lock(mu);
  output = foo_bar;
}
void WriterThread() {
  // Simulate business logic in the thread.
  this_thread::sleep_for(chrono::milliseconds(100));

  unique_lock<shared_mutex> lock(mu);
  foo_bar++;
}

} // namespace

int main() {
  list<thread> threads;
  vector<int> results(6, -1);

  // Add readers.
  for (int i = 0; i < 3; i++) {
    threads.emplace_back(ReaderThread, ref(results[i]));
  }

  // Add writers.
  threads.emplace_back(WriterThread);

  // Add more readers.
  for (int i = 0; i < 3; i++) {
    threads.emplace_back(ReaderThread, ref(results[i + 3]));
  }

  for (list<thread>::iterator it = threads.begin(); it != threads.end(); it++) {
    it->join();
  }

  for (int result : results) {
    cout << result << " ";
  }
  cout << endl;

  return 0;
}
