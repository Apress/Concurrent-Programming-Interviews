#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

namespace {

int result = 0;

void Sum(vector<int> input) {
  for (int elem : input) {
    result += elem;
  }
}

} // namespace

int main() {
  vector<int> input1(1000, 1);
  vector<int> input2(1000, 2);

  thread t1(Sum, input1);
  thread t2(Sum, input2);

  t1.join();
  t2.join();
  cout << "result is: " << result << endl;

  return 0;
}
