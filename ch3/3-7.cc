#include <chrono>
#include <condition_variable>
#include <iostream>
#include <list>
#include <mutex>
#include <thread>

using namespace std;

namespace {
class RWLock {
private:
  mutex mu_;
  condition_variable cv_;
  int reader_count_ = 0;
  bool is_writing_ = false;

public:
  void LockRead();
  void UnlockRead();
  void LockWrite();
  void UnlockWrite();
};

void RWLock::LockRead() {
  unique_lock<mutex> lock(mu_);
  cv_.wait(lock, [this]() { return !is_writing_; });
  reader_count_++;
}

void RWLock::UnlockRead() {
  unique_lock<mutex> lock(mu_);
  reader_count_--;
  if (reader_count_ == 0) {
    cv_.notify_all();
  }
}

void RWLock::LockWrite() {
  unique_lock<mutex> lock(mu_);
  cv_.wait(lock, [this]() { return !is_writing_ && reader_count_ == 0; });
  is_writing_ = true;
}

void RWLock::UnlockWrite() {
  unique_lock<mutex> lock(mu_);
  is_writing_ = false;
  cv_.notify_all();
}

void ReaderThread(RWLock &lock) {
  lock.LockRead();
  cout << "Acquired reader lock" << endl;
  this_thread::sleep_for(chrono::milliseconds(100));
  lock.UnlockRead();
  cout << "Released reader lock" << endl;
}

void WriterThread(RWLock &lock) {
  lock.LockWrite();
  cout << "Acquired writer lock" << endl;
  this_thread::sleep_for(chrono::milliseconds(500));
  lock.UnlockWrite();
  cout << "Released writer lock" << endl;
}
} // namespace

int main() {
  RWLock lock;

  list<thread> threads;
  for (int i = 0; i < 5; i++) {
    threads.emplace_back(ReaderThread, ref(lock));
    threads.emplace_back(WriterThread, ref(lock));
  }

  for (list<thread>::iterator it = threads.begin(); it != threads.end(); it++) {
    it->join();
  }

  return 0;
}