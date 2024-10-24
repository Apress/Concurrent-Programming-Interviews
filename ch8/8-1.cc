#include <iostream>

using namespace std;

namespace {

int Solution(const vector<int> &input, int target) {
  for (int i = 0; i < input.size(); i++) {
    if (target == input[i]) {
      return i;
    }
  }
  return -1;
}

} // namespace

int main() {
  // Create a test case.
  vector<int> input(1000, 0);
  for (int i = 0; i < input.size(); i++) {
    input[i] = i;
  }

  cout << Solution(input, 500) << endl; // Expect 500
  return 0;
}
