#include <algorithm>
#include <iostream>
#include <random>

using namespace std;

namespace {

void Merge(vector<int> &input, int start, int end, int mid) {
  vector<int> temp;
  int left = start;
  int right = mid + 1;
  while (left < mid + 1 && right < end) {
    if (input[left] < input[right]) {
      temp.push_back(input[left++]);
    } else {
      temp.push_back(input[right++]);
    }
  }

  while (left < mid + 1) {
    temp.push_back(input[left++]);
  }

  while (right < end) {
    temp.push_back(input[right++]);
  }

  for (int i = 0; i < temp.size(); i++) {
    input[start + i] = temp[i];
  }
}

// `start` is inclusive. `end` is exclusive.
void Sort(vector<int> &input, int start, int end) {
  if (start + 1 == end) {
    // one element
    return;
  }

  if (start + 2 == end) {
    // two elements
    if (input[start] > input[start + 1]) {
      swap(input[start], input[start + 1]);
    }
    return;
  }

  int mid = start + (end - start) / 2;
  Sort(input, start, mid + 1);
  Sort(input, mid + 1, end);
  Merge(input, start, end, mid);
}

} // namespace

int main() {
  // Create a test case.
  vector<int> input(20, 0);
  for (int i = 0; i < input.size(); i++) {
    input[i] = i;
  };
  shuffle(input.begin(), input.end(), default_random_engine());

  Sort(input, 0, input.size());

  for (int elem : input) {
    cout << elem << " ";
  }
  cout << endl;
  return 0;
}
