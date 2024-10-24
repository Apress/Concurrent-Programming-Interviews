#include <barrier>
#include <iostream>
#include <list>
#include <thread>

using namespace std;

namespace {

barrier ba{5};

void ThreadOne() {
  cout << "Thread started: " << this_thread::get_id() << endl;
  ba.arrive_and_wait();
  cout << "Thread proceeded: " << this_thread::get_id() << endl;
}

} // namespace

int main() {
  list<thread> threads;
  for (int i = 0; i < 20; i++) {
    threads.emplace_back(ThreadOne);
  }
  for (list<thread>::iterator it = threads.begin(); it != threads.end(); it++) {
    it->join();
  }

  return 0;
}
