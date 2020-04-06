#include "task_manager.hxx"

void task_manager::startup(const platform_manager& platform) {
    auto max_threads { platform.max_threads() };
    if (!max_threads) {
        max_threads = 2;
    }
    const auto worker_count { max_threads - 1 };

    LOG(INFO)
        << "Task manager startup using "
        << worker_count
        << " workers (" << max_threads << " available)";

    for (auto i { 0U }; i < worker_count; i++) {
        pool_.emplace_back(&task_manager::worker, this);
    }
}

void task_manager::shutdown() {
    {
        std::lock_guard<std::mutex> lock { worker_mutex_ };
        is_running = false;
        worker_cv_.notify_all();
    }
    for (auto& thread : pool_) {
        thread.join();
    }
}

void task_manager::worker() {
    while (true) {
        std::unique_lock<std::mutex> lock { worker_mutex_ };
        worker_cv_.wait(lock, [this] {
            return !tasks_.empty() || !is_running;
        });

        if (!is_running) {
            return;
        }
        auto task { tasks_.back() };
        tasks_.pop_back();

        const bool was_last { tasks_.empty() };

        lock.unlock();

        worker_cv_.notify_all();

        task->run();
        
        if (was_last) {
            scheduler_cv_.notify_one();
        }
    }
}

void task_manager::process(const system_map_t& systems) {
    {
        std::lock_guard<std::mutex> lock { worker_mutex_ };
        for (auto& system_entry : systems) {
            tasks_.push_back(&system_entry.second->scene().primary_task());
        }

        if (tasks_.empty()) {
            return;
        }
        worker_cv_.notify_one();
    }

    std::unique_lock<std::mutex> sched_lock { scheduler_mutex_ };

    scheduler_cv_.wait(sched_lock, [this] {
        return tasks_.empty() || !is_running;
    });
}