#include <atomic>

using namespace std;

int main() {
  atomic<int> value;
  value.store(100);

  int previous = value.load();
  value.compare_exchange_weak(previous, 200);
  return 0;
}
