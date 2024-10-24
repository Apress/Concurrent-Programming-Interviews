#include <iostream>
#include <list>
#include <thread>

using namespace std;

namespace {

int shared_value = 0;

void ThreadOne() { shared_value++; }

} // namespace

int main() {
  list<thread> threads;

  while (shared_value < 10) {
    // The cout here is a simulation of business logic.
    cout << "Current value of shared_value: " << shared_value << endl;
    threads.emplace_back(ThreadOne);
  }

  for (list<thread>::iterator it = threads.begin(); it != threads.end(); it++) {
    it->join();
  }

  return 0;
}
