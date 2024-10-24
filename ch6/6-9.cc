#include <functional>
#include <iostream>
#include <list>
#include <optional>
#include <shared_mutex>
#include <thread>

using namespace std;

namespace {

class ThreadSafeQueue {
public:
  optional<int64_t> Front();
  void Pop();
  void Push(int64_t value);
  bool PopIf(function<bool(int64_t)> predicate);

private:
  shared_mutex mu_;
  list<int64_t> data_;
};

optional<int64_t> ThreadSafeQueue::Front() {
  shared_lock<shared_mutex> lock(mu_);
  if (data_.empty()) {
    return nullopt;
  }
  return data_.front();
}

void ThreadSafeQueue::Pop() {
  unique_lock<shared_mutex> lock(mu_);
  data_.pop_front();
}

void ThreadSafeQueue::Push(int64_t value) {
  unique_lock<shared_mutex> lock(mu_);
  data_.push_back(value);
}

bool ThreadSafeQueue::PopIf(function<bool(int64_t)> predicate) {
  {
    shared_lock<shared_mutex> lock(mu_);
    if (data_.empty() || !predicate(data_.front())) {
      // Did not pop the element.
      return false;
    }
  }

  unique_lock<shared_mutex> lock(mu_);
  if (data_.empty() || !predicate(data_.front())) {
    // Did not pop the element.
    return false;
  }
  data_.pop_front();
  return true;
}

void ThreadOne(ThreadSafeQueue &queue) {
  for (int i = 0; i < 1000; i++) {
    queue.Push(i);
  }

  for (int i = 0; i < 1000; i++) {
    queue.PopIf([](int64_t front) { return front < 5; });
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

  // Expect the first elements is 5.
  cout << "The first 10 elements are: ";
  int output_counter = 0;
  while (queue.Front().has_value() && output_counter < 10) {
    output_counter++;
    cout << queue.Front().value() << " ";
    queue.Pop();
  }
  cout << endl;
  return 0;
}
