#ifndef NOEDEN_ACTION_PROCESSOR_HXX
#define NOEDEN_ACTION_PROCESSOR_HXX

class IActionProcessor {
public:
    virtual void process_actions() = 0;

    virtual ~IActionProcessor() = default;
};

#endif
