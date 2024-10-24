#include <iostream>
#include <list>
#include <mutex>
#include <optional>
#include <thread>

using namespace std;

namespace {

class ThreadSafeQueue {
public:
  optional<int64_t> Front();
  void Pop();
  void Push(int64_t value);

private:
  mutex mu_;
  list<int64_t> data_;
};

optional<int64_t> ThreadSafeQueue::Front() {
  unique_lock<mutex> lock(mu_);
  if (data_.empty()) {
    return nullopt;
  }
  return data_.front();
}

void ThreadSafeQueue::Pop() {
  unique_lock<mutex> lock(mu_);
  data_.pop_front();
}

void ThreadSafeQueue::Push(int64_t value) {
  unique_lock<mutex> lock(mu_);
  data_.push_back(value);
}

void ThreadOne(ThreadSafeQueue &queue) {
  for (int i = 0; i < 1000; i++) {
    queue.Push(i);
  }

  for (int i = 0; i < 1000; i++) {
    queue.Pop();
  }
}

} // namespace

int main() {
  ThreadSafeQueue queue;
  thread t1(ThreadOne, ref(queue));
  thread t2(ThreadOne, ref(queue));

  // Expect no errors from the threads.
  t1.join();
  t2.join();

  // Expect no element left.
  while (queue.Front().has_value()) {
    cout << queue.Front().value() << " ";
    queue.Pop();
  }
  cout << endl;
  return 0;
}
