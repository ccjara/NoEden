#pragma once

enum EnqueueResult {
    Ok,
    ErrStopping
};

class ThreadPool {
public:
    explicit ThreadPool(size_t num_threads);

    ~ThreadPool();

    template<typename Fn>
    [[nodiscard]] EnqueueResult run(Fn&& fn) {
        {
            std::unique_lock lock(queue_mutex_);
            if (stop_) {
                return ErrStopping;
            }
            tasks_.emplace(std::forward<Fn>(fn));
        }
        condition_.notify_one();
        return Ok;
    }
private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex queue_mutex_;
    std::condition_variable condition_;
    bool stop_ = false;
};
