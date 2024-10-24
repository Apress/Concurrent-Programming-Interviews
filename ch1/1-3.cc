#include <cstdlib>
#include <iostream>

using namespace std;

int main() {
  string command = "echo Hello, World!";
  int result = std::system(command.c_str());

  cout << "subprocess execution return code: " << result << endl;
  return 0;
}
