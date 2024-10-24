#include <future>
#include <iostream>
#include <thread>

using namespace std;

namespace {

int Add(int x, int y) {
  cout << "Add function is called with " << x << " and " << y << endl;
  return x + y;
}

} // namespace

int main() {
  packaged_task<int(int, int)> task(Add);
  future<int> result = task.get_future();
  task(1, 2);
  cout << "The result is: " << result.get() << endl;

  return 0;
}
