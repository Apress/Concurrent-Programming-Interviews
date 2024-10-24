#include <iostream>
#include <list>
#include <mutex>
#include <thread>

using namespace std;

namespace {

mutex stdout_mu;
atomic<int> value;

void ThreadOne() {
  value++;

  unique_lock<mutex> stdout_lock(stdout_mu);
  cout << "current value is: " << value << endl;
}

} // namespace

int main() {
  value.store(0);

  list<thread> threads;

  for (int i = 0; i < 10; i++) {
    threads.emplace_back(ThreadOne);
  }

  for (list<thread>::iterator it = threads.begin(); it != threads.end(); it++) {
    it->join();
  }

  return 0;
}
