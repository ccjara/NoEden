#ifndef JARALYN_DISPLAY_HXX
#define JARALYN_DISPLAY_HXX

#include "../grid.hxx"

struct j_display_cell {
public:
    float_t letter { 0.0f };
    j_rgb<float_t> color { 1.0f, 1.0f, 1.0f };

    static const j_display_cell null;
};

class j_display: public j_grid<j_display_cell> {
};

#endif
