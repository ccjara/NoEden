#ifndef JARALYN_STATE_WRITER_HXX
#define JARALYN_STATE_WRITER_HXX

#include "state.hxx"

class j_state_writer {
public:
    /**
     * @brief Pushes the given state onto the stack
     */
    virtual void push(j_state_id state_id) = 0;

    /**
     * @brief Removes the topmost game state from the stack
     */
    virtual void pop() = 0;

    /**
     * @brief Updates all active states
     */
    virtual void update() = 0;
};

#endif
