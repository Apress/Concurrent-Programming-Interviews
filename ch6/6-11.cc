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
    atomic<Node *> next;
  };
  atomic<Node *> head_;
  atomic<Node *> tail_;
};

ThreadSafeQueue::ThreadSafeQueue() {
  Node *dummy = new Node;
  dummy->next = nullptr;
  head_.store(dummy);
  tail_.store(dummy);
}

optional<int64_t> ThreadSafeQueue::Front() {
  Node *front = head_.load();
  if (front == tail_.load()) {
    return nullopt;
  }
  return front->next.load()->data;
}

void ThreadSafeQueue::Pop() {
  Node *front;
  while (true) {
    front = head_.load();
    Node *back = tail_.load();
    Node *next = front->next;

    if (front == back && next == nullptr) {
      cout << "ERROR: Pop operation is called for an empty queue." << endl;
      return;
    }

    // Tail pointer is not updated to the right value.
    if (front == back && next != nullptr) {
      tail_.compare_exchange_weak(back, next);
      continue;
    }

    if (head_.compare_exchange_weak(front, front->next)) {
      break;
    }
  }
  free(front);
}

void ThreadSafeQueue::Push(int64_t value) {
  Node *elem = new Node;
  elem->data = value;
  elem->next = nullptr;

  Node *back;
  while (true) {
    back = tail_.load();
    Node *next = back->next;

    if (next != nullptr) {
      tail_.compare_exchange_weak(back, next);
      continue;
    }

    if (back->next.compare_exchange_weak(next, elem)) {
      break;
    }
  }

  tail_.compare_exchange_weak(back, elem);
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

  while (queue.Front().has_value()) {
    cout << queue.Front().value() << " ";
    queue.Pop();
  }
  cout << endl;
  return 0;
}
