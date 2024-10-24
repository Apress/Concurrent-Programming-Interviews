#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

namespace {

class FooSingleton {
private:
  inline static FooSingleton *foo_instance_;
  inline static once_flag foo_flag_;

  // Private constructor.
  FooSingleton() { cout << "FooSingleton is constructed." << endl; }

public:
  static FooSingleton *GetInstance() {
    call_once(foo_flag_, [&]() { foo_instance_ = new FooSingleton(); });
    return foo_instance_;
  }
  void Bar() { cout << "This is Bar method." << endl; }
};

void ThreadOne() {
  FooSingleton *foo = FooSingleton::GetInstance();
  foo->Bar();
}

} // namespace

int main() {
  thread t1(ThreadOne);
  t1.join();

  thread t2(ThreadOne);
  t2.join();

  return 0;
}
