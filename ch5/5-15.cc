#include <iostream>
#include <latch>
#include <list>
#include <thread>

using namespace std;

namespace {

latch la{5};

void ThreadOne() {
  la.arrive_and_wait();
  cout << "Thread proceeded: " << this_thread::get_id() << endl;
}

} // namespace

int main() {
  list<thread> threads;
  for (int i = 0; i < 5; i++) {
    cout << "Thread started." << endl;
    threads.emplace_back(ThreadOne);
  }
  for (list<thread>::iterator it = threads.begin(); it != threads.end(); it++) {
    it->join();
  }

  return 0;
}
