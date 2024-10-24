#include <functional>
#include <iostream>
#include <list>
#include <mutex>
#include <thread>

using namespace std;

namespace {

class ThreadSafeCout {
public:
  static void Cout(function<void(void)> statement) {
    unique_lock<mutex> lock(stdout_mu_);
    cout << "[" << this_thread::get_id() << "] ";
    statement();
  }

private:
  // Use static so that the caller does not need to instantiate the class.
  static mutex stdout_mu_;
};

mutex ThreadSafeCout::stdout_mu_;

atomic<int> value;

void ThreadOne() {
  value++;
  ThreadSafeCout::Cout([]() { cout << "current value is: " << value << endl; });
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
