#include "framework/thread_pool.hxx"

bool TaskGroup::await() {
    std::unique_lock lock(mutex_);
    task_cv_.wait(lock, [this] { return aborted_ || num_tasks_ == 0; });
    return !aborted_;
}

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
    for (auto& worker : workers_) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

[[nodiscard]] EnqueueResult ThreadPool::run(Task&& task) {
    {
        std::lock_guard lock(queue_mutex_);
        if (stop_) {
            return ErrStopping;
        }
        tasks_.emplace(std::move(task));
    }
    condition_.notify_one();
    return Ok;
}

[[nodiscard]] EnqueueResult ThreadPool::run(TaskGroup& task_group, Task&& task) {
    {
        std::lock_guard lock(task_group.mutex_);

        if (task_group.aborted_) {
            return ErrStopping;
        }

        ++task_group.num_tasks_;
        task_group.task_cv_.notify_all();
    }

    const auto result = run([&, task = std::move(task)]() {
        task();

        std::lock_guard lock(task_group.mutex_);
        --task_group.num_tasks_;
        task_group.task_cv_.notify_all();
    });

    if (result == ErrStopping) {
        std::lock_guard lock(task_group.mutex_);
        task_group.aborted_ = true;
        task_group.task_cv_.notify_all();
        return ErrStopping;
    }

    return Ok;
}
