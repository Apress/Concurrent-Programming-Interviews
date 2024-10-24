#include <future>
#include <iostream>
#include <thread>

using namespace std;

namespace {

void ThreadOne(promise<int> &promised_value) {
  cout << "This is ThreadOne." << endl;
  promised_value.set_value(1);
}

} // namespace

int main() {
  promise<int> a;
  future<int> a_future = a.get_future();

  thread t1(ThreadOne, ref(a));
  cout << "The value of a is: " << a_future.get() << endl;

  t1.join();
  return 0;
}
