#include <future>
#include <iostream>
#include <thread>

using namespace std;

namespace {

int ThreadOne() {
  cout << "This is ThreadOne." << endl;
  return 1;
}

} // namespace

int main() {
  future a = async(ThreadOne);
  cout << "The value of a is: " << a.get() << endl;

  return 0;
}
