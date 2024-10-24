#include <iostream>
#include <list>
#include <thread>

using namespace std;

namespace {

void ThreadOne() {
  cout << "This is ThreadOne. The ID is: " << this_thread::get_id() << endl;
}

} // namespace

int main() {
  list<thread> threads;

  for (int i = 0; i < 10; i++) {
    threads.emplace_back(ThreadOne);
  }

  for (list<thread>::iterator it = threads.begin(); it != threads.end(); it++) {
    it->join();
  }

  return 0;
}
