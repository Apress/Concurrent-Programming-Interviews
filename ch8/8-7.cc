#include <iostream>
#include <list>
#include <shared_mutex>
#include <thread>

using namespace std;

namespace {

class MaxStack {
public:
  void Pop();

  void Push(int value);

  int Top();

  int Max();

private:
  list<int> data_;
  list<int> max_;
  shared_mutex mu_;
};

void MaxStack::Pop() {
  unique_lock<shared_mutex> lock(mu_);
  if (data_.empty()) {
    cout << "ERROR: the stack is empty" << endl;
    return;
  }

  if (data_.back() == max_.back()) {
    max_.pop_back();
  }
  data_.pop_back();
}

void MaxStack::Push(int value) {
  unique_lock<shared_mutex> lock(mu_);
  data_.push_back(value);

  if (max_.empty() || value >= max_.back()) {
    max_.push_back(value);
  }
}

int MaxStack::Top() {
  shared_lock<shared_mutex> lock(mu_);
  if (data_.empty()) {
    cout << "ERROR: the stack is empty" << endl;
    return 0;
  }
  return data_.back();
}

int MaxStack::Max() {
  shared_lock<shared_mutex> lock(mu_);
  if (data_.empty()) {
    cout << "ERROR: the stack is empty" << endl;
    return 0;
  }
  return max_.back();
}

void TestThread(MaxStack &stack) {
  stack.Push(rand());
  stack.Top();
  stack.Max();
  stack.Pop();
}

} // namespace

int main() {
  // Create a test case.
  MaxStack stack;

  // Put an element so that it will never throw error.
  stack.Push(1);

  list<thread> threads;
  // We should see no errors.
  for (int i = 0; i < 100; i++) {
    threads.emplace_back(TestThread, ref(stack));
  }
  for (list<thread>::iterator it = threads.begin(); it != threads.end(); it++) {
    it->join();
  }

  return 0;
}
