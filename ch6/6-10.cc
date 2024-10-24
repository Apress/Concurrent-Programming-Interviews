#include <functional>
#include <iostream>
#include <optional>
#include <thread>

using namespace std;

namespace {

class ThreadSafeQueue {
public:
  ThreadSafeQueue();
  optional<int64_t> Front();
  void Pop();
  void Push(int64_t value);

private:
  struct Node {
    int64_t data;
    Node *next;
  };
  Node *head_;
  Node *tail_;
};

} // namespace

int main() { return 0; }
