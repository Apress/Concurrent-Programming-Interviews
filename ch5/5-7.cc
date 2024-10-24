#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

namespace {

mutex mu;

void RecursiveFactorial(int n, int &result) {
  unique_lock<mutex> lock(mu);
  if (n <= 1) {
    result = 1;
    return;
  }

  int next_result = -1;
  RecursiveFactorial(n - 1, next_result);
  result = n * next_result;
}

} // namespace

int main() {

  // Assume main thread will pass `result` to other threads,
  // so we need use a mutex to synchronize the access.
  int result = -1;

  thread t1(RecursiveFactorial, 10, ref(result));

  // The program will hang by design.
  t1.join();

  cout << "The result is " << result << endl;
  return 0;
}
