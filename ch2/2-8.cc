#include <iostream>
#include <list>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>

#include "fake_storage.h"

using namespace std;

namespace {
class NetworkFileHandlerCache {
private:
  fake_storage::Client *client_; // NOT OWNED
  unordered_map<string, unique_ptr<fake_storage::BucketHandler>> bucket_cache_;
  unordered_map<string, unique_ptr<fake_storage::FileHandler>> file_cache_;
  mutex bucket_mu_, file_mu_;

public:
  NetworkFileHandlerCache(fake_storage::Client *client) : client_(client) {}

  fake_storage::BucketHandler *GetBucketHandler(const string &bucket_name) {
    unique_lock<mutex> lock(bucket_mu_);
    if (bucket_cache_.find(bucket_name) != bucket_cache_.end()) {
      return bucket_cache_[bucket_name].get();
    }
    lock.unlock();

    cout << "Initial new bucket handler for " << bucket_name << endl;
    unique_ptr<fake_storage::BucketHandler> bucket =
        client_->InitBucketHandler(bucket_name);

    lock.lock();
    bucket_cache_[bucket_name] = std::move(bucket);
    return bucket_cache_[bucket_name].get();
  }

  fake_storage::FileHandler *GetFileHandler(const string &bucket_name,
                                            const string &file_path) {
    unique_lock<mutex> lock(file_mu_);
    if (file_cache_.find(file_path) != file_cache_.end()) {
      return file_cache_[file_path].get();
    }
    lock.unlock();

    fake_storage::BucketHandler *bucket = GetBucketHandler(bucket_name);

    cout << "Initial new file handler for " << file_path << endl;
    unique_ptr<fake_storage::FileHandler> file =
        client_->InitFileHandler(bucket, file_path);

    lock.lock();
    file_cache_[file_path] = std::move(file);
    return file_cache_[file_path].get();
  }
};

void ThreadOne(NetworkFileHandlerCache &cache) {
  cache.GetFileHandler("bucket1", "/path/to/file1");
}

void ThreadTwo(NetworkFileHandlerCache &cache) {
  cache.GetFileHandler("bucket2", "/path/to/file2");
}
} // namespace

int main() {
  unique_ptr<fake_storage::Client> client = make_unique<fake_storage::Client>();
  NetworkFileHandlerCache cache(client.get());

  list<thread> threads;
  for (int i = 0; i < 5; i++) {
    threads.emplace_back(ThreadOne, ref(cache));
    threads.emplace_back(ThreadTwo, ref(cache));

    // Tune the sleep here to view different results.
    this_thread::sleep_for(chrono::milliseconds(100));
  }

  for (list<thread>::iterator it = threads.begin(); it != threads.end(); it++) {
    it->join();
  }

  return 0;
}
