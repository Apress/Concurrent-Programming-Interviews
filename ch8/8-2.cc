#include <iostream>
#include <unordered_map>

using namespace std;

namespace {

unordered_map<int, int> value_index_mapping;

void Preprocess(const vector<int> &input) {
  for (int i = 0; i < input.size(); i++) {
    value_index_mapping[input[i]] = i;
  }
}

int Lookup(int target) {
  unordered_map<int, int>::iterator it = value_index_mapping.find(target);
  if (it == value_index_mapping.end()) {
    return -1;
  }
  return it->second;
}

} // namespace

int main() {
  // Create a test cases.
  vector<int> input(1000, 0);
  for (int i = 0; i < input.size(); i++) {
    input[i] = i;
  }

  Preprocess(input);
  cout << Lookup(500) << endl; // Expect 500
  cout << Lookup(600) << endl; // Expect 600
  cout << Lookup(700) << endl; // Expect 700
  return 0;
}
