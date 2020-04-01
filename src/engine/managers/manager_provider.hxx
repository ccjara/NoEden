#ifndef JARA_MANAGER_PROVIDER_HXX
#define JARA_MANAGER_PROVIDER_HXX

#include "resource/resource_manager.hxx"
#include "env/env_manager.hxx"
#include "platform/platform_manager.hxx"
#include "task/task_manager.hxx"

class task_manager;

struct manager_provider {
    resource_manager* const resource;
    env_manager* const env;
    platform_manager* const platform;
    task_manager* const task;
};

#endif
