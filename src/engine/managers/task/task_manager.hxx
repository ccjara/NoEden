#ifndef JARALYN_TASK_MANAGER_HXX
#define JARALYN_TASK_MANAGER_HXX

#include "../platform/platform_manager.hxx"
#include "../../framework/task.hxx"
#include "../../framework/game_system.hxx"

class platform_manager;

class task_manager {
private:
    friend class manager_provider;

    std::mutex worker_mutex_;
    std::mutex scheduler_mutex_;
    std::condition_variable worker_cv_;
    std::condition_variable scheduler_cv_;
    std::vector<task*> tasks_;
    std::vector<std::thread> pool_;

    bool is_running { true };

    void worker();

    void startup(const platform_manager& platform);
    void shutdown();
public:
    void process(const system_map_t& systems);

    task_manager() = default;
    task_manager(const task_manager&) = delete;
    task_manager(task_manager&&) = delete;
    task_manager& operator=(task_manager&&) = delete;
    const task_manager& operator=(const task_manager&) = delete;
};

#endif