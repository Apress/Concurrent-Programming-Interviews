#ifndef __FAKE_STORAGE__
#define __FAKE_STORAGE__

#include <string>

using namespace std;

namespace fake_storage {
class FileHandler {};

class BucketHandler {};

class Client {
public:
  unique_ptr<BucketHandler> InitBucketHandler(const string &bucket_name) {
    return make_unique<BucketHandler>();
  }
  unique_ptr<FileHandler> InitFileHandler(BucketHandler *bucket,
                                          const string &file_path) {
    return make_unique<FileHandler>();
  }
  void CopyFiles(FileHandler *file_src, FileHandler *file_dest) {}
};
} // namespace fake_storage

#endif