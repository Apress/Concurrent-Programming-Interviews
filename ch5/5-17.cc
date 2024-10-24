#include <iostream>

using namespace std;

namespace {
const int kVectorLength = 10;
}

int main() {
  srand(time(nullptr));

  vector<int> results;
  for (int i = 0; i < kVectorLength; i++) {
    results.push_back(rand());
  }

  for (int i = 0; i < kVectorLength; i++) {
    cout << results[i] << " ";
  }
  cout << endl;
  return 0;
}
