#include <condition_variable>
#include <iostream>
#include <list>
#include <thread>

using namespace std;

namespace {

mutex mu;
condition_variable cv;
int counter = 5;

void ThreadOne() {
  // Omit business logic.
  unique_lock<mutex> lock(mu);
  counter--;
  cv.wait(lock, [&]() { return counter == 0; });
  cout << "Thread proceeded: " << this_thread::get_id() << endl;
  cv.notify_all();
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
