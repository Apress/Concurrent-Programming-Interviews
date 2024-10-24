#include <iostream>
#include <list>
#include <mutex>
#include <thread>

using namespace std;

namespace {

class ConfigFile {
public:
  ConfigFile() { cout << "ConfigFile is initialized." << endl; }
};

ConfigFile *configfile;
once_flag config_flag;

void InitializeConfigFile() {
  call_once(config_flag, [&]() { configfile = new ConfigFile(); });
}

void ThreadOne() {
  InitializeConfigFile();

  // Omit other business logic.
}

} // namespace

int main() {
  list<thread> threads;
  for (int i = 0; i < 10; i++) {
    threads.emplace_back(ThreadOne);
  }
  for (list<thread>::iterator it = threads.begin(); it != threads.end(); it++) {
    it->join();
  }

  return 0;
}
