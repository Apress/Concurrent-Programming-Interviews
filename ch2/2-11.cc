#include <mutex>
#include <string>
#include <unordered_map>

#include "fake_storage.h"

using namespace std;

namespace {
class NetworkFileHandlerCache {
  fake_storage::Client *client_; // NOT OWNED

public:
  NetworkFileHandlerCache(fake_storage::Client *client) : client_(client) {}

  fake_storage::FileHandler *GetFileHandler(const string &bucket_name,
                                            const string &file_path) {
    // Assume the cache is a black box in this example. We do not
    // need actual implementation here.
    return new fake_storage::FileHandler();
  }
};

class FileCopier {
  fake_storage::Client *client_; // NOT OWNED
  NetworkFileHandlerCache cache_;
  unordered_map<string, mutex> mutex_map_;
  mutex mutex_map_mu_;

public:
  FileCopier(fake_storage::Client *client)
      : client_(client), cache_(NetworkFileHandlerCache(client)) {}

  mutex *GetFileMutex(const string &file_path) {
    lock_guard<mutex> lock(mutex_map_mu_);
    if (mutex_map_.find(file_path) == mutex_map_.end()) {
      mutex_map_.try_emplace(file_path);
    }
    return &mutex_map_[file_path];
  }

  void CopyFiles(const string &bucket_src, const string &file_path_src,
                 const string &bucket_dest, const string &file_path_dest) {
    fake_storage::FileHandler *file_src =
        cache_.GetFileHandler(bucket_src, file_path_src);
    fake_storage::FileHandler *file_dest =
        cache_.GetFileHandler(bucket_dest, file_path_dest);

    mutex *mu1 = GetFileMutex(file_path_src);
    mutex *mu2 = GetFileMutex(file_path_dest);

    if (file_path_src > file_path_dest) {
      swap(mu1, mu2);
    }

    lock_guard<mutex> lock_src(*mu1);
    lock_guard<mutex> lock_dest(*mu2);

    // Assume copyFileInternal function will do the actual work to copy the
    // files.
    client_->CopyFiles(file_src, file_dest);
  }
};

} // namespace

int main() {
  unique_ptr<fake_storage::Client> client = make_unique<fake_storage::Client>();
  FileCopier copier(client.get());

  // Copy files.
  copier.CopyFiles("bucket1", "/path/to/file1", "bucket2", "/path/to/file2");

  return 0;
}