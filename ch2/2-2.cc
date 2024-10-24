#include <iostream>
#include <thread>

using namespace std;

namespace {
// Shared variable
int counter = 0;

void incrementByOne() { counter += 1; }

void incrementByTwo() { counter += 2; }

} // namespace

int main() {
  thread thread1(incrementByOne);
  thread thread2(incrementByTwo);

  thread1.join();
  thread2.join();

  cout << "Final counter value: " << counter << endl;
  return 0;
}
