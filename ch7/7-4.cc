#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <unordered_map>

using namespace std;

namespace {

class DelayInjection {
public:
  static void InjectDelayForTesting(const string &tag) {
#ifdef TEST_ENV
    // If you see this in the output, the delay injection happened.
    cout << "Delay injection happened." << endl;

    unordered_map<string, chrono::milliseconds>::iterator it =
        injection_instructions_.find(tag);
    if (it == injection_instructions_.end()) {
      return;
    }
    this_thread::sleep_for(it->second);
#endif
  }

  static void SetDelay(const string &tag, const chrono::milliseconds &delay) {
    injection_instructions_[tag] = delay;
  }

private:
  static unordered_map<string, chrono::milliseconds> injection_instructions_;
};

unordered_map<string, chrono::milliseconds>
    DelayInjection::injection_instructions_ =
        unordered_map<string, chrono::milliseconds>();

mutex mu;
int data = 0;

void Writer() {
  // Inject delay to writer thread.
  DelayInjection::InjectDelayForTesting("writer");

  unique_lock<mutex> lock(mu);
  data++;
}

void Reader(int &output) {
  // Inject delay to reader thread.
  DelayInjection::InjectDelayForTesting("reader");

  unique_lock<mutex> lock(mu);
  output = data;
}

} // namespace

int main() {
  int result = 0;

  // Test case 1. Writer has delay.
  DelayInjection::SetDelay("writer", chrono::milliseconds(500));
  thread t1(Writer);
  thread t2(Reader, ref(result));

  t1.join();
  t2.join();
  // Expected result is 0.
  cout << "current result is: " << result << endl;

  // Test case 2. Reader has delay.
  DelayInjection::SetDelay("reader", chrono::milliseconds(500));
  DelayInjection::SetDelay("writer", chrono::milliseconds(0));
  thread t3(Writer);
  thread t4(Reader, ref(result));

  t3.join();
  t4.join();
  // Expected result is 2.
  cout << "current result is: " << result << endl;

  return 0;
}
