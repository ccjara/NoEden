#ifndef JARALYN_GFX_TASK_HXX
#define JARALYN_GFX_TASK_HXX

#include "../../engine/framework/task.hxx"
#include "renderer.hxx"

class gfx_task : public task {
protected:
    renderer* renderer_;
public:
    gfx_task(renderer* r) {
        assert(r);
        renderer_ = r;
    }

    void run() override {
        renderer_->start_rendering();

        renderer_->text->begin();

        renderer_->text->render_text("TEST", { 10, 10 });

        renderer_->text->end();

        renderer_->finish_rendering();
    }
};

#endif
