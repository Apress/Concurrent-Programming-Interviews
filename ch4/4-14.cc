#include <condition_variable>
#include <cstdlib>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

using namespace std;

namespace {

class TaskExecutor {
private:
  queue<packaged_task<int(int)>> tasks_;
  vector<thread> workers_;

  mutex mu_;
  condition_variable cv_;
  bool stop_;

public:
  TaskExecutor(size_t num_of_threads);
  ~TaskExecutor();

  void EnqueueTask(packaged_task<int(int)> &&task);
};

TaskExecutor::TaskExecutor(size_t num_of_threads) : stop_(false) {
  for (size_t i = 0; i < num_of_threads; ++i)
    workers_.emplace_back([this] {
      while (true) {
        packaged_task<int(int)> task;
        {
          unique_lock<mutex> lock(mu_);
          cv_.wait(lock, [this] { return stop_ || !tasks_.empty(); });
          if (stop_ && tasks_.empty()) {
            return;
          }
          task = std::move(tasks_.front());
          tasks_.pop();
        }

        // Assuming the task is called with a random number.
        task(rand());
      }
    });
}

TaskExecutor::~TaskExecutor() {
  {
    unique_lock<mutex> lock(mu_);
    stop_ = true;
  }
  cv_.notify_all();
  for (thread &worker : workers_)
    worker.join();
}

void TaskExecutor::EnqueueTask(packaged_task<int(int)> &&task) {
  {
    unique_lock<mutex> lock(mu_);

    if (stop_) {
      throw runtime_error("Enqueue on stopped TaskExecutor");
    }
    tasks_.emplace(std::move(task));
  }
  cv_.notify_one();
}

mutex stdout_mu_;

int FooTask(int input) {
  {
    lock_guard<mutex> lock(stdout_mu_);
    cout << "Start FooTask thread. input is " << input << endl;
  }
  std::this_thread::sleep_for(chrono::seconds(3));
  return 0;
}

} // namespace

int main() {
  TaskExecutor executor(2);
  for (int i = 0; i < 10; i++) {
    executor.EnqueueTask(packaged_task<int(int)>(FooTask));
  }
  return 0;
}