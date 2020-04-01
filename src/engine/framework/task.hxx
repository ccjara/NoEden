#ifndef JARALYN_TASK_HXX
#define JARALYN_TASK_HXX

class task {
public:
    virtual ~task() = default;
    virtual void run() = 0;
};

#endif
