#include <iostream>
#include <thread>

using namespace std;

namespace {

void ThreadOne() { cout << "This is ThreadOne." << endl; }

} // namespace

int main() {
  thread t(ThreadOne);
  t.join();
  return 0;
}
