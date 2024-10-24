#include <fstream>
#include <mutex>
#include <unordered_map>

using namespace std;

namespace {
class FileHandlerCache {
private:
  unordered_map<string, ifstream> cache_;
  mutex mu_;

public:
  ifstream *GetFileHandler(const string &file_path) {
    lock_guard<mutex> lock(mu_);
    if (cache_.find(file_path) == cache_.end()) {
      cache_.emplace(file_path, ifstream{});
      cache_[file_path].open(file_path);
    }
    return &cache_[file_path];
  }
};
} // namespace

int main() {
  FileHandlerCache cache;
  ifstream *unused = cache.GetFileHandler("/tmp/demo.txt");
  return 0;
}