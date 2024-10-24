#include <iostream>
#include <list>

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
};

void MaxStack::Pop() {
  if (data_.empty()) {
    cout << "ERROR: the stack is empty" << endl;
    return;
  }
  data_.pop_back();
}

void MaxStack::Push(int value) { data_.push_back(value); }

int MaxStack::Top() {
  if (data_.empty()) {
    cout << "ERROR: the stack is empty" << endl;
    return 0;
  }
  return data_.back();
}

int MaxStack::Max() {
  if (data_.empty()) {
    cout << "ERROR: the stack is empty" << endl;
    return 0;
  }

  // Find the largest element.
  int result = numeric_limits<int>::min();
  for (int elem : data_) {
    result = max(elem, result);
  }
  return result;
}

} // namespace

int main() {
  // Create a test case.
  MaxStack stack;
  stack.Push(4);
  stack.Push(3);
  stack.Push(5);
  stack.Push(1);
  stack.Push(2);

  for (int i = 0; i < 5; i++) {
    cout << "Top is: " << stack.Top() << ", Max is: " << stack.Max() << endl;
    stack.Pop();
  }

  // Expect errors.
  stack.Top();
  stack.Max();
  stack.Pop();

  return 0;
}
