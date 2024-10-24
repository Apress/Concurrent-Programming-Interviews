#include <iostream>
#include <thread>

using namespace std;

int main() {
  thread t([]() { cout << "This is ThreadOne." << endl; });
  t.join();
  return 0;
}
