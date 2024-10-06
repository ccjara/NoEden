#pragma once

enum EnqueueResult {
    Ok,
    ErrStopping
};

class TaskGroup {
    friend class ThreadPool;
public:
    /**
     * @brief Blocks until all tasks in the group have completed or the group is aborted.
     *
     * Returns true if all tasks completed, false if a task failed to complete or the group was aborted.
     */
    [[nodiscard]] bool await();
private:
    i32 num_tasks_ = 0;
    bool aborted_ = false;
    std::mutex mutex_ = {};
    std::condition_variable task_cv_ = {};
};

class ThreadPool {
    using Task = std::function<void()>;
public:
    explicit ThreadPool(size_t num_threads);

    ~ThreadPool();

    /**
     * @brief Schedules the given task
     */
    [[nodiscard]] EnqueueResult run(Task&& task);

    /**
     * @brief Schedules the given task and adds it to the given task group
     */
    [[nodiscard]] EnqueueResult run(TaskGroup& task_group, Task&& task);
private:
    std::vector<std::thread> workers_;
    std::queue<Task> tasks_;
    std::mutex queue_mutex_;
    std::condition_variable condition_;
    bool stop_ = false;
};
