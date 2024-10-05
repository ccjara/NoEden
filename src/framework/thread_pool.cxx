#include "framework/thread_pool.hxx"

ThreadPool::ThreadPool(size_t num_threads) {
    for (size_t i = 0; i < num_threads; ++i) {
        workers_.emplace_back([this] {
            while (true) {
                std::function<void()> task;

                {
                    std::unique_lock lock(queue_mutex_);
                    condition_.wait(lock, [this] { return stop_ || !tasks_.empty(); });

                    if (stop_ && tasks_.empty()) {
                        return;
                    }

                    task = std::move(tasks_.front());
                    tasks_.pop();
                }

                task();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock lock(queue_mutex_);
        stop_ = true;
    }
    condition_.notify_all();
    for (auto& worker: workers_) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}
