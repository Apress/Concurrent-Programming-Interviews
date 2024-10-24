#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <thread>

using namespace std;

namespace {

const int kMaxRecursionLevel = 5;

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
void Sort(vector<int> &input, int start, int end, int recursion_level) {
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
  if (recursion_level < kMaxRecursionLevel) {
    thread left_worker(Sort, ref(input), start, mid + 1, recursion_level + 1);
    thread right_worker(Sort, ref(input), mid + 1, end, recursion_level + 1);

    left_worker.join();
    right_worker.join();
  } else {
    Sort(input, start, mid + 1, recursion_level + 1);
    Sort(input, mid + 1, end, recursion_level + 1);
  }

  Merge(input, start, end, mid);
}

} // namespace

int main() {
  // Create a test case.
  vector<int> input(10000, 0);
  for (int i = 0; i < input.size(); i++) {
    input[i] = i;
  };
  shuffle(input.begin(), input.end(), default_random_engine());

  chrono::time_point t0 = chrono::high_resolution_clock::now();
  Sort(input, 0, input.size(), 0);
  chrono::time_point t1 = chrono::high_resolution_clock::now();

  cout << "Sorting takes "
       << chrono::duration_cast<chrono::milliseconds>(t1 - t0).count()
       << " milliseconds." << endl;
  return 0;
}
