#ifndef JARALYN_TASK_MANAGER_HXX
#define JARALYN_TASK_MANAGER_HXX

#include "../manager.hxx"
#include "../platform/platform_manager.hxx"
#include "../../framework/task.hxx"
#include "../../framework/game_system.hxx"

class game_system;

class task_manager : public manager<task_manager> {
private:
    std::mutex worker_mutex_;
    std::mutex scheduler_mutex_;
    std::condition_variable worker_cv_;
    std::condition_variable scheduler_cv_;
    std::vector<std::unique_ptr<task>> tasks_;
    std::vector<std::thread> pool_;

    bool is_running { true };

    void worker();
public:
    void startup(const platform_manager& platform);
    void shutdown();

    void process(std::vector<game_system*> systems);

    task_manager() = default;
    task_manager(const task_manager&) = delete;
    task_manager(task_manager&&) = delete;
    task_manager& operator=(task_manager&&) = delete;
    const task_manager& operator=(const task_manager&) = delete;
};

#endif